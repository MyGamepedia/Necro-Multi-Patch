#pragma newdecls required
#pragma semicolon 1

//Purpose: fix stuff before restore world
public MRESReturn Hook_RestoreWorld(DHookReturn hReturn)
{
    if (GetConVarBool(g_ConvarNecroAllowRestoreWorld) == false)
    {
		DHookSetReturn(hReturn, 0);
		return MRES_Supercede;
    }

	return MRES_Ignored;
}

//Purpose: let us play with everyone if we had team spawn delay 
public MRESReturn Hook_RestoreWorldPost(DHookReturn hReturn)
{
    if (GetConVarBool(g_ConvarNecroAllowRestoreWorld) == false)
    {
        return MRES_Ignored;
    }

    for (int i = 1; i <= MaxClients; i++)
    {
        g_fClientSpectatorJoinTeamDelay[i] = 0.0;
    }

	return MRES_Ignored;
}