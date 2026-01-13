#pragma newdecls required
#pragma semicolon 1

//Purpose: Enable VPhysics for this frag by disabling multiplayer override
public void Frag_Path(int entity)
{
	if (!GetConVarBool(g_ConvarNecroClassicFrags))
	{
		g_bIsMultiplayerOverride = false;
	}

	CBlackMesaBaseDetonator pFrag = CBlackMesaBaseDetonator(entity);
	pFrag.SetRenderMode(RENDER_NONE);
    pFrag.AcceptInput("DisableShadow");
    pFrag.AddOutput("OnUser4", "!self", "Addoutput", "rendermode 0", 0.01, 1);
    pFrag.AddOutput("OnUser4", "!self", "EnableShadow", "", 0.01, 1);
    pFrag.FireOutput("OnUser4"); 
}

//Purpose: Post spawn hook for grenade_frag to reset multiplayer override
public void Frag_PathPost(int entity)
{
//	PrintToServer("Frag_Path: m_hOriginalThrower: %d", GetEntPropEnt(entity, Prop_Data, "m_hOriginalThrower")); //frag creator
//	PrintToServer("Frag_Path: m_hThrower: %d", GetEntPropEnt(entity, Prop_Data, "m_hThrower"));					//frag creator or somebody who last picked up
	if (!GetConVarBool(g_ConvarNecroClassicFrags))
	{
		g_bIsMultiplayerOverride = true;
	}
}