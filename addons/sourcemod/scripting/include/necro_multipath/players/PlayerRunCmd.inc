#pragma newdecls required
#pragma semicolon 1

//Purpose: Various fixes when player is sending commands
public Action OnPlayerRunCmd(int iClient, int &iButtons, int &iImpulse, float fVel[3], float fAngles[3], int &iWeapon, 
							int& subtype, int& cmdnum, int& tickcount, int& seed, int mouse[2])
{
	if (IsFakeClient(iClient))
		return Plugin_Continue;
	
	//Hide broken specmenu for spectators
	int observermode = GetEntProp(iClient, Prop_Data, "m_iObserverMode"); //get observer mode
	
	if (mouse[0] || mouse[1])
	{
		g_bPostTeamSelect[iClient] = true;
	}
	
	if (observermode > 1) //hide panel if we are spectator
	{
		if (g_bPostTeamSelect[iClient] && tickcount % 10 == 0)
		{
			ShowVGUIPanel(iClient, "specmenu", _, false);
		}
	}
	
	//Client don't use weapon when used custom classname to load custom script, force use with client command by checking classname
	char classname[64];
	GetEntityClassname(iWeapon, classname, sizeof(classname));
	
	if(!StrEqual(classname, "worldspawn")) //send use command if we want to use a weapon
	{
		char command[64];
		Format(command, sizeof(command), "use %s", classname);
		FakeClientCommand(iClient, command);
		
		#if defined DEBUG
		PrintToServer("OnPlayerRunCmd: Client (%d) used %s", iClient, classname);
		#endif
	}
	
	return Plugin_Continue;
}

//Purpose: Allow fast respawn when player presses the buttons
public void OnPlayerRunCmdPost(int client, int buttons, int impulse, const float vel[3], const float angles[3], int weapon, int subtype, int cmdnum, int tickcount, int seed, const int mouse[2])
{
	//Respawn player if: allowed by convar, player pressed any of the buttons, player is dead, player is not on spectator team and the delay time is passed
    if(GetConVarBool(g_ConvarNecroAllowFastRespawn) && buttons & (IN_ATTACK|IN_JUMP|IN_DUCK|IN_FORWARD|IN_BACK|IN_ATTACK2) 
	   && !IsPlayerAlive(client) && GetClientTeam(client) != 1 && GetGameTime() >= g_fClientFastRespawnDelay[client])
	{
		#if defined DEBUG
		PrintToServer("OnPlayerRunCmdPost: Client (%d) respawned without waiting.", client);
		PrintToServer("OnPlayerRunCmdPost: GetGameTime() == %d, g_fClientFastRespawnDelay[%d] == %f", GetGameTime(), client, g_fClientFastRespawnDelay[client]);
		#endif
		
        SetEntPropFloat(client, Prop_Send, "m_flDeathTime", 0.0); //set 0.0 time for death time, this is a proper way to respawn dead player when we need
	}
}