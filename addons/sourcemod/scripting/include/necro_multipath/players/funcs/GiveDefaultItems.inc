#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_GiveDefaultItems(int _this)
{
	if (!GetConVarBool(g_ConvarNecroGiveDefaultItems))
	{
		return MRES_Supercede;
	}
	return MRES_Ignored;
}