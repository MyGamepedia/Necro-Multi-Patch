#pragma newdecls required
#pragma semicolon 1

#define MAX_SHOTGUN_CLIP 8 //todo: it's better not hardcode this

//Purpurse: Auto reload shotgun when holster time exceeds defined convar time
public MRESReturn Hook_WeaponShotgunItemHolsterFramePost(int _this)
{
	CBaseCombatWeapon pWeapon = CBaseCombatWeapon(_this);
	
	if (GetConVarBool(g_ConvarNecroAllowWeaponAutoReload) && 
        (GetTickedTime() - view_as<float>(pWeapon.GetUserData("m_flHolsterTime"))) > 
        GetConVarFloat(g_ConvarNecroAutoReloadTime_Shotgun))
	{
		pWeapon.SetUserData("m_flHolsterTime", GetTickedTime());

		if (pWeapon.GetOwner() == NULL_CBASEENTITY)
            return MRES_Ignored;

        int iPrimaryAmmo = pWeapon.GetPrimaryAmmo();
        if(iPrimaryAmmo == MAX_SHOTGUN_CLIP)
            return MRES_Ignored;

        CBaseCombatCharacter pOwner = pWeapon.GetOwner();

        int iPrimaryAmmoType = pWeapon.GetPrimaryAmmoType();
        int iAmmoFromIndex = pOwner.GetAmmoFromIndex(view_as<AmmoType_t>(iPrimaryAmmoType));
        int iLeftAmmo = MAX_SHOTGUN_CLIP - iPrimaryAmmo;

        int iAmmoFill = iLeftAmmo < iAmmoFromIndex ? iLeftAmmo : iAmmoFromIndex;

        pWeapon.SetPrimaryAmmo(iPrimaryAmmo + iAmmoFill);
        pOwner.SetAmmoFromIndex(view_as<AmmoType_t>(iPrimaryAmmoType), iAmmoFromIndex - iAmmoFill);
	}
	
	return MRES_Ignored;
}