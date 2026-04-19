#pragma newdecls required
#pragma semicolon 1

#include <sourcemod>
#include <sdktools>
#include <sdktools_functions>
#include <sdkhooks>
#include <dhooks>
#include <entity>
#include <geoip>

#include <sourcescramble>

#include <necro_multipath/macros_srccoop>
#include <necro_multipath/typedef_srccoop>
#include <necro_multipath/globals_srccoop>
#include <necro_multipath/public>
#include <srccoop_api/util>

#include <necro_multipath/globals>

#include <necro_multipath/classdef_srccoop>

#include <necro_multipath/entities/ai_goal_lead>
#include <necro_multipath/entities/ai_script_conditions>
#include <necro_multipath/entities/env_cascade_light>
#include <necro_multipath/entities/env_credits>
#include <necro_multipath/entities/env_explosion>
#include <necro_multipath/entities/env_introcredits>
#include <necro_multipath/entities/env_laser_dot>
#include <necro_multipath/entities/env_screenoverlay>
#include <necro_multipath/entities/env_sprite>
#include <necro_multipath/entities/env_xen_portal_effect>
#include <necro_multipath/entities/env_zoom>
#include <necro_multipath/entities/func_rotating>
#include <necro_multipath/entities/grenade_bolt>
#include <necro_multipath/entities/grenade_frag>
#include <necro_multipath/entities/grenade_hornet>
#include <necro_multipath/entities/grenade_mp5_contact>
#include <necro_multipath/entities/item_ammo_canister>
#include <necro_multipath/entities/item_battery>
#include <necro_multipath/entities/item_weapon_snark>
#include <necro_multipath/entities/misc_marionettist>
#include <necro_multipath/entities/music_track>
#include <necro_multipath/entities/newlight_point>
#include <necro_multipath/entities/npc_barnacle>
#include <necro_multipath/entities/npc_gargantua>
#include <necro_multipath/entities/npc_human_medic>
#include <necro_multipath/entities/npc_ichthyosaur>
#include <necro_multipath/entities/npc_lav>
#include <necro_multipath/entities/npc_puffballfungus>
#include <necro_multipath/entities/npc_snark>
#include <necro_multipath/entities/npc_sniper>
#include <necro_multipath/entities/npc_xenturret>
#include <necro_multipath/entities/player_loadsaved>
#include <necro_multipath/entities/player_manager>
#include <necro_multipath/entities/player_speedmod>
#include <necro_multipath/entities/point_clientcommand>
#include <necro_multipath/entities/point_teleport>
#include <necro_multipath/entities/point_viewcontrol>
#include <necro_multipath/entities/prop_hev_charger>
#include <necro_multipath/entities/prop_radiation_charger>
#include <necro_multipath/entities/prop_ragdoll>
#include <necro_multipath/entities/scripted_sequence>
#include <necro_multipath/entities/waterbullet>
#include <necro_multipath/entities/weapon_357>
#include <necro_multipath/entities/weapon_assassin_glock>
#include <necro_multipath/entities/weapon_crossbow>
#include <necro_multipath/entities/weapon_glock>
#include <necro_multipath/entities/weapon_mp5>
#include <necro_multipath/entities/weapon_rpg>
#include <necro_multipath/entities/weapon_satchel>
#include <necro_multipath/entities/weapon_shotgun>
//#include <necro_multipath/entities/weapon_snark>

#include <necro_multipath/entities/classes/CAI_BaseNPC>
#include <necro_multipath/entities/classes/CAI_GoalEntity>
#include <necro_multipath/entities/classes/CAI_MoveProbe>
#include <necro_multipath/entities/classes/CBaseAnimating>
#include <necro_multipath/entities/classes/CBaseCombatWeapon>
#include <necro_multipath/entities/classes/CBaseClient>
#include <necro_multipath/entities/classes/CBaseEntity>
#include <necro_multipath/entities/classes/CBasePickup>
#include <necro_multipath/entities/classes/CBasePlayer>
#include <necro_multipath/entities/classes/CBlackMesaBaseWeaponIronSights>
#include <necro_multipath/entities/classes/CBlackMesaKillStreaks>
#include <necro_multipath/entities/classes/CBM_MP_GameRules>
#include <necro_multipath/entities/classes/CBoneSetup>
#include <necro_multipath/entities/classes/CMultiplayRules>
#include <necro_multipath/entities/classes/CEventQueue>

#include <necro_multipath/entities/classes/CNPC_PlayerCompanion>
#include <necro_multipath/entities/classes/CRecipientFilter>
#include <necro_multipath/entities/classes/CSceneEntity>
#include <necro_multipath/entities/classes/CTempEntsSystem>

//#include <necro_multipath/instancing> //TODO: create our own

#include <necro_multipath/entities/general/special/Hook_NoDmg>
#include <necro_multipath/entities/general/OnTakeDamage>
#include <necro_multipath/entities/general/Physics_RunThinkFunctions>

#include <necro_multipath/engine/CLagCompensationManagerStartLagCompensation>

#include <necro_multipath/convars/host_timescale>
#include <necro_multipath/convars/necro_fastrespawndelay>
#include <necro_multipath/convars/necro_spectatorjointeamdelay>
#include <necro_multipath/convars/jointeam>
#include <necro_multipath/convars/say>

#include <necro_multipath/functions/GetChild>
#include <necro_multipath/functions/AddOutput>

#include <necro_multipath/gamemodes/gungame/gungame>


public Plugin myinfo = {
	name = "Dr.Necro's Black Mesa Servers Multipatch",
	author = "MyGamepedia",
	description = "This addon is used to significantly expands and improves Dr.Necro's Black Mesa multiplayer servers.",
	version = "1.2.1",
	url = ""
};

public void OnPluginStart()
{
	//don't load this plugin in singleplayer 
	if (MaxClients < 2)
	{
		SetFailState("Don't use this plugin in singleplayer!");
	}
	
	//existing convars
	player_respawn_protection_time = FindConVar("player_respawn_protection_time");
	sk_crossbow_tracer_enabled = FindConVar("sk_crossbow_tracer_enabled");
	sv_long_jump_manacost = FindConVar("sv_long_jump_manacost");
	player_respawn_alpha = FindConVar("player_respawn_alpha");
	sv_jump_long_enabled = FindConVar("sv_jump_long_enabled");
	sv_ladder_useonly = FindConVar("sv_ladder_useonly");
	mp_friendlyfire = FindConVar("mp_friendlyfire");
	sv_speed_sprint = FindConVar("sv_speed_sprint");
	mp_forcerespawn = FindConVar("mp_forcerespawn");
	sv_always_run = FindConVar("sv_always_run");
	sv_speed_walk = FindConVar("sv_speed_walk");
	mp_flashlight = FindConVar("mp_flashlight");
	mp_teamplay = FindConVar("mp_teamplay");
	sv_cheats = FindConVar("sv_cheats");
	hostname = FindConVar("hostname");

	
	//new convars
	g_ConvarNecroGiveDefaultItems = CreateConVar("necro_givedefaultitems", "1", "Enable default give items list for on player spawn.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroOverrideDefaultWeaponParams = CreateConVar("necro_overridedefaultweaponparams", "1", "Enable weapon values override for parameters by loading custom weapon script.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroBoltParticles = CreateConVar("necro_boltparticles", "1", "Enables trail for explosive crossbow bolts, the trial makes it easier to determine where the shot was fired from.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroClassicFrags = CreateConVar("necro_classicfrags", "0", "Enable simplified physics for frag grenades.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroClassicLaserDot = CreateConVar("necro_classiclaserdot", "0", "Enable original RPG laser dot rendering.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroMp5ContactParticles = CreateConVar("necro_mp5contactparticles", "1", "Enables smoke for MP5 barrel grenade.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroBoltHitscanDamage = CreateConVar("necro_bolthitscandamage", "125.0", "Amount of damage for the crossbow bolt hitscan.");
	g_ConvarNecroAllowFastRespawn = CreateConVar("necro_allowfastrespawn","1","Allow player respawn by pressing the buttons before spec_freeze_time and spec_freeze_traveltime is finished.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroFastRespawnDelay = CreateConVar("necro_fastrespawndelay", "0.5", "Amount of time in seconds before player can respawn by pressing the buttons with enabled fast respawn.");
	g_ConvarNecroExplodingBolt = CreateConVar("necro_explodingbolt","1","Enables exploding bolt for the crossbow.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroSatchelDelayOverride = CreateConVar("necro_satcheldelayoverride","1","Enables primary and secondary attack delay override for weapon_satchel", 0, true, 0.0, true, 1.0);
	g_ConvarNecroSatchelDelay_Attack1_Primary = CreateConVar("necro_satcheldelay_attack1_primary","1.0","Sets delay for satchel weapon primary attack when the satchel thrown.  Recommended 0.84 at least to avoid bugs with the satchel rendering.");
	g_ConvarNecroSatchelDelay_Attack1_Secondary = CreateConVar("necro_satcheldelay_attack1_secondary","1.2","Sets delay for satchel weapon primary secondary when the satchel thrown. Recommended 1.2 at least to avoid bugs with the radio rendering.");
	g_ConvarNecroSatchelDelay_Attack2_Primary = CreateConVar("necro_satcheldelay_attack2_primary","0.3","Sets delay for satchel weapon primary attack when the radio is used.");
	g_ConvarNecroSatchelDelay_Attack2_Secondary = CreateConVar("necro_satcheldelay_attack2_secondary","0.2","Sets delay for satchel weapon secondary attack when the radio is used.");
	g_ConvarNecroSatchelDelay_Reload_Primary = CreateConVar("necro_satcheldelay_reload_primary","0.4","Sets delay for satchel weapon primary attack when the owner take out a new satchel.");
	g_ConvarNecroSatchelDelay_Reload_Secondary = CreateConVar("necro_satcheldelay_reload_secondary","0.4","Sets delay for satchel weapon secondary attack when the owner take out a new satchel.");
	g_ConvarNecroSpectatorJoinTeamDelay = CreateConVar("necro_spectatorjointeamdelay", "15.0", "Amount of time in seconds before spectator can join a team or play deathmatch again after the player joined spectators.");
	g_ConvarNecroOtherPlayersFlashlight = CreateConVar("necro_otherplayersflashlight", "1", "Allow creation of flashlight effects, used as flashlight from other players perspective.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroOtherPlayersFlashlightTransmit = CreateConVar("necro_otherplayersflashlighttransmit", "1", "Enable \"other players flashlight\" transmit hooks to hide the flashlight effects for owners.", 0, true, 0.0, true, 1.0);
	//TODO: FIX necro_playerscollide
	g_ConvarNecroPlayersCollide = CreateConVar("necro_playerscollide", "1", "Enable player collision with each other.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroForceTeam = CreateConVar("necro_forceteam", "-1", "Force players to join a specific team. -1 - disabled force team, 0 - unassigned or spectators, 1 - spectators, 2 - team one, 3 - team two.", 0, true, -1.0, true, 3.0);
	g_ConvarNecroItemSpawnOverride = CreateConVar("necro_itemspawnoverride", "1", "Enable item spawn override for specicif specific item spawn variants via \"Item Type\" keyvalue.\n0 - Default item. \n1 - Singleplayer item.\n2 - SourceCoop item.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroAllowWeaponAutoReload = CreateConVar("necro_alloeweaponautoreload", "1", "Enable automatic weapon reload in inventory if passed required amount of time.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroAutoReloadTime_357 = CreateConVar("necro_autoreloadtime_357", "3", "Amount of time in seconds before weapon_357 automatic reload is performed if the weapon is idle in inventory.");
	g_ConvarNecroAutoReloadTime_Glock = CreateConVar("necro_autoreloadtime_glock", "3", "Amount of time in seconds before weapon_glock automatic reload is performed if the weapon is idle in inventory.");
	g_ConvarNecroAutoReloadTime_Assassin_Glock = CreateConVar("necro_autoreloadtime_assassin_glock", "3", "Amount of time in seconds before weapon_assassin_glock automatic reload is performed if the weapon is idle in inventory.");
	g_ConvarNecroAutoReloadTime_Mp5 = CreateConVar("necro_autoreloadtime_mp5", "3", "Amount of time in seconds before weapon_mp5 automatic reload is performed if the weapon is idle in inventory.");
	g_ConvarNecroAutoReloadTime_Shotgun = CreateConVar("necro_autoreloadtime_shotgun", "3", "Amount of time in seconds before weapon_shotgun automatic reload is performed if the weapon is idle in inventory.");
	g_ConvarNecroAutoReloadTime_Crossbow = CreateConVar("necro_autoreloadtime_crossbow", "3", "Amount of time in seconds before weapon_crossbow automatic reload is performed if the weapon is idle in inventory.");
	g_ConvarNecroCreateNewViewmodel = CreateConVar("necro_createnewviewmodel", "1", "Before we give new weapon, certain code may want to kill and create new weapon model to avoid prediction issues.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroAllowPickupObjects = CreateConVar("necro_allowpickupobjects", "0", "Enable the ability to pick up certain objects, such as prop_physics and prop_ragdoll.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroBlockRestoreWorld = CreateConVar("necro_blockrestoreworld", "0", "Block world restore after warmup intermission time.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroBlockRestoreWorldRespawnPlayers = CreateConVar("necro_blockrestoreworldrespawnplayers", "0", "Block all players respawn after warmup intermission time.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroWaterBulletEnable = CreateConVar("necro_waterbulletenable", "0", "Enable water bullet effects.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroCoopSnarks = CreateConVar("necro_coopsnarks", "0", "Enable SourceCoop snark AI version.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroForceAutoModeForCSM = CreateConVar("necro_forceautomodeforcsm", "1", "Fix static cascade shadows not appearing by forcing auto.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroHevSuitSounds = CreateConVar("necro_hevsuitsounds", "0", "Allow HEV Suit voice lines from the campaign.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroSpawnProtectionColorRed = CreateConVar("necro_spawnprotectioncolorred", "0", "Red color value for spawn protection that appears with player spawn.", 0, true, 0.0, true, 255.0);
	g_ConvarNecroSpawnProtectionColorBlue = CreateConVar("necro_spawnprotectioncolorblue", "234", "Blue color value for spawn protection that appears with player spawn.", 0, true, 0.0, true, 255.0);
	g_ConvarNecroSpawnProtectionColorGreen = CreateConVar("necro_spawnprotectioncolorgreen", "255", "Green color value for spawn protection that appears with player spawn.", 0, true, 0.0, true, 255.0);
	g_ConvarNecroSpawnProtectionColorAlpha = CreateConVar("necro_spawnprotectioncoloralpha", "255", "Alpha color value for spawn protection that appears with player spawn.", 0, true, 0.0, true, 255.0);
	g_ConvarNecroSpawnProtectionRenderFX = CreateConVar("necro_spawnprotectionrenderfx", "16", "Render effects for spawn protection that appears with player spawn.");
	g_ConvarNecroUseSuicidePenalty = CreateConVar("necro_usesuicidepenalty", "0", "Enable suicide penalty.", 0, true, 0.0, true, 1.0);
	//g_ConvarNecroForceShotgunFireInReload = CreateConVar("necro_forceshotgunfireinreload", "1", "Force shotgun to fire even if in reload.", 0, true, 0.0, true, 1.0);
	//g_ConvarNecroMp5ContactBlockSecondAttack = CreateConVar("necro_mp5contactblocksecondattack", "1", "Block secondary attack for MP5 contact grenade's owner if holding MP5 while the grenade is alive.", 0, true, 0.0, true, 1.0);

	//Load custom modes
	LoadCustomGameModes();

	//hook convars
	g_ConvarNecroFastRespawnDelay.AddChangeHook(OnFastRespawnDelayChanged);
	g_ConvarNecroSpectatorJoinTeamDelay.AddChangeHook(OnSpectatorJoinTeamDelayChanged);
	HookConVarChange(FindConVar("host_timescale"), OnHostTimeScaleChanged);
	AddCommandListener(Listener_Jointeam, "jointeam"); //note: it was planned to use player_team event hook, but it doesn't store team nums (always 0)  
	AddCommandListener(Listener_Say, "say");

	//player sounds fixes
	AddNormalSoundHook(PlayerSoundListener);
	
	//HACK! Use first (unused) element in the array to store cvar value
	g_fClientFastRespawnDelay[0] = GetConVarFloat(g_ConvarNecroFastRespawnDelay);
	g_fClientSpectatorJoinTeamDelay[0] = GetConVarFloat(g_ConvarNecroSpectatorJoinTeamDelay);
	
	gGreetedAuthIds = new StringMap();
	HookEvent("player_disconnect", OnPlayerDisconnect_CleanGreet);

	//Load detours offsets + some vars from memory
	LoadGameData();

	LoadTranslations("necro.phrases");
}

void LoadCustomGameModes()
{
	LoadGunGame();
}

//Purpose: Load our main game config file and offsets + some vars from memory
void LoadGameData()
{
	//Load our main game config file
	GameData pGameConfig_Necro = LoadGameConfigFile("necro.games");
	GameData pGameConfig_Srccoop = LoadGameConfigFile("srccoop.games");
	
	//Check if is valid 
	if (pGameConfig_Necro == null || pGameConfig_Srccoop == null)
		SetFailState("Couldn't load one of the configs!!!");

	InitClassdef(pGameConfig_Srccoop);

	LoadDHookVirtual(pGameConfig_Srccoop, hkLevelInit, "CServerGameDLL::LevelInit");
	if (hkLevelInit.HookRaw(Hook_Pre, IServerGameDLL.Get().GetAddress(), Hook_OnLevelInit) == INVALID_HOOK_ID)
		SetFailState("Could not hook CServerGameDLL::LevelInit");

	//Detours
	LoadDHookDetour(pGameConfig_Necro, hkGiveDefaultItems, "CBlackMesaPlayer::GiveDefaultItems", Hook_GiveDefaultItems);
	LoadDHookDetour(pGameConfig_Necro, hkBaseCombatWeaponPrecache, "CBaseCombatWeapon::Precache", Hook_BaseCombatWeaponPrecache, Hook_BaseCombatWeaponPrecachePost);
	LoadDHookDetour(pGameConfig_Necro, hkToggleIronsights, "CBlackMesaBaseWeaponIronSights::ToggleIronSights", Hook_ToggleIronsights);	
	LoadDHookDetour(pGameConfig_Necro, hkStartLagCompensation, "CLagCompensationManager::StartLagCompensation", Hook_StartLagCompensation);
	//LoadDHookDetour(pGameConfig_Necro, hkGetUserSettings, "CBaseClient::GetUserSetting", Hook_GetUserSettings);
	LoadDHookDetour(pGameConfig_Necro, hkPostChatMessage, "CBlackMesaKillStreaks::PostChatMessage", Hook_PostChatMessage);
	LoadDHookDetour(pGameConfig_Srccoop, hkPropBreakableRagdollInitRagdoll, "CPropBreakableRagdoll::InitRagdoll", Hook_PropBreakableRagdollInitRagdoll, Hook_PropBreakableRagdollInitRagdollPost);
	//LoadDHookDetour(pGameConfig_Srccoop, hkKeyValue, "CBaseEntity::KeyValue", Hook_BaseEntityKeyValue);
	LoadDHookDetour(pGameConfig_Necro, hkNoteWeaponFired, "CBlackMesaPlayer::NoteWeaponFired", Hook_NoteWeaponFired);

	#if defined PLAYERPATCH_SUIT_SOUNDS
	LoadDHookDetour(pGameConfig_Necro, hkCheckSuitUpdate, "CBasePlayer::CheckSuitUpdate", Hook_CheckSuitUpdate, Hook_CheckSuitUpdatePost);
	#endif

	//Offsets
	LoadDHookVirtual(pGameConfig_Necro, hkAcceptInput, "CBaseEntity::AcceptInput");
	LoadDHookVirtual(pGameConfig_Necro, hkWeaponCrossbowFireBolt, "CWeapon_Crossbow::FireBolt");
	LoadDHookVirtual(pGameConfig_Necro, hkBaseCombatPrimaryAttack, "CBaseCombatWeapon::PrimaryAttack");
	LoadDHookVirtual(pGameConfig_Necro, hkBaseCombatSecondaryAttack, "CBaseCombatWeapon::SecondaryAttack");
	LoadDHookVirtual(pGameConfig_Necro, hkBaseCombatReload, "CBaseCombatWeapon::Reload");
	LoadDHookVirtual(pGameConfig_Necro, hkBaseCombatHasAnyAmmo, "CBaseCombatWeapon::HasAnyAmmo");
	LoadDHookVirtual(pGameConfig_Necro, hkBaseCombatDeploy, "CBaseCombatWeapon::Deploy");
	LoadDHookVirtual(pGameConfig_Necro, hkPlayerSpawn, "CBasePlayer::Spawn");
	LoadDHookVirtual(pGameConfig_Necro, hkFlashlightOff, "CBlackMesaPlayer::FlashlightTurnOff");
	LoadDHookVirtual(pGameConfig_Necro, hkFlashlightOn, "CBlackMesaPlayer::FlashlightTurnOn");
	LoadDHookVirtual(pGameConfig_Necro, hkBaseCombatWeaponHolster, "CBaseCombatWeapon::Holster");
	LoadDHookVirtual(pGameConfig_Necro, hkBaseCombatWeaponItemHolsterFrame, "CBaseCombatWeapon::ItemHolsterFrame");
	LoadDHookVirtual(pGameConfig_Necro, hkBlackMesaPlayerCreateAmmoBox, "CBlackMesaPlayer::CreateAmmoBox");
	LoadDHookVirtual(pGameConfig_Srccoop, hkBlackMesaPlayerPickupObject, "CBlackMesaPlayer::PickupObject");
	LoadDHookVirtual(pGameConfig_Srccoop, hkIsValidEnemy, "CAI_BaseNPC::IsValidEnemy");
	LoadDHookVirtual(pGameConfig_Srccoop, hkUpdateEnemyMemory, "CAI_BaseNPC::UpdateEnemyMemory");
	//LoadDHookVirtual(pGameConfig_Necro, hkBlackMesaBaseDetonatorExplodeTouch, "CBlackMesaBaseDetonator::ExplodeTouch");
	LoadDHookVirtual(pGameConfig_Necro, hkUnfreezeAllPlayers, "CBM_MP_GameRules::UnfreezeAllPlayers");
	LoadDHookVirtual(pGameConfig_Necro, hkFreezeAllPlayers, "CBM_MP_GameRules::FreezeAllPlayers");
	LoadDHookVirtual(pGameConfig_Necro, hkUseSuicidePenalty, "CMultiplayRules::UseSuicidePenalty");
	
	//Memory Vars
	g_iUserCmdOffset = pGameConfig_Necro.GetOffset("CBasePlayer::GetCurrentUserCommand");
	
	pGameConfig_Necro.Close();

	LoadGameData_Srccoop(pGameConfig_Srccoop);

	#if defined ENTPATCH_BARNACLE_PREDICTION
	HookEntityOutput("npc_barnacle", "OnGrab", Hook_Barnacle_OnGrab);
	HookEntityOutput("npc_barnacle", "OnRelease", Hook_Barnacle_OnRelease);
	#endif
}

//Purpose: Load SourceCoop game config file offsets + some vars from memory
void LoadGameData_Srccoop(const GameData pGameConfig)
{
	g_serverOS = view_as<OperatingSystem>(pGameConfig.GetOffset("_OS_Detector_"));

	LoadDHookVirtual(pGameConfig, hkLevelInit, "CServerGameDLL::LevelInit");
	LoadDHookVirtual(pGameConfig, hkChangeTeam, "CBasePlayer::ChangeTeam");
	LoadDHookVirtual(pGameConfig, hkShouldCollide, "CBaseEntity::ShouldCollide");
	LoadDHookVirtual(pGameConfig, hkPlayerSpawn, "CBasePlayer::Spawn");
	LoadDHookVirtual(pGameConfig, hkSetModel, "CBaseEntity::SetModel");
	LoadDHookVirtual(pGameConfig, hkAcceptInput, "CBaseEntity::AcceptInput");
	LoadDHookVirtual(pGameConfig, hkThink, "CBaseEntity::Think");
	LoadDHookVirtual(pGameConfig, hkUpdateOnRemove, "CBaseEntity::UpdateOnRemove");
	LoadDHookVirtual(pGameConfig, hkEvent_Killed, "CBaseEntity::Event_Killed");
	LoadDHookVirtual(pGameConfig, hkKeyValue_char, "CBaseEntity::KeyValue_char");
	
	#if defined SRCCOOP_BLACKMESA
	LoadDHookDetour(pGameConfig, hkGiveDefaultItems, "*Player::GiveDefaultItems", Hook_GiveDefaultItems);
	#endif

	#if defined ENTPATCH_PLAYER_ALLY
	LoadDHookVirtual(pGameConfig, hkIsPlayerAlly, "CAI_BaseNPC::IsPlayerAlly");
	#endif

	#if defined ENTPATCH_NAVIGATION_URGENT
	LoadDHookVirtual(pGameConfig, hkIsNavigationUrgent, "CAI_BaseNPC::IsNavigationUrgent");
	#endif

	#if defined ENTPATCH_NPC_DIALOGUE
	LoadDHookVirtual(pGameConfig, hkFindNamedEntity, "CSceneEntity::FindNamedEntity");
	LoadDHookVirtual(pGameConfig, hkFindNamedEntityClosest, "CSceneEntity::FindNamedEntityClosest");
	LoadDHookDetour(pGameConfig, hkExpresserHostDoModifyOrAppendCriteria, "CAI_ExpresserHost_NPC_DoModifyOrAppendCriteria", _, Hook_ExpresserHost_DoModifyOrAppendCriteriaPost);
	#endif

	#if defined ENTPATCH_SNIPER
	LoadDHookVirtual(pGameConfig, hkProtoSniperSelectSchedule, "CProtoSniper::SelectSchedule");
	#endif

	#if defined GAMEPATCH_ALLOW_FLASHLIGHT
	LoadDHookVirtual(pGameConfig, hkFAllowFlashlight, "CMultiplayRules::FAllowFlashlight");
	#endif

	#if defined GAMEPATCH_IS_MULTIPLAYER
	LoadDHookVirtual(pGameConfig, hkIsMultiplayer, "CMultiplayRules::IsMultiplayer");
	#endif

	#if defined GAMEPATCH_BLOCK_RESTOREWORLD
	LoadDHookVirtual(pGameConfig, hkRestoreWorld, "CBM_MP_GameRules::RestoreWorld");
	#endif

	#if defined GAMEPATCH_BLOCK_RESPAWNPLAYERS
	LoadDHookVirtual(pGameConfig, hkRespawnPlayers, "CBM_MP_GameRules::RespawnPlayers");
	#endif

	#if defined SRCCOOP_BLACKMESA
	LoadDHookVirtual(pGameConfig, hkOnTryPickUp, "CBasePickup::OnTryPickUp");
	#endif

	#if defined ENTPATCH_BM_ICHTHYOSAUR
	LoadDHookVirtual(pGameConfig, hkIchthyosaurIdleSound, "CNPC_Ichthyosaur::IdleSound");
	#endif
	
	#if defined ENTPATCH_NPC_RUNTASK
	LoadDHookVirtual(pGameConfig, hkBaseNpcRunTask, "CAI_BaseNPC::RunTask");
	#endif
	
	#if defined ENTPATCH_BM_PROP_CHARGERS
	LoadDHookVirtual(pGameConfig, hkPropChargerThink, "CPropChargerBase::ChargerThink");
	#endif

	#if defined PLAYERPATCH_RESTORE_MP_FORCERESPAWN
	LoadDHookVirtual(pGameConfig, hkForceRespawn, "CBasePlayer::ForceRespawn");
	#endif

	#if defined PLAYERPATCH_OVERRIDE_DEATH_OBSMODE
	LoadDHookVirtual(pGameConfig, hkStartObserverMode, "CBasePlayer::StartObserverMode");
	#endif
	
	#if defined PLAYERPATCH_HITREG
	LoadDHookVirtual(pGameConfig, hkPlayerWeaponShootPosition, "CBasePlayer::Weapon_ShootPosition");
	#endif
	
	#if defined ENTPATCH_GOALENTITY_RESOLVENAMES
	LoadDHookDetour(pGameConfig, hkResolveNames, "CAI_GoalEntity::ResolveNames", Hook_ResolveNames, Hook_ResolveNamesPost);
	#endif

	#if defined ENTPATCH_GOAL_LEAD
	LoadDHookDetour(pGameConfig, hkCanSelectSchedule, "CAI_LeadBehavior::CanSelectSchedule", Hook_CanSelectSchedule);
	#endif

	#if defined ENTPATCH_SETPLAYERAVOIDSTATE
	LoadDHookDetour(pGameConfig, hkSetPlayerAvoidState, "CAI_BaseNPC::SetPlayerAvoidState", Hook_SetPlayerAvoidState);
	#endif

	#if defined PLAYERPATCH_SUIT_SOUNDS
	LoadDHookDetour(pGameConfig, hkSetSuitUpdate, "CBasePlayer::SetSuitUpdate", Hook_SetSuitUpdate, Hook_SetSuitUpdatePost);
	#endif

	#if defined ENTPATCH_NPC_SLEEP
	LoadDHookDetour(pGameConfig, hkBaseNpcUpdateSleepState, "CAI_BaseNPC::UpdateSleepState", Hook_BaseNpcUpdateSleepState);
	#endif

	#if defined GAMEPATCH_UTIL_GETLOCALPLAYER
	LoadDHookDetour(pGameConfig, hkUTIL_GetLocalPlayer, "UTIL_GetLocalPlayer", Hook_UTIL_GetLocalPlayer);
	#endif

	#if defined PLAYERPATCH_PICKUP_FORCEPLAYERTODROPTHISOBJECT
	LoadDHookDetour(pGameConfig, hkPickup_ForcePlayerToDropThisObject, "Pickup_ForcePlayerToDropThisObject", Hook_ForcePlayerToDropThisObject);
	#endif

	#if defined ENTPATCH_NPC_THINK_LOCALPLAYER
	LoadDHookDetour(pGameConfig, hkPhysics_RunThinkFunctions, "Physics_RunThinkFunctions", Hook_Physics_RunThinkFunctions);
	#endif

	#if defined ENTPATCH_BM_DISSOLVE
	LoadDHookDetour(pGameConfig, hkDissolve, "CBaseAnimating::Dissolve", Hook_Dissolve);
	#endif

	#if defined ENTPATCH_WATERBULLET
	LoadDHookDetour(pGameConfig, hkHandleShotImpactingWater, "CBaseEntity::HandleShotImpactingWater", Hook_HandleShotImpactingWater, Hook_HandleShotImpactingWaterPost);
	LoadDHookDetour(pGameConfig, hkFireBullets, "CBaseEntity::FireBullets", Hook_FireBullets, Hook_FireBulletsPost);
	LoadDHookVirtual(pGameConfig, hkFindNamedEntity, "CSceneEntity::FindNamedEntity");
	LoadDHookVirtual(pGameConfig, hkUpdateTransmitState, "CBaseEntity::UpdateTransmitState");
	#endif

	#if defined ENTPATCH_BM_SNARK
	LoadDHookVirtual(pGameConfig, hkIRelationType, "CBaseCombatCharacter::IRelationType");
	#endif

	#if defined SRCCOOP_BLACKMESA
	LoadDHookDetour(pGameConfig, hkEventQueueAddEvent, "CEventQueue::AddEvent", Hook_EventQueueAddEvent_GetAddress);
	#endif
	
	#if defined GAMEPATCH_UTIL_FINDCLIENT
	if (g_serverOS == OS_Windows)
	{
		LoadDHookDetour(pGameConfig, hkUtilFindClientInPVSGuts, "UTIL_FindClientInPVSGuts", Hook_UTIL_FindClient);
	}
	else if (g_serverOS == OS_Linux)
	{
		//`UTIL_FindClientInPVSGuts` is inlined on Linux into these functions.
		LoadDHookDetour(pGameConfig, hkUtilFindClientInPVS, "UTIL_FindClientInPVS", Hook_UTIL_FindClient);
		LoadDHookDetour(pGameConfig, hkUtilFindClientInVisibilityPVS, "UTIL_FindClientInVisibilityPVS", Hook_UTIL_FindClient);
	}
	#endif

	#if defined ENTPATCH_SCRIPTED_SEQUENCE
	LoadDHookDetour(pGameConfig, hkScriptedSequenceStartScript, "CAI_ScriptedSequence::StartScript", Hook_ScriptedSequenceStartScript);
	#endif

	#if defined GAMEPATCH_PREDICTED_EFFECTS
	LoadDHookDetour(pGameConfig, hkIgnorePredictionCull, "CRecipientFilter::IgnorePredictionCull", Hook_IgnorePredictionCull);
	LoadDHookVirtual(pGameConfig, hkDispatchEffect, "CTempEntsSystem::DispatchEffect");
	if (hkDispatchEffect.HookRaw(Hook_Pre, IServerTools.Get().GetTempEntsSystem(), Hook_DispatchEffect) == INVALID_HOOK_ID)
		SetFailState("Could not hook CTempEntsSystem::DispatchEffect");
	#endif

	#if defined SRCCOOP_BLACKMESA
	if (g_serverOS == OS_Linux)
	{
		LoadDHookDetour(pGameConfig, hkAccumulatePose, "CBoneSetup::AccumulatePose", Hook_AccumulatePose);
		LoadDHookDetour(pGameConfig, hkTestGroundMove, "CAI_MoveProbe::TestGroundMove", Hook_TestGroundMove);
	}
	#endif

	// Memory Patches
	g_pCoopModeMemPatchList = new ArrayList();
	
	#if defined ENTPATCH_LAGCOMP_POSE_PARAMS
	LoadMemPatch(pGameConfig, "CLagCompensationManager::RestoreEntityFromRecords::SetPoseParameter", true, false);
	LoadMemPatch(pGameConfig, "CLagCompensationManager::BacktrackEntity::SetPoseParameter", true, false);
	#endif
	
	#if defined GAMEPATCH_BM_GRAVITY
	g_pCoopModeMemPatchList.Push(LoadMemPatch(pGameConfig, "CBM_MP_GameRules::Activate::DoNotHardCodeGravityThnx", false, false));
	#endif
	
	pGameConfig.Close();
}

public void OnPlayerDisconnect_CleanGreet(Event event, const char[] name, bool dontBroadcast)
{
	int client = GetClientOfUserId(GetEventInt(event, "userid"));

	//player gone from server, we can greet the player again 
	if (CBlackMesaPlayer(client).IsValid())
	{
		char authId[MAX_AUTHID_LENGTH];
		GetClientAuthId(client, AuthId_Engine, authId, sizeof(authId));
		gGreetedAuthIds.Remove(authId);
	}
}

public MRESReturn Hook_OnLevelInit(DHookReturn hReturn, DHookParam hParams)
{
	OnMapEnd(); // this does not always get called, so call it here

	char szMapName[MAX_MAPNAME];
	hParams.GetString(1, szMapName, sizeof(szMapName));
	g_szPrevMapName = g_szMapName;
	g_szMapName = szMapName;

	static char szMapEntities[ENTITYSTRING_LENGTH];
	hParams.GetString(2, szMapEntities, sizeof(szMapEntities));

	//save original string for dumps
	g_szEntityString = szMapEntities;

	SwitchGameModes();

	return MRES_Ignored;
}

//this is our schedule based gamemode pool system
public void SwitchGameModes()
{
    char hour[4];  
    FormatTime(hour, sizeof(hour), "%H"); //24 hour format
    int currentHour = StringToInt(hour);

	//pick one of the valid gamemodes based on the current hour
    if (IsInSchedule(g_iShedTimeDeathmatch, currentHour))
    {
        GameModeToggle_Dm(true, false);
		g_ngmCurrentGameMode = MODE_DEATHMATCH;
    }
    else if (IsInSchedule(g_iShedTimeTeamDeathmatch, currentHour))
    {
        GameModeToggle_Dm(true, true);
		g_ngmCurrentGameMode = MODE_TEAMDEATHMATCH;
    }
    else if (IsInSchedule(g_iShedTimeGunGame, currentHour))
    {
        GameModeToggle_GunGame(true, false);
		g_ngmCurrentGameMode = MODE_GUNGAME;
    }
    else if (IsInSchedule(g_iShedTimeTeamGunGame, currentHour))
    {
        GameModeToggle_GunGame(true, true);
		g_ngmCurrentGameMode = MODE_TEAMGUNGAME;
    }
}

//Purpose: easy way to define by shed arrays what gamemode we should load, сode by Gemini (AI)
bool IsInSchedule(int schedule[3][2], int currentHour)
{
    for (int i = 0; i < 3; i++)
    {
        // check: hour >= start AND hour < end
        if (currentHour >= schedule[i][0] && currentHour < schedule[i][1])
        {
            // save boundaries to global variables
            g_iShedMinTime = schedule[i][0];
            g_iShedMaxTime = schedule[i][1];
            return true;
        }
    }
    return false;
}

//Purpose: Load  gamerule offsets to control various game mechanics when map is loaded 
public void OnMapStart()
{
	//gamerules hooks
	DHookGamerules(hkFAllowFlashlight, false, _, Hook_FAllowFlashlight);
	DHookGamerules(hkIsMultiplayer, false, _, Hook_IsMultiplayer);

	#if defined GAMEPATCH_BLOCK_RESTOREWORLD
	DHookGamerules(hkRestoreWorld, false, _, Hook_RestoreWorld);
	DHookGamerules(hkRestoreWorld, true, _, Hook_RestoreWorldPost);
	#endif

	#if defined GAMEPATCH_BLOCK_RESPAWNPLAYERS
	DHookGamerules(hkRespawnPlayers, false, _, Hook_RespawnPlayers);
	#endif

	DHookGamerules(hkFreezeAllPlayers, true, _, Hook_FreezeAllPlayers);
	DHookGamerules(hkUnfreezeAllPlayers, false, _, Hook_UnfreezeAllPlayers);

	DHookGamerules(hkUseSuicidePenalty, false, _, Hook_UseSuicidePenalty);

	MapStartCustomGameModes();

	MapStartServerMessages();

	g_bMapStarted = true;
}

void MapStartCustomGameModes()
{
	GunGameMapStart();
}

void MapStartServerMessages()
{
	//CreateTimer(GUNGAME_VOTEINFO_DELAY, Timer_GunGameServerMessage, _, TIMER_FLAG_NO_MAPCHANGE); 
}

public void OnMapEnd()
{
	for (int i = 1; i <= MaxClients; i++)
	{
		g_bPAFTFT[i] = true;
	}

	MapEndCustomModes();
	g_bMapStarted = false;
}

void MapEndCustomModes()
{
	GunGameMapEnd();
}

void GameModeToggle_Dm(bool bEnable, bool bTeam)
{
	if (bEnable)
	{
		SetConVarBool(g_ConvarNecroGunGameEnabled, false);
	}

	if (bTeam)
	{
		SetConVarBool(mp_teamplay, true);
	}
	else
	{
		SetConVarBool(mp_teamplay, false);
	}
}

void GameModeToggle_GunGame(bool bEnable, bool bTeam)
{
	if (bEnable)
	{
		SetConVarBool(g_ConvarNecroGunGameEnabled, true);
	}
	else
	{
		SetConVarBool(g_ConvarNecroGunGameEnabled, false);
	}

	if (bTeam)
	{
		SetConVarBool(mp_teamplay, true);
	}
	else
	{
		SetConVarBool(mp_teamplay, false);
	}
}

public void OnClientAuthorized(int client, const char[] auth)
{
	int sid = GetSteamAccountID(client, false);
	if (sid)
	{
		IntToString(sid, g_szSteamIds[client], sizeof(g_szSteamIds[]));
	}
}

public void OnClientPutInServer(int client)
{
	if (IsFakeClient(client))
		return;

	if (!g_iPlayerCount++)
	{
		#if defined ENTPATCH_NPC_THINK_LOCALPLAYER
		// resume entity thinking
		hkPhysics_RunThinkFunctions.Disable(Hook_Pre, Hook_Physics_RunThinkFunctions);
		#endif
	}
	
	DHookEntity(hkPlayerSpawn, false, client, _, Hook_PlayerSpawn);
	DHookEntity(hkPlayerSpawn, true, client, _, Hook_PlayerSpawnPost);
	DHookEntity(hkFlashlightOff, false, client, _, Hook_FlashlightOff);
	DHookEntity(hkFlashlightOn, false, client, _, Hook_FlashlightOn);
	DHookEntity(hkEvent_Killed, false, client, _, Hook_PlayerKilled);
	DHookEntity(hkEvent_Killed, true, client, _, Hook_PlayerKilledPost);
	DHookEntity(hkBlackMesaPlayerPickupObject, false, client, _, Hook_PickupObject);
	DHookEntity(hkBlackMesaPlayerPickupObject, true, client, _, Hook_PickupObjectPost);


	#if defined PLAYERPATCH_HITREG
	DHookEntity(hkPlayerWeaponShootPosition, true, client, _, Hook_PlayerWeaponShootPosition_Post);
	#endif
	DHookEntity(hkChangeTeam, false, client, _, Hook_PlayerChangeTeam); //maybe will be used later 
	DHookEntity(hkChangeTeam, true, client, _, Hook_PlayerChangeTeamPost);
	DHookEntity(hkShouldCollide, false, client, _, Hook_PlayerShouldCollide);
	DHookEntity(hkAcceptInput, false, client, _, Hook_PlayerAcceptInput);
	DHookEntity(hkForceRespawn, false, client, _, Hook_PlayerForceRespawn);
	DHookEntity(hkStartObserverMode, false, client, _, Hook_PlayerStartObserverMode);

	SDKHook(client, SDKHook_PreThinkPost, Hook_PlayerPreThinkPost);
	SDKHook(client, SDKHook_PreThink, Hook_PlayerPreThink);

	//reset these vars
	g_fClientFastRespawnDelay[client] = 0.0;
	g_fClientSpectatorJoinTeamDelay[client] = 0.0;

	//disable "other players flashlight"
	SendConVarValue(client, sv_cheats, "1"); //HACK! This thing needs cheats ON, enable cheats ON, disable the thing, set cheats OFF after 
	ClientCommand(client, "r_flashlight_3rd_draw 0");
	ClientCommand(client, "developer 0"); //now you will not see tau cannon debug beams

	// `item_ammo_canister` has a client side dlight that will
	// always appear even if the ammo canister is not being transmitted.
	// If this ConVar is set too late, then the dlight will have already been
	// created on the client in a frozen spot.
	#if defined SRCCOOP_BLACKMESA
	CBasePlayer pPlayer = CBasePlayer(client);
	pPlayer.SendCommand("cl_ammo_box_dlights 0");
	pPlayer.SetUserData("m_flProtectionTime", -1.0);
	#endif
}

public void OnClientPostAdminCheck(int iClient)
{
	GreetPlayer(iClient);
}

void GreetPlayer(int iClient)
{
	//delay it so it can display
	CreateTimer(5.0, Timer_GreetPlayerWelcome, iClient, TIMER_FLAG_NO_MAPCHANGE);
}

public Action Timer_GreetPlayerWelcome(Handle timer, int iClient)
{
	CBlackMesaPlayer pPlayer = CBlackMesaPlayer(iClient);

	//the client isn't valid anymore ? go out
	if (!pPlayer.IsValid())
		return Plugin_Stop;

	char authId[MAX_AUTHID_LENGTH];
	GetClientAuthId(iClient, AuthId_Engine, authId, sizeof(authId));

	//check if already greet here, cuz the timer can be removed after map change, so we didn't greet the player
	bool exists;
	if (gGreetedAuthIds.GetValue(authId, exists))
		return Plugin_Stop;

	//add the player so we don't greet the player anymore
	gGreetedAuthIds.SetValue(authId, true);

	int col1[4] = {255, 255, 255, 255}; //main col (final)
	int col2[4] = {0, 0, 0, 255}; //scnd col (when printed)

	//pick col by team (green for marines)
	if (pPlayer.GetTeam() != TEAM_ONE)
	{
		col1 = {255, 194, 6, 255};
		col2 = {189, 57, 20, 255};
	}
	else
	{
		col1 = {0, 255, 0, 255};
		col2 = {29, 160, 91, 255};
	}

	//setup text
	SetHudTextParamsEx(-1.0, 0.05, 15.0, col1, col2, 2, 5.0, 0.2, 0.2);

	//get client user name
	char szClientName[MAX_NAME_LENGTH];
	GetClientName(iClient, szClientName, sizeof(szClientName));

	//print the welcome msg
	ShowHudText(iClient, 5, "%t", "#Necro_WelcomeMsg", szClientName);

	return Plugin_Stop;
}

public void OnConfigsExecuted()
{
	RequestFrame(OnConfigsExecutedPost); // prevents a bug where this is fired too early if map changes in OnMapStart
}

public void OnConfigsExecutedPost()
{
	#if defined SRCCOOP_BLACKMESA
	PrecacheScriptSound("HL2Player.SprintStart");

	#if defined ENTPATCH_BM_XENTURRET
	AddFileToDownloadsTable("models/props_xen/xen_turret_mpfix.dx80.vtx");
	AddFileToDownloadsTable("models/props_xen/xen_turret_mpfix.dx90.vtx");
	AddFileToDownloadsTable("models/props_xen/xen_turret_mpfix.mdl");
	AddFileToDownloadsTable("models/props_xen/xen_turret_mpfix.phy");
	AddFileToDownloadsTable("models/props_xen/xen_turret_mpfix.sw.vtx");
	AddFileToDownloadsTable("models/props_xen/xen_turret_mpfix.vvd");
	#endif

	#endif // SRCCOOP_BLACKMESA

	char szDownloadContent[PLATFORM_MAX_PATH];

	//Weapon scripts
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_357_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_ASSASSIN_GLOCK_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_CROSSBOW_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_CROWBAR_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_FRAG_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_GLOCK_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_GLUON_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_HEADCRAB_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_HIVEHAND_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_MP5_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_RPG_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_SATCHEL_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_SHOTGUN_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_SNARK_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_TAU_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
	Format(szDownloadContent, sizeof(szDownloadContent), "scripts/gameplay/weapons/%s.dmx", WEAPON_TRIPMINE_SERVER);
	AddFileToDownloadsTable(szDownloadContent);
}

//Purpose: Hook entity creation to setup our custom entity hooks
public void OnEntityCreated(int iEntIndex, const char[] szClassname)
{
	if (CBaseEntity(iEntIndex).IsWeapon())
		DHookEntity(hkSetModel, false, iEntIndex, _, Hook_WeaponSetModel);

	CBaseEntity pEntity = CBaseEntity(iEntIndex);
	if (pEntity == NULL_CBASEENTITY)
		return;

	bool bIsNPC = pEntity.IsNPC();

	//char szClassname[MAX_CLASSNAME];
	//GetEntityClassname(iEntIndex, szClassname, sizeof(szClassname));

	#if defined ENTPATCH_WATERBULLET
	if(StrEqual(szClassname, "waterbullet"))
	{
		if (g_ConvarNecroWaterBulletEnable.BoolValue)
		{
			SDKHook(iEntIndex, SDKHook_Spawn, WaterBullet_Path);
			DHookEntity(hkUpdateTransmitState, false, iEntIndex, _, Hook_WaterBulletUpdateTransmitState);
		}
		return;
	}
	#endif

	if(StrEqual(szClassname, "npc_snark"))
	{
		SDKHook(iEntIndex, SDKHook_SpawnPost, Snark_PathPost);
		DHookEntity(hkUpdateOnRemove, false, iEntIndex, _, Hook_SnarkUpdateOnRemove);
		#if defined ENTPATCH_BM_SNARK
		DHookEntity(hkIsValidEnemy, false, iEntIndex, _, Hook_SnarkIsValidEnemy);
		DHookEntity(hkIsPlayerAlly, false, iEntIndex, _, Hook_SnarkIsPlayerAlly);
		#endif
		return;
	}

	if(StrEqual(szClassname, "grenade_hornet"))
	{
		SDKHook(iEntIndex, SDKHook_SpawnPost, Hornet_PathPost);
		DHookEntity(hkUpdateOnRemove, false, iEntIndex, _, Hook_HornetUpdateOnRemove);
		return;
	}

	#if defined ENTPATCH_BM_XENTURRET
	if (strcmp(szClassname, "npc_xenturret") == 0)
	{
		SDKHook(iEntIndex, SDKHook_SpawnPost, Hook_XenTurretSpawnPost);
		return;
	}
	#endif

	#if defined ENTPATCH_BM_LAV
	if (strcmp(szClassname, "npc_lav") == 0)
	{
		SDKHook(iEntIndex, SDKHook_SpawnPost, Hook_LAVSpawnPost);
		return;
	}
	#endif

	if (StrEqual(szClassname, "env_laser_dot"))
	{
		SDKHook(iEntIndex, SDKHook_Spawn, LaserDot_Path); //hide laser dot before using new rendering method
		SDKHook(iEntIndex, SDKHook_SpawnPost, LaserDot_PathPost); //set new rendering way after spawn
		return;
	}
		
	if (StrEqual(szClassname, "grenade_frag"))
	{
		SDKHook(iEntIndex, SDKHook_Spawn, Frag_Path); //Use VPhysics for frag grenade if classic frags is disabled
		SDKHook(iEntIndex, SDKHook_SpawnPost, Frag_PathPost); //reset multiplayer state back for this entity after setting VPhysics
		return;
	}
		
	if (StrEqual(szClassname, "grenade_mp5_contact"))
	{
		SDKHook(iEntIndex, SDKHook_Spawn, Mp5Contact_Path); //Add smoke particle effect to MP5 barrel grenade if enabled
		//DHookEntity(hkThink, false, iEntIndex, _, Hook_Mp5ContactThink);
		//DHookEntity(hkUpdateOnRemove, false, iEntIndex, _, Hook_Mp5ContactUpdateOnRemove);
		return;
	}

	if (strncmp(szClassname, "item_", 5) == 0 || strcmp(szClassname, "prop_soda", false) == 0)
	{
		if (pEntity.IsPickupItem())
		{
			SDKHook(iEntIndex, SDKHook_Spawn, Item_Path);
			SDKHook(iEntIndex, SDKHook_SpawnPost, Item_PathPost);
			DHookEntity(hkKeyValue_char, false, iEntIndex, _, Hook_BasePickupKeyValue);
		}

		if (strcmp(szClassname, "item_battery") == 0)
		{
			RequestFrame(Battery_Path, view_as<CItem>(pEntity));
		}

		return;
	}

	if (StrEqual(szClassname, "grenade_bolt"))
	{
		SDKHook(iEntIndex, SDKHook_Spawn, Bolt_Path);
	//	DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_GrenadeBoltAcceptInput); //Disable exploading bolt if is not allowed
		return;
	}

	//remove buggy sprite in the sky
	if (StrEqual(szClassname, "env_sun"))
	{
		pEntity.AcceptInput("Kill");
	}

	//HACK! HACK! This is a pretty bad way to get an address, I tried Init hook or getting address from config, but none of my signs worked 
	if(StrEqual(szClassname, "worldspawn"))
	{
		//don't if address is already loaded
		if (g_EventQueue != Address_Null)
			return;	

		pEntity.AddOutput("OnUser4", "!self", "FireUser4", "", 0.01, 1);
		pEntity.AcceptInput("FireUser4");
		return;
	}

	if (StrEqual(szClassname, "weapon_hivehand"))
	{
		PrecacheSound(")weapons/hivehand/pickup.wav"); //fix console spam
		return;
	}

	if (StrEqual(szClassname, "weapon_rpg"))
	{
		DHookEntity(hkBaseCombatWeaponHolster, true, iEntIndex, _, Hook_WeaponRpgHolsterPost);
		DHookEntity(hkBaseCombatDeploy, true, iEntIndex, _, Hook_WeaponRpgDeploy);
		DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_WeaponRpgAcceptInput);
		return;
	}

	if (StrEqual(szClassname, "weapon_glock"))
	{
		DHookEntity(hkBaseCombatWeaponHolster, true, iEntIndex, _, Hook_BaseCombatWeaponHolsterPost);
		DHookEntity(hkBaseCombatWeaponItemHolsterFrame, true, iEntIndex, _, Hook_WeaponGlockItemHolsterFramePost);
		return;
	}

	if (StrEqual(szClassname, "weapon_assassin_glock"))
	{
		DHookEntity(hkBaseCombatWeaponHolster, true, iEntIndex, _, Hook_BaseCombatWeaponHolsterPost);
		DHookEntity(hkBaseCombatWeaponItemHolsterFrame, true, iEntIndex, _, Hook_WeaponAssassinGlockItemHolsterFramePost);
		return;
	}

	if (StrEqual(szClassname, "weapon_357"))
	{
		DHookEntity(hkBaseCombatWeaponHolster, true, iEntIndex, _, Hook_BaseCombatWeaponHolsterPost);
		DHookEntity(hkBaseCombatWeaponItemHolsterFrame, true, iEntIndex, _, Hook_Weapon357ItemHolsterFramePost);
		return;		
	}

	if (StrEqual(szClassname, "weapon_mp5"))
	{
		DHookEntity(hkBaseCombatWeaponHolster, true, iEntIndex, _, Hook_BaseCombatWeaponHolsterPost);
		DHookEntity(hkBaseCombatWeaponItemHolsterFrame, true, iEntIndex, _, Hook_WeaponMp5ItemHolsterFramePost);
		return;
	}

	if (StrEqual(szClassname, "weapon_shotgun"))
	{
		pEntity.SetUserData("m_iClipType", 2);
		DHookEntity(hkBaseCombatWeaponHolster, true, iEntIndex, _, Hook_BaseCombatWeaponHolsterPost);
		DHookEntity(hkBaseCombatWeaponItemHolsterFrame, true, iEntIndex, _, Hook_WeaponShotgunItemHolsterFramePost);
		return;
	}

	if (StrEqual(szClassname, "env_sprite"))
	{
		#if defined ENTPATCH_ENV_SPRITE
		if (strcmp(szClassname, "env_sprite") == 0)
		{
			SDKHook(iEntIndex, SDKHook_SpawnPost, Hook_EnvSpriteSpawnPost);
			return;
		}
		#endif

		RequestFrame(Sprite_PathCanister, iEntIndex); //Control canister's sprite visibility depending on pick up state
													  //used RequestFrame because parent is not immediately set
		return;
	}

	if (StrEqual(szClassname, "weapon_crossbow"))
	{
		DHookEntity(hkBaseCombatWeaponHolster, true, iEntIndex, _, Hook_BaseCombatWeaponHolsterPost);
		DHookEntity(hkBaseCombatWeaponItemHolsterFrame, true, iEntIndex, _, Hook_WeaponCrossbowItemHolsterFramePost);
		DHookEntity(hkWeaponCrossbowFireBolt, false, iEntIndex, _, Hook_CrossbowFireBolt); //Use singleplayer rules for bolt creation to make sk_crossbow_tracer_enabled work
		DHookEntity(hkWeaponCrossbowFireBolt, true, iEntIndex, _, Hook_CrossbowFireBoltPost); //Set back multiplayer rules after bolt creation
		DHookEntity(hkBaseCombatDeploy, true, iEntIndex, _, Hook_CrossbowDeploy); //Set skin we need
		PrecacheSound("npc/sniper/sniper1_close.wav"); //fix console spam for sp bolts
		return;
	}

	if (StrEqual(szClassname, "weapon_satchel"))
	{
		//Set custom attack delays on different stages
		DHookEntity(hkBaseCombatPrimaryAttack, true, iEntIndex, _, Hook_SatchelPrimaryAttackPost);
		DHookEntity(hkBaseCombatSecondaryAttack, true, iEntIndex, _, Hook_SatchelSecondaryAttackPost);
		DHookEntity(hkBaseCombatReload, true, iEntIndex, _, Hook_SatchelReloadPost);
		return;
	}

	//the rest is SourceCoop code mainly 
	if (bIsNPC)
	{
		#if defined ENTPATCH_NPC_THINK_LOCALPLAYER
		DHookEntity(hkThink, false, iEntIndex, _, Hook_BaseNPCThink);
		DHookEntity(hkThink, true, iEntIndex, _, Hook_BaseNPCThinkPost);
		#endif
		
		#if defined ENTPATCH_NPC_RUNTASK
		DHookEntity(hkBaseNpcRunTask, false, iEntIndex, _, Hook_BaseNPCRunTask);
		DHookEntity(hkBaseNpcRunTask, true, iEntIndex, _, Hook_BaseNPCRunTaskPost);
		#endif
		
		#if defined ENTPATCH_UPDATE_ENEMY_MEMORY
		DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_BaseNPCAcceptInput);
		#endif

		#if defined ENTPATCH_CUSTOM_NPC_MODELS
		DHookEntity(hkKeyValue_char, false, iEntIndex, _, Hook_BaseNPCKeyValue);
		#endif
		
		#if defined ENTPATCH_SNIPER
		if (strcmp(szClassname, "npc_sniper", false) == 0 || strcmp(szClassname, "proto_sniper", false) == 0)
		{
			DHookEntity(hkProtoSniperSelectSchedule, false, iEntIndex, _, Hook_ProtoSniperSelectSchedule);
			return;
		}
		#endif

		#if defined SRCCOOP_BLACKMESA
		if (strncmp(szClassname, "npc_human_scientist", 19) == 0 || strcmp(szClassname, "npc_human_security") == 0 || strcmp(szClassname, "npc_human_eli") == 0 ||
			strcmp(szClassname, "npc_human_kleiner") == 0 || strcmp(szClassname, "npc_gman") == 0)
		{
			#if defined ENTPATCH_PLAYER_ALLY
			DHookEntity(hkIsPlayerAlly, false, iEntIndex, _, Hook_IsPlayerAlly);
			#endif
			
			#if defined ENTPATCH_NAVIGATION_URGENT
			DHookEntity(hkIsNavigationUrgent, false, iEntIndex, _, Hook_IsNavigationUrgent);
			#endif

			#if defined ENTPATCH_BM_SNARK
			DHookEntity(hkIsValidEnemy, false, iEntIndex, _, Hook_PlayerCompanionIsValidEnemy);
			DHookEntity(hkUpdateEnemyMemory, false, iEntIndex, _, Hook_UpdateEnemyMemory);
			#endif

			return;
		}
		#endif // SRCCOOP_BLACKMESA

		#if defined ENTPATCH_BM_ICHTHYOSAUR
		if (strcmp(szClassname, "npc_ichthyosaur") == 0)
		{
			DHookEntity(hkIchthyosaurIdleSound, false, iEntIndex, _, Hook_IchthyosaurIdleSound);
			DHookEntity(hkIchthyosaurIdleSound, true, iEntIndex, _, Hook_IchthyosaurIdleSoundPost);
			return;
		}
		#endif

		#if defined ENTPATCH_BM_GARGANTUA
		if (strcmp(szClassname, "npc_gargantua") == 0)
		{
			SDKHook(iEntIndex, SDKHook_SpawnPost, Hook_GargSpawnPost);
			DHookEntity(hkAcceptInput, true, iEntIndex, _, Hook_GargAcceptInputPost);
			return;
		}
		#endif

		#if defined ENTPATCH_BM_PUFFBALLFUNGUS
		if (strcmp(szClassname, "npc_puffballfungus") == 0)
		{
			SDKHook(iEntIndex, SDKHook_OnTakeDamage, Hook_PuffballFungusDmg);
			return;
		}
		#endif

		#if defined ENTPATCH_BM_MEDIC
		if (strcmp(szClassname, "npc_human_medic") == 0)
		{
			DHookEntity(hkEvent_Killed, false, iEntIndex, _, Hook_HumanMedicKilled);
			return;
		}
		#endif

		#if defined ENTPATCH_BM_BARNACLE
		if (strcmp(szClassname, "npc_barnacle") == 0)
		{
			//SDKHook(iEntIndex, SDKHook_OnTakeDamagePost, Hook_BarnacleOnTakeDamagePost);
			return;
		}
		#endif
	}
	else // !isNPC
	{
		#if defined ENTPATCH_BM_XENPORTAL_PUSH_PLAYERS
		if (strcmp(szClassname, "env_xen_portal_effect") == 0)
		{
			DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_XenPortalEffect_AcceptInput);
			return;
		}
		#endif

		#if defined ENTPATCH_POINT_TELEPORT
		if (strcmp(szClassname, "point_teleport") == 0)
		{
			DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_PointTeleportAcceptInput);
			return;
		}
		#endif

		#if defined ENTPATCH_POINT_VIEWCONTROL
		if (strcmp(szClassname, "point_viewcontrol") == 0)
		{
			DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_PointViewcontrolAcceptInput);
			return;
		}
		#endif
		
		#if defined ENTPATCH_PLAYER_SPEEDMOD
		if (strcmp(szClassname, "player_speedmod") == 0)
		{
			DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_SpeedmodAcceptInput);
			return;
		}
		#endif
		
		#if defined ENTPATCH_POINT_CLIENTCOMMAND
		if (strcmp(szClassname, "point_clientcommand") == 0)
		{
			DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_ClientCommandAcceptInput);
			return;
		}
		#endif
		
		#if defined ENTPATCH_ENV_ZOOM
		if (strcmp(szClassname, "env_zoom") == 0)
		{
			DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_EnvZoomAcceptInput);
			return;
		}
		#endif
		
		#if defined ENTPATCH_ENV_CREDITS
		if (strcmp(szClassname, "env_credits") == 0)
		{
			DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_EnvCreditsAcceptInput);
			return;
		}
		#endif

		#if defined ENTPATCH_BM_CASCADELIGHT
		if (strcmp(szClassname, "env_cascade_light") == 0)
		{
			if (GetConVarBool(g_ConvarNecroForceAutoModeForCSM))
			{
				DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_EnvCascadeLightAcceptInput);
				DHookEntity(hkKeyValue_char, false, iEntIndex, _, Hook_CascadeLightKeyValueHACK);
			}
			
			return;
		}
		#endif
		
		#if defined ENTPATCH_AI_SCRIPT_CONDITIONS
		if (strcmp(szClassname, "ai_script_conditions") == 0)
		{
			DHookEntity(hkThink, false, iEntIndex, _, Hook_AIConditionsThink);
			DHookEntity(hkThink, true, iEntIndex, _, Hook_AIConditionsThinkPost);
			return;
		}
		#endif
		
		#if defined ENTPATCH_FUNC_ROTATING
		if (strcmp(szClassname, "func_rotating") == 0)
		{
			CreateTimer(30.0, Timer_FixRotatingAngles, pEntity, TIMER_FLAG_NO_MAPCHANGE|TIMER_REPEAT);
			return;
		}
		#endif
		
		#if defined ENTPATCH_PLAYER_LOADSAVED
		if (strcmp(szClassname, "player_loadsaved") == 0)
		{
			DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_LoadSavedAcceptInput);
			return;
		}
		#endif
		
		#if defined ENTPATCH_NPC_DIALOGUE
		if ((strcmp(szClassname, "instanced_scripted_scene", false) == 0) ||
				(strcmp(szClassname, "logic_choreographed_scene", false) == 0) ||
				(strcmp(szClassname, "scripted_scene", false) == 0))
		{
			DHookEntity(hkFindNamedEntity, true, iEntIndex, _, Hook_FindNamedEntity);
			DHookEntity(hkFindNamedEntityClosest, true, iEntIndex, _, Hook_FindNamedEntity);
			return;
		}
		#endif

		#if defined ENTPATCH_REMOVE_BONE_FOLLOWERS
		if (strcmp(szClassname, "phys_bone_follower") == 0)
		{
			SDKHook(iEntIndex, SDKHook_VPhysicsUpdatePost, Hook_BoneFollowerVPhysicsUpdatePost);
			return;
		}
		#endif

		#if defined ENTPATCH_BM_PROP_CHARGERS
		if (strcmp(szClassname, "prop_radiation_charger") == 0)
		{
			DHookEntity(hkPropChargerThink, false, iEntIndex, _, Hook_PropRadiationChargerThink);
			return;
		}

		if (strcmp(szClassname, "prop_hev_charger") == 0)
		{
			DHookEntity(hkPropChargerThink, false, iEntIndex, _, Hook_PropHevChargerThink);
			return;
		}
		#endif

		#if defined ENTPATCH_BM_MISC_MARIONETTIST
		if (strcmp(szClassname, "misc_marionettist") == 0)
		{
			DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_MarionettistAcceptInput);
			return;
		}
		#endif

		#if defined ENTPATCH_BM_MUSIC_TRACK
		if (strcmp(szClassname, "music_track") == 0)
		{
			DHookEntity(hkThink, false, iEntIndex, _, Hook_MusicTrackThink);
			DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_MusicTrackAcceptInput);
			return;
		}
		#endif

		#if defined ENTPATCH_ENV_SCREENOVERLAY
		if (strcmp(szClassname, "env_screenoverlay") == 0)
		{
			pEntity.SetUserData("m_bIsEnabled", false); //needed to fix not working switching overlays
			DHookEntity(hkUpdateOnRemove, false, iEntIndex, _, Hook_EnvScreenoverlayUpdateOnRemove);
			DHookEntity(hkAcceptInput, false, iEntIndex, _, Hook_EnvScreenoverlayAcceptInput);
			return;
		}
		#endif

		
		// if some explosions turn out to be damaging all players except one, this is the fix
		// if (strcmp(szClassname, "env_explosion") == 0)
		// {
		// 	SDKHook(iEntIndex, SDKHook_SpawnPost, Hook_ExplosionSpawn);
		// 	return;
		// }
	}

	if (g_ConvarNecroBoltHitscanDamage.FloatValue != 125.0)
		SDKHook(iEntIndex, SDKHook_OnTakeDamage, Hook_OnTakeDamage);
}