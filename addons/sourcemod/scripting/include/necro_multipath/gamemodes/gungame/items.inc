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
public void GunGameRemoveWeapons(Handle timer, CBlackMesaPlayer pPlayer)
{
 //   if(IsFakeClient(client))
//		return;

    int offset = FindSendPropInfo("CBlackMesaPlayer", "m_hMyWeapons"); //offset num for m_hMyWeapons

    int maxGuns = (g_iNecroGunGameClientLevel[pPlayer.entindex] < 100) ? 20 : 0; //?get max guns? isn't my code, not sure for what this is

    char weaponClass[MAX_CLASSNAME], punishingWeaponName[MAX_CLASSNAME]; //weapon classname to compare and punishing weapon name we set in the cvar

    g_ConvarNecroGunGamePunishingWeaponName.GetString(punishingWeaponName, sizeof(punishingWeaponName)); //get punishing weapon name we set in the cvar

    int weaponId; //weapon index
    
    for(int x = 0; x < maxGuns; x += 4)
    {
        weaponId = GetEntDataEnt2(pPlayer.entindex, offset + x); //get weapon
        
        if(weaponId > 0)
        {	
            GetEdictClassname(weaponId, weaponClass, sizeof(weaponClass)); //get classname for weapon we found
            
			if (StrEqual(weaponClass, punishingWeaponName, false)) //never remove punishing weapons
            {
                continue;
            }
			
            //remove not punishing weapon
            pPlayer.RemoveWeapon(CBaseCombatWeapon(weaponId));
            RemoveEdict(weaponId);
        }
    }

    return;
}

//Gives weapon and resources needed for the next level
public Action GunGameGiveResources(int casenum, CBlackMesaPlayer pPlayer)
{
	char primaryWeapon[MAX_CLASSNAME], punishingWeaponName[MAX_CLASSNAME]; //weapon names

	int primaryAmmo, secondaryAmmo; //ammo values
		
	g_ConvarNecroGunGameWeaponState[casenum].GetString(primaryWeapon, sizeof(primaryWeapon)); //get weapon name needed for this level
    g_ConvarNecroGunGamePunishingWeaponName.GetString(punishingWeaponName, sizeof(punishingWeaponName)); //get punishing weapon name

	primaryAmmo = g_ConvarNecroGunGameAmmoPrimary[casenum].IntValue; //get int value for main ammo
	secondaryAmmo = g_ConvarNecroGunGameAmmoSecondary[casenum].IntValue; //get int value for second ammo
	
    pPlayer.GiveWeapon(primaryWeapon); //give weapon we want for the next level

	GunGameClientSetWeaponPlayerAmmo(pPlayer, primaryWeapon, primaryAmmo, secondaryAmmo); //set ammo for level's weapon

    int PunishingWeaponState = GetConVarInt(g_ConvarNecroGunGamePunishingWeaponState);
		
	if(PunishingWeaponState <= casenum) //also give punishing weapon if current level value is <=
	{
        pPlayer.GiveWeapon(punishingWeaponName); //give punishing weapon
	}

    RequestFrame(GunGameForceSwitchToPrimary, pPlayer); //switch to level's weapon (in case if we are killer's active weapon is punishing weapon)

	return Plugin_Continue;
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
    
    char primaryWeapon[MAX_CLASSNAME];

	int level = g_iNecroGunGameClientLevel[pPlayer.entindex]; //get current level
	
	g_ConvarNecroGunGameWeaponState[level].GetString(primaryWeapon, sizeof(primaryWeapon)); //get weapon we need switch to force current level

    ClientCommand(pPlayer.entindex, "use %s", primaryWeapon); //use weapon via console command

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
public void GunGameClientSetWeaponPlayerAmmo(CBlackMesaPlayer pPlayer, char primaryWeapon[MAX_CLASSNAME], int primaryAmmo, int secondaryAmmo)
{
    if (StrEqual(primaryWeapon, "weapon_glock", false) || StrEqual(primaryWeapon, "weapon_assassin_glock", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_9MM, primaryAmmo);
        return;
    }

    else if(StrEqual(primaryWeapon, "weapon_357", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_357, primaryAmmo);
        return;
    }

    else if(StrEqual(primaryWeapon, "weapon_shotgun", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_SHELLS, primaryAmmo);
        return;
    }

    else if(StrEqual(primaryWeapon, "weapon_gluon", false) || StrEqual(primaryWeapon, "weapon_tau", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_ENERGY, primaryAmmo);
        return;
    }

    else if(StrEqual(primaryWeapon, "weapon_mp5", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_9MM, primaryAmmo);
        pPlayer.SetAmmoFromIndex(AMMO_MP5NADE, secondaryAmmo);
        return;
    }

    else if(StrEqual(primaryWeapon, "weapon_rpg", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_ROCKETS, primaryAmmo);
        return;
    }   

    else if(StrEqual(primaryWeapon, "weapon_frag", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_NADES, primaryAmmo);
        return;
    }

    else if(StrEqual(primaryWeapon, "weapon_satchel", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_SATCHELS, primaryAmmo);
        return;
    }

    else if(StrEqual(primaryWeapon, "weapon_tripmine", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_TRIPMINES, primaryAmmo);
        return;
    }

    else if(StrEqual(primaryWeapon, "weapon_hivehand", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_HIVEGUN, primaryAmmo);
        return;
    }

    else if(StrEqual(primaryWeapon, "weapon_snark", false))
    {
        pPlayer.SetAmmoFromIndex(AMMO_SNARKS, primaryAmmo);
        return;
    }
}