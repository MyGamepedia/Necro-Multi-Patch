#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_NewLightPointAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
    PrintToChatAll("HOOKED");
	//skip in case if input name is nullprt
	if (DHookIsNullParam(hParams, 1))
	{
		DHookSetReturn(hReturn, true);
		return MRES_Ignored;
	}

	//get input name
	char szInputName[MAX_FORMAT];
	DHookGetParamString(hParams, 1, szInputName, sizeof(szInputName));



    if (strcmp(szInputName, "Test", false) == 0)
    {
        PrintToChatAll("TEST INPUT RECEIVED");
        CNewLightPoint pNewLightPoint = CNewLightPoint(_this);

        SDKHook(_this, SDKHook_SetTransmit, NewLightPoint_Hook_Transmit);
        
        DHookSetReturn(hReturn, true);
        return MRES_Supercede;
    }

    return MRES_Ignored;
}

public Action NewLightPoint_Hook_Transmit(int entity, int client) 
{
	if (client == 1) //hide for owner
	{	
		return Plugin_Stop;
	}

    return Plugin_Continue;
}