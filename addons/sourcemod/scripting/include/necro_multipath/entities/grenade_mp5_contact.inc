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

//undone "you can't use second attack while mp5 contact grenade is alive" feature
/*
public MRESReturn Hook_Mp5ContactThink(int _this)
{
	if (!g_ConvarNecroMp5ContactBlockSecondAttack.BoolValue)
		return MRES_Ignored;

	CBlackMesaPlayer pOwner = CBlackMesaBaseDetonator(_this).GetOwner();

	if (pOwner == NULL_CBASEENTITY || !pOwner.IsValid() || pOwner.GetActiveWeapon() == NULL_CBASEENTITY)
		return MRES_Ignored;

	CBaseCombatWeapon pWeapon = pOwner.GetActiveWeapon();

	char szWeaponClassname[MAX_CLASSNAME];
	pWeapon.GetClassname(szWeaponClassname, sizeof(szWeaponClassname));
	
	//i can't fire the weapon while mp5 contact grenade is alive
	if (StrEqual(szWeaponClassname, "weapon_mp5"))
	{
		float flInitialNextSecondaryAttack = pWeapon.GetUserData("InitialNextSecondaryAttack");

		if (flInitialNextSecondaryAttack != flInitialNextSecondaryAttack)
		{
			PrintToChatAll("initial next secondary attack time stored: %.2f", pWeapon.GetNextSecondaryAttack());
			//store the initial next secondary attack time when the grenade is thrown, so we can restore it later
			pWeapon.SetUserData("InitialNextSecondaryAttack", pWeapon.GetNextSecondaryAttack());
		}
		pWeapon.SetNextSecondaryAttack(FLT_MAX);
	}

	return MRES_Ignored;
}

public MRESReturn Hook_Mp5ContactUpdateOnRemove(int _this)
{
	PrintToChatAll("removed");
	if (!g_ConvarNecroMp5ContactBlockSecondAttack.BoolValue)
		return MRES_Ignored;

	CBlackMesaPlayer pOwner = CBlackMesaBaseDetonator(_this).GetOwner();

	if (pOwner == NULL_CBASEENTITY || !pOwner.IsValid() || pOwner.GetActiveWeapon() == NULL_CBASEENTITY || !pOwner.GetActiveWeapon())
		return MRES_Ignored;

	CBaseCombatWeapon pWeapon = pOwner.GetActiveWeapon();

	char szWeaponClassname[MAX_CLASSNAME];
	pWeapon.GetClassname(szWeaponClassname, sizeof(szWeaponClassname));

	PrintToChatAll("weapon seq: %d", pWeapon.GetSequence());

	int iSeq = pWeapon.GetSequence();

	//don't in case of "use" seq 
	if (iSeq == 9 || iSeq == 10)
		return MRES_Ignored;
	
	//now i can shoot
	if (strncmp(szWeaponClassname, "weapon_mp5", 6) == 0)
	{
		if (pWeapon.GetNextSecondaryAttack() == FLT_MAX)
		{
			pWeapon.SetNextSecondaryAttack(GetGameTime());
		}
		else
		{
			float flInitialNextSecondaryAttack = pWeapon.GetUserData("InitialNextSecondaryAttack");

			if (flInitialNextSecondaryAttack == flInitialNextSecondaryAttack)
			{
				pWeapon.SetNextSecondaryAttack(flInitialNextSecondaryAttack);
			}
		}
	}

	return MRES_Ignored;
}
*/