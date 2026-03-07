#pragma newdecls required
#pragma semicolon 1

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

    pSnark.SetSnarkOwner(pPlayer.entindex); //store here if we lost owner, because it does somehow, right before it should be removed, idk why

    //trigger the first snark explosion and clean from the array
    if (g_hClientSnarks[pPlayer.entindex][19] != 0)
    {
        CNpc_Snark pOldSnark = CNpc_Snark(g_hClientSnarks[pPlayer.entindex][19]);
        if (pOldSnark.IsValid())
        {
            pOldSnark.AcceptInputInt("SetHealth", 0);
        }

        g_hClientSnarks[pPlayer.entindex][19] = 0;
    }

    //shift to right everyone
    for (int i = 19; i > 0; i--)
    {
        int iOldEnt = g_hClientSnarks[pPlayer.entindex][i - 1];
        g_hClientSnarks[pPlayer.entindex][i] = iOldEnt;

        if (iOldEnt != 0)
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


public void Hook_Snark_OnCreated(const int iEntIndex)
{
	if (CBaseEntity(iEntIndex).GetHammerID() != 0)
        return;
    
    CBasePlayer pPlayer = CBasePlayer(CNpc_Snark(iEntIndex).GetSnarkOwner());
    if (!pPlayer.IsValid())
        return;

    int iTeam = (mp_friendlyfire.BoolValue) ? TEAM_ANY : pPlayer.GetTeam();
	SetEntProp(iEntIndex, Prop_Data, "m_iInitialTeamNum", iTeam);
	SetEntProp(iEntIndex, Prop_Data, "m_iTeamNum", iTeam);
}

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

public MRESReturn Hook_SnarkIsValidEnemy(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!g_ConvarNecroCoopSnarks.BoolValue || DHookIsNullParam(hParams, 1) ||  mp_friendlyfire.BoolValue)
        return MRES_Ignored;
    
    CAI_BaseNPC pEnemy = CAI_BaseNPC(DHookGetParam(hParams, 1));
    if (!pEnemy.IsValid())
        return MRES_Ignored;

    CBasePlayer pOwner = CBasePlayer(CNpc_Snark(_this).GetSnarkOwner());

    if (!pOwner.IsValid())
        return MRES_Ignored;

    if (HasEntProp(pEnemy.entindex, Prop_Data, "m_afMemory") && pEnemy.m_afMemory & bits_MEMORY_PROVOKED)
        return MRES_Ignored;

    if (pEnemy.IRelationType(pOwner) == D_HT)
        return MRES_Ignored;

    //Is it owner's friend player ? 
    bool bIsAllyPlayer = (pEnemy.IsPlayer()) ? (pEnemy.GetTeam() == pOwner.GetTeam()) : false;

    //Does it targets my owner ? 
    bool bMyOwnerIsTargetEnemy = (HasEntProp(pEnemy.entindex, Prop_Data, "m_hEnemy")) ? (pEnemy.GetEnemy() == pOwner) : false;
    
    if (bIsAllyPlayer || pEnemy.IsPlayerAlly(pOwner) || !bMyOwnerIsTargetEnemy)
    {       
        DHookSetReturn(hReturn, false);
        return MRES_Supercede;
    }

	return MRES_Ignored;
}

public MRESReturn Hook_SnarkIsPlayerAlly(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!g_ConvarNecroCoopSnarks.BoolValue || mp_friendlyfire.BoolValue || hParams.IsNull(1))
        return MRES_Ignored;

    CBlackMesaPlayer pPlayer = CBlackMesaPlayer(DHookGetParam(hParams, 1));

    if (!pPlayer.IsValid())
        return MRES_Ignored;

    CBlackMesaPlayer pOwner = CBlackMesaPlayer(CNpc_Snark(_this).GetSnarkOwner());

    if (!pOwner.IsValid())
        return MRES_Ignored;
    
    if (pPlayer == pOwner)
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