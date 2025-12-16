#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_Weapon357ItemHolsterFramePost(int _this)
{
	Hook_BaseCombatWeaponItemHolsterFramePost(_this, GetConVarFloat(g_ConvarNecroAutoReloadTime_357));
	return MRES_Ignored;
}