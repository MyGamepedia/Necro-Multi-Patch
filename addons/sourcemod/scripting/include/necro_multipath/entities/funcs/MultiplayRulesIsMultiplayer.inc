//Purpose: Override multiplayer check to force singleplayer mode for certain entities
public MRESReturn Hook_IsMultiplayer(DHookReturn hReturn)
{
	DHookSetReturn(hReturn, g_bIsMultiplayerOverride);
	return MRES_Supercede;
}