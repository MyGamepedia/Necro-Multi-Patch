#pragma newdecls required
#pragma semicolon 1

//Remove item_weapon_ ents so the players can't own other weapons in regular maps (should we do this for ammo ents ?)
public void GunGameRemoveItems()
{
    char classname[MAX_CLASSNAME];
	
    int maxents = GetMaxEntities(); //get max ents

	for (int i = 1; i < maxents; i++)
	{
		if (IsValidEntity(i)) //get classname for a valid entity
		{
			GetEntityClassname(i, classname, sizeof(classname));

			if  ( StrContains(classname, "item_weapon_") != -1 ) //if contains item_weapon_ - remove
					RemoveEdict(i);
		}
	}	
}

//Remove weapon used for the past level
public void GunGameRemoveWeapons(CBlackMesaPlayer pPlayer)
{
    //   if(IsFakeClient(client))
    //     return;

    int entidx = pPlayer.entindex;
    int iLevel = g_iNecroGunGameClientLevel[entidx] - 1; // get past level
    if (iLevel < 0)
    {
        #if defined DEBUG
        PrintToChatAll("Invalid level for weapon removal: %d", iLevel);
        #endif
        return;
    }

    char szWeaponName[MAX_CLASSNAME];
    g_ConvarNecroGunGameWeaponState[iLevel].GetString(szWeaponName, sizeof(szWeaponName));
    CBaseCombatWeapon pWeaponState = pPlayer.GetWeapon(szWeaponName);

    char szPunishingWeapon[MAX_CLASSNAME];
    CBaseCombatWeapon pWeaponPunishing = NULL_CBASEENTITY;
    bool bHasPunishing = false;
    if (g_iNecroGunGameClientLevel[entidx] >= GetConVarInt(g_ConvarNecroGunGamePunishingWeaponState))
    {
        g_ConvarNecroGunGamePunishingWeaponName.GetString(szPunishingWeapon, sizeof(szPunishingWeapon));
        pWeaponPunishing = pPlayer.GetWeapon(szPunishingWeapon);
        bHasPunishing = pWeaponPunishing.IsValid();
    }

    #if defined DEBUG
    PrintToChatAll("Removing for %N: state '%s' (entidx %d), punishing '%s' (entidx %d)", 
    entidx, szWeaponName, pWeaponState.entindex, bHasPunishing ? szPunishingWeapon : "none", bHasPunishing ? pWeaponPunishing.entindex : -1);
    #endif

    if (IsValidEntity(pWeaponState.entindex))
    {
        if (pPlayer.GetActiveWeapon().entindex == pWeaponState.entindex)
        {
            pWeaponState.Holster(pWeaponState); //note: use active weap for this func, doesn't cause issues
            pWeaponState.SetNextPrimaryAttack(FLT_MAX);
            pWeaponState.SetNextSecondaryAttack(FLT_MAX);

            //this should fix issues for certain weapons if clip is empty
            if(StrEqual(szWeaponName, "weapon_glock") || StrEqual(szWeaponName, "weapon_assassin_glock") || StrEqual(szWeaponName, "weapon_mp5") 
                || StrEqual(szWeaponName, "weapon_357") || StrEqual(szWeaponName, "weapon_shotgun") || StrEqual(szWeaponName, "weapon_crossbow"))
            {
                pWeaponState.FinishReload();
            }

            if(pWeaponState.entindex != pWeaponPunishing.entindex && pWeaponPunishing.IsValid())
            {
                pWeaponPunishing.Kill(); //immediate kill to avoid issues with switching to it
            }
        }

        //0.22 - 0.25, for hidding anim
        pWeaponState.AddOutput("OnUser1", "!activator", "GiveGunGameResources", "", 0.22, 1); //give resources after removal
        pWeaponState.AddOutput("OnUser1", "!self", "Kill", "", 0.22, 1); //remove weapon
        pWeaponState.FireOutput("OnUser1", pPlayer.entindex);
    }
    else 
    {
        #if defined DEBUG
        PrintToChatAll("Weapon state entity invalid for '%s'", szWeaponName);
        #endif
        return;
    }

    if (bHasPunishing)
    {
        if (pPlayer.GetActiveWeapon().entindex == pWeaponPunishing.entindex) 
        {
            pWeaponPunishing.Holster(pWeaponPunishing);
            pWeaponPunishing.SetNextPrimaryAttack(FLT_MAX);
            pWeaponPunishing.SetNextSecondaryAttack(FLT_MAX);
            
            if(pWeaponState.entindex != pWeaponPunishing.entindex)
                pWeaponState.Kill(); //immediate kill to avoid issues with switching to it
        }
        pWeaponPunishing.KillDelayed(0.22); //0.22 - 0.25, for hidding anim
    }

    return;
}

//Gives weapon and resources needed for the next level
public Action GunGameGiveResources(int iLevel, CBlackMesaPlayer pPlayer)
{
	char primaryWeapon[MAX_CLASSNAME], punishingWeaponName[MAX_CLASSNAME]; //weapon names

	int primaryAmmo, secondaryAmmo; //ammo values
		
	g_ConvarNecroGunGameWeaponState[iLevel].GetString(primaryWeapon, sizeof(primaryWeapon)); //get weapon name needed for this level
    g_ConvarNecroGunGamePunishingWeaponName.GetString(punishingWeaponName, sizeof(punishingWeaponName)); //get punishing weapon name

	primaryAmmo = g_ConvarNecroGunGameAmmoPrimary[iLevel].IntValue; //get int value for main ammo
	secondaryAmmo = g_ConvarNecroGunGameAmmoSecondary[iLevel].IntValue; //get int value for second ammo

    //fix viewmodel disappear issues
    if(GetConVarBool(g_ConvarNecroCreateNewViewmodel))
    {
        for (int i = 0; i < MAX_VIEWMODELS; i++)
        {
            CPredictedViewModel vm = pPlayer.GetViewModel(i);
            if (vm != NULL_CBASEENTITY)
            {
                vm.Kill();
                pPlayer.SetViewModel(NULL_CBASEENTITY, i);
                CPredictedViewModel nvm = CPredictedViewModel(pPlayer.CreateViewModel(i));

                DataPack pack = new DataPack();
                pack.WriteCell(pPlayer.entindex);
                pack.WriteCell(nvm.entindex);
                RequestFrame(GunGameResetViewModelBodyGroup, pack);
            }
        }
    }
	
    pPlayer.GiveWeapon(primaryWeapon); //give weapon we want for the next level

	GunGameClientSetWeaponPlayerAmmo(pPlayer, primaryWeapon, primaryAmmo, secondaryAmmo); //set ammo for level's weapon

    int PunishingWeaponState = GetConVarInt(g_ConvarNecroGunGamePunishingWeaponState);
		
	if(PunishingWeaponState <= iLevel) //also give punishing weapon if current level value is <=
	{
        pPlayer.GiveWeapon(punishingWeaponName); //give punishing weapon
	}

    RequestFrame(GunGameForceSwitchToPrimary, pPlayer); //switch to level's weapon (in case if we are killer's active weapon is punishing weapon)

	return Plugin_Continue;
}

//Fixes issue when the player is using HEV hands while should use character's hands
//NOTE: I tried SetBodygroup call, but it doesn't work with a hight delay
public void GunGameResetViewModelBodyGroup(DataPack pack)
{
    pack.Reset();

    int playerEnt = pack.ReadCell();
    int nvmEnt = pack.ReadCell();
    delete pack;

    CBlackMesaPlayer pPlayer = CBlackMesaPlayer(playerEnt);
    CPredictedViewModel nvm = CPredictedViewModel(nvmEnt);

    if (!pPlayer.IsValid() || !nvm.IsValid())
        return;

    if (nvm != NULL_CBASEENTITY)
    {         
        char szCharacterType[MAX_MODELNAME];
        pPlayer.GetModelName(szCharacterType, sizeof(szCharacterType));

        #if defined DEBUG
        PrintToChatAll("Model: %s", szCharacterType);
        #endif

        //TODO: This isn't perfect, because it may switch other bodygroups, should be replaced with SetBodyGroup call
        if((StrContains(szCharacterType, "mp_scientist") != -1)) //2
        {
            if(StrContains(szCharacterType, "mp_scientist_hev") == -1)
                nvm.SetBody(2);
        }

        else if(StrContains(szCharacterType, "mp_gman") != -1) //6
        {
            nvm.SetBody(6);
        }

        else if(StrContains(szCharacterType, "mp_zombie_sci") != -1) //4
        {
            nvm.SetBody(4);
        }

        else if(StrContains(szCharacterType, "mp_zombie_guard") != -1) //5
        {
            nvm.SetBody(5);
        }

        else if(StrContains(szCharacterType, "mp_marine") != -1) //1
        {
            nvm.SetBody(1);
        }

        else if(StrContains(szCharacterType, "mp_guard") != -1) //3
        {
            nvm.SetBody(3);
        }
    }
}

//Timer to give resources
public Action GunGameWeapons(Handle timer, CBlackMesaPlayer pPlayer)
{
//	if(IsFakeClient(client))
//		return Plugin_Continue;

	GunGameGiveResources(g_iNecroGunGameClientLevel[pPlayer.entindex], pPlayer); //give resources needed for this level

	return Plugin_Continue;
}

//Switch weapon via command
public void GunGameForceSwitchToPrimary(CBlackMesaPlayer pPlayer)
{
 //   if(IsFakeClient(client))
//		return Plugin_Continue;
    
    char iszPrimaryWeapon[MAX_CLASSNAME];

	int iLevel = g_iNecroGunGameClientLevel[pPlayer.entindex]; //get current level
	
	g_ConvarNecroGunGameWeaponState[iLevel].GetString(iszPrimaryWeapon, sizeof(iszPrimaryWeapon)); //get weapon we need switch to force current level
    ClientCommand(pPlayer.entindex, "use %s", iszPrimaryWeapon); //use weapon via console command

    return;
}

//Sets full weapon name for the cvar. Returns true if weapon is known.
public bool GunGameGetFullWeaponName(char[] weaponName, int size)
{
    // Category 1
    if (StrEqual(weaponName, "crowbar", false))
    {
        strcopy(weaponName, size, "weapon_crowbar");
    }

    // Category 2
    else if (StrEqual(weaponName, "glock", false))
    {
        strcopy(weaponName, size, "weapon_glock");
    }
    else if (StrEqual(weaponName, "assassin_glock", false))
    {
        strcopy(weaponName, size, "weapon_assassin_glock");
    }
    else if (StrEqual(weaponName, "357", false))
    {
        strcopy(weaponName, size, "weapon_357");
    }

    // Category 3
    else if (StrEqual(weaponName, "mp5", false) || StrEqual(weaponName, "mp5_contact", false))
    {
        strcopy(weaponName, size, "weapon_mp5");
    }
    else if (StrEqual(weaponName, "shotgun", false))
    {
        strcopy(weaponName, size, "weapon_shotgun");
    }
    else if (StrEqual(weaponName, "crossbow", false) || StrEqual(weaponName, "bolt", false) || StrEqual(weaponName, "tracerbullet", false))
    {
        strcopy(weaponName, size, "weapon_crossbow");
    }

    // Category 4
    else if (StrEqual(weaponName, "rpg", false))
    {
        strcopy(weaponName, size, "weapon_rpg");
    }
    else if (StrEqual(weaponName, "tau", false))
    {
        strcopy(weaponName, size, "weapon_tau");
    }
    else if (StrEqual(weaponName, "gluon", false))
    {
        strcopy(weaponName, size, "weapon_gluon");
    }
    else if (StrEqual(weaponName, "hornet", false))
    {
        strcopy(weaponName, size, "weapon_hivehand");
    }

    // Category 5
    else if (StrEqual(weaponName, "frag", false))
    {
        strcopy(weaponName, size, "weapon_frag");
    }
    else if (StrEqual(weaponName, "satchel", false))
    {
        strcopy(weaponName, size, "weapon_satchel");
    }
    else if (StrEqual(weaponName, "tripmine", false))
    {
        strcopy(weaponName, size, "weapon_tripmine");
    }
    else if (StrEqual(weaponName, "snark", false))
    {
        strcopy(weaponName, size, "weapon_snark");
    }

    //If unknown weapon
    else
    {
        strcopy(weaponName, size, "unknown");
		return false; 
    }

	return true;
}

//Set ammo for player by checking level's weapon
public void GunGameClientSetWeaponPlayerAmmo(CBlackMesaPlayer pPlayer, char iszPrimaryWeapon[MAX_CLASSNAME], int iPrimaryAmmo, int iSecondaryAmmo)
{
    if (StrEqual(iszPrimaryWeapon, "weapon_glock", false) || StrEqual(iszPrimaryWeapon, "weapon_assassin_glock", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_9MM, iPrimaryAmmo);
        return;
    }

    if(StrEqual(iszPrimaryWeapon, "weapon_357", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_357, iPrimaryAmmo);
        return;
    }

    if(StrEqual(iszPrimaryWeapon, "weapon_shotgun", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_SHELLS, iPrimaryAmmo);
        return;
    }

    if(StrEqual(iszPrimaryWeapon, "weapon_gluon", false) || StrEqual(iszPrimaryWeapon, "weapon_tau", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_ENERGY, iPrimaryAmmo);
        return;
    }

    if(StrEqual(iszPrimaryWeapon, "weapon_mp5", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_9MM, iPrimaryAmmo);
        pPlayer.SetAmmoFromIndex(AMMO_MP5NADE, iSecondaryAmmo);
        return;
    }

    if(StrEqual(iszPrimaryWeapon, "weapon_rpg", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_ROCKETS, iPrimaryAmmo);
        return;
    }   

    if(StrEqual(iszPrimaryWeapon, "weapon_frag", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_NADES, iPrimaryAmmo);
        return;
    }

    if(StrEqual(iszPrimaryWeapon, "weapon_satchel", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_SATCHELS, iPrimaryAmmo);
        return;
    }

    if(StrEqual(iszPrimaryWeapon, "weapon_tripmine", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_TRIPMINES, iPrimaryAmmo);
        return;
    }

    if(StrEqual(iszPrimaryWeapon, "weapon_hivehand", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_HIVEGUN, iPrimaryAmmo);
        return;
    }

    if(StrEqual(iszPrimaryWeapon, "weapon_snark", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_SNARKS, iPrimaryAmmo);
        return;
    }

    if(StrEqual(iszPrimaryWeapon, "weapon_crossbow", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_BOLTS, iPrimaryAmmo);
        return;
    }
}