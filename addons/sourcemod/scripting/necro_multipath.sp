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

#include "necro_multipath/entities/env_laser_dot"
#include "necro_multipath/entities/env_sprite"
#include "necro_multipath/entities/grenade_bolt"
#include "necro_multipath/entities/grenade_frag"
#include "necro_multipath/entities/grenade_mp5_contact"
#include "necro_multipath/entities/item_ammo_canister"
#include "necro_multipath/entities/weapon_crossbow"
#include "necro_multipath/entities/weapon_satchel"
//#include "necro_multipath/entities/weapon_gluon"

#include "necro_multipath/entities/funcs/BaseCombatWeaponPrecache"
#include "necro_multipath/entities/funcs/BlackMesaBaseWeaponIronSightsToggleIronSights"
#include "necro_multipath/entities/funcs/MultiplayRulesIsMultiplayer"

#include "necro_multipath/players/funcs/FAllowFlashlight"
#include "necro_multipath/players/funcs/GiveDefaultItems"
#include "necro_multipath/players/funcs/PlayerForceRespawn"

#include "necro_multipath/engine/CLagCompensationManagerStartLagCompensation"

#include "necro_multipath/convars/host_timescale"
#include "necro_multipath/convars/necro_fastrespawndelay"

//#include "necro_multipath/functions/GetChild"
#include "necro_multipath/functions/AddOutput"

public Plugin myinfo = {
    name = "Dr.Necro's Black Mesa Servers Multipath",
    author = "MyGamepedia",
    description = "This addon is used to significantly expand and improve Dr.Necro's Black Mesa multiplayer servers.",
    version = "1.0.8",
    url = ""
};

public void OnPluginStart()
{	
	mp_flashlight = FindConVar("mp_flashlight");
	mp_forcerespawn = FindConVar("mp_forcerespawn");
	sk_crossbow_tracer_enabled = FindConVar("sk_crossbow_tracer_enabled");
	
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
	g_ConvarNecroatchelDelayOverride = CreateConVar("necro_satcheldelayoverride","1","Enables primary and secondary attack delay override for weapon_satchel", 0, true, 0.0, true, 1.0);
	g_ConvarNecroSatchelDelay_Attack1_Primary = CreateConVar("necro_satcheldelay_attack1_primary","1.0","Sets delay for satchel weapon primary attack when the satchel thrown.  Recommended 0.84 at least to avoid bugs with the satchel rendering.");
	g_ConvarNecroSatchelDelay_Attack1_Secondary = CreateConVar("necro_satcheldelay_attack1_secondary","1.2","Sets delay for satchel weapon primary secondary when the satchel thrown. Recommended 1.2 at least to avoid bugs with the radio rendering.");
	g_ConvarNecroSatchelDelay_Attack2_Primary = CreateConVar("necro_satcheldelay_attack2_primary","0.3","Sets delay for satchel weapon primary attack when the radio is used.");
	g_ConvarNecroSatchelDelay_Attack2_Secondary = CreateConVar("necro_satcheldelay_attack2_secondary","0.2","Sets delay for satchel weapon secondary attack when the radio is used.");
	g_ConvarNecroSatchelDelay_Reload_Primary = CreateConVar("necro_satcheldelay_reload_primary","0.4","Sets delay for satchel weapon primary attack when the owner take out a new satchel.");
	g_ConvarNecroSatchelDelay_Reload_Secondary = CreateConVar("necro_satcheldelay_reload_secondary","0.4","Sets delay for satchel weapon secondary attack when the owner take out a new satchel.");

	
	HookEvent("player_death", Event_PlayerDeath);	
	
	g_ConvarNecroFastRespawnDelay.AddChangeHook(OnFastRespawnDelayChanged);
	HookConVarChange(FindConVar("host_timescale"), OnHostTimeScaleChanged);
	
	g_fClientFastRespawnDelay[0] = GetConVarFloat(g_ConvarNecroFastRespawnDelay); //HACK! Use fist (unused) element in the array to store cvar delay value
	
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
	
	//Memory Vars
	g_iUserCmdOffset = pGameConfig.GetOffset("CBasePlayer::GetCurrentUserCommand");
}

//Purspose: Load  gamerule offsets to control various game mechanics when map is loaded
public void OnMapStart()
{
	DHookGamerules(hkFAllowFlashlight, false, _, Hook_FAllowFlashlight);
	DHookGamerules(hkIsMultiplayer, false, _, Hook_IsMultiplayer);
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
	
	g_fClientFastRespawnDelay[client] = 0.0;
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

//Purpose: Modify crossbow bolt hitscan damage
public Action Hook_OnTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon, float damageForce[3], float damagePosition[3])
{
	#if defined DEBUG
	PrintToServer("Hook_OnTakeDamage: victim=%d, attacker=%d, inflictor=%d, damage=%.2f, damagetype=%d, weapon=%d, damageForce[%.2f, %.2f, %.2f], damagePosition[%.2f, %.2f, %.2f]",
					victim, attacker, inflictor, damage, damagetype, weapon,
					damageForce[0], damageForce[1], damageForce[2],
					damagePosition[0], damagePosition[1], damagePosition[2]);
	#endif
					
	//Check if damage is from crossbow hitscan bolt
	if(IsValidEntity(weapon))
	{
		char classname[64];
		GetEntityClassname(weapon, classname, sizeof(classname));
		
		//Modify damage value if is weapon_crossbow and damage type is 4096 
		if(StrEqual(classname, "weapon_crossbow") && damagetype == 4096)
		{
			if(damage == 125)
			{
				damage = GetConVarFloat(g_ConvarNecroBoltHitscanDamage);
				
				#if defined DEBUG
				PrintToServer("Normal crossbow damage");
				#endif
			}
			
			//Headshot multiplier fix
			if(damage == 125 * GetConVarFloat(FindConVar("sk_player_head")))
			{
				damage = GetConVarFloat(g_ConvarNecroBoltHitscanDamage) * GetConVarFloat(FindConVar("sk_player_head"));
				
				#if defined DEBUG
				PrintToServer("Head crossbow damage");
				#endif
			}
			//TODO: Call the programmer to add more multipliers if needed
			
			return Plugin_Changed;
		}
	}

    return Plugin_Continue;
}

//Purpose: Set fast respawn delay when player dies
public void Event_PlayerDeath(Event event, const char[] name, bool dontBroadcast)
{
    int client = GetClientOfUserId(event.GetInt("userid"));
	
	g_fClientFastRespawnDelay[client] = GetGameTime() + g_fClientFastRespawnDelay[0];
		
	#if defined DEBUG
	PrintToServer("Event_PlayerDeath: g_fClientFastRespawnDelay[%d]: %f", client, g_fClientFastRespawnDelay[client]);
	#endif
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
		
        SetEntPropFloat(client, Prop_Send, "m_flDeathTime", 0.0);
	}
}