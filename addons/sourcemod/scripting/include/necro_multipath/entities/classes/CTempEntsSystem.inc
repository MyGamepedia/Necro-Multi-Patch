#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_DispatchEffect(DHookParam hParams)
{
	// World impacts are correctly predicted by client.
	// Filter them out out of the server dispatched ones.
	char szName[16];
	hParams.GetString(4, szName, sizeof(szName));
	if (StrEqual(szName, "Impact"))
	{
		g_bIgnorePredictionCull = false;
	}
	#if defined SRCCOOP_HL2DM
	if (StrEqual(szName, "StunstickImpact"))
	{
		g_bIgnorePredictionCull = false;
	}
	#endif
	return MRES_Ignored;
}