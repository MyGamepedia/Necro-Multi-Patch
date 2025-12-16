#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_WeaponAssassinGlockItemHolsterFramePost(int _this)
{
	Hook_BaseCombatWeaponItemHolsterFramePost(_this, GetConVarFloat(g_ConvarNecroAutoReloadTime_Assassin_Glock));
	return MRES_Ignored;
}