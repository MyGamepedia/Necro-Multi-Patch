#pragma newdecls required
#pragma semicolon 1

//Purpose: fix stuff before restore world
public MRESReturn Hook_RestoreWorld(int _this)
{
    g_iActiveScreenOverlayEntity[0] = 0; //mark that no global overlay entity is active
    //stop overlay for active env_screenoverlays
    for (int i = 1; i <= MaxClients; i++)
    {
        if(g_iActiveScreenOverlayEntity[i] != 0)
        {
            AcceptEntityInput(g_iActiveScreenOverlayEntity[i], "StopOverlaysForEveryone");
        }
    }

	return MRES_Ignored;
}

//Purpose: let us player with everyone if we have team spawn delay 
public MRESReturn Hook_RestoreWorldPost(int _this)
{
    for (int i = 1; i <= MaxClients; i++)
    {
        g_fClientSpectatorJoinTeamDelay[i] = 0.0;
    }

	return MRES_Ignored;
}