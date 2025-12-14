#pragma newdecls required
#pragma semicolon 1

//initialization.
public void LoadGunGame()
{
    g_ConvarNecroGunGameWeaponState[0] = CreateConVar("necro_gungame_state_0_weapon", "weapon_glock", "Weapon to give on level 0.");
    g_ConvarNecroGunGameWeaponState[1] = CreateConVar("necro_gungame_state_1_weapon", "weapon_357", "Weapon to give on level 1.");
    g_ConvarNecroGunGameWeaponState[2] = CreateConVar("necro_gungame_state_2_weapon", "weapon_shotgun", "Weapon to give on level 2.");
    g_ConvarNecroGunGameWeaponState[3] = CreateConVar("necro_gungame_state_3_weapon", "weapon_hivehand", "Weapon to give on level 3.");
    g_ConvarNecroGunGameWeaponState[4] = CreateConVar("necro_gungame_state_4_weapon", "weapon_satchel", "Weapon to give on level 4.");
    g_ConvarNecroGunGameWeaponState[5] = CreateConVar("necro_gungame_state_5_weapon", "weapon_frag", "Weapon to give on level 5.");
    g_ConvarNecroGunGameWeaponState[6] = CreateConVar("necro_gungame_state_6_weapon", "weapon_snark", "Weapon to give on level 6.");
    g_ConvarNecroGunGameWeaponState[7] = CreateConVar("necro_gungame_state_7_weapon", "weapon_mp5", "Weapon to give on level 7.");
    g_ConvarNecroGunGameWeaponState[8] = CreateConVar("necro_gungame_state_8_weapon", "weapon_crossbow", "Weapon to give on level 8.");
    g_ConvarNecroGunGameWeaponState[9] = CreateConVar("necro_gungame_state_9_weapon", "weapon_tau", "Weapon to give on level 9.");
    g_ConvarNecroGunGameWeaponState[10] = CreateConVar("necro_gungame_state_10_weapon", "weapon_rpg", "Weapon to give on level 10.");
    g_ConvarNecroGunGameWeaponState[11] = CreateConVar("necro_gungame_state_11_weapon", "weapon_crowbar", "Weapon to give on level 11.");
    g_ConvarNecroGunGameWeaponState[12] = CreateConVar("necro_gungame_state_12_weapon", "weapon_gluon", "Weapon to give on level 12.");
    g_ConvarNecroGunGameWeaponState[13] = CreateConVar("necro_gungame_state_13_weapon", "weapon_tripmine", "Weapon to give on level 13.");
    g_ConvarNecroGunGameWeaponState[14] = CreateConVar("necro_gungame_state_14_weapon", "weapon_assassin_glock", "Weapon to give on level 14.");
    
    g_ConvarNecroGunGameAmmoPrimary[0] = CreateConVar("necro_gungame_state_0_primary", "15", "Number of primary items/clips/grenades to give on level 0.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[1] = CreateConVar("necro_gungame_state_1_primary", "7", "Number of primary items/clips/grenades to give on level 1.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[2] = CreateConVar("necro_gungame_state_2_primary", "8", "Number of primary items/clips/grenades to give on level 2.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[3] = CreateConVar("necro_gungame_state_3_primary", "8", "Number of primary items/clips/grenades to give on level 3.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[4] = CreateConVar("necro_gungame_state_4_primary", "15", "Number of primary items/clips/grenades to give on level 4.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[5] = CreateConVar("necro_gungame_state_5_primary", "20", "Number of primary items/clips/grenades to give on level 5.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[6] = CreateConVar("necro_gungame_state_6_primary", "100", "Number of primary items/clips/grenades to give on level 6.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[7] = CreateConVar("necro_gungame_state_7_primary", "30", "Number of primary items/clips/grenades to give on level 7.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[8] = CreateConVar("necro_gungame_state_8_primary", "5", "Number of primary items/clips/grenades to give on level 8.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[9] = CreateConVar("necro_gungame_state_9_primary", "400", "Number of primary items/clips/grenades to give on level 9.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[10] = CreateConVar("necro_gungame_state_10_primary", "999", "Number of primary items/clips/grenades to give on level 10.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[11] = CreateConVar("necro_gungame_state_11_primary", "0", "Number of primary items/clips/grenades to give on level 11.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[12] = CreateConVar("necro_gungame_state_12_primary", "9999", "Number of primary items/clips/grenades to give on level 12.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[13] = CreateConVar("necro_gungame_state_13_primary", "9999", "Number of primary items/clips/grenades to give on level 13.", FCVAR_NONE, true, 0.0, true, 9999.0);
    g_ConvarNecroGunGameAmmoPrimary[14] = CreateConVar("necro_gungame_state_14_primary", "69", "Number of primary items/clips/grenades to give on level 14.", FCVAR_NONE, true, 0.0, true, 9999.0);
    
    g_ConvarNecroGunGameAmmoSecondary[0] = CreateConVar("necro_gungame_state_0_secondary", "0", "Number of secondary items/clips/grenades to give on level 0.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[1] = CreateConVar("necro_gungame_state_1_secondary", "0", "Number of secondary items/clips/grenades to give on level 1.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[2] = CreateConVar("necro_gungame_state_2_secondary", "0", "Number of secondary items/clips/grenades to give on level 2.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[3] = CreateConVar("necro_gungame_state_3_secondary", "0", "Number of secondary items/clips/grenades to give on level 3.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[4] = CreateConVar("necro_gungame_state_4_secondary", "0", "Number of secondary items/clips/grenades to give on level 4.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[5] = CreateConVar("necro_gungame_state_5_secondary", "0", "Number of secondary items/clips/grenades to give on level 5.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[6] = CreateConVar("necro_gungame_state_6_secondary", "0", "Number of secondary items/clips/grenades to give on level 6.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[7] = CreateConVar("necro_gungame_state_7_secondary", "4", "Number of secondary items/clips/grenades to give on level 7.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[8] = CreateConVar("necro_gungame_state_8_secondary", "0", "Number of secondary items/clips/grenades to give on level 8.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[9] = CreateConVar("necro_gungame_state_9_secondary", "0", "Number of secondary items/clips/grenades to give on level 9.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[10] = CreateConVar("necro_gungame_state_10_secondary", "0", "Number of secondary items/clips/grenades to give on level 10.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[11] = CreateConVar("necro_gungame_state_11_secondary", "0", "Number of secondary items/clips/grenades to give on level 11.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[12] = CreateConVar("necro_gungame_state_12_secondary", "0", "Number of secondary items/clips/grenades to give on level 12.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[13] = CreateConVar("necro_gungame_state_13_secondary", "0", "Number of secondary items/clips/grenades to give on level 13.", FCVAR_NONE, true, 0.0, true, 999.0);
    g_ConvarNecroGunGameAmmoSecondary[14] = CreateConVar("necro_gungame_state_14_secondary", "0", "Number of secondary items/clips/grenades to give on level 14.", FCVAR_NONE, true, 0.0, true, 999.0);

    g_ConvarNecroGunGameStateKills[0] = CreateConVar("necro_gungame_state_0_kills", "1", "Number of kills needed to level up in level 0.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[1] = CreateConVar("necro_gungame_state_1_kills", "2", "Number of kills needed to level up in level 1.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[2] = CreateConVar("necro_gungame_state_2_kills", "2", "Number of kills needed to level up in level 2.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[3] = CreateConVar("necro_gungame_state_3_kills", "2", "Number of kills needed to level up in level 3.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[4] = CreateConVar("necro_gungame_state_4_kills", "2", "Number of kills needed to level up in level 4.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[5] = CreateConVar("necro_gungame_state_5_kills", "5", "Number of kills needed to level up in level 5.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[6] = CreateConVar("necro_gungame_state_6_kills", "4", "Number of kills needed to level up in level 6.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[7] = CreateConVar("necro_gungame_state_7_kills", "4", "Number of kills needed to level up in level 7.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[8] = CreateConVar("necro_gungame_state_8_kills", "4", "Number of kills needed to level up in level 8.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[9] = CreateConVar("necro_gungame_state_9_kills", "5", "Number of kills needed to level up in level 9.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[10] = CreateConVar("necro_gungame_state_10_kills", "6", "Number of kills needed to level up in level 10.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[11] = CreateConVar("necro_gungame_state_11_kills", "6", "Number of kills needed to level up in level 11.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[12] = CreateConVar("necro_gungame_state_12_kills", "6", "Number of kills needed to level up in level 12.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[13] = CreateConVar("necro_gungame_state_13_kills", "6", "Number of kills needed to level up in level 13.", FCVAR_NONE, true, 1.0);
    g_ConvarNecroGunGameStateKills[14] = CreateConVar("necro_gungame_state_14_kills", "6", "Number of kills needed to level up in level 14.", FCVAR_NONE, true, 1.0);

    g_ConvarNecroGunGamePunishingWeaponState = CreateConVar("necro_gungame_give_punishing_weapon_state", "5", "Number of state we want to give the punishing weapon.", FCVAR_NONE, true, 0.0, true, 14.0);
    g_ConvarNecroGunGamePunishingWeaponName = CreateConVar("necro_gungame_give_punishing_weapon_classname", "weapon_crowbar", "Classname we want to use for the punishing weapon.");
    
    g_ConvarNecroGunGameFinalLevelNum = CreateConVar("necro_gungame_finish_state", "12", "Level that will finish the round.", FCVAR_NONE, true, 0.0, true, 14.0);

    g_ConvarNecroGunGameTeammateKarma = CreateConVar("necro_gungame_enable_teammete_karma", "1", "Deducts 50 points for killing your ally if used 1 for this console variable.", FCVAR_NONE, true, 0.0, true, 1.0);

    g_ConvarNecroGunGameEnabled = CreateConVar("necro_gungame_state", "0", "Toggle game mode state between enabled and disabled.");
    HookConVarChange(g_ConvarNecroGunGameEnabled, OnGunGameStateChanged);
}