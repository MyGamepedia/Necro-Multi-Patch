public MRESReturn Hook_CanisterInputRespawn(int _this, DHookReturn hReturn, DHookParam hParams)
{
	char szInputType[MAX_FORMAT];
	DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));
	
	if (strcmp(szInputType, "Respawn", false) == 0)
	{
		AcceptEntityInput(_this, "FireUser4");
		return MRES_Ignored;
	}
	
	return MRES_Ignored;
}