#pragma newdecls required
#pragma semicolon 1

#include <sourcemod>
#include <sdktools>
#include <sdktools_functions>
#include <sdkhooks>
#include <dhooks>
#include <entity>


#include "srccoop_api/util/common/dhooks"

#include "necro_multipath/globals"

#include "necro_multipath/gameevents/player_death"

#include "necro_multipath/entities/env_laser_dot"
#include "necro_multipath/entities/env_sprite"
#include "necro_multipath/entities/grenade_bolt"
#include "necro_multipath/entities/grenade_frag"
#include "necro_multipath/entities/grenade_mp5_contact"
#include "necro_multipath/entities/item_ammo_canister"
#include "necro_multipath/entities/weapon_crossbow"
#include "necro_multipath/entities/weapon_satchel"

#include "necro_multipath/entities/funcs/BaseCombatWeaponPrecache"
#include "necro_multipath/entities/funcs/BlackMesaBaseWeaponIronSightsToggleIronSights"
#include "necro_multipath/entities/funcs/MultiplayRulesIsMultiplayer"
#include "necro_multipath/entities/funcs/OnTakeDamage"

#include "necro_multipath/players/PlayerRunCmd"
#include "necro_multipath/players/PlayerSpawnPost"

#include "necro_multipath/players/funcs/GiveDefaultItems"
#include "necro_multipath/players/funcs/PlayerForceRespawn"
#include "necro_multipath/players/funcs/FlashlightOff"
#include "necro_multipath/players/funcs/FlashlightOn"
#include "necro_multipath/players/funcs/StartObserverMode"

#include "necro_multipath/engine/CLagCompensationManagerStartLagCompensation"

#include "necro_multipath/gamerules/RestoreWorld"
#include "necro_multipath/gamerules/FAllowFlashlight"

#include "necro_multipath/convars/host_timescale"
#include "necro_multipath/convars/necro_fastrespawndelay"
#include "necro_multipath/convars/necro_spectatorjointeamdelay"
#include "necro_multipath/convars/jointeam"

#include "necro_multipath/functions/GetChild"
#include "necro_multipath/functions/AddOutput"

public Plugin myinfo = {
    name = "Dr.Necro's Black Mesa Servers Multipath",
    author = "MyGamepedia",
    description = "This addon is used to significantly expands and improves Dr.Necro's Black Mesa multiplayer servers.",
    version = "1.1.0",
    url = ""
};

public void OnPluginStart()
{
	//existing convars
	mp_flashlight = FindConVar("mp_flashlight");
	mp_forcerespawn = FindConVar("mp_forcerespawn");
	sk_crossbow_tracer_enabled = FindConVar("sk_crossbow_tracer_enabled");
	mp_teamplay = FindConVar("mp_teamplay");
	sv_cheats = FindConVar("sv_cheats");
	
	//new convars
	g_ConvarNecroGiveDefaultItems = CreateConVar("necro_givedefaultitems", "1", "Enable default give items list for on player spawn.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroOverrideDefaultWeaponParams = CreateConVar("necro_overridedefaultweaponparams", "1", "Enable weapon values override for parameters by loading custom weapon script.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroBoltParticles = CreateConVar("necro_boltparticles", "1", "Enables trail for explosive crossbow bolts, the trial makes it easier to determine where the shot was fired from.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroClassicFrags = CreateConVar("necro_classicfrags", "0", "Enable simplified physics for frag grenades.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroClassicLaserDot = CreateConVar("necro_classiclaserdot", "0", "Enable original RPG laser dot rendering.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroMp5ContactParticles = CreateConVar("necro_mp5contactparticles", "1", "Enables smoke for MP5 barrel grenade.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroBoltHitscanDamage = CreateConVar("necro_bolthitscandamage", "65.0", "Amount of damage for the crossbow bolt hitscan.");
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

	//hook convars
	g_ConvarNecroFastRespawnDelay.AddChangeHook(OnFastRespawnDelayChanged);
	g_ConvarNecroSpectatorJoinTeamDelay.AddChangeHook(OnSpectatorJoinTeamDelayChanged);
	HookConVarChange(FindConVar("host_timescale"), OnHostTimeScaleChanged);
	AddCommandListener(Listener_Jointeam, "jointeam"); //note: it was planned to use player_team event hook, but it doesn't store team nums (always 0)
	
	//HACK! Use fist (unused) element in the array to store cvar delay value
	g_fClientFastRespawnDelay[0] = GetConVarFloat(g_ConvarNecroFastRespawnDelay);
	g_fClientSpectatorJoinTeamDelay[0] = GetConVarFloat(g_ConvarNecroSpectatorJoinTeamDelay);
	
	//Load detours offsets + some vars from memory
	LoadGameData();
}

//Purpose: Load our main game config file and offsets + some vars from memory
void LoadGameData()
{
	//Load our main game config file
	GameData pGameConfig = LoadGameConfigFile("necro_gamedata");
	
	//Check if is valid 
	if (pGameConfig == null)
		SetFailState("Couldn't load game config: \"necro_gamedata\"");
	
	//Detours
	LoadDHookDetour(pGameConfig, hkGiveDefaultItems, "CBlackMesaPlayer::GiveDefaultItems", Hook_GiveDefaultItems);
	LoadDHookDetour(pGameConfig, hkBaseCombatWeaponPrecache, "CBaseCombatWeapon::Precache", Hook_BaseCombatWeaponPrecache, Hook_BaseCombatWeaponPrecachePost);
	LoadDHookDetour(pGameConfig, hkToggleIronsights, "CBlackMesaBaseWeaponIronSights::ToggleIronSights", Hook_ToggleIronsights);	
	LoadDHookDetour(pGameConfig, hkStartLagCompensation, "CLagCompensationManager::StartLagCompensation", Hook_StartLagCompensation);
	
	//Offsets
	LoadDHookVirtual(pGameConfig, hkFAllowFlashlight, "CMultiplayRules::FAllowFlashlight");
	LoadDHookVirtual(pGameConfig, hkForceRespawn, "CBasePlayer::ForceRespawn");
	LoadDHookVirtual(pGameConfig, hkIsMultiplayer, "CMultiplayRules::IsMultiplayer");
	LoadDHookVirtual(pGameConfig, hkAcceptInput, "CBaseEntity::AcceptInput");
	LoadDHookVirtual(pGameConfig, hkWeaponCrossbowFireBolt, "CWeapon_Crossbow::FireBolt");
	LoadDHookVirtual(pGameConfig, hkBaseCombatPrimaryAttack, "CBaseCombatWeapon::PrimaryAttack");
	LoadDHookVirtual(pGameConfig, hkBaseCombatSecondaryAttack, "CBaseCombatWeapon::SecondaryAttack");
	LoadDHookVirtual(pGameConfig, hkBaseCombatReload, "CBaseCombatWeapon::Reload");
	LoadDHookVirtual(pGameConfig, hkBaseCombatHasAnyAmmo, "CBaseCombatWeapon::HasAnyAmmo");
	LoadDHookVirtual(pGameConfig, hkBaseCombatDeploy, "CBaseCombatWeapon::Deploy");
	LoadDHookVirtual(pGameConfig, hkRestoreWorld, "CBM_MP_GameRules::RestoreWorld");
	LoadDHookVirtual(pGameConfig, hkStartObserverMode, "CBasePlayer::StartObserverMode");
	LoadDHookVirtual(pGameConfig, hkPlayerSpawn, "CBasePlayer::Spawn");
	LoadDHookVirtual(pGameConfig, hkFlashlightOff, "CBlackMesaPlayer::FlashlightTurnOff");
	LoadDHookVirtual(pGameConfig, hkFlashlightOn, "CBlackMesaPlayer::FlashlightTurnOn");
	
	//Memory Vars
	g_iUserCmdOffset = pGameConfig.GetOffset("CBasePlayer::GetCurrentUserCommand");
}

//Purspose: Load  gamerule offsets to control various game mechanics when map is loaded
public void OnMapStart()
{
	//gamerules hooks
	DHookGamerules(hkFAllowFlashlight, false, _, Hook_FAllowFlashlight);
	DHookGamerules(hkIsMultiplayer, false, _, Hook_IsMultiplayer);
	DHookGamerules(hkRestoreWorld, true, _, Hook_RestoreWorldPost);
	
	//events hooks
	HookEvent("player_death", Event_PlayerDeath);

	//set teamplay variable state to know if teamplay is enabled or not
	if(GetConVarBool(mp_teamplay))
	{
		g_iTeamplay = true;
	}
	else
	{
		g_iTeamplay = false;
	}
}

//Purpose: Fix following when player is on the server:
/* 1. Fix mp_forcerespawn not working properly
   2. Initialize fast respawn delay variable for player
*/
public void OnClientPutInServer(int client)
{
	if (IsFakeClient(client))
		return;
	
	//DHookEntity(hkChangeTeam, false, client, _, Hook_PlayerChangeTeam);
	DHookEntity(hkForceRespawn, false, client, _, Hook_PlayerForceRespawn);
	DHookEntity(hkStartObserverMode, false, client, _, Hook_PlayerStartObserverMode);
	DHookEntity(hkPlayerSpawn, false, client, _, Hook_PlayerSpawnPost);
	DHookEntity(hkFlashlightOff, false, client, _, Hook_FlashlightOff);
	DHookEntity(hkFlashlightOn, false, client, _, Hook_FlashlightOn);

	//reset these vars
	g_fClientFastRespawnDelay[client] = 0.0;
	g_fClientSpectatorJoinTeamDelay[client] = 0.0;

	//disable "other players flashlight"
	SendConVarValue(client, sv_cheats, "1"); //HACK! This thing needs cheats ON, enable cheats ON, disable the thing, set cheats OFF after
	ClientCommand(client, "r_flashlight_3rd_draw 0");
}

public void OnClientDisconnect_Post(int client)
{
	g_bPostTeamSelect[client] = false;
}

//Purpose: Hook entity creation to setup our custom entity hooks
public void OnEntityCreated(int entity, const char[] classname)
{
	SDKHook(entity, SDKHook_Spawn, OnEntitySpawned);
	SDKHook(entity, SDKHook_SpawnPost, OnEntitySpawnedPost); //needed for some entities
	SDKHook(entity, SDKHook_OnTakeDamage, Hook_OnTakeDamage);
}

//Purpose: Check entity classname and apply our custom entity hooks
public void OnEntitySpawned(int entity)
{
	char classname[64];
	GetEntityClassname(entity, classname, sizeof(classname));
	
	if(StrEqual(classname, "grenade_bolt"))
	{
		Bolt_Path(entity); //Set proper skin, make it explosive if needed and add trail if enabled
		DHookEntity(hkAcceptInput, false, entity, _, Hook_GrenadeBoltAcceptInput); //Disable exploading bolt if is not allowed
	}
	
	if(StrEqual(classname, "env_laser_dot"))
	{
		LaserDot_Path(entity); //hide laser dot before using new rendering method
	}
	
	if(StrEqual(classname, "grenade_frag"))
	{
		Frag_Path(entity); //Use VPhysics for frag grenade if classic frags is disabled
	}
	
	if(StrEqual(classname, "env_sprite"))
	{
		RequestFrame(Sprite_PathCanister, entity); //Control canister's sprite visibility depending on pick up state
												   //used RequestFrame because parent is not immediately set
	}
	
	if(StrEqual(classname, "grenade_mp5_contact"))
	{
		Mp5Contact_Path(entity); //Add smoke particle effect to MP5 barrel grenade if enabled
	}
	
	if(StrEqual(classname, "weapon_crossbow"))
	{
		DHookEntity(hkWeaponCrossbowFireBolt, false, entity, _, Hook_CrossbowFireBolt); //Use singleplayer rules for bolt creation to make sk_crossbow_tracer_enabled work
		DHookEntity(hkWeaponCrossbowFireBolt, true, entity, _, Hook_CrossbowFireBoltPost); //Set back multiplayer rules after bolt creation
		DHookEntity(hkBaseCombatDeploy, true, entity, _, Hook_CrossbowDeploy); //Set back multiplayer rules after bolt creation
	}
	
	if(StrEqual(classname, "weapon_satchel"))
	{
		//Set custom attack delays on different stages
		DHookEntity(hkBaseCombatPrimaryAttack, true, entity, _, Hook_SatchelPrimaryAttackPost);
		DHookEntity(hkBaseCombatSecondaryAttack, true, entity, _, Hook_SatchelSecondaryAttackPost);
		DHookEntity(hkBaseCombatReload, true, entity, _, Hook_SatchelReloadPost);
	}
}

//Purpose: Post spawn fixes for some entities
public void OnEntitySpawnedPost(int entity)
{
	char classname[64];
	GetEntityClassname(entity, classname, sizeof(classname));

	if(StrEqual(classname, "env_laser_dot"))
	{
		LaserDot_PathPost(entity); //set new rendering after spawn
	}
	{
		Frag_PathPost(entity); //reset multiplayer state back for this entity after setting VPhysics
	}
	
	if(StrEqual(classname, "grenade_frag"))
	{
		Frag_PathPost(entity); //reset multiplayer state back for this entity after setting VPhysics
	}
}