#pragma newdecls required
#pragma semicolon 1

//#define DEBUG
#define MAX_FORMAT 256

ConVar mp_flashlight;
ConVar mp_forcerespawn;

ConVar g_ConvarNecroGiveDefaultItems;
ConVar g_ConvarNecroOverrideDefaultWeaponParams;
ConVar g_ConvarNecroBoltParticles;
ConVar g_ConvarNecroClassicFrags;
ConVar g_ConvarNecroClassicLaserDot;
ConVar g_ConvarNecroMp5ContactParticles;
ConVar g_ConvarNecroBoltHitscanDamage;
ConVar g_ConvarNecroAllowFastRespawn;
ConVar g_ConvarNecroFastRespawnDelay;

DynamicDetour hkToggleIronsights;
DynamicDetour hkGiveDefaultItems;
DynamicDetour hkBaseCombatWeaponPrecache;

DynamicHook hkFAllowFlashlight;
//DynamicHook hkBlackMesaBaseDetonatorDetonate;
DynamicHook hkForceRespawn;
DynamicHook hkIsMultiplayer;
DynamicHook hkAcceptInput;

bool g_bPostTeamSelect[MAXPLAYERS+1];
int g_bUnblockRespawn[MAXPLAYERS+1];
bool g_bIsMultiplayerOverride = true;

float g_fClientFastRespawnDelay[MAXPLAYERS+1];