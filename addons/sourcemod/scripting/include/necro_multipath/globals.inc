#pragma newdecls required
#pragma semicolon 1

//===========================
//NECRO PLUGIN GENERAL VARS
//===========================

//#define DEBUG

ConVar sk_crossbow_tracer_enabled;
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
ConVar g_ConvarNecroOtherPlayersFlashlightTransmit;
ConVar g_ConvarNecroPlayersCollide;
ConVar g_ConvarNecroForceTeam;
ConVar g_ConvarNecroItemSpawnOverride;
ConVar g_ConvarNecroAllowWeaponAutoReload;
ConVar g_ConvarNecroAutoReloadTime_357;
ConVar g_ConvarNecroAutoReloadTime_Glock;
ConVar g_ConvarNecroAutoReloadTime_Assassin_Glock;
ConVar g_ConvarNecroAutoReloadTime_Mp5;
ConVar g_ConvarNecroAutoReloadTime_Shotgun;
ConVar g_ConvarNecroAutoReloadTime_Crossbow;
ConVar g_ConvarNecroCreateNewViewmodel;
ConVar g_ConvarNecroAllowPickupObjects;
	

DynamicDetour hkBaseCombatWeaponPrecache;
//DynamicDetour hkBaseCombatDefaultReload;
//DynamicDetour hkBaseCombatReload;
DynamicDetour hkStartLagCompensation;
//DynamicDetour hkGetUserSettings;
DynamicDetour hkPostChatMessage;
//DynamicDetour hkDropAmmoBox;
//DynamicDetour hkUTIL_Remove;
//DynamicDetour hkEventQueuAddEvent;
DynamicDetour hkPropBreakableRagdollInitRagdoll;

//DynamicHook hkBlackMesaBaseDetonatorDetonate;
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
//DynamicHook hkAttemptToExitFreezeCam;
//DynamicHook hkShowViewPortPanel;
DynamicHook hkFlashlightOff;
DynamicHook hkFlashlightOn;
//DynamicHook hkPlayerResourceUpdatePlayerData;
//DynamicHook hkPlayerResourceUpdateConnectedPlayer;
//DynamicHook hkPlayerResourceUpdateDisconnectedPlayer;
DynamicHook hkBaseCombatWeaponHolster;
DynamicHook hkBaseCombatWeaponItemHolsterFrame;
DynamicHook hkBlackMesaPlayerCreateAmmoBox;
//DynamicHook hkWeaponSnarkOperatorHandleAnimEvent;
DynamicHook hkBlackMesaPlayerPickupObject;
//DynamicHook hkBlackMesaBaseDetonatorExplodeTouch;


float g_fClientFastRespawnDelay[MAX_PLAYERS+1];
float g_fClientSpectatorJoinTeamDelay[MAX_PLAYERS+1];

#define MAX_SNARKS 20
#define MAX_HORNETS 20

int g_hClientSnarks[MAX_PLAYERS + 1][MAX_SNARKS]; //store snark ents for each client
int g_hClientHornets[MAX_PLAYERS + 1][MAX_HORNETS]; //store hornets ents for each client
int g_iUserCmdOffset;

//Handle g_pCreateWeaponTracedBoneMergeEntity;

Address g_EventQueue;


//===========================
//NECRO GUN GAME GLOBALS
//===========================

int g_iNecroGunGameClientLevel[MAX_PLAYERS + 1];
int g_iNecroGunGameClientKills[MAX_PLAYERS + 1];
int g_iNecroGunGameLastTeam[MAX_PLAYERS + 1];

int g_iNecroGunGameCurrentState = -1; //-1 the mode is disabled, 1 the mode isn't finished, 2 the mode is won by a player

ConVar g_ConvarNecroGunGameWeaponState[15];
ConVar g_ConvarNecroGunGameAmmoPrimary[15];
ConVar g_ConvarNecroGunGameAmmoSecondary[15];
ConVar g_ConvarNecroGunGameStateKills[15];

ConVar g_ConvarNecroGunGamePunishingWeaponName;
ConVar g_ConvarNecroGunGamePunishingWeaponState;
ConVar g_ConvarNecroGunGameFinalLevelNum;
ConVar g_ConvarNecroGunGameTeammateKarma;
ConVar g_ConvarNecroGunGameEnabled;

#define GUNGAME_HUDTICK 1.2

int g_iGunGameNextVoteTime = -1;
int g_iGunGameVoteEnable = 0;
int g_iGunGameVoteDisable = 0;
int g_iGunGameVoteSkip = 0;