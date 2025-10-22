#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_FireBolt(int _this, DHookReturn hReturn, DHookParam hParams)
{
	g_bIsMultiplayerOverride = false;
	return MRES_Ignored;
}

public MRESReturn Hook_FireBoltPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
	g_bIsMultiplayerOverride = true;
	return MRES_Ignored;
}