#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CNPC_PlayerCompanion - npc_human_scientist*, npc_human_security
// scientists should classify as allies (in MP, base function returns false when player is not passed in)
// this allows npcs to pass filter in "player ally only" triggers
//------------------------------------------------------
public MRESReturn Hook_IsPlayerAlly(int _this, DHookReturn hReturn, DHookParam hParams)
{
	DHookSetReturn(hReturn, true);
	return MRES_Supercede;
}

//------------------------------------------------------
// `CNPC_PlayerCompanion` - `npc_human_scientist*`, `npc_human_security`
// This fixes a crash that happens upon dereferencing a null `AI_GetSinglePlayer` instance inside the function `CNPC_PlayerCompanion::IsNavigationUrgent`.
// This crash occurs when the player that this entity is following goes to a unreachable location.
//
// https://github.com/ValveSoftware/source-sdk-2013/blob/39f6dde8fbc238727c020d13b05ecadd31bda4c0/src/game/server/hl2/npc_playercompanion.cpp#L3687C1-L3687C23
//
// The below implementation is not a rebuild of the original code.
//------------------------------------------------------
public MRESReturn Hook_IsNavigationUrgent(int _this, DHookReturn hReturn, DHookParam hParams)
{
	DHookSetReturn(hReturn, true);
	return MRES_Supercede;
}