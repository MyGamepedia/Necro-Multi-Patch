#pragma newdecls required
#pragma semicolon 1

//Purpose: Add particle trail effect to MP5 contact grenade
public void Mp5Contact_Path(int entity)
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
		
		SetEntProp(particle, Prop_Data, "m_bStartActive", 0);

		char szTargetname[MAX_FORMAT];
		Format(szTargetname, sizeof(szTargetname), "mp5contact_particles_%d", entity);
		DispatchKeyValue(particle, "targetname", szTargetname);
		
		float pos[3];
		GetEntPropVector(entity, Prop_Send, "m_vecOrigin", pos);
		
		TeleportEntity(particle, pos, NULL_VECTOR, NULL_VECTOR);
		
		SetVariantString("!activator");
		AcceptEntityInput(particle, "SetParent", entity);
		
		DispatchSpawn(particle);
		
		ActivateEntity(particle);	//needed to start particle effect
	}

	//this entity appears in owners head for a while, fix it by hidding this entity for a while
	CBaseEntity pMp5Contact = CBaseEntity(entity);

    pMp5Contact.SetRenderMode(RENDER_NONE);
    pMp5Contact.AcceptInput("DisableShadow");
    pMp5Contact.AddOutput("OnUser4", "!self", "Addoutput", "rendermode 0", 0.005, 1);
    pMp5Contact.AddOutput("OnUser4", "!self", "EnableShadow", "", 0.005, 1);

	if (GetConVarBool(g_ConvarNecroMp5ContactParticles))
	{
		char szTargetname[MAX_FORMAT];
		Format(szTargetname, sizeof(szTargetname), "mp5contact_particles_%d", entity);
		pMp5Contact.AddOutput("OnUser4", szTargetname, "Start", "", 0.005, 1);
	}

    pMp5Contact.FireOutput("OnUser4");
}