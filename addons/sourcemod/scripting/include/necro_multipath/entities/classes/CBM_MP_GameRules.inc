#pragma newdecls required
#pragma semicolon 1

#if defined GAMEPATCH_BLOCK_RESTOREWORLD
public MRESReturn Hook_RestoreWorld(DHookReturn hReturn)
{
    if (GetConVarBool(g_ConvarNecroBlockRestoreWorld))
    {
		DHookSetReturn(hReturn, 0);
		return MRES_Supercede;
    }

	return MRES_Ignored;
}

public MRESReturn Hook_RestoreWorldPost(DHookReturn hReturn)
{
    if (GetConVarBool(g_ConvarNecroBlockRestoreWorld))
    {
        return MRES_Ignored;
    }

    for (int i = 1; i <= MaxClients; i++)
    {
        g_fClientSpectatorJoinTeamDelay[i] = 0.0;

        if(CBlackMesaPlayer(i).IsValid() && !CBlackMesaPlayer(i).IsBot())
            CBlackMesaPlayer(i).SendCommand("r_cleardecals"); //clear decals on restoreworld, so the world looks clean
    }

	return MRES_Ignored;
}
#endif

#if defined GAMEPATCH_BLOCK_RESPAWNPLAYERS
public MRESReturn Hook_RespawnPlayers(DHookReturn hReturn)
{
	if (GetConVarBool(g_ConvarNecroBlockRestoreWorldRespawnPlayers))
	{
		DHookSetReturn(hReturn, 0);
		return MRES_Supercede;
	}
	return MRES_Ignored;
}
#endif
