#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// Allow flashlight
//------------------------------------------------------
public MRESReturn Hook_FAllowFlashlight(DHookReturn hReturn, DHookParam hParams)
{
	DHookSetReturn(hReturn, mp_flashlight.BoolValue);
	return MRES_Supercede;
}

//------------------------------------------------------
// Override multiplayer check to force singleplayer mode for certain entities
//------------------------------------------------------
public MRESReturn Hook_IsMultiplayer(DHookReturn hReturn)
{
	DHookSetReturn(hReturn, g_bIsMultiplayerOverride);
	return MRES_Supercede;
}