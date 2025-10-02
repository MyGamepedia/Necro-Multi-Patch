public Frag_Path(int entity)
{
	if (!GetConVarBool(g_ConvarNecroClassicFrags))
	{
		g_bIsMultiplayerOverride = false;
	}
}

public Frag_PathPost(int entity)
{
	if (!GetConVarBool(g_ConvarNecroClassicFrags))
	{
		g_bIsMultiplayerOverride = true;
	}
}