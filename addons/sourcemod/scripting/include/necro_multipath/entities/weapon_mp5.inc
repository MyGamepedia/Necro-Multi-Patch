#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_WeaponMp5ItemHolsterFramePost(int _this)
{
	Hook_BaseCombatWeaponItemHolsterFramePost(_this, GetConVarFloat(g_ConvarNecroAutoReloadTime_Mp5));
	return MRES_Ignored;
}