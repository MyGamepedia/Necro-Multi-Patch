#pragma newdecls required
#pragma semicolon 1

//Purpose: Allow pick up objects if we want
public MRESReturn Hook_PropBreakableRagdollInitRagdoll(int _this)
{
	if (GetConVarBool(g_ConvarNecroAllowPickupObjects))
	{
		g_bIsMultiplayerOverride = false;
	}
	return MRES_Ignored;
}

//Purpose: Post spawn hook for pickup object to reset multiplayer override
public MRESReturn Hook_PropBreakableRagdollInitRagdollPost(int _this)
{
	if (GetConVarBool(g_ConvarNecroAllowPickupObjects))
	{
		g_bIsMultiplayerOverride = true;
	}
	return MRES_Ignored;
}