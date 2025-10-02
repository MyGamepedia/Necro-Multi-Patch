#pragma semicolon 1

#include <sourcemod>
#include <sdktools>
#include <sdkhooks>
#include <dhooks>
#include <entity>

#include "srccoop_api/util/common/dhooks"

#include "necro_multipath/globals"

#include "necro_multipath/entities/env_laser_dot"
#include "necro_multipath/entities/env_sprite"
#include "necro_multipath/entities/grenade_bolt"
#include "necro_multipath/entities/grenade_frag"
//#include "necro_multipath/entities/grenade_mp5_contact"
#include "necro_multipath/entities/item_ammo_canister"

#include "necro_multipath/entities/funcs/BaseCombatWeaponPrecache"
#include "necro_multipath/entities/funcs/BlackMesaBaseWeaponIronSightsToggleIronSights"
#include "necro_multipath/entities/funcs/MultiplayRulesIsMultiplayer"

#include "necro_multipath/players/funcs/FAllowFlashlight"
#include "necro_multipath/players/funcs/GiveDefaultItems"
//#include "necro_multipath/players/funcs/PlayerForceRespawn"

#include "necro_multipath/functions/GetChild"
#include "necro_multipath/functions/AddOutput"

public Plugin myinfo = {
    name = "Dr.Necro's Black Mesa Servers Multipath",
    author = "MyGamepedia. Used a part of the source code from ampreeT/SourceCoop.",
    description = "This addon used for Dr.Necro's Black Mesa servers to fix issues in Black Mesa multiplayer.",
    version = "1.0",
    url = ""
};

public void OnPluginStart()
{
	mp_flashlight = FindConVar("mp_flashlight");
//	mp_forcerespawn = FindConVar("mp_forcerespawn");
	
	g_ConvarNecroGiveDefaultItems = CreateConVar("necro_givedefaultitems", "1", "Enable default give items list for on player spawn.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroOverrideDefaultWeaponParams = CreateConVar("necro_overridedefaultweaponparams", "1", "Enable weapon values override for parameters by loading custom weapon script.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroBoltSpriteTrail = CreateConVar("necro_boltspritetrail", "0", "Enable trail for explosive crossbow bolts, the trial makes it easier to determine where the shot was fired from.", 0, true, 0.0, true, 1.0);
//	g_pConvarDmRespawnTime = CreateConVar("necro_respawntime", "2.0", "Sets player respawn time in seconds.", _, true, 0.1);
	g_ConvarNecroClassicFrags = CreateConVar("necro_classicfrags", "0", "Enable simplified physics for frag grenades.", 0, true, 0.0, true, 1.0);
	g_ConvarNecroClassicLaserDot = CreateConVar("necro_classiclaserdot", "0", "Enable original RPG laser dot rendering.", 0, true, 0.0, true, 1.0);
//	g_ConvarNecroMp5ContactParticles = CreateConVar("necro_mp5contactparticles", "1", "Enable particles for MP5 barrel grenade.", 0, true, 0.0, true, 1.0);
	
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
	
	LoadDHookVirtual(pGameConfig, hkFAllowFlashlight, "CMultiplayRules::FAllowFlashlight");
	LoadDHookVirtual(pGameConfig, hkBlackMesaBaseDetonatorDetonate, "CBlackMesaBaseDetonator::Detonate");
	LoadDHookVirtual(pGameConfig, hkForceRespawn, "CBasePlayer::ForceRespawn");
	LoadDHookVirtual(pGameConfig, hkIsMultiplayer, "CMultiplayRules::IsMultiplayer");
	LoadDHookVirtual(pGameConfig, hkAcceptInput, "CBaseEntity::AcceptInput");
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
	//DHookEntity(hkForceRespawn, false, client, _, Hook_PlayerForceRespawn);
}

public void OnEntityCreated(int entity, const char[] classname)
{
	SDKHook(entity, SDKHook_Spawn, OnEntitySpawned);
	SDKHook(entity, SDKHook_SpawnPost, OnEntitySpawnedPost);
}

public OnEntitySpawned(int entity)
{
	char classname[64];
	GetEntityClassname(entity, classname, sizeof(classname));
	
	if(StrEqual(classname, "grenade_bolt"))
	{
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
	
//	if(StrEqual(classname, "grenade_mp5_contact"))
//	{
//		Mp5Contact_Path(entity);
//	}
}

public OnEntitySpawnedPost(int entity)
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
		
//		PrintToServer("use %s", classname);
	}
	
	return Plugin_Continue;
}

public void OnClientDisconnect_Post(int client)
{
	g_bPostTeamSelect[client] = false;
}