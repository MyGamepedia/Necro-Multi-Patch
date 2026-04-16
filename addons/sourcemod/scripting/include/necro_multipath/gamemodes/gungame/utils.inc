#pragma newdecls required
#pragma semicolon 1

//Let everyone in the lobby know if gungame will be disabled on the next map change
public void OnGunGameStateChanged(ConVar convar, const char[] oldValue, const char[] newValue)
{
    if (StringToInt(newValue) != 0)
    {
        PrintToServer("Gun Game is enabled. Start a new map or restart current.");
    }
	else
	{
		PrintToServer("Gun Game is disabled. Start a new map or restart current.");
	}
}

//+50 to current clients score
public void GunGameApplyGunModeScoreToClient(int client)
{
	CBlackMesaPlayer pPlayer = CBlackMesaPlayer(client);

	pPlayer.ModifyScore(100);
}

//-50 to current clients score, also takes 1 kill if the current value is not 0
public void GunGameApplyGunModeNegScoreToClient(int client)
{
	if(GetConVarBool(g_ConvarNecroGunGameTeammateKarma))
	{
		CBlackMesaPlayer pPlayer = CBlackMesaPlayer(client);

        //make sure it's not below 0
        if(g_iNecroGunGameClientKills[client] != 0)
            g_iNecroGunGameClientKills[client]--;

        //don't for starting clients for instant
        if(pPlayer.GetScore() > 0)
		    pPlayer.ModifyScore(-100);
	}
}

//Disable canister drops in this mode by creating entity and sending inputs for it (not  perfect, but easy), also sets no score limit
public void GunGameDisableCanisterDrops()
{
	CBaseEntity pGameGamerules = CBaseEntity.Create("game_mp_gamerules");
	if (pGameGamerules.IsValid())
	{
		pGameGamerules.Spawn();

		SetVariantString("0"); //parametr value for SetPointsPerFrag

		pGameGamerules.AcceptInputStr("SetPointsPerFrag");
		pGameGamerules.AcceptInputStr("DisableCanisterDrops");
		pGameGamerules.Kill();
	}
}

//Displays ui for the mode
public Action GunGameDisplayHud(Handle timer, any client)
{
    for (int i = 1; i <= MaxClients; i++)  
    {
        //we don't display main hud in intermission, don't fir gungame as well
        bool bNoInter = (CBM_MP_GameRules.GetCurrentState() != STATE_ROUND_INTERMISSION_INDEX && CBM_MP_GameRules.GetCurrentState() != STATE_INTERMISSION_INDEX) ? true : false;

        if (IsClientConnected(i) && IsClientInGame(i) && IsPlayerAlive(i) && g_iNecroGunGameCurrentState == 1 && bNoInter)  
        {  
            int iLevel = g_iNecroGunGameClientLevel[i];  
            int iKillsForNextLevel = g_ConvarNecroGunGameStateKills[iLevel].IntValue;

            int col1[4] = {255, 255, 255, 255}; //col

            //pick col by team (green for marines)
            if (CBasePlayer(i).GetTeam() != TEAM_ONE)
            {
                col1 = {255, 194, 6, 255};
            }
            else
                col1 = {0, 255, 0, 255};

            float flPosX = GetConVarFloat(g_ConvarNecroGunGameTextPosX);
            float flPosY = GetConVarFloat(g_ConvarNecroGunGameTextPosY);
  
            SetHudTextParams(flPosX, flPosY, GUNGAME_HUDTICK + 0.1, col1[0], col1[1], col1[2], col1[3], 0, 6.0, 0.1, 0.2);  
           
            ShowHudText(i, 1, "%t", "#GunGame_HUD", iLevel, g_ConvarNecroGunGameFinalLevelNum.IntValue, g_iNecroGunGameClientKills[i], iKillsForNextLevel);  
        }  
    }

    return Plugin_Continue;
}

//Punish victim when the victim was killed with punishing weapon. Prints punish info for everyone in the chat, removes and give past level wpns
public void GunGameProcessHumiliation(CBlackMesaPlayer pPlayerVictim, const char[] iszVictimName, const char[] iszAttackerName)
{
    //process humiliation if level allows
    if (g_iNecroGunGameClientLevel[pPlayerVictim.entindex] >= 1 && g_iNecroGunGameClientLevel[pPlayerVictim.entindex] <= 11)
    {
        //level down
        g_iNecroGunGameClientLevel[pPlayerVictim.entindex]--;

        //let everyone know that this player is humiliated
        PrintToChatAll("%t", "#GunGame_Humiliation",iszAttackerName, iszVictimName);

        GunGameRemoveWeapons(pPlayerVictim);
    }
}

//Level up player or trigger victory
public bool GunGameLevelUpActions(int iAttackerLevel, CBlackMesaPlayer pPlayerAttacker, const char[] weaponName, const char[] iszAttackerName)
{
	char primaryWeapon[MAX_CLASSNAME];
	g_ConvarNecroGunGameWeaponState[iAttackerLevel].GetString(primaryWeapon, sizeof(primaryWeapon)); //get weapon needed for this level

    int level = g_iNecroGunGameClientLevel[pPlayerAttacker.entindex]; //get current level
    int killsRequired = g_ConvarNecroGunGameStateKills[level].IntValue; //get amount of kills we need for current state

    //if attacker's level is what we get from the passed on level and the attacker's weapon is what passed on - do level up actions
	if (g_iNecroGunGameClientLevel[pPlayerAttacker.entindex] == iAttackerLevel && StrEqual(weaponName, primaryWeapon, false))
	{
        //if didn't enought kills - just add +1 and return, don't level up
        if(!(g_iNecroGunGameClientKills[pPlayerAttacker.entindex] + 1 >= killsRequired))
        {
            g_iNecroGunGameClientKills[pPlayerAttacker.entindex]++; 
            return false;
        }

        g_iNecroGunGameClientKills[pPlayerAttacker.entindex] = 0; //reset kills amount

		g_iNecroGunGameClientLevel[pPlayerAttacker.entindex]++; //level up
		GunGameApplyGunModeScoreToClient(pPlayerAttacker.entindex); //add +50 score value
		
        //if attackers level is the level we need to finish the round - print the winner in the chat and trigger victory
		if (g_ConvarNecroGunGameFinalLevelNum.IntValue == iAttackerLevel && CBM_MP_GameRules.GetCurrentState() == STATE_ROUND_INDEX)
		{
			CreateTimer(0.0, WonRound, pPlayerAttacker.entindex); //trigger the victory
		}
		else //remove level's gun and give new for the next level
		{
			GunGameRemoveWeapons(pPlayerAttacker);
		}
	}
	
    return true;
}
