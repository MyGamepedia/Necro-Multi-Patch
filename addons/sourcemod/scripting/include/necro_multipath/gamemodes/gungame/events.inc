#pragma newdecls required
#pragma semicolon 1

//WORKAROUND! Fixes "draw" when timer is finished
public void GunGameOnRoundIntermission(Event event, const char[] theGoatName, bool dontBroadcast)
{
	if (!CMultiplayRules.IsTeamplay()) //it works fine with no teamplay, ignore this event in deathmatch
		return;
	
	int topLevel, topClient, secondClient = -1; //get highest current level and first 2 clients

	for (int i = 1; i <= MaxClients; i++)
	{
		if (!IsClientInGame(i) || !IsPlayerAlive(i) || (GetClientTeam(i) == 0 || GetClientTeam(i) == 1)) //not valid or spec? skip
				continue;

		if (g_iNecroGunGameClientLevel[i] > topLevel) //if the level is higher than what we find during past iterations - store it
		{
			topLevel = g_iNecroGunGameClientLevel[i]; //store level
			topClient = i; //store level's owner
			secondClient = -1; //don't set on this state
		}
		else if (g_iNecroGunGameClientLevel[i] == topLevel && topClient != -1)
		{
			secondClient = i; //set second client num if client's level the same as highest current level and not def val for first client
		}
	}

	if (topClient == -1) //no clients? don't compare anything
		return;

    //get team nums for both clients to compare
	int iClienTopTeam = GetClientTeam(topClient);
	int iClientSecondTeam = GetClientTeam(secondClient);

	//if two plrs with the same level and they have two dif teams - it's a draw
	if (secondClient != -1 &&
		iClienTopTeam != iClientSecondTeam &&
		(iClienTopTeam == TEAM_ONE || iClienTopTeam == TEAM_TWO) &&
		(iClientSecondTeam == TEAM_ONE || iClientSecondTeam == TEAM_TWO))
	{
		PrintToChatAll("%t", "#GunGame_Draw");
		return;
	}
    else //else trigger victory for one of the clients
    {
        CreateTimer(0.0, WonRound, topClient, TIMER_FLAG_NO_MAPCHANGE);
    }
}

//After round start, disable canisters and remove items again, level down everyone and reset all kill counters
public Action GunGameOnRoundStart(Handle event, const char[] name, bool dontBroadcast)
{
	GunGameDisableCanisterDrops();
	GunGameRemoveItems();
	
    for (int i = 1; i <= MaxClients; i++)
    {
        g_iNecroGunGameClientLevel[i] = 0;
        g_iNecroGunGameClientKills[i] = 0;
    }
	
	return Plugin_Continue;
}

//Solves if we should reset progress after joining a team or not
public void GunGamePlayerTeamChanged(Event event, const char[] name, bool dontBroadcast)
{
    if(!CMultiplayRules.IsTeamplay()) //don't do anything if no teamplay, not needed
        return;

    int client = GetClientOfUserId(event.GetInt("userid")); //get client
//    if (IsFakeClient(client))
//		return;

    int oldTeam = event.GetInt("oldteam"); //old team
    int newTeam = event.GetInt("team"); //team client want to join

    if ((newTeam == TEAM_SPECTATOR || newTeam == TEAM_UNASSIGNED) && (oldTeam == TEAM_ONE || oldTeam == TEAM_TWO))
    {
        g_iNecroGunGameLastTeam[client] = oldTeam;
        return;
    }
	
    if ((oldTeam == TEAM_SPECTATOR || oldTeam == TEAM_UNASSIGNED) && (newTeam == g_iNecroGunGameLastTeam[client]))
    {
        return;
    }

    //simple team switch
    bool bFromOldToNewTeam = (oldTeam == TEAM_ONE && newTeam == TEAM_TWO) || (oldTeam == TEAM_TWO && newTeam == TEAM_ONE);

    //team switch by going from old team to spec and going from spec to new team
    bool bFromOldToNewTeamViaSpec = ((oldTeam == TEAM_SPECTATOR || oldTeam == TEAM_UNASSIGNED) && 
                                    (newTeam == TEAM_ONE || newTeam == TEAM_TWO) && newTeam != g_iNecroGunGameLastTeam[client]);

    if (bFromOldToNewTeam || bFromOldToNewTeamViaSpec) //if we joined a new team - reset old progress
    {
        g_iNecroGunGameClientLevel[client] = 0;
        g_iNecroGunGameClientKills[client] = 0;

		SetEntProp(client, Prop_Data, "m_iFrags", 0);
		SetEntProp(client, Prop_Data, "m_iDeaths", 0);
    }
}

//Give gamemode items for spawned player
public void GunGameEventSpawn(Handle event, const char[] name, bool dontBroadcast)
{
    CBlackMesaPlayer pPlayer = CBlackMesaPlayer(GetClientOfUserId(GetEventInt(event, "userid")));

    if(!IsValidEntity(pPlayer.entindex))
        return;
    
    CreateTimer(0.4, GunGameWeapons, pPlayer.entindex, TIMER_FLAG_NO_MAPCHANGE);
}

//A player killed other player event
public Action GunGameEventDeath(Handle event, const char[] name, bool dontBroadcast)
{
    CBlackMesaPlayer pPlayerVictim = CBlackMesaPlayer(GetClientOfUserId(GetEventInt(event, "userid")));     //victim
    CBlackMesaPlayer pPlayerAttacker = CBlackMesaPlayer(GetClientOfUserId(GetEventInt(event, "attacker"))); //attacker/killer

    //get team for both to compare
    int iTeamVictim = GetClientTeam(pPlayerVictim.entindex);
    int iTeamAttacker = GetClientTeam(pPlayerAttacker.entindex);

    //if the player killed self - ignore
    if(pPlayerAttacker.entindex == pPlayerVictim.entindex)
    {
        return Plugin_Continue;
    }

    //if the player killed ally in tdm - apply neg score
    if (CMultiplayRules.IsTeamplay() && iTeamVictim == iTeamAttacker)
    {
        GunGameApplyGunModeNegScoreToClient(pPlayerAttacker.entindex);
        return Plugin_Continue;
    }
    
    char weaponName[MAX_CLASSNAME];
    GetEventString(event, "weapon", weaponName, sizeof(weaponName)); //get the weapon name that performed the kill

	//WORKAROUND: for some reason weapon_gluon is not registered with EventDeath so the field is empty,
    //we can workaround this by thinking that attacker killed with gloun if wpn name from the event is empty BUT the attacker's active weapon is gluon
	if (weaponName[0] == '\0')
	{
        char weaponHeld[MAX_CLASSNAME]; //store active weapon's classname here

        CBaseCombatWeapon pWeaponActive = CBaseCombatWeapon(GetEntPropEnt(pPlayerAttacker.entindex, Prop_Data, "m_hActiveWeapon")); //get attackers current weapon
        pWeaponActive.GetClassname(weaponHeld, sizeof(weaponHeld)); //get classname for active weapon
        
        //guess the active weapon is gluon if attacker's active weapon is gluon
        if(StrEqual(weaponHeld, "weapon_gluon", false))
        {
            strcopy(weaponName, sizeof(weaponName), "gluon"); //set gluon for the func that sets full name
        }
	}
	
	GunGameGetFullWeaponName(weaponName, sizeof(weaponName)); //get full name of the weapon
    
    char iszVictimName[MAX_NAME_LENGTH], iszAttackerName[MAX_NAME_LENGTH], iszPunishingWeaponName[MAX_CLASSNAME];

    //get players name to write in the chat
    pPlayerVictim.GetName(iszVictimName, sizeof(iszVictimName));
    pPlayerAttacker.GetName(iszAttackerName, sizeof(iszAttackerName));

    //get punishing weapon name to define if we need to punish victim
    g_ConvarNecroGunGamePunishingWeaponName.GetString(iszPunishingWeaponName, sizeof(iszPunishingWeaponName));

    //if is punishing weapon - do punishing logic if victim's level > 0
    if (StrEqual(weaponName, iszPunishingWeaponName, false) && g_iNecroGunGameClientLevel[pPlayerVictim.entindex] > 0)
    {
        int iVictimLevel = g_iNecroGunGameClientLevel[pPlayerVictim.entindex];
        int iVictimKills = g_iNecroGunGameClientKills[pPlayerVictim.entindex];

        //if victim has more than 50% of needed kills - set kills to 50% of current kills
        if (iVictimKills > GetConVarInt(g_ConvarNecroGunGameStateKills[iVictimLevel]) / 2)
        {
            g_iNecroGunGameClientKills[pPlayerVictim.entindex] = iVictimKills / 2;
        }
        else //else - set 50% of kills needed to level up from prev level and process humiliation
        { 
            g_iNecroGunGameClientKills[pPlayerVictim.entindex] = GetConVarInt(g_ConvarNecroGunGameStateKills[iVictimLevel - 1]) / 2;
            GunGameProcessHumiliation(pPlayerVictim, iszVictimName, iszAttackerName);
        }

        //if punishing weapon is also used for the current level - also process normal kill
        if (StrEqual(weaponName, iszPunishingWeaponName, false)) 
        {
            GunGameLevelUpActions(g_iNecroGunGameClientLevel[pPlayerAttacker.entindex], pPlayerAttacker, weaponName, iszAttackerName);
        }
    }

    else //if is not punishing weapon - just process normal kill
    {
        GunGameLevelUpActions(g_iNecroGunGameClientLevel[pPlayerAttacker.entindex], pPlayerAttacker, weaponName, iszAttackerName);
    }

    return Plugin_Continue;
}