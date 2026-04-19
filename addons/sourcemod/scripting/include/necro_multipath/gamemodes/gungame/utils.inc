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
    float flPosX = GetConVarFloat(g_ConvarNecroGunGameTextPosX);
    float flPosY = GetConVarFloat(g_ConvarNecroGunGameTextPosY);

    int iHightestLevel = -1;

    CBasePlayer pHightestLevelPlayer;

    for (int i = 1; i <= MaxClients; i++)
    {
        if (CBasePlayer(i).IsValid())
        {
            if (g_iNecroGunGameClientLevel[i] > iHightestLevel)
            {
                iHightestLevel = g_iNecroGunGameClientLevel[i];
                pHightestLevelPlayer = CBasePlayer(i);
            }
        }
    }

    char szHightestLevelPlayerName[MAX_NAME_LENGTH];
    pHightestLevelPlayer.GetName(szHightestLevelPlayerName, sizeof(szHightestLevelPlayerName));

    for (int i = 1; i <= MaxClients; i++)  
    {
        //we don't display main hud in intermission, don't fir gungame as well
        bool bNoInter = (CBM_MP_GameRules.GetCurrentState() != STATE_ROUND_INTERMISSION_INDEX && CBM_MP_GameRules.GetCurrentState() != STATE_INTERMISSION_INDEX) ? true : false;

        if (IsClientConnected(i) && IsClientInGame(i) && IsPlayerAlive(i) && g_iNecroGunGameCurrentState == 1 && bNoInter)  
        {  
            int iLevel = g_iNecroGunGameClientLevel[i];  
            int iKillsForNextLevel = g_ConvarNecroGunGameStateKills[iLevel].IntValue;
            int iFinalLevel = g_ConvarNecroGunGameFinalLevelNum.IntValue;

            int col1[4] = {255, 255, 255, 255}; //col

            //pick col by team (green for marines)
            if (CBasePlayer(i).GetTeam() != TEAM_ONE)
            {
                col1 = {255, 194, 6, 255};
            }
            else
                col1 = {0, 255, 0, 255};

            char weaponName[MAX_CLASSNAME+1];
            g_ConvarNecroGunGameWeaponState[iLevel].GetString(weaponName, sizeof(weaponName));
            Format(weaponName, sizeof(weaponName), "#%s", weaponName);

            //translate weapon name
            Format(weaponName, sizeof(weaponName), "%t", weaponName);
  
            SetHudTextParams(flPosX, flPosY, GUNGAME_HUDTICK + 0.1, col1[0], col1[1], col1[2], col1[3], 0, 6.0, 0.1, 0.2);

            char szNextWpn[MAX_CLASSNAME];

            if (iLevel < iFinalLevel)
            {
                g_ConvarNecroGunGameWeaponState[iLevel + 1].GetString(szNextWpn, sizeof(szNextWpn));
                Format(szNextWpn, sizeof(szNextWpn), "#%s", szNextWpn);
                Format(szNextWpn, sizeof(szNextWpn), "%t", szNextWpn);
            }
            else
            {
                int iVoidChance = GetRandomInt(1, 1500); //~1 every ~30 rounds if played ~2 mins

                if (iVoidChance == 42) //the answer
                {
                    Format(szNextWpn, sizeof(szNextWpn), "Void-995"); //thanks dude
                }
                else
                {
                    Format(szNextWpn, sizeof(szNextWpn), "%t", "#GunGame_FinalLevelWeaponNote");
                }
            }
           
            ShowHudText(i, 1, "%t", "#GunGame_HUD", weaponName, g_iNecroGunGameClientKills[i], iKillsForNextLevel, szNextWpn, iLevel, iFinalLevel);  
        }  
    }

    return Plugin_Continue;
}

//Punish victim when the victim was killed with punishing weapon. Prints punish info for everyone in the chat, removes and give past level wpns
public void GunGameProcessHumiliation(CBlackMesaPlayer pPlayerVictim, const char[] iszVictimName, const char[] iszAttackerName)
{
    //process humiliation if level allows and not prevented
    if (pPlayerVictim.GetUserData("m_bWasPunished") == true)
        return;

    if (!(g_iNecroGunGameClientLevel[pPlayerVictim.entindex] >= 1 && g_iNecroGunGameClientLevel[pPlayerVictim.entindex] <= 11))
        return;

    //level down
    g_iNecroGunGameClientLevel[pPlayerVictim.entindex]--;

    //let everyone know that this player is humiliated
    PrintToChatAll("%t", "#GunGame_Humiliation", COL_YELLOWGREEN, COL_NORMAL, COL_LIGHTBLUE, iszAttackerName, COL_NORMAL, 
    (pPlayerVictim.GetTeam() == TEAM_ONE) ? COL_GREEN : COL_SEMIRED, iszVictimName, COL_NORMAL);

    GunGameRemoveWeapons(pPlayerVictim);

    if (pPlayerVictim.GetScore() > 0)
        pPlayerVictim.SetScore(pPlayerVictim.GetScore() - 100); //remove score for humiliation

    //particle effects on punishment
    int particle = CreateEntityByName("info_particle_system");
		
    if (IsValidEntity(particle))
    {
        CBaseEntity pParticle = CBaseEntity(particle);
        SetEntPropString(particle, Prop_Data, "m_iszEffectName", "crystalholder_mouthglow"); 
        SetEntProp(particle, Prop_Data, "m_bStartActive", 0);

        char szTargetname[MAX_FORMAT];
        Format(szTargetname, sizeof(szTargetname), "crystalholdermouthglow_particles_%d", pPlayerVictim.entindex);
        DispatchKeyValue(particle, "targetname", szTargetname);
            
        float pos[3];
        GetEntPropVector(pPlayerVictim.entindex, Prop_Send, "m_vecOrigin", pos); 
        TeleportEntity(particle, pos, NULL_VECTOR, NULL_VECTOR);
            
        DispatchSpawn(particle);
        ActivateEntity(particle);	//needed to start particle effect

        pParticle.AddOutput("OnUser1", "!self", "Start", "", 0.0, 1); //start particle effect immediately
        pParticle.AddOutput("OnUser1", "!self", "Kill", "", 2.0, 1); //remove particle after a while
        pParticle.FireOutput("OnUser1");
    }

    //punishment is fun, but also very toxic, to balance, don't let the victim be punished again until reaches the next level
    pPlayerVictim.SetUserData("m_bWasPunished", true);

    int iPitch = GetRandomInt(80, 100);

    //play sound for humiliation
    PrecacheSound("sound/npc/roller/mine/rmine_blip3.wav");
    EmitSoundToAll("sound/npc/roller/mine/rmine_blip3.wav", pPlayerVictim.entindex, SNDCHAN_VOICE, SNDLEVEL_NORMAL, SND_NOFLAGS, 0.8, iPitch);
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
        pPlayerAttacker.SetUserData("m_bWasPunished", false); //allow to be punished again after level up

        //if didn't enought kills - just add +1 and return, don't level up
        if(!(g_iNecroGunGameClientKills[pPlayerAttacker.entindex] + 1 >= killsRequired))
        {
            g_iNecroGunGameClientKills[pPlayerAttacker.entindex]++; 
            return false;
        }

        g_iNecroGunGameClientKills[pPlayerAttacker.entindex] = 0; //reset kills amount

		g_iNecroGunGameClientLevel[pPlayerAttacker.entindex]++; //level up
		GunGameApplyGunModeScoreToClient(pPlayerAttacker.entindex); //add +100 score value
		
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
