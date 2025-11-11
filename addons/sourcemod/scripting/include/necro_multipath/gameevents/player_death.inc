//Purpose: Set fast respawn delay when player dies
public void Event_PlayerDeath(Event event, const char[] name, bool dontBroadcast)
{
    int client = GetClientOfUserId(event.GetInt("userid"));
	
	g_fClientFastRespawnDelay[client] = GetGameTime() + g_fClientFastRespawnDelay[0];
		
	#if defined DEBUG
	PrintToServer("Event_PlayerDeath: g_fClientFastRespawnDelay[%d]: %f", client, g_fClientFastRespawnDelay[client]);
	#endif
}