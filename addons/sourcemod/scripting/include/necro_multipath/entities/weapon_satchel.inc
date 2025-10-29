#pragma newdecls required
#pragma semicolon 1

//Purpose: Set satchel weapon attack delay based on convar settings after primary attack
public MRESReturn Hook_SatchelPrimaryAttackPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if(GetConVarBool(g_ConvarNecroSatchelDelayOverride))
	{
		SetEntPropFloat(_this, Prop_Data, "m_flNextPrimaryAttack", GetGameTime() + GetConVarFloat(g_ConvarNecroSatchelDelay_Attack1_Primary)); //0.84 is needed to fix 
																																			   //bugs with satchel rendering

		SetEntPropFloat(_this, Prop_Data, "m_flNextSecondaryAttack", GetGameTime() + GetConVarFloat(g_ConvarNecroSatchelDelay_Attack1_Secondary)); //1.2 is needed to fix 
																																				   //bugs with radio rendering
	}

	return MRES_Ignored;
}

//Purpose: Set satchel weapon attack delay based on convar settings after secondary attack
public MRESReturn Hook_SatchelSecondaryAttackPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if(GetConVarBool(g_ConvarNecroSatchelDelayOverride))
	{
		SetEntPropFloat(_this, Prop_Data, "m_flNextPrimaryAttack", GetGameTime() + GetConVarFloat(g_ConvarNecroSatchelDelay_Attack2_Primary));
		SetEntPropFloat(_this, Prop_Data, "m_flNextSecondaryAttack", GetGameTime() + GetConVarFloat(g_ConvarNecroSatchelDelay_Attack2_Secondary));
	}

	return MRES_Ignored;
}

//Purpose: The method resets delay values, we need to set again so they are not overwritten
public MRESReturn Hook_SatchelReloadPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if(GetConVarBool(g_ConvarNecroSatchelDelayOverride))
	{
		//the method is in endless loop if radio is enabled but no ammo, fix infinit delay by checking player ammo
		int owner = GetEntPropEnt(_this, Prop_Data, "m_hOwner");
		
		if(IsValidEntity(owner)) //check if owner is valid
		{
			int ammo = GetEntProp(owner, Prop_Send, "m_iAmmo", _, 9); //grenade_satchel ammo count
			
			if(ammo > 0) //set values if we have ammo
			{
				SetEntPropFloat(_this, Prop_Data, "m_flNextPrimaryAttack", GetGameTime() + GetConVarFloat(g_ConvarNecroSatchelDelay_Reload_Primary));
				SetEntPropFloat(_this, Prop_Data, "m_flNextSecondaryAttack", GetGameTime() + GetConVarFloat(g_ConvarNecroSatchelDelay_Reload_Secondary));
			}
		}
	}
	
	return MRES_Ignored;
}