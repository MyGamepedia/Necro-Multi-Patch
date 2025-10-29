//Purpose: Update spectator join team delay value when convar is changed
public void OnSpectatorJoinTeamDelayChanged(ConVar convar, const char[] oldValue, const char[] newValue)
{
	g_fClientSpectatorJoinTeamDelay[0] = GetConVarFloat(g_ConvarNecroSpectatorJoinTeamDelay); //HACK! Use fist (unused) element in the array to store cvar delay value
}