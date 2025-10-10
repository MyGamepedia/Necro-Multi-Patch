public Mp5Contact_Path(int entity)
{
    if (!IsValidEntity(entity))
        return;
	
	if (GetConVarBool(g_ConvarNecroMp5ContactParticles))
	{
		#if defined DEBUG
		PrintToServer("Mp5Contact_Path: Allowed to create particle for entity (%d)", entity);
		#endif
		
		int particle = CreateEntityByName("info_particle_system");
		
		SetEntPropString(particle, Prop_Data, "m_iszEffectName", "grenade_mp5_trail"); 
		
		SetEntProp(particle, Prop_Data, "m_bStartActive", 1);
		
		float pos[3];
		GetEntPropVector(entity, Prop_Send, "m_vecOrigin", pos);
		
		TeleportEntity(particle, pos, NULL_VECTOR, NULL_VECTOR);
		
		SetVariantString("!activator");
		AcceptEntityInput(particle, "SetParent", entity);
		
		DispatchSpawn(particle);
		
		ActivateEntity(particle);	
	}
}