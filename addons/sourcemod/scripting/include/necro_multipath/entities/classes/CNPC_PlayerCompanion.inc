#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CNPC_PlayerCompanion - npc_human_scientist*, npc_human_security
// In case a player is not passed in, consider as ally so long we don't dislike any player.
// This allows friendly NPCs to pass filter in "player ally only" triggers in MP.
//------------------------------------------------------
public MRESReturn Hook_IsPlayerAlly(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (hParams.IsNull(1)) // pPlayer
	{
		CAI_BaseNPC pThis = CAI_BaseNPC(_this);
		for (int i = 1; i <= MaxClients; i++)
		{
			CBasePlayer pPlayer = CBasePlayer(i);
			if (pPlayer != NULL_CBASEENTITY && !pPlayer.IsBot() && pPlayer.IsAlive())
			{
				if (pThis.IRelationType(pPlayer) != D_LI)
				{
					DHookSetReturn(hReturn, false);
					return MRES_Supercede;
				}
			}
		}
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}
	return MRES_Ignored;
}

//------------------------------------------------------
// `CNPC_PlayerCompanion` - `npc_human_scientist*`, `npc_human_security`
// This fixes a crash that happens upon dereferencing a null `AI_GetSinglePlayer` instance inside the function `CNPC_PlayerCompanion::IsNavigationUrgent`.
// This crash occurs when the player that this entity is following goes to a unreachable location.
//
// https://github.com/ValveSoftware/source-sdk-2013/blob/39f6dde8fbc238727c020d13b05ecadd31bda4c0/src/game/server/hl2/npc_playercompanion.cpp#L3687C1-L3687C23
//
// The below implementation is not a rebuild of the original code.
//------------------------------------------------------
public MRESReturn Hook_IsNavigationUrgent(int _this, DHookReturn hReturn, DHookParam hParams)
{
	DHookSetReturn(hReturn, true);
	return MRES_Supercede;
}

//------------------------------------------------------
// CNPC_PlayerCompanion
// Ignore player's snarks with mp_friendlyfire unless this player is not my friend  
//------------------------------------------------------
public MRESReturn Hook_PlayerCompanionIsValidEnemy(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!g_ConvarNecroCoopSnarks.BoolValue || DHookIsNullParam(hParams, 1) ||  mp_friendlyfire.BoolValue)
        return MRES_Ignored;
    
    CAI_BaseNPC pEnemy = CAI_BaseNPC(DHookGetParam(hParams, 1));
    if (!pEnemy.IsValid())
        return MRES_Ignored;

	char szClassname[MAX_CLASSNAME];
	pEnemy.GetClassname(szClassname, sizeof(szClassname));

	if (strcmp(szClassname, "npc_snark") == 0)
	{
		CBasePlayer pOwner = CBasePlayer(CNpc_Snark(pEnemy.entindex).GetSnarkOwner());
		CAI_BaseNPC pThis = CAI_BaseNPC(_this);

		//not valid OR owner is not player (f. e. npc_maker) OR i don't like the player OR not player's snark
		if (!pOwner.IsValid() || !pOwner.IsPlayer() || (pThis.m_afMemory & bits_MEMORY_PROVOKED || pThis.IRelationType(pOwner) == D_HT) || pEnemy.GetTeam() != pOwner.GetTeam())
			return MRES_Ignored;
		
		DHookSetReturn(hReturn, false);
		return MRES_Supercede;
	}

	return MRES_Ignored;
}