public MRESReturn Hook_PlayerForceRespawn(int iClient)
{
	float flDeathTime = GetEntPropFloat(iClient, Prop_Send, "m_flDeathTime");
	if (g_bUnblockRespawn[iClient]
		|| flDeathTime == 0.0
		|| (mp_forcerespawn.BoolValue && (GetGameTime() - flDeathTime) > GetConVarFloat(FindConVar("spec_freeze_time")) + GetConVarFloat(FindConVar("spec_freeze_traveltime"))))
	{
		g_bUnblockRespawn[iClient] = false;
		return MRES_Ignored;
	}
	return MRES_Supercede;
}