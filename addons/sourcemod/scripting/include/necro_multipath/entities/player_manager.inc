#pragma newdecls required
#pragma semicolon 1


//TODO: fix me!!!
public MRESReturn Hook_PlayerResourceUpdatePlayerData(int _this, DHookReturn hReturn)
{
    CPlayerResource pPlayerResource = CPlayerResource(_this);

    CBasePlayer pPlayer;

    for (int i = 1; i <= MaxClients; i++)
    {
        pPlayer = CBasePlayer(i);

        if (pPlayer.IsPlayer())
        {
            pPlayerResource.SetScore(pPlayer, pPlayer.GetScore());
            pPlayerResource.SetDeaths(pPlayer, pPlayer.GetDeaths());
            pPlayerResource.SetConnected(pPlayer, 1);

            int iPlayerTeam = pPlayer.GetTeam();

            if(iPlayerTeam == TEAM_SPECTATOR)
            {
                iPlayerTeam = TEAM_UNASSIGNED;
            }

            pPlayerResource.SetTeam(pPlayer, iPlayerTeam);
            pPlayerResource.SetAlive(pPlayer, pPlayer.IsAlive());
            pPlayerResource.SetHealth(pPlayer, pPlayer.GetHealth());
            pPlayerResource.SetValidPlayer(pPlayer, 1);
            pPlayerResource.SetPing(pPlayer, -1);
            pPlayerResource.SetAccountId(pPlayer, GetSteamAccountID(pPlayer.entindex));
            //SetEntProp(pPlayerResource.entindex, Prop_Send, "m_iUserID", 1, _, pPlayer.entindex);

        }
        else
        {
            pPlayerResource.SetConnected(pPlayer, 0);
            pPlayerResource.SetAccountId(pPlayer, 0);
            pPlayerResource.SetValidPlayer(pPlayer, 0);
           // SetEntProp(pPlayerResource.entindex, Prop_Send, "m_iUserID", 0, _, pPlayer.entindex);
        }
    }
    
	return MRES_Supercede;
}