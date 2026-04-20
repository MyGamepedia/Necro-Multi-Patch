#pragma newdecls required
#pragma semicolon 1

public void Snark_PathPost(const int iEntIndex)
{
    CNpc_Snark pSnark = CNpc_Snark(iEntIndex);
    if (!pSnark.IsValid())
        return;

    if (g_ConvarNecroCoopSnarks.BoolValue)
        RequestFrame(Hook_Snark_OnCreated, iEntIndex);

    CBlackMesaPlayer pPlayer = CBlackMesaPlayer(pSnark.GetOwner());
    if (!pPlayer.IsValid())
        return;

    pSnark.SetSnarkOwner(pPlayer); //store here if we lost owner, because it does somehow, right before it should be removed, idk why

    //trigger the first snark explosion and clean from the array
    if (g_hClientSnarks[pPlayer.entindex][MAX_SNARKS - 1] > 0)
    {
        CNpc_Snark pOldSnark = CNpc_Snark(g_hClientSnarks[pPlayer.entindex][MAX_SNARKS - 1]);
		if (pOldSnark.IsValid())
		{
			// Invalidate index so UpdateOnRemove ignores it
			pOldSnark.SetUserData("m_iSnarkNum", -1);

			pOldSnark.AcceptInputInt("SetHealth", 0);
		}

        g_hClientSnarks[pPlayer.entindex][MAX_SNARKS - 1] = 0;
    }

    //shift to right everyone
    for (int i = MAX_SNARKS - 1; i > 0; i--)
    {
        int iOldEnt = g_hClientSnarks[pPlayer.entindex][i - 1];
        g_hClientSnarks[pPlayer.entindex][i] = iOldEnt;

        if (iOldEnt > 0)
        {
            CNpc_Snark pOldSnark = CNpc_Snark(iOldEnt);
            if (pOldSnark.IsValid())
                pOldSnark.SetUserData("m_iSnarkNum", i);
        }
    }

    //add new snark
    g_hClientSnarks[pPlayer.entindex][0] = pSnark.entindex;
    pSnark.SetUserData("m_iSnarkNum", 0);

    //snark appears in owners head for a while, fix it by hidding snark for a while
    pSnark.SetRenderMode(RENDER_NONE);
    pSnark.AcceptInput("DisableShadow");
    pSnark.AddOutput("OnUser4", "!self", "Addoutput", "rendermode 0", 0.02, 1);
    pSnark.AddOutput("OnUser4", "!self", "EnableShadow", "", 0.02, 1);
    pSnark.FireOutput("OnUser4");
}

//------------------------------------------------------
// npc_snark
// Set snark's team to make it evil or peaceful to the players.
//------------------------------------------------------
public void Hook_Snark_OnCreated(const int iEntIndex)
{
    CNpc_Snark pSnark = CNpc_Snark(iEntIndex);
    if (!pSnark.IsValid() || pSnark.GetHammerID() != 0) //don't for map-placed
        return;
    
    CBaseEntity pOwner = pSnark.GetSnarkOwner();
    if (!pOwner.IsValid() || !pOwner.IsPlayer())
        return;

    CBasePlayer pPlayer = view_as<CBasePlayer>(pOwner);
    if (!pPlayer.IsValid())
        return;

    int iTeam = (mp_friendlyfire.BoolValue) ? TEAM_ANY : pPlayer.GetTeam();
    pSnark.SetInitialTeam(iTeam);
    pSnark.SetTeam(iTeam);
}

//------------------------------------------------------
// npc_snark
// Shift to left snark counter once i'm dead
//------------------------------------------------------
MRESReturn Hook_SnarkUpdateOnRemove(int _this)
{
	CNpc_Snark pSnark = CNpc_Snark(_this);

	CBlackMesaPlayer pPlayer = CBlackMesaPlayer(pSnark.GetSnarkOwner());
	if (!pPlayer.IsValid())
		return MRES_Ignored;

	int iSnarkNum = pSnark.GetUserData("m_iSnarkNum");

	if (iSnarkNum < 0 || iSnarkNum >= MAX_SNARKS)
		return MRES_Ignored;

	//shift to left
	for (int i = iSnarkNum; i < MAX_SNARKS - 1; i++)
	{
		CBaseEntity nextEnt = CBaseEntity(g_hClientSnarks[pPlayer.entindex][i + 1]);
		g_hClientSnarks[pPlayer.entindex][i] = nextEnt.entindex;

		if (nextEnt.entindex > 0)
		{
			CNpc_Snark pNextSnark = CNpc_Snark(nextEnt);
			if (pNextSnark.IsValid())
			{
				pNextSnark.SetUserData("m_iSnarkNum", i);
			}
		}
	}

	//clean the last element
	g_hClientSnarks[pPlayer.entindex][MAX_SNARKS - 1] = 0;

	return MRES_Ignored;
}

//------------------------------------------------------
// npc_snark
// Make snark owned by a player peaceful without mp_friendlyfire, filter players (by teamnum) and ally to owner NPCs
//------------------------------------------------------
public MRESReturn Hook_SnarkIsValidEnemy(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!g_ConvarNecroCoopSnarks.BoolValue || DHookIsNullParam(hParams, 1) ||  mp_friendlyfire.BoolValue)
        return MRES_Ignored;
    
    CAI_BaseNPC pEnemy = CAI_BaseNPC(DHookGetParam(hParams, 1));

    //not valid enemy
    if (!pEnemy.IsValid())
        return MRES_Ignored;

    CNpc_Snark pSnark = CNpc_Snark(_this);
    CBaseEntity pOwner = pSnark.GetSnarkOwner();

    //i don't have owner or the owner is not a player
    if (pOwner == NULL_CBASEENTITY || !pOwner.IsPlayer())
        return MRES_Ignored;
        
    CBasePlayer pPlayer = view_as<CBasePlayer>(pOwner);
    if (!pPlayer.IsValid())
        return MRES_Ignored;

    //filter players only by their team, nothing else needed, in srccoop, we using forced teams anyway
	if (pEnemy.IsPlayer())
	{
		if (pEnemy.GetTeam() == pOwner.GetTeam())
		{
			DHookSetReturn(hReturn, false);
			return MRES_Supercede;
		}
		return MRES_Ignored;
	}

    //i'm not sure if this can happen, but this method takes cbasecombatcharacter as param
	//the class doesn't have m_afMemory, unlike npcs, so this prevents possible issues
	if (!pEnemy.IsNPC())
		return MRES_Ignored;

    //is this npc PROVOKED (security) ? 
    if (pEnemy.m_afMemory & bits_MEMORY_PROVOKED)
        return MRES_Ignored;

    //target hates my owner
    if (pEnemy.IRelationType(pOwner) == D_HT)
        return MRES_Ignored;

    //Does it targets my owner ? 
    bool bMyOwnerIsTargetEnemy = (HasEntProp(pEnemy.entindex, Prop_Data, "m_hEnemy")) ? (pEnemy.GetEnemy() == pOwner) : false;
    
    if (pEnemy.IsPlayerAlly(pPlayer) && !bMyOwnerIsTargetEnemy)
    {       
        DHookSetReturn(hReturn, false);
        return MRES_Supercede;
    }

	return MRES_Ignored;
}


//------------------------------------------------------
// npc_snark
// Make everyone think i'm ally under certain conditions.
// TODO: Make sure it doesn't break anything.
//------------------------------------------------------
public MRESReturn Hook_SnarkIsPlayerAlly(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!g_ConvarNecroCoopSnarks.BoolValue || mp_friendlyfire.BoolValue || hParams.IsNull(1))
        return MRES_Ignored;

    CBlackMesaPlayer pPlayer = CBlackMesaPlayer(DHookGetParam(hParams, 1));
    CNpc_Snark pSnark = CNpc_Snark(_this);
    
    //ally if we are in the same team
    if (pPlayer.IsValid() && pPlayer.GetTeam() == pSnark.GetTeam())
	{
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}

	return MRES_Ignored;
}

/*
public MRESReturn Hook_SnarkIRelationType(int _this, DHookReturn hReturn, DHookParam hParams)
{
    if(!g_ConvarNecroCoopSnarks.BoolValue || mp_friendlyfire.BoolValue || hParams.IsNull(1))
        return MRES_Ignored;

    CAI_BaseNPC pEntity = CAI_BaseNPC(DHookGetParam(hParams, 1));
    
    if (!pEntity.IsValid())
        return MRES_Ignored;

    CBasePlayer pOwner = CBasePlayer(CNpc_Snark(_this).GetSnarkOwner());

    if (!pOwner.IsValid())
        return MRES_Ignored;

    char szClassname[MAX_CLASSNAME];
    pEntity.GetClassname(szClassname, sizeof(szClassname));

    if (pEntity.IsPlayerAlly(pOwner))
	{
        //ignore for provoked guards
        if (HasEntProp(pEntity.entindex, Prop_Data, "m_afMemory") && pEntity.m_afMemory & bits_MEMORY_PROVOKED)
            return MRES_Ignored;

        //Does it targets my owner ? 
        bool bMyOwnerIsTargetEnemy = (HasEntProp(pEntity.entindex, Prop_Data, "m_hEnemy")) ? (pEntity.GetEnemy() == pOwner) : false;

        if (!bMyOwnerIsTargetEnemy)
        {
            PrintToChatAll("BOBA");
		    DHookSetReturn(hReturn, D_LI);
		    return MRES_Supercede;            
        }
    }

    return MRES_Ignored;
}
*/