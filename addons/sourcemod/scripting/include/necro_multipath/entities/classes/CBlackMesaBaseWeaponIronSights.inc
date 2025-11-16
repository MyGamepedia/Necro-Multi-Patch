//Purpose: Toggle crossbow viewmodel skin when toggling ironsights to use the same skin as hitscan bolts
public MRESReturn Hook_ToggleIronsights(int entity, DHookParam hParams)
{
	//use def skin for the crossbow when used xbow scope and mp skin on zoom disabled
	char classname[64];
	GetEntityClassname(entity, classname, sizeof(classname));
	
	if(StrEqual(classname, "weapon_crossbow"))
	{
		//dob't toggle if not using exploding bolts
		if(GetConVarBool(sk_crossbow_tracer_enabled) || GetConVarBool(g_ConvarNecroExplodingBolt) && !GetConVarBool(sk_crossbow_tracer_enabled))
		{
			int owner = GetEntPropEnt(entity, Prop_Send, "m_hOwner"); //get weapon owner
			
			if((0 < owner <= MaxClients)) //make sure owner is valid
			{		
				int viewmodel = GetEntPropEnt(owner, Prop_Send, "m_hViewModel"); //get player's viewmodel
				
				if(IsValidEntity(viewmodel)) //make sure viewmodel is valid
				{
					int skinnum = GetEntProp(viewmodel, Prop_Send, "m_nSkin");

					//set skin based on the current skin
					if(skinnum == 1)
					{
						SetEntProp(viewmodel, Prop_Send, "m_nSkin", 0);
					}
					else if(skinnum == 0)
					{
						SetEntProp(viewmodel, Prop_Send, "m_nSkin", 1);
					}
				}
			}
		}
	}
	
	return MRES_Ignored;
}