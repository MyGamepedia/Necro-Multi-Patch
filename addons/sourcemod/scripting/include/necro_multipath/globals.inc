#pragma newdecls required
#pragma semicolon 1

//#define DEBUG
#define MAX_FORMAT 256

ConVar mp_flashlight;
ConVar mp_forcerespawn;
ConVar sk_crossbow_tracer_enabled;
ConVar mp_teamplay;
ConVar sv_cheats;

ConVar g_ConvarNecroGiveDefaultItems;
ConVar g_ConvarNecroOverrideDefaultWeaponParams;
ConVar g_ConvarNecroBoltParticles;
ConVar g_ConvarNecroClassicFrags;
ConVar g_ConvarNecroClassicLaserDot;
ConVar g_ConvarNecroMp5ContactParticles;
ConVar g_ConvarNecroBoltHitscanDamage;
ConVar g_ConvarNecroAllowFastRespawn;
ConVar g_ConvarNecroFastRespawnDelay;
ConVar g_ConvarNecroExplodingBolt;
ConVar g_ConvarNecroSatchelDelay_Attack1_Primary;
ConVar g_ConvarNecroSatchelDelay_Attack1_Secondary;
ConVar g_ConvarNecroSatchelDelay_Attack2_Primary;
ConVar g_ConvarNecroSatchelDelay_Attack2_Secondary;
ConVar g_ConvarNecroSatchelDelay_Reload_Primary;
ConVar g_ConvarNecroSatchelDelay_Reload_Secondary;
ConVar g_ConvarNecroSatchelDelayOverride;
ConVar g_ConvarNecroSpectatorJoinTeamDelay;
ConVar g_ConvarNecroOtherPlayersFlashlight;

DynamicDetour hkToggleIronsights;
DynamicDetour hkGiveDefaultItems;
DynamicDetour hkBaseCombatWeaponPrecache;
//DynamicDetour hkBaseCombatDefaultReload;
//DynamicDetour hkBaseCombatReload;
DynamicDetour hkStartLagCompensation;

DynamicHook hkFAllowFlashlight;
//DynamicHook hkBlackMesaBaseDetonatorDetonate;
DynamicHook hkForceRespawn;
DynamicHook hkIsMultiplayer;
DynamicHook hkAcceptInput;
//DynamicHook hkItemPostFrame;
//DynamicHook hkBlackMesaBaseWeaponThrownReload;
//DynamicHook hkBaseCombatWeaponFinishReload;
//DynamicHook hkBlackMesaBaseProjectileInit;
DynamicHook hkBaseCombatPrimaryAttack;
DynamicHook hkBaseCombatSecondaryAttack;
DynamicHook hkBaseCombatDeploy;
DynamicHook hkBaseCombatReload;
DynamicHook hkBaseCombatHasAnyAmmo;
DynamicHook hkWeaponCrossbowFireBolt;
DynamicHook hkRestoreWorld;
//DynamicHook hkAttemptToExitFreezeCam;
DynamicHook hkStartObserverMode;
//DynamicHook hkShowViewPortPanel;
DynamicHook hkPlayerSpawn;
DynamicHook hkFlashlightOff;
DynamicHook hkFlashlightOn;

bool g_bPostTeamSelect[MAXPLAYERS+1];
int g_bUnblockRespawn[MAXPLAYERS+1];
bool g_bIsMultiplayerOverride = true;

float g_fClientFastRespawnDelay[MAXPLAYERS+1];
float g_fClientSpectatorJoinTeamDelay[MAXPLAYERS+1];

int g_iUserCmdOffset;

bool g_iTeamplay;