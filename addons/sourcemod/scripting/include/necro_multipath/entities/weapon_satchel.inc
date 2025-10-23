#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_SatchelPrimaryAttackPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
	SetEntPropFloat(_this, Prop_Data, "m_flNextPrimaryAttack", GetGameTime() + GetConVarFloat(g_ConvarNecroSatchelDelay_Attack1_Primary));	

	SetEntPropFloat(_this, Prop_Data, "m_flNextSecondaryAttack", GetGameTime() + GetConVarFloat(g_ConvarNecroSatchelDelay_Attack1_Secondary)); //1.2 is needed to fix 
																																			   //bugs with radio rendering
	
	return MRES_Ignored;
}

public MRESReturn Hook_SatchelSecondaryAttackPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
	SetEntPropFloat(_this, Prop_Data, "m_flNextPrimaryAttack", GetGameTime() + GetConVarFloat(g_ConvarNecroSatchelDelay_Attack2_Primary));
	SetEntPropFloat(_this, Prop_Data, "m_flNextSecondaryAttack", GetGameTime() + GetConVarFloat(g_ConvarNecroSatchelDelay_Attack2_Secondary));
	return MRES_Ignored;
}

public MRESReturn Hook_SatchelReloadPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
	//this func is in endless loop if radio is enabled but no ammo, fix infinit delay by checking player ammo
	int owner = GetEntPropEnt(_this, Prop_Data, "m_hOwner");
	
	if(IsValidEntity(owner))
	{
		int ammo = GetEntProp(owner, Prop_Data, "m_iAmmo", 8);	//grenade_satchel ammo count
		
		if(ammo > 0)
		{
			SetEntPropFloat(_this, Prop_Data, "m_flNextPrimaryAttack", GetGameTime() + GetConVarFloat(g_ConvarNecroSatchelDelay_Reload_Primary));
			SetEntPropFloat(_this, Prop_Data, "m_flNextSecondaryAttack", GetGameTime() + GetConVarFloat(g_ConvarNecroSatchelDelay_Reload_Secondary));
		}
	}
	
	return MRES_Ignored;
}