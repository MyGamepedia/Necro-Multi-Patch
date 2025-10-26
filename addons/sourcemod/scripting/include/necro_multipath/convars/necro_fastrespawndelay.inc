//Purpose: Update fast respawn delay value when convar is changed
public void OnFastRespawnDelayChanged(ConVar convar, const char[] oldValue, const char[] newValue)
{
	g_fClientFastRespawnDelay[0] = GetConVarFloat(g_ConvarNecroFastRespawnDelay); //HACK! Use fist (unused) element in the array to store cvar delay value
}