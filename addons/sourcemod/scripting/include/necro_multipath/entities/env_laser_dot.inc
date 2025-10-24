#pragma newdecls required
#pragma semicolon 1

//Purpose: Set laser_dot disabled on start, so we don't see flickering when toggle on
public void LaserDot_Path(int entity)
{
	if (!GetConVarBool(g_ConvarNecroClassicLaserDot))
	{
		SDKHook(entity, SDKHook_SetTransmit, Hook_LaserDot_HideOnSpawn_Transmit);
	}
}

//Purpose: Post spawn hook for env_laser_dot to fix rendering issues
public void LaserDot_PathPost(int entity)
{
	if (!IsValidEntity(entity))
	{
		return;
	}
	
	//we need a delay, because owner isn't set on spawn
	if (!GetConVarBool(g_ConvarNecroClassicLaserDot))
	{
		RequestFrame(LaserDot_RenderFix, entity);
	}
}

//Purpose: Create a sprite to follow the laser dot, and set transmit hooks
public void LaserDot_RenderFix(int entity)
{
	if (!IsValidEntity(entity))
	{
		return;
	}
	
	int owner = GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity"); //get owner (player)
	
	if(!(0 < owner <= MaxClients)) //check if this owner is a valid player
	{
		return;
	}

	int sprite = CreateEntityByName("env_sprite");
	if (sprite == -1)
		return;
	
	#if defined DEBUG
	PrecacheModel("sprites/glow1.vmt", true);
	DispatchKeyValue(sprite, "model", "sprites/glow1.vmt"); //use this texture for debug, so we know what/where/when the sprite
	#else
	PrecacheModel("sprites/redglow1_path.vmt", true); 
	DispatchKeyValue(sprite, "model", "sprites/redglow1_path.vmt"); //orig "sprites/redglow1.vmt"
	#endif
	
	DispatchKeyValue(sprite, "rendercolor", "255 255 255"); //def
	DispatchKeyValue(sprite, "renderamt", "255"); //alpha, also def
	DispatchKeyValue(sprite, "rendermode", "9"); //world space glow
	DispatchKeyValue(sprite, "renderfx", "15"); //Distort
	DispatchKeyValue(sprite, "scale", "0.1"); //sprite scale, orig is 0.5 iicr, but too big for in this case
	DispatchKeyValue(sprite, "spawnflags", "1"); //enable on spawn
//	DispatchKeyValue(sprite, "friction", "41414242424242424242"); //mark

	DispatchSpawn(sprite);
	
	SetEntPropEnt(sprite, Prop_Data, "m_hDamageFilter", entity); //HACK! use damage filter value to store env_laser_dot (this fieled is never used for sprites anyway)
																 //needed cuz if sprite is paranted, it will use parent's transmit hook
	
	#if defined DEBUG
	//the name is the new name for sprite + dot's index
	char name[64];
	Format(name, sizeof(name), "sprite_laserdot_%d", entity);
	DispatchKeyValue(sprite, "targetname", name);
	#endif
			
	float pos[3];
	GetEntPropVector(entity, Prop_Send, "m_vecOrigin", pos); //get current laserdot position
			
	TeleportEntity(sprite, pos, NULL_VECTOR, NULL_VECTOR); //note: orig sprite displaced by ~4 units down, any reason we need do so as well ?
	
	SDKUnhook(entity, SDKHook_SetTransmit, Hook_LaserDot_HideOnSpawn_Transmit); //disable this hook
	
	SDKHook(entity, SDKHook_SetTransmit, Hook_LaserDot_ShowForOwnerOnly_Transmit); //show for owner pov only
	
	//set these flags so the sprite can work with custom transmit hooks
	SetEdictFlags(sprite, GetEdictFlags(sprite) & ~(FL_EDICT_ALWAYS|FL_EDICT_DONTSEND|FL_EDICT_PVSCHECK));
	
	SDKHook(sprite, SDKHook_SetTransmit, Hook_Sprite_HideForLaserDotOwner_Transmit); //don't show for owner pov
	
	RequestFrame(LaserDot_TeleportSprite, sprite); //TODO: maybe there is a better way to teleport sprite ?
}

//Purpose: Teleport the sprite to the laserdot position every frame
public void LaserDot_TeleportSprite(int entity)
{
	if (!IsValidEntity(entity))
	{
		return;
	}
	
	int laserdot = GetEntPropEnt(entity, Prop_Data, "m_hDamageFilter");
	
	//kill the sprite if no more laserdot we can teleport to
	if(!IsValidEntity(laserdot))
	{
		AcceptEntityInput(entity, "Kill");
		return;
	}
	
	float pos[3];
	GetEntPropVector(laserdot, Prop_Send, "m_vecOrigin", pos);
			
	TeleportEntity(entity, pos, NULL_VECTOR, NULL_VECTOR);
	
	//cycle teleport
	RequestFrame(LaserDot_TeleportSprite, entity);
}

//Purpose: Hide laserdot on spawn to avoid flickering
public Action Hook_LaserDot_HideOnSpawn_Transmit(int entity, int client) 
{
	return Plugin_Stop;
}

//Purpose: Show for laserdot's owner and spectators if  ObserverTarget == owner and if in first person mode
public Action Hook_LaserDot_ShowForOwnerOnly_Transmit(int entity, int client) 
{
	int owner = GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity");
	
	if (client == owner)
	{
		return Plugin_Continue;
	}

	if (IsClientObserver(client)) //check if client is in observer mode
	{
		int observertarget = GetEntPropEnt(client, Prop_Data, "m_hObserverTarget"); //get target being observed
		int observermode = GetEntProp(client, Prop_Data, "m_iObserverMode"); //get observer mode
		
		if (observertarget == owner && observermode == 4) //if target is owner and in first person mode - show laserdot
		{
			return Plugin_Continue;
		}
	}

	return Plugin_Stop;
}

//Purpose: Hide sprite for laserdot's owner
public Action Hook_Sprite_HideForLaserDotOwner_Transmit(int entity, int client) 
{
	int laserdot = GetEntPropEnt(entity, Prop_Data, "m_hDamageFilter"); //get laserdot we need from this fieled
			
	if(!IsValidEntity(laserdot)) //chechk if laserdot is valid
	{
		AcceptEntityInput(entity, "Kill");
		return Plugin_Continue;
	}
			
	int owner = GetEntPropEnt(laserdot, Prop_Send, "m_hOwnerEntity"); //get laserdot owner
	
	if(!IsValidEntity(owner)) //remove sprite if no valid owner
	{
		AcceptEntityInput(entity, "Kill");
		return Plugin_Continue;
	}
	
	if (client == owner) //hide for owner
	{	
		return Plugin_Stop;
	}

	if (IsClientObserver(client)) //check if client is in observer mode
	{
		int observertarget = GetEntPropEnt(client, Prop_Data, "m_hObserverTarget"); //get target being observed
		int observermode = GetEntProp(client, Prop_Data, "m_iObserverMode"); //get observer mode
		
		if (observertarget == owner && observermode == 4) //if target is owner and in first person mode - hide sprite
		{
			return Plugin_Stop;
		}
	}

	return Plugin_Continue;
}