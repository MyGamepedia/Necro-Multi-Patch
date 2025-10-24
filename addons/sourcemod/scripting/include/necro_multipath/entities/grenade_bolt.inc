#pragma newdecls required
#pragma semicolon 1

//Purpose: Set exploding bolt (if enabled) and add particle trail effect
public void Bolt_Path(int entity)
{
	if (GetConVarBool(g_ConvarNecroExplodingBolt))
	{
		//set proper skin for the bolt
		SetEntProp(entity, Prop_Send, "m_nSkin", 1);
		
		//enable exploding bolt
		SetEntProp(entity, Prop_Data, "m_bExplodingBolt", 1);
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