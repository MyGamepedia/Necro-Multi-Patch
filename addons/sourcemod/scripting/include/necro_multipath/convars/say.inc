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
		//PrintToChat(iClient, "%t", "#Necro_ShedInfo", COL_GREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_RtvInfo", COL_GREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_NominateMapInfo", COL_GREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_Top10Info", COL_GREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_StatsmeInfo", COL_GREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_SessionInfo", COL_GREEN, COL_NORMAL);
		return Plugin_Handled;
	}

	//we need time zone setup
	/*
	if (StrEqual(iszArgument, "!sched"))
	{
		PrintToChat(iClient, "%t", "#Necro_ShedGamemodeForm");
		PrintToChat(iClient, "%t", "#Necro_ShedGamemodeMain");
		return Plugin_Handled;
	}
	*/

	//it's done by a say command so i can debug
	if (StrEqual(iszArgument, "!spawnhelp"))
	{
		//get hostname for the message
		char iszHostname[256];
		hostname.GetString(iszHostname, sizeof(iszHostname));
		
		//define current gamemode name for the message
		char iszGameMode[64];
		if (g_ngmCurrentGameMode == MODE_DEATHMATCH)
			Format(iszGameMode, sizeof(iszGameMode), "%t", "#Necro_Deathmatch");
		else if (g_ngmCurrentGameMode == MODE_TEAMDEATHMATCH)
			Format(iszGameMode, sizeof(iszGameMode), "%t", "#Necro_Team_Deathmatch");
		else if (g_ngmCurrentGameMode == MODE_GUNGAME)
			Format(iszGameMode, sizeof(iszGameMode), "%t", "#Necro_GunGame");
		else if (g_ngmCurrentGameMode == MODE_TEAMGUNGAME)
			Format(iszGameMode, sizeof(iszGameMode), "%t", "#Necro_Team_GunGame");
		else
			Format(iszGameMode, sizeof(iszGameMode), "%t", "#Necro_NoGamemode");

		//print the message
		PrintToChat(iClient, "%t", "#Necro_WhiteLine", COL_LIGHTGREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_WelcomeServer", COL_NORMAL, COL_SEMIRED, iszHostname, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_HelpInfo", COL_NORMAL, COL_GREEN, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_TelegramInfo", COL_NORMAL, COL_LIGHTBLUE, COL_NORMAL);
		//PrintToChat(iClient, "%t", "#Necro_FeedbackInfo", COL_NORMAL, COL_LIGHTBLUE, COL_NORMAL);
		PrintToChat(iClient, "%t", "#Necro_SponsorInfo", COL_NORMAL, COL_DARKBLUE, COL_NORMAL);

		//alright, alright, guess who did the time code ? yep, not me 
		char szTime[10];

		// 1. get time HMS
		char buffer[4];
		FormatTime(buffer, sizeof(buffer), "%H");
		int h = StringToInt(buffer);
		FormatTime(buffer, sizeof(buffer), "%M");
		int m = StringToInt(buffer);
		FormatTime(buffer, sizeof(buffer), "%S");
		int s = StringToInt(buffer);

		// 2. Check, in which array and interval we are in
		int nextHour = -1;

		//  Check each mode by one
		switch (g_ngmCurrentGameMode)
		{
			case MODE_DEATHMATCH:
				nextHour = GetEndHour(g_iShedTimeDeathmatch, h);
			case MODE_TEAMDEATHMATCH:
				nextHour = GetEndHour(g_iShedTimeTeamDeathmatch, h);
			case MODE_GUNGAME:
				nextHour = GetEndHour(g_iShedTimeGunGame, h);
			case MODE_TEAMGUNGAME:
				nextHour = GetEndHour(g_iShedTimeTeamGunGame, h);
		}

		// 3. If found active mode, count rest
		if (nextHour != -1)
		{
			// Transl all in seconds
			// End of the interval is nextHour:00:00
			int totalSecondsLeft = (nextHour * 3600) - (h * 3600 + m * 60 + s);

			int resH = totalSecondsLeft / 3600;
			int resM = (totalSecondsLeft % 3600) / 60;
			int resS = totalSecondsLeft % 60;

			Format(szTime, sizeof(szTime), "%02d:%02d:%02d", resH, resM, resS);
		}

		PrintToChat(iClient, "%t", "#Necro_GameModeInfo", COL_NORMAL, COL_YELLOWGREEN, iszGameMode, COL_NORMAL, szTime);

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

//time code was done by Gemeni (AI), AI love things like this for what ever reason
int GetEndHour(int schedule[3][2], int currentHour)
{
    for (int i = 0; i < 3; i++)
    {
        if (currentHour >= schedule[i][0] && currentHour < schedule[i][1])
        {
            return schedule[i][1]; // return max time of this interval
        }
    }
    return -1;
}