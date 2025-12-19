#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_WeaponRpgAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	//skip in case if input name is nullprt
	if (DHookIsNullParam(hParams, 1))
	{
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}
    
    //get input name
	char szInputName[MAX_FORMAT];
	DHookGetParamString(hParams, 1, szInputName, sizeof(szInputName));

	//Reload input
	if (strcmp(szInputName, "Reload", false) == 0)
	{
        CWeapon_Rpg pRpg = CWeapon_Rpg(_this);

        //don't if owner is not alive
        if(pRpg.GetOwner().IsAlive())
        {
            pRpg.Reload();
            DHookSetReturn(hReturn, true);
            return MRES_Supercede;
        }

		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}

    return MRES_Ignored;
}

public MRESReturn Hook_WeaponRpgHolsterPost(int _this)
{
	return MRES_Ignored;
}

public MRESReturn Hook_WeaponRpgDeploy(int _this)
{
	return MRES_Ignored;
}