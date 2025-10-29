#pragma newdecls required
#pragma semicolon 1

public Action Listener_Jointeam(int client, const char[] command, int args)
{
	//-2 - auto assign
	//1 - spectators
	//0 - def team (0 = spectators in teamplay)
	//3 - team2
	//2 - team1

	char sArg1[3];
    GetCmdArg(1, sArg1, sizeof(sArg1)); 

    int team = StringToInt(sArg1); //team I want to join
	int oldteam = GetEntProp(client, Prop_Data, "m_iTeamNum"); //my current team
	bool teamplay = GetConVarBool(mp_teamplay); //check if we are in tdm


	//Todo: make it smaller there possible, this code is massive 
	if(!teamplay)
	{
		if(team == -2 || (team != 0 && team != 1)) //IS auto assign OR is not auto assign but also not dm team
		{
			FakeClientCommandEx(client, "jointeam 0"); //this is the assigned value under these conditions
			return Plugin_Handled;
		}

		if (team == oldteam) //don't join if team I want to join my current team
		{
			return Plugin_Continue;
		}

		if(oldteam == 1 && team == 0) //if i'm spectator and i want to join dm team - check if my time runned out
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
				ChangeClientTeam(client, 0); //HACK! This thing doesn't work as expected, force team switch
				return Plugin_Continue;
			}
		}

		if(oldteam == 0 && team == 1) //if my current team is dm team and want to join spectator team - let me in no matter what and reset delay
		{
			g_fClientSpectatorJoinTeamDelay[client] = GetGameTime() + g_fClientSpectatorJoinTeamDelay[0];
			return Plugin_Continue;
		}

		return Plugin_Continue;
	}

	else
	{
		if(team == -2 || (team != 0 && team != 1 && team != 2 && team != 3)) //IS auto assign OR is not auto assign but also not a valid team num
		{
			int sci = GetTeamClientCount(2);
			int mar = GetTeamClientCount(3);

			//Repeats logic from CHL2MP_Player::PickDefaultSpawnTeam
			//(https://github.com/ValveSoftware/source-sdk-2013/blob/39f6dde8fbc238727c020d13b05ecadd31bda4c0/src/game/server/hl2mp/hl2mp_player.cpp#L280)
			if(sci == 0 || mar == 0)
			{
				if(oldteam == 2)
				{
					FakeClientCommandEx(client, "jointeam 3");
					return Plugin_Handled;
				}

				else if (oldteam == 3)
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
				if(sci > mar && oldteam != 2)
				{
					FakeClientCommandEx(client, "jointeam 2");
					return Plugin_Handled;
				}
				else if(sci < mar && oldteam != 3)
				{
					FakeClientCommandEx(client, "jointeam 3");
					return Plugin_Handled;
				}
				else
				{
					if(oldteam == 2)
					{
						FakeClientCommandEx(client, "jointeam 3");
						return Plugin_Handled;
					}

					else if (oldteam == 3)
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

		if (team == oldteam || ((team <= 1) && (oldteam <= 1)))  //for teamplay, of check if I'm trying to join spec under other num
		{
			return Plugin_Continue;
		}

		if((oldteam == 1 || oldteam == 0) && (team == 2 || team == 3)) //if i'm spectator and i want to join t3 or t3 - check if my time runned out
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

		if((oldteam == 2 || oldteam == 3) && (team == 1 || team == 0)) //if my current team is t2 or t3 and want to join spectator team - let me in no matter what and reset delay
		{
			g_fClientSpectatorJoinTeamDelay[client] = GetGameTime() + g_fClientSpectatorJoinTeamDelay[0];
			return Plugin_Continue;
		}

		return Plugin_Continue;
	}
}