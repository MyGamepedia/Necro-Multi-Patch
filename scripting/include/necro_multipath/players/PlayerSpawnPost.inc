public MRESReturn Hook_PlayerSpawnPost(int iClient, DHookReturn hReturn, DHookParam hParams)
{
	bool alive = GetEntProp(iClient, Prop_Send, "m_bAlive");
	if (alive)
	{
	}
	else
	{
		pPlayer.QueryConvar("r_bloomtintexponent_nextgen", QueryConVar_Bloom, 2.2);
		pPlayer.QueryConvar("r_bloomtintr_nextgen", QueryConVar_Bloom, 0.25);
		pPlayer.QueryConvar("r_bloomtintg_nextgen", QueryConVar_Bloom, 0.25);
		pPlayer.QueryConvar("r_bloomtintb_nextgen", QueryConVar_Bloom, 0.25);
		pPlayer.QueryConvar("gb_flashlight_intensity", QueryConVar_Bloom, 0.8);
		QueryClientConVar(this.entindex, szCvar, fnCallback, value);
	}
	
	return MRES_Ignored;
}