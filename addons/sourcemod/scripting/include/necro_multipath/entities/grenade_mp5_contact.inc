public Mp5Contact_Path(int entity)
{
	if (GetConVarBool(g_ConvarNecroMp5ContactParticles))
	{
		int particlesystem = CreateEntityByName("info_particle_system");
		
		if (particlesystem == -1)
			return;

		DispatchKeyValue(particlesystem, "effect_name", "handgrenade_spark");
		DispatchKeyValue(particlesystem, "start_active", "1");
//		DispatchKeyValue(particlesystem, "friction", "41424242424242424242.0"); //mark

		SetEntPropEnt(particlesystem, Prop_Send, "m_hOwnerEntity", entity);
		
		float pos[3];
		GetEntPropVector(entity, Prop_Send, "m_vecOrigin", pos);
		
		//Attach
		TeleportEntity(particlesystem, pos, NULL_VECTOR, NULL_VECTOR);
		
		DispatchSpawn(particlesystem);
	}
}