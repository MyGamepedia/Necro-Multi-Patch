#pragma newdecls required
#pragma semicolon 1
 
 public Action Listener_Say(int iClient, const char[] iszCommand, int iArgs)
 {
	
	char iszArgument[256];
	GetCmdArg(iArgs, iszArgument, sizeof(iszArgument));

	if (StrEqual(iszArgument, "!helpme"))
	{
		PrintToChat(iClient, "%t", "#Necro_HelpMainInfo");
		PrintToChat(iClient, "%t", "#Necro_ModelsInfo", COL_GREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_RtvInfo", COL_GREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_NominateMapInfo", COL_GREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_Top10Info", COL_GREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_StatsmeInfo", COL_GREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_SessionInfo", COL_GREEN, COL_NORMAL);
		return Plugin_Handled;
	}

	//it's done by a say command so i can debug
	if (StrEqual(iszArgument, "!spawnhelp"))
	{
		char iszHostname[256];
		hostname.GetString(iszHostname, sizeof(iszHostname));

		PrintToChat(iClient, "%t", "#Necro_WhiteLine", COL_LIGHTGREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_WelcomeServer", COL_NORMAL, COL_SEMIRED, iszHostname, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_HelpInfo", COL_NORMAL, COL_GREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_TelegramInfo", COL_NORMAL, COL_LIGHTBLUE, COL_NORMAL);
		//PrintToChat(iClient, "%t", "#Necro_FeedbackInfo", COL_NORMAL, COL_LIGHTBLUE, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_SponsorInfo", COL_NORMAL, COL_DARKBLUE, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_WhiteLine", COL_LIGHTGREEN, COL_NORMAL);

		return Plugin_Handled;
	}

	/*
	such freedom didn't worked well for us, as our veterans preferred the usual dm
	instead, we're using "gamemode pools" based on OS time schedules, it takes away the possibility of choice,
	but at the same time it doesn't force anybody to play something as they can just join later on diff time if they want to play another mode, 
	and it also allows us to have more players in the same mode at the same time, which is good for the overall health of the servers, as we keep diversity
	*/
	/*
	if(StrEqual(iszArgument, "!gungame") && g_iNecroGunGameCurrentState != 2)
	{
		if(GetTime() < g_iGunGameNextVoteTime)
		{
			PrintToChat(iClient, "%t", "#GunGame_VoteDelay", g_iGunGameNextVoteTime - GetTime());
			return Plugin_Handled;
		}

		CBlackMesaPlayer pPlayer = CBlackMesaPlayer(iClient);

		char iszPlayerName[MAX_NAME_LENGTH];
    	pPlayer.GetName(iszPlayerName, sizeof(iszPlayerName));

		PrintToChatAll("%t", "#GunGame_VoteStart", iszPlayerName);

		g_iGunGameNextVoteTime = GetTime() + 60;

		CreateTimer(4.00, GunGameStartVote, pPlayer.entindex, TIMER_FLAG_NO_MAPCHANGE);
		return Plugin_Handled;
	}
	*/

	return Plugin_Continue;
 }