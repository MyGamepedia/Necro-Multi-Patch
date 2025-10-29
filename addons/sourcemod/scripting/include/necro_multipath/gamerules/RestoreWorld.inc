#pragma newdecls required
#pragma semicolon 1

//Purpose: let us player with everyone if we have team spawn delay 
public MRESReturn Hook_RestoreWorldPost(int _this)
{
    for (int i = 1; i <= MaxClients; i++)
    {
        g_fClientSpectatorJoinTeamDelay[i] = 0.0;
    }

	return MRES_Ignored;
}