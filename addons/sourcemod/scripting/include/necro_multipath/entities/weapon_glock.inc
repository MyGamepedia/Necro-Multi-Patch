#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_WeaponGlockItemHolsterFramePost(int _this)
{
	Hook_BaseCombatWeaponItemHolsterFramePost(_this, GetConVarFloat(g_ConvarNecroAutoReloadTime_Glock));
	return MRES_Ignored;
}