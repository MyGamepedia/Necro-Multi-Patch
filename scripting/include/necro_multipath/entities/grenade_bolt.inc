public Bolt_Path(int entity)
{
	//set proper skin for the bolt
	SetEntProp(entity, Prop_Send, "m_nSkin", 1);
	
	if (GetConVarBool(g_ConvarNecroBoltSpriteTrail))
	{
		int spritetrail = CreateEntityByName("env_spritetrail");
		if (spritetrail == -1) return;

		PrecacheModel("sprites/xbeam4.vmt", true);
		DispatchKeyValue(spritetrail, "spritename", "sprites/xbeam4.vmt");
		DispatchKeyValue(spritetrail, "endwidth", "1");
		DispatchKeyValue(spritetrail, "lifetime", "0.5");
		DispatchKeyValue(spritetrail, "startwidth", "2");
		DispatchKeyValue(spritetrail, "rendermode", "5");
		DispatchKeyValue(spritetrail, "rendercolor", "255 255 255");
		DispatchKeyValue(spritetrail, "renderamt", "220");
		DispatchKeyValue(spritetrail, "renderfx", "0");
		DispatchKeyValue(spritetrail, "disablereceiveshadows", "0");
//		DispatchKeyValue(spritetrail, "friction", "41424242424242424242.0"); //mark

		DispatchSpawn(spritetrail);
		
		char name[64];
		Format(name, sizeof(name), "spritetrail_bolt_%d_necro", entity);
		DispatchKeyValue(spritetrail, "targetname", name);
		
		float pos[3];
		GetEntPropVector(entity, Prop_Send, "m_vecOrigin", pos);
		
		//Attach
		TeleportEntity(spritetrail, pos, NULL_VECTOR, NULL_VECTOR);
		SetVariantString("!activator");
		AcceptEntityInput(spritetrail, "SetParent", entity);
		
		DHookEntity(hkBlackMesaBaseDetonatorDetonate, false, entity, _, Hook_BoltDetonate);
	}
}

public MRESReturn Hook_BoltDetonate(int _this, DHookParam hParams)
{
	PrintToServer("Hook_BoltDetonate");
	int spritetrail = GetChild(_this, "env_spritetrail");
	
	if(!IsValidEntity(spritetrail))
	{
		PrintToServer("No valid env_spritetrail");
		return MRES_Ignored;
	}
	else
	{
		PrintToServer("env_spritetrail: %d", spritetrail);
	}
	
	AcceptEntityInput(spritetrail, "ClearParent");
	
	float pos[3];
	GetEntPropVector(_this, Prop_Send, "m_vecOrigin", pos);	
	
	TeleportEntity(spritetrail, pos, NULL_VECTOR, NULL_VECTOR);
	
	char output[128];
	Format(output, sizeof(output), "OnUser1 !self,Kill,,3.00,1");
	SetVariantString(output);
	AcceptEntityInput(spritetrail, "AddOutput");
	
	AcceptEntityInput(spritetrail, "FireUser1");
	
	return MRES_Ignored;
}