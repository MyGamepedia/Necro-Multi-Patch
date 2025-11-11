#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_GiveDefaultItems(int _this)
{
	//don't give items to specs
	int team = GetEntProp(_this, Prop_Data, "m_iTeamNum");

	if((g_iTeamplay && (team == 1 || team == 0)))
		return MRES_Supercede;

	if(team == 1)
		return MRES_Supercede;

	//give items if allowed
	if (!GetConVarBool(g_ConvarNecroGiveDefaultItems))
	{
		return MRES_Supercede;
	}

	return MRES_Ignored;
}