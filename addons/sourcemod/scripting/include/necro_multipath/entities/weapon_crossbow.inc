#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_CrossbowDeploy(int _this, DHookReturn hReturn, DHookParam hParams)
{
	//Set def skin if exploding bolts are disabled
	int owner = GetEntPropEnt(_this, Prop_Data, "m_hOwner");

	if(IsValidEntity(owner) && (0 < owner <= MaxClients))
	{
		int blackmesaviewmodel = GetEntPropEnt(owner, Prop_Data, "m_hViewModel");

		if(IsValidEntity(blackmesaviewmodel))
		{
			if(GetConVarBool(sk_crossbow_tracer_enabled) || (!GetConVarBool(g_ConvarNecroExplodingBolt) && !GetConVarBool(sk_crossbow_tracer_enabled)))
			{
				SetEntProp(blackmesaviewmodel, Prop_Send, "m_nSkin", 0);
			}
			else
			{
				SetEntProp(blackmesaviewmodel, Prop_Send, "m_nSkin", 1);
			}
		}
	}

	return MRES_Ignored;
}

public MRESReturn Hook_CrossbowFireBolt(int _this, DHookReturn hReturn, DHookParam hParams)
{
	g_bIsMultiplayerOverride = false;

	//We don't need to check every frame OR use complex logic when it's changed, just check and fix on fire
	int owner = GetEntPropEnt(_this, Prop_Data, "m_hOwner");

	if(IsValidEntity(owner) && (0 < owner <= MaxClients))
	{
		int blackmesaviewmodel = GetEntPropEnt(owner, Prop_Data, "m_hViewModel");

		if(IsValidEntity(blackmesaviewmodel))
		{
			if(GetConVarBool(sk_crossbow_tracer_enabled) || (!GetConVarBool(g_ConvarNecroExplodingBolt) && !GetConVarBool(sk_crossbow_tracer_enabled)))
			{
				SetEntProp(blackmesaviewmodel, Prop_Send, "m_nSkin", 0);
			}
			else
			{
				SetEntProp(blackmesaviewmodel, Prop_Send, "m_nSkin", 1);
			}
		}
	}

	return MRES_Ignored;
}

public MRESReturn Hook_CrossbowFireBoltPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
	g_bIsMultiplayerOverride = true;
	return MRES_Ignored;
}

public MRESReturn Hook_WeaponCrossbowItemHolsterFramePost(int _this)
{
	Hook_BaseCombatWeaponItemHolsterFramePost(_this, GetConVarFloat(g_ConvarNecroAutoReloadTime_Crossbow));
	return MRES_Ignored;
}