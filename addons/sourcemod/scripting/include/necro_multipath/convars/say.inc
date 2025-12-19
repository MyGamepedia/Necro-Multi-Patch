#pragma newdecls required
#pragma semicolon 1
 
 public Action Listener_Say(int iClient, const char[] iszCommand, int iArgs)
 {
	char iszArgument[256];
	GetCmdArg(iArgs, iszArgument, sizeof(iszArgument));

	if(StrEqual(iszArgument, "!gungame") && g_iNecroGunGameCurrentState != 2)
	{
		if(GetTime() < g_iGunGameNextVoteTime)
		{
			CPrintToChat(iClient, "%t", "#GunGame_VoteDelay", g_iGunGameNextVoteTime - GetTime());
			return Plugin_Handled;
		}

		CBlackMesaPlayer pPlayer = CBlackMesaPlayer(iClient);

		char iszPlayerName[MAX_NAME_LENGTH];
    	pPlayer.GetName(iszPlayerName, sizeof(iszPlayerName));

		PrintToChatAll("%t", "#GunGame_VoteStart", iszPlayerName);

		g_iGunGameNextVoteTime = GetTime() + 60;

		GunGameStartVote();

		return Plugin_Handled;
	}

	return Plugin_Continue;
 }