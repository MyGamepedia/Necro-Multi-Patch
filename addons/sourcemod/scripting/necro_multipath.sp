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

#include "necro_multipath/entities/funcs/BaseCombatWeaponPrecache"
#include "necro_multipath/entities/funcs/BlackMesaBaseWeaponIronSightsToggleIronSights"
#include "necro_multipath/entities/funcs/MultiplayRulesIsMultiplayer"

#include "necro_multipath/players/funcs/FAllowFlashlight"
#include "necro_multipath/players/funcs/GiveDefaultItems"
#include "necro_multipath/players/funcs/PlayerForceRespawn"

#include "necro_multipath/engine/CLagCompensationManagerStartLagCompensation"

#include "necro_multipath/functions/GetChild"
#include "necro_multipath/functions/AddOutput"

public Plugin myinfo = {
    name = "Dr.Necro's Black Mesa Servers Multipath",
    author = "MyGamepedia",
    description = "This addon used for Dr.Necro's Black Mesa servers to fix issues in Black Mesa multiplayer.",
    version = "1.0.5",
    url = ""
};

public void OnPluginStart()
{	
	mp_flashlight = FindConVar("mp_flashlight");
	mp_forcerespawn = FindConVar("mp_forcerespawn");
	
	g_ConvarNecroGiveDefaultItems = CreateConVar("necro_givedefaultitems", "1", "Enable default give items list for on player spawn.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroOverrideDefaultWeaponParams = CreateConVar("necro_overridedefaultweaponparams", "1", "Enable weapon values override for parameters by loading custom weapon script.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroBoltParticles = CreateConVar("necro_boltparticles", "1", "Enables trail for explosive crossbow bolts, the trial makes it easier to determine where the shot was fired from.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroClassicFrags = CreateConVar("necro_classicfrags", "0", "Enable simplified physics for frag grenades.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroClassicLaserDot = CreateConVar("necro_classiclaserdot", "0", "Enable original RPG laser dot rendering.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroMp5ContactParticles = CreateConVar("necro_mp5contactparticles", "1", "Enables smoke for MP5 barrel grenade.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroBoltHitscanDamage = CreateConVar("necro_bolthitscandamage", "65.0", "Amount of damage for the crossbow bolt hitscan.");
	g_ConvarNecroAllowFastRespawn = CreateConVar("necro_allowfastrespawn","1","Allow player respawn by pressing the buttons before spec_freeze_time and spec_freeze_traveltime is finished.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroFastRespawnDelay = CreateConVar("necro_fastrespawndelay", "0.5", "Amount of time in seconds before player can respawn by pressing the buttons with enabled fast respawn.");
	g_ConvarNecroExplodingBolt = CreateConVar("necro_explodingbolt","1","Enable exploding bolt for the crossbow.", 0, true, 0.0, true, 1.0);
	
	HookEvent("player_death", Event_PlayerDeath);	
	
	g_ConvarNecroFastRespawnDelay.AddChangeHook(OnFastRespawnDelayChanged);
	
	g_fClientFastRespawnDelay[0] = GetConVarFloat(g_ConvarNecroFastRespawnDelay);
	
	LoadGameData();
}

void LoadGameData()
{
	GameData pGameConfig = LoadGameConfigFile("necro_gamedata");
	
	if (pGameConfig == null)
		SetFailState("Couldn't load game config: \"necro_gamedata\"");
		
	LoadDHookDetour(pGameConfig, hkGiveDefaultItems, "CBlackMesaPlayer::GiveDefaultItems", Hook_GiveDefaultItems);
	LoadDHookDetour(pGameConfig, hkBaseCombatWeaponPrecache, "CBaseCombatWeapon::Precache", Hook_BaseCombatWeaponPrecache, Hook_BaseCombatWeaponPrecachePost);
	LoadDHookDetour(pGameConfig, hkToggleIronsights, "CBlackMesaBaseWeaponIronSights::ToggleIronSights", Hook_ToggleIronsights);	
	LoadDHookDetour(pGameConfig, hkStartLagCompensation, "CLagCompensationManager::StartLagCompensation", Hook_StartLagCompensation);
	
	LoadDHookVirtual(pGameConfig, hkFAllowFlashlight, "CMultiplayRules::FAllowFlashlight");
	LoadDHookVirtual(pGameConfig, hkForceRespawn, "CBasePlayer::ForceRespawn");
	LoadDHookVirtual(pGameConfig, hkIsMultiplayer, "CMultiplayRules::IsMultiplayer");
	LoadDHookVirtual(pGameConfig, hkAcceptInput, "CBaseEntity::AcceptInput");
	LoadDHookVirtual(pGameConfig, hkWeaponCrossbowFireBolt, "CWeapon_Crossbow::FireBolt");
	
	g_iUserCmdOffset = pGameConfig.GetOffset("CBasePlayer::GetCurrentUserCommand");
}

public void OnMapStart()
{
	DHookGamerules(hkFAllowFlashlight, false, _, Hook_FAllowFlashlight);
	DHookGamerules(hkIsMultiplayer, false, _, Hook_IsMultiplayer);
}

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

public void OnEntityCreated(int entity, const char[] classname)
{
	SDKHook(entity, SDKHook_Spawn, OnEntitySpawned);
	SDKHook(entity, SDKHook_SpawnPost, OnEntitySpawnedPost);
	SDKHook(entity, SDKHook_OnTakeDamage, Hook_OnTakeDamage);
}

public Action Hook_OnTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon, float damageForce[3], float damagePosition[3])
{
	#if defined DEBUG
	PrintToServer("Hook_OnTakeDamage: victim=%d, attacker=%d, inflictor=%d, damage=%.2f, damagetype=%d, weapon=%d, damageForce[%.2f, %.2f, %.2f], damagePosition[%.2f, %.2f, %.2f]",
					victim, attacker, inflictor, damage, damagetype, weapon,
					damageForce[0], damageForce[1], damageForce[2],
					damagePosition[0], damagePosition[1], damagePosition[2]);
	#endif
					
	
	if(IsValidEntity(weapon))
	{
		char classname[64];
		GetEntityClassname(weapon, classname, sizeof(classname));
		
		if(StrEqual(classname, "weapon_crossbow") && damagetype == 4096)
		{
			if(damage == 125)
			{
				damage = GetConVarFloat(g_ConvarNecroBoltHitscanDamage);
				
				#if defined DEBUG
				PrintToServer("Normal crossbow damage");
				#endif
			}
			
			if(damage == 125 * GetConVarFloat(FindConVar("sk_player_head")))
			{
				damage = GetConVarFloat(g_ConvarNecroBoltHitscanDamage) * GetConVarFloat(FindConVar("sk_player_head"));
				
				#if defined DEBUG
				PrintToServer("Head crossbow damage");
				#endif
			}
			
			return Plugin_Changed;
		}
	}

    return Plugin_Continue;
}

public void Event_PlayerDeath(Event event, const char[] name, bool dontBroadcast)
{
    int client = GetClientOfUserId(event.GetInt("userid"));
	
	g_fClientFastRespawnDelay[client] = GetGameTime() + g_fClientFastRespawnDelay[0];
		
	#if defined DEBUG
	PrintToServer("Event_PlayerDeath: g_fClientFastRespawnDelay[%d]: %f", client, g_fClientFastRespawnDelay[client]);
	#endif
}

public void OnEntitySpawned(int entity)
{
	char classname[64];
	GetEntityClassname(entity, classname, sizeof(classname));
	
	if(StrEqual(classname, "grenade_bolt"))
	{
	//	DHookEntity(hkBlackMesaBaseProjectileInit, false, entity, _, Hook_BoltInit);
		Bolt_Path(entity);
	}
	
	if(StrEqual(classname, "env_laser_dot"))
	{
		LaserDot_Path(entity);
	}
	
	if(StrEqual(classname, "grenade_frag"))
	{
		Frag_Path(entity);
	}
	
	if(StrEqual(classname, "env_sprite"))
	{
		RequestFrame(Sprite_PathCanister, entity); //we need RequestFrame because parent is not immediately set
	}
	
	if(StrEqual(classname, "grenade_mp5_contact"))
	{
		Mp5Contact_Path(entity);
	}
	
	if(StrEqual(classname, "weapon_crossbow"))
	{
		DHookEntity(hkWeaponCrossbowFireBolt, false, entity, _, Hook_FireBolt);
		DHookEntity(hkWeaponCrossbowFireBolt, true, entity, _, Hook_FireBoltPost);
	}
}

public void OnEntitySpawnedPost(int entity)
{
	char classname[64];
	GetEntityClassname(entity, classname, sizeof(classname));
	
	if(StrEqual(classname, "env_laser_dot"))
	{
		LaserDot_PathPost(entity);
	}
	
	if(StrEqual(classname, "grenade_frag"))
	{
		Frag_PathPost(entity);
	}
}

public Action OnPlayerRunCmd(int iClient, int &iButtons, int &iImpulse, float fVel[3], float fAngles[3], int &iWeapon, int& subtype, int& cmdnum, int& tickcount, int& seed, int mouse[2])
{
	if (IsFakeClient(iClient))
		return Plugin_Continue;
	
	//hide broken ctrl menu when in spectator
	int observermode = GetEntProp(iClient, Prop_Data, "m_iObserverMode");
	
	if (mouse[0] || mouse[1])
	{
		g_bPostTeamSelect[iClient] = true;
	}
	
	if (observermode > 1)
	{
		if (g_bPostTeamSelect[iClient] && tickcount % 10 == 0)
		{
			ShowVGUIPanel(iClient, "specmenu", _, false);
		}
	}
	
	//client don't use weapon when used custom classname to load custom script, force use with client command
	char classname[64];
	GetEntityClassname(iWeapon, classname, sizeof(classname));
	
	if(!StrEqual(classname, "worldspawn"))
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

public void OnPlayerRunCmdPost(int client, int buttons)
{
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

public void OnFastRespawnDelayChanged(ConVar convar, const char[] oldValue, const char[] newValue)
{
	g_fClientFastRespawnDelay[0] = GetConVarFloat(g_ConvarNecroFastRespawnDelay);
}