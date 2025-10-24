//Purpose: Get child entity of specific classname
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
			#if defined DEBUG
			PrintToServer("GetChild: Child classname: %s. Parameter classname: %s", childclassname, classname);
			#endif
			
			//used for now as mark for spawned ents by this plugin, maybe we will find better way
			int hammerid = GetEntProp(child, Prop_Data, "m_iHammerID");
			//float friction = GetEntPropFloat(child, Prop_Data, "m_flFriction");
			
			if(hammerid == 0) 
			{
				#if defined DEBUG
				PrintToServer("GetChild: Return child: index - %d, child classname - %s, parameter classname - %s", child, childclassname, classname);
				#endif
				
				return child;
			}
		}
		
		child = GetEntPropEnt(child, Prop_Data, "m_hMovePeer");
	}
	
	#if defined DEBUG
	PrintToServer("GetChild: Return -1");
	#endif
	return -1;
}