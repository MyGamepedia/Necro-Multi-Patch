#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CFuncTank - func_50cal, func_tank, func_tank_combine_cannon, func_tankairboatgun, func_tanklaser,
//			   func_tankmortar, func_tankphyscannister, func_tow, func_tow_mp
// Marines lose their guns after a tank control (actually the weapon becomes broken).
// This hook sets back proper desired weapon state and visibility for it.
//------------------------------------------------------
public void Hook_BaseTank_OnLostController(const char[] szOutput, const int iCaller, const int iActivator, const float flDelay)
{
    CBaseCombatCharacter pController = CBaseCombatCharacter(GetEntPropEnt(iCaller, Prop_Data, "m_hController"));  

	//no controller
    if (pController == NULL_CBASEENTITY)  
        return;  
		
    CBaseCombatWeapon pWeapon = pController.GetActiveWeapon();  
  
	//we don't have a weapon
    if (pWeapon == NULL_CBASEENTITY)  
        return;
		
	SetEntProp(pController.entindex, Prop_Data, "m_iDesiredWeaponState", DESIREDWEAPONSTATE_IGNORE); //restores shooting ability
	pWeapon.m_fEffects &= ~EF_NODRAW; //restores weapon visibility
}