#pragma newdecls required
#pragma semicolon 1

//===========================
//NECRO PLUGIN GENERAL VARS
//===========================

//#define DEBUG
#define DEV

#define COL_NORMAL  	"\x01"
#define COL_GREEN	    "\x0700ff00"
#define COL_DARKBLUE	"\x075999ff"
#define COL_LIGHTBLUE	"\x0700FFFF"
#define COL_SEMIRED	    "\x07f24522"
#define COL_LIGHTGREEN	"\x0739f75f"

ConVar sk_crossbow_tracer_enabled;
ConVar sv_cheats;
ConVar mp_teamplay;
ConVar player_respawn_alpha;
ConVar player_respawn_protection_time;
ConVar hostname;
/*
ConVar mp_warmup_intermission_time;
ConVar mp_warmup_time;
ConVar mp_round_intermission_time;
ConVar mp_timelimit;
*/

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
ConVar g_ConvarNecroBlockRestoreWorld;
ConVar g_ConvarNecroBlockRestoreWorldRespawnPlayers;
ConVar g_ConvarNecroWaterBulletEnable;
ConVar g_ConvarNecroCoopSnarks;
ConVar g_ConvarNecroForceAutoModeForCSM;
ConVar g_ConvarNecroHevSuitSounds;
ConVar g_ConvarNecroSpawnProtectionColorRed;
ConVar g_ConvarNecroSpawnProtectionColorBlue;
ConVar g_ConvarNecroSpawnProtectionColorGreen;
ConVar g_ConvarNecroSpawnProtectionColorAlpha;
ConVar g_ConvarNecroSpawnProtectionRenderFX;

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
DynamicDetour hkNoteWeaponFired;

#if defined PLAYERPATCH_SUIT_SOUNDS
DynamicDetour hkCheckSuitUpdate;
#endif

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
DynamicHook hkIsValidEnemy;
DynamicHook hkUpdateEnemyMemory;
DynamicHook hkFreezeAllPlayers;
DynamicHook hkUnfreezeAllPlayers;


float g_fClientFastRespawnDelay[MAX_PLAYERS+1];
float g_fClientSpectatorJoinTeamDelay[MAX_PLAYERS+1];


#define MAX_SNARKS 20
#define MAX_HORNETS 20

//TODO: replace with methodmap
int g_hClientSnarks[MAX_PLAYERS + 1][20]; //store snark ents for each client
int g_hClientHornets[MAX_PLAYERS + 1][20]; //store hornets ents for each client

bool g_bPAFTFT[MAX_PLAYERS + 1]  = { true, ... }; //Player Appeared For The First Time on one of the valid teams

StringMap gGreetedAuthIds;

int g_iUserCmdOffset;

int g_iShedMinTime;
int g_iShedMaxTime;

int g_iShedTimeDeathmatch[3][2] = 
{
    {0, 2},
    {8, 10},
    {16, 18}
};

int g_iShedTimeTeamDeathmatch[3][2] = 
{
    {2, 4},
    {10, 12},
    {18, 20}
};

int g_iShedTimeGunGame[3][2] = 
{
    {4, 6},
    {12, 14},
    {20, 22}
};

int g_iShedTimeTeamGunGame[3][2] = 
{
    {6, 8},
    {14, 16},
    {22, 24}
};

//Handle g_pCreateWeaponTracedBoneMergeEntity;

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
ConVar g_ConvarNecroGunGameTextPosX;
ConVar g_ConvarNecroGunGameTextPosY;

#define GUNGAME_HUDTICK 1.2
#define GUNGAME_VOTEINFO_DELAY 120.0

//int g_iGunGameNextVoteTime = -1;
//int g_iGunGameVoteEnable = 0;
//int g_iGunGameVoteDisable = 0;
//int g_iGunGameVoteSkip = 0;

Handle g_hGunGameHudTimer = INVALID_HANDLE;