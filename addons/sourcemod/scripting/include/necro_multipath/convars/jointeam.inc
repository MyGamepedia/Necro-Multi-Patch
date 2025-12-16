#pragma newdecls required
#pragma semicolon 1

//Purpose: Ovverides "jointeam" command entirely to:
// - prevent auto-assign in dm (sort of instant teleport)
// - add delay when switching from spectator to other team
//TODO: Add messages with translations support
public Action Listener_Jointeam(int client, const char[] command, int args)
{
	//-2 - auto assign
	//1 - spectators
	//0 - def team (0 = spectators in teamplay)
	//2 - team1
	//3 - team2

	char sArg1[3];
    GetCmdArg(1, sArg1, sizeof(sArg1)); 

    int team = StringToInt(sArg1); //team I want to join
	int oldteam = GetEntProp(client, Prop_Data, "m_iTeamNum"); //my current team

	//Todo: make it smaller there possible, this code is massive 
	if(!CMultiplayRules.IsTeamplay())
	{
		if(team == TEAM_ANY || (team != TEAM_UNASSIGNED && team != TEAM_SPECTATOR)) //IS auto assign OR is not auto assign but also not dm team
		{
			FakeClientCommandEx(client, "jointeam 0"); //this is the assigned value under these conditions
			return Plugin_Handled;
		}

		if (team == oldteam) //don't join if team I want to join my current team
		{
			return Plugin_Continue;
		}

		if(oldteam == TEAM_SPECTATOR && team == TEAM_UNASSIGNED) //if i'm spectator and i want to join dm team - check if my time runned out
		{
			if (GetGameTime() < g_fClientSpectatorJoinTeamDelay[client]) //if IS NOT runned out - don't let me and tell me when the time runs out
			{
				char szClientName[64];
				GetClientName(client, szClientName, sizeof(szClientName));

				//Todo: Remove "*DEAD*" if we fix it for spectators (sourcecoop did)
				PrintToChat(client, "*DEAD* %s : %t", szClientName, "#SpectatorDelay", g_fClientSpectatorJoinTeamDelay[client] - GetGameTime());
				return Plugin_Handled;
			}
			else
			{
				ChangeClientTeam(client, 0); //HACK! This thing doesn't work as expected, force team switch
				return Plugin_Continue;
			}
		}

		if(oldteam == TEAM_UNASSIGNED && team == TEAM_SPECTATOR) //if my current team is dm team and want to join spectator team - let me in no matter what and reset delay
		{
			g_fClientSpectatorJoinTeamDelay[client] = GetGameTime() + g_fClientSpectatorJoinTeamDelay[0];
			return Plugin_Continue;
		}

		return Plugin_Continue;
	}

	else
	{
		//IS auto assign OR is not auto assign but also not a valid team num
		if(team == TEAM_ANY || (team != TEAM_UNASSIGNED && team != TEAM_SPECTATOR && team != TEAM_ONE && team != TEAM_TWO))
		{
			int sci = GetTeamClientCount(2);
			int mar = GetTeamClientCount(3);

			//Mainly repeats logic from CHL2MP_Player::PickDefaultSpawnTeam
			//(https://github.com/ValveSoftware/source-sdk-2013/blob/39f6dde8fbc238727c020d13b05ecadd31bda4c0/src/game/server/hl2mp/hl2mp_player.cpp#L280)
			if(sci == 0 || mar == 0)
			{
				if(oldteam == TEAM_ONE)
				{
					FakeClientCommandEx(client, "jointeam 3");
					return Plugin_Handled;
				}

				else if (oldteam == TEAM_TWO)
				{
					FakeClientCommandEx(client, "jointeam 2");
					return Plugin_Handled;
				}

				else
				{
					FakeClientCommandEx(client, "jointeam %d", GetRandomInt(2, 3));
					return Plugin_Handled;
				}
			}

			else
			{
				if(sci > mar && oldteam != TEAM_ONE)
				{
					FakeClientCommandEx(client, "jointeam 2");
					return Plugin_Handled;
				}
				else if(sci < mar && oldteam != TEAM_TWO)
				{
					FakeClientCommandEx(client, "jointeam 3");
					return Plugin_Handled;
				}
				else
				{
					if(oldteam == TEAM_ONE)
					{
						FakeClientCommandEx(client, "jointeam 3");
						return Plugin_Handled;
					}

					else if (oldteam == TEAM_TWO)
					{
						FakeClientCommandEx(client, "jointeam 2");
						return Plugin_Handled;
					}

					else
					{
						FakeClientCommandEx(client, "jointeam %d", GetRandomInt(2, 3));
						return Plugin_Handled;
					}
				}
			}
		}

		if (team == oldteam || ((team <= TEAM_SPECTATOR) && (oldteam <= TEAM_SPECTATOR)))  //for teamplay, of check if I'm trying to join spec under other num
		{
			return Plugin_Continue;
		}

		//if i'm spectator and i want to join sci or mar - check if my time runned out
		if((oldteam == TEAM_SPECTATOR || oldteam == TEAM_UNASSIGNED) && (team == TEAM_ONE || team == TEAM_TWO))
		{
			if (GetGameTime() < g_fClientSpectatorJoinTeamDelay[client]) //if IS NOT runned out - don't let me and tell me when the time runs out
			{
				char szClientName[64];
				GetClientName(client, szClientName, sizeof(szClientName));

				//Todo: Remove "*DEAD*" if we fix it for spectators (sourcecoop did)
				PrintToChat(client, "*DEAD* %s : I can join after %.2f seconds!", szClientName, g_fClientSpectatorJoinTeamDelay[client] - GetGameTime());
				return Plugin_Handled;
			}

			else
			{
				ChangeClientTeam(client, team); //HACK! This thing doesn't work as expected, force team switch
				return Plugin_Continue;
			}
		}

		//if my current team is t2 or t3 and want to join spectator team - let me in no matter what and reset delay
		if((oldteam == TEAM_ONE || oldteam == TEAM_TWO) && (team == TEAM_SPECTATOR || team == TEAM_UNASSIGNED))
		{
			g_fClientSpectatorJoinTeamDelay[client] = GetGameTime() + g_fClientSpectatorJoinTeamDelay[0];
			return Plugin_Continue;
		}

		return Plugin_Continue;
	}
}