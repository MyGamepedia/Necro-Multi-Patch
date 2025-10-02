public MRESReturn Hook_IsMultiplayer(DHookReturn hReturn)
{
	DHookSetReturn(hReturn, g_bIsMultiplayerOverride);
	return MRES_Supercede;
}