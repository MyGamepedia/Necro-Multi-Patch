int GetChild(const int entity, const char[] classname)
{
	if (!IsValidEntity(entity)) return -1;
	
	int child = GetEntPropEnt(entity, Prop_Data, "m_hMoveChild");
	char childclassname[64];
	
	while (child != -1)
	{
		GetEntityClassname(child, childclassname, sizeof(childclassname));
		
		if(StrEqual(childclassname, classname))
		{
			PrintToServer("GetChild: childclassname %s, classname %s", childclassname, classname);
			
			//used for now as mark for spawned ents by this plugin, maybe we will find better way
			int hammerid = GetEntProp(child, Prop_Data, "m_iHammerID");
			//float friction = GetEntPropFloat(child, Prop_Data, "m_flFriction");
			
			//PrintToServer("friction: %f hammerid %d", friction, hammerid);
			
			if(hammerid == 0) 
			{		
				return child;
			}
		}
		
		child = GetEntPropEnt(child, Prop_Data, "m_hMovePeer");
	}
	
//	PrintToServer("GetChild: Nothing found");
	return -1;
}