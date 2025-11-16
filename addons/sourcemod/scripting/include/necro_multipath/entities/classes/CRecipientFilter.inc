#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// Unblocks sending missing TE's (impact effects)
// by disabling serverside prediction recipient culling
//------------------------------------------------------
bool g_bIgnorePredictionCull = true;

public MRESReturn Hook_IgnorePredictionCull(DHookReturn hReturn)
{
	hReturn.Value = g_bIgnorePredictionCull;
	g_bIgnorePredictionCull = true; // reset for next call
	return MRES_Supercede;
}