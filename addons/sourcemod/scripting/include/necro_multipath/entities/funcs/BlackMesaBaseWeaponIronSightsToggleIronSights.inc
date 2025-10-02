public MRESReturn Hook_ToggleIronsights(int entity, DHookParam hParams)
{
	//use def skin for the crossbow when used xbow scope and mp skin on zoom disabled
	char classname[64];
	GetEntityClassname(entity, classname, sizeof(classname));
	
	if(StrEqual(classname, "weapon_crossbow"))
	{
		int owner = GetEntPropEnt(entity, Prop_Send, "m_hOwner");
		
		if((0 < owner <= MaxClients))
		{		
			int viewmodel = GetEntPropEnt(owner, Prop_Send, "m_hViewModel");
			
			if(IsValidEntity(viewmodel))
			{
				int skinnum = GetEntProp(viewmodel, Prop_Send, "m_nSkin");
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
	
	return MRES_Ignored;
}