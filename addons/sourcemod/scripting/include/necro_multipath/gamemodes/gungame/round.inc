#pragma newdecls required
#pragma semicolon 1

//Victory logic, unhook stuff, reset values, trigger victory by creating entity and sending inputs for it (not  perfect, but easy)
public Action WonRound(Handle timer, CBlackMesaPlayer pPlayer)
{
    g_iNecroGunGameCurrentState = 2; //tell the ui text and giving items func that the mode is finished

    //unhook events, so starting new level will not add extra hooks 
    UnhookEvent("player_death", GunGameEventDeath, EventHookMode_Pre);
	UnhookEvent("player_spawn", GunGameEventSpawn, EventHookMode_Pre);	
	UnhookEvent("player_team", GunGamePlayerTeamChanged, EventHookMode_Post);
	UnhookEvent("round_start", GunGameOnRoundStart, EventHookMode_Post);
    UnhookEvent("round_end", GunGameOnRoundIntermission, EventHookMode_Post);


    //get winner name
    char iszWinnerName[MAX_NAME_LENGTH];
    pPlayer.GetName(iszWinnerName, sizeof(iszWinnerName));

    int iWinnerTeam = GetClientTeam(pPlayer.entindex); //team to define as winner

    if(!CMultiplayRules.IsTeamplay()) //print name and level for solo version
    {
        PrintToChatAll("%t", "#GunGame_Win", iszWinnerName, g_iNecroGunGameClientLevel[pPlayer.entindex] -1);
    }
    else //print team name for team versin
    {
        if(iWinnerTeam == TEAM_ONE) //mar
        {
            PrintToChatAll("%t", "#GunGame_Marines");
        }
        else //sci
        {
            PrintToChatAll("%t", "#GunGame_Scientists");
        }
    }

    //reset values to def for everyone
    for(int i = 1; i <= MaxClients; i++)
    {  
        g_iNecroGunGameClientLevel[i] = 0;
        g_iNecroGunGameClientKills[i] = 0;
	}

    //create entity to trigger victory
    CBaseEntity roundwin = CBaseEntity.Create("game_round_win");

	if (!roundwin.IsValid())
		return Plugin_Continue;

	roundwin.KeyValue("switch_teams", "1"); //TODO: should be optional ?
	roundwin.Spawn();

	SetVariantInt(iWinnerTeam); //parametr value for SetTeam

	roundwin.AcceptInput("SetTeam");
	roundwin.AcceptInput("RoundWin"); //will trigger team win in teamplay or generic round end for dm
	roundwin.Kill();

    return Plugin_Continue;
}