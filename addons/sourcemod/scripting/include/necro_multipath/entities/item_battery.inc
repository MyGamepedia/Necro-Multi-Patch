#pragma newdecls required
#pragma semicolon 1

void Battery_Path(CItem pItem)
{
    if (!pItem.IsValid())
		return;

    //don't create dlights if not allowed
	if (pItem.GetUserData("m_bNoNewLight") == 1)
        return;
	
	CNewLightPoint pBatteryLight = CNewLightPoint.Create();
	
	if (pBatteryLight == NULL_CBASEENTITY)
		return;
	
	pBatteryLight.SetLightColor(20, 181, 214);
	pBatteryLight.SetEnabled(true);
	pBatteryLight.SetStyle(true);
	pBatteryLight.SetIntensity(256.0);
	pBatteryLight.SetTexName("lights/lightcookie_battery.vtf");
	pBatteryLight.SetTexLight(true);
	pBatteryLight.SetSpecMultiplier(0.0);
	pBatteryLight.SetLightType(2);
	pBatteryLight.SetRange(100.0);
	
	pBatteryLight.Spawn();

	pBatteryLight.SetParent(pItem);
	pBatteryLight.Teleport({0.0, 0.0, 6.0}, vec3_origin); //TODO: Should this be removed ? Orig light is always at battery origin

	// Without modifying these flags, the light's parent will be forced to always transmit.
	pBatteryLight.edictFlags &= ~FL_EDICT_ALWAYS;
	pBatteryLight.edictFlags |= FL_EDICT_PVSCHECK;

	//SDKHook(pBatteryLight.entindex, SDKHook_SetTransmit, Hook_Instancing_ItemParent_Transmit);

    //we need to control dlight state for classic dm batteries
    if (pItem.GetUserData("m_iItemType") == 1 || pItem.GetUserData("m_iItemType") == 2)
        return;

    DHookEntity(hkAcceptInput, false, pItem.entindex, _, Hook_BatteryInputRespawn); //hook battery's inputs so we surely know that this battery is respawned 

	char name[64];
	Format(name, sizeof(name), "newlightpoint_battery_%d", pItem.entindex); //set sprite to control with entity IO
	DispatchKeyValue(pBatteryLight.entindex, "targetname", name);
				
	AddOutput(pItem.entindex, "OnUser4", name, "TurnOn"); //once battery is respawned - it fires "FireUser4" on self, this allows us to enable the dlight
	AddOutput(pItem.entindex, "OnPlayerPickup", name, "TurnOff"); //hide dlight on pickup
}

//Purpose: Check battery's input to trigger dlight show on respawn
public MRESReturn Hook_BatteryInputRespawn(int _this, DHookReturn hReturn, DHookParam hParams)
{
	char szInputType[MAX_FORMAT];
	DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));
	
	if (strcmp(szInputType, "Respawn", false) == 0)
	{
		AcceptEntityInput(_this, "FireUser4");
		return MRES_Ignored;
	}
	
	return MRES_Ignored;
}