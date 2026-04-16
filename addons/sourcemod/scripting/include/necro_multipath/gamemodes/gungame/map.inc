#pragma newdecls required
#pragma semicolon 1

//Hook events from gameevents.res and set gamemode rules
public void GunGameMapStart()
{	
	if(GetConVarBool(g_ConvarNecroGunGameEnabled)) //check if we want the mode to be enabled
	{
        g_iNecroGunGameCurrentState = 1; //tell the ui and giving items func that text that the mode isn't finished

	    GunGameDisableCanisterDrops(); //disable canister drops and set no score limit
	    GunGameRemoveItems(); //remove all entities with item_weapon_ prefix (we guess this mode will not be launched on a "special map")

        //init hud timer to display the ui
        if (g_hGunGameHudTimer == INVALID_HANDLE)  
            g_hGunGameHudTimer = CreateTimer(GUNGAME_HUDTICK, GunGameDisplayHud, _, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);  

        HookEventEx("player_death", GunGameEventDeath, EventHookMode_Pre);
		HookEventEx("player_spawn", GunGameEventSpawn, EventHookMode_Pre);	
		HookEventEx("player_team", GunGamePlayerTeamChanged, EventHookMode_Post);
		HookEventEx("round_start", GunGameOnRoundStart, EventHookMode_Post);
		HookEventEx("round_end", GunGameOnRoundIntermission, EventHookMode_Post);
	}
    else
    {
        g_iNecroGunGameCurrentState = -1; //tell the ui and giving items func that text that the mode is disabled
    }
}

//Disable hooks if a map unexpectedly runs out
public void GunGameMapEnd()
{
    if(g_iNecroGunGameCurrentState == 1)
    {
        UnhookEvent("player_death", GunGameEventDeath, EventHookMode_Pre);
        UnhookEvent("player_spawn", GunGameEventSpawn, EventHookMode_Pre);	
        UnhookEvent("player_team", GunGamePlayerTeamChanged, EventHookMode_Post);
        UnhookEvent("round_start", GunGameOnRoundStart, EventHookMode_Post);
        UnhookEvent("round_end", GunGameOnRoundIntermission, EventHookMode_Post);
        g_iNecroGunGameCurrentState = 2;

        //no need for the ui now, remove the timer
        if (g_hGunGameHudTimer != INVALID_HANDLE)  
        {  
            KillTimer(g_hGunGameHudTimer);  
            g_hGunGameHudTimer = INVALID_HANDLE;  
        }
    }
}
