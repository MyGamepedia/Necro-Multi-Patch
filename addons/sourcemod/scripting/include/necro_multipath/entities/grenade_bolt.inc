#pragma newdecls required
#pragma semicolon 1

//Purpose: Set exploding bolt (if enabled) and add particle trail effect
public void Bolt_Path(int entity)
{
	if(GetConVarBool(sk_crossbow_tracer_enabled) || (!GetConVarBool(g_ConvarNecroExplodingBolt) && !GetConVarBool(sk_crossbow_tracer_enabled)))
	{
		//set proper skin for the bolt
		SetEntProp(entity, Prop_Send, "m_nSkin", 1);
	}
	
	if (GetConVarBool(g_ConvarNecroBoltParticles))
	{
		#if defined DEBUG
		PrintToServer("Bolt_Path: Allowed to create particle for entity (%d)", entity);
		#endif
		
		int particle = CreateEntityByName("info_particle_system");
		
		SetEntPropString(particle, Prop_Data, "m_iszEffectName", "grenade_bolt_trail"); //todo: life time seems to be very short, replace with custom ?
		
		SetEntProp(particle, Prop_Data, "m_bStartActive", 1);
		
		float pos[3];
		GetEntPropVector(entity, Prop_Send, "m_vecOrigin", pos);
		
		TeleportEntity(particle, pos, NULL_VECTOR, NULL_VECTOR);
		
		SetVariantString("!activator");
		AcceptEntityInput(particle, "SetParent", entity);
		
		DispatchSpawn(particle);
		
		ActivateEntity(particle);	//needed to start particle effect
	}
}

//Purpose: Decline "SetToExplode" input if exploading bolts are disabled
public MRESReturn Hook_GrenadeBoltAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if(GetConVarBool(sk_crossbow_tracer_enabled) || (!GetConVarBool(g_ConvarNecroExplodingBolt) && !GetConVarBool(sk_crossbow_tracer_enabled)))
	{
		char szInputType[MAX_FORMAT];
		DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));
		if (strcmp(szInputType, "SetToExplode", false) == 0)
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}
	}

	return MRES_Ignored;
}