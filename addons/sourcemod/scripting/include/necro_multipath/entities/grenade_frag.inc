public Frag_Path(int entity)
{
	if (!GetConVarBool(g_ConvarNecroClassicFrags))
	{
		g_bIsMultiplayerOverride = false;
	}
}

public Frag_PathPost(int entity)
{
//	PrintToServer("Frag_Path: m_hOriginalThrower: %d", GetEntPropEnt(entity, Prop_Data, "m_hOriginalThrower")); //frag creator
//	PrintToServer("Frag_Path: m_hThrower: %d", GetEntPropEnt(entity, Prop_Data, "m_hThrower"));					//frag creator or somebody who last picked up
	if (!GetConVarBool(g_ConvarNecroClassicFrags))
	{
		g_bIsMultiplayerOverride = true;
	}
}