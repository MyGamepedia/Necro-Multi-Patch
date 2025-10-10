public LaserDot_Path(int entity)
{
	if (!GetConVarBool(g_ConvarNecroClassicLaserDot))
	{
		SDKHook(entity, SDKHook_SetTransmit, Hook_LaserDot_HideOnSpawn_Transmit);
	}
}

public void LaserDot_PathPost(int entity)
{
	if (!IsValidEntity(entity))
	{
		return;
	}
	
	if (!GetConVarBool(g_ConvarNecroClassicLaserDot))
	{
		RequestFrame(LaserDot_RenderFix, entity);
	}
}

public void LaserDot_RenderFix(int entity)
{	
	if (!IsValidEntity(entity))
	{
		return;
	}
	
	int owner = GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity");
	
	if(!(0 < owner <= MaxClients))
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
	DispatchKeyValue(sprite, "renderamt", "255"); //alpha
	DispatchKeyValue(sprite, "rendermode", "9"); //todo: renderfx 15 doesn't work with Additive (5) that could fix some issues with rondom 
												 //appearing through walls on spawn, maybe we can imitate it using logic
	DispatchKeyValue(sprite, "renderfx", "15"); //Distort
	DispatchKeyValue(sprite, "scale", "0.1"); //sprite scale (approximate, compared with the max sprite size for the client's env_laser_dot)
	DispatchKeyValue(sprite, "spawnflags", "1"); //enable on spawn
//	DispatchKeyValue(sprite, "friction", "41414242424242424242"); //mark

	DispatchSpawn(sprite); //spawn new sprite
	
	SetEntPropEnt(sprite, Prop_Data, "m_hDamageFilter", entity); //HACK! use damage filter value to store env_laser_dot (this fieled is never used for sprites anyway)
	
	//the name is the new name for sprite + dot's index
	char name[64];
	Format(name, sizeof(name), "sprite_laserdot_%d", entity);
	DispatchKeyValue(sprite, "targetname", name);
			
	float pos[3];
	GetEntPropVector(entity, Prop_Send, "m_vecOrigin", pos);
			
	TeleportEntity(sprite, pos, NULL_VECTOR, NULL_VECTOR); //note: orig sprite displaced by ~4 units down, any reason we need do so as well ?
	
	SDKUnhook(entity, SDKHook_SetTransmit, Hook_LaserDot_HideOnSpawn_Transmit);
	SDKHook(entity, SDKHook_SetTransmit, Hook_LaserDot_ShowForOwnerOnly_Transmit);
	
	SetEdictFlags(sprite, GetEdictFlags(sprite) & ~(FL_EDICT_ALWAYS|FL_EDICT_DONTSEND|FL_EDICT_PVSCHECK));
	SDKHook(sprite, SDKHook_SetTransmit, Hook_Sprite_HideForLaserDotOwner_Transmit);
	
	RequestFrame(LaserDot_TeleportSprite, sprite); //TODO: maybe there is a better way to teleport sprite ?
}

public void LaserDot_TeleportSprite(int entity)
{
	if (!IsValidEntity(entity))
	{
		return;
	}
	
	int laserdot = GetEntPropEnt(entity, Prop_Data, "m_hDamageFilter");
			
	if(!IsValidEntity(laserdot))
	{
		AcceptEntityInput(entity, "Kill");
		return;
	}
	
	float pos[3];
	GetEntPropVector(laserdot, Prop_Send, "m_vecOrigin", pos);
			
	TeleportEntity(entity, pos, NULL_VECTOR, NULL_VECTOR);
	
	RequestFrame(LaserDot_TeleportSprite, entity);
}

public Action Hook_LaserDot_HideOnSpawn_Transmit(int entity, int client) 
{
	return Plugin_Stop;
}

public Action Hook_LaserDot_ShowForOwnerOnly_Transmit(int entity, int client) 
{
	int owner = GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity");
	
	if (client == owner)
	{
		return Plugin_Continue;
	}

	if (IsClientObserver(client))
	{
		int observertarget = GetEntPropEnt(client, Prop_Data, "m_hObserverTarget");
		int observermode = GetEntProp(client, Prop_Data, "m_iObserverMode");
		
		if (observertarget == owner && observermode == 4)
		{
			return Plugin_Continue;
		}
	}

	return Plugin_Stop;
}

public Action Hook_Sprite_HideForLaserDotOwner_Transmit(int entity, int client) 
{
	int laserdot = GetEntPropEnt(entity, Prop_Data, "m_hDamageFilter");
			
	if(!IsValidEntity(laserdot))
	{
		AcceptEntityInput(entity, "Kill");
		return Plugin_Continue;
	}
			
	int owner = GetEntPropEnt(laserdot, Prop_Send, "m_hOwnerEntity");
	
	if(!IsValidEntity(owner))
	{
		AcceptEntityInput(entity, "Kill");
		return Plugin_Continue;
	}
	
	if (client == owner)
	{	
		return Plugin_Stop;
	}

	if (IsClientObserver(client))
	{
		int observertarget = GetEntPropEnt(client, Prop_Data, "m_hObserverTarget");
		int observermode = GetEntProp(client, Prop_Data, "m_iObserverMode");
		
		if (observertarget == owner && observermode == 4)
		{
			return Plugin_Stop;
		}
	}

	return Plugin_Continue;
}