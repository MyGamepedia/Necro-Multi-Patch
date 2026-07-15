#pragma newdecls required
#pragma semicolon 1

void Hook_NewLightPointSpawnPost(int iEntIndex)
{
	CBaseEntity pEntity = CBaseEntity(iEntIndex);
	pEntity.edictFlags &= ~FL_EDICT_ALWAYS;
	pEntity.edictFlags |= FL_EDICT_PVSCHECK;
}

public Action Hook_NewLightPointTransmit(int iEntIndex, int iClient) 
{
	if (iClient != 0) //hide for owner
	{	
		return Plugin_Stop;
	}
	return Plugin_Continue;
}

public MRESReturn Hook_NewLightPointAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	PrintToChatAll("NEWLIGHT TEST START");

	//skip in case if input name is nullprt
	if (DHookIsNullParam(hParams, 1))
	{
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}

	PrintToChatAll("NEWLIGHT TEST NOT NULL");
    
    //get input name
	char szInputName[MAX_FORMAT];
	DHookGetParamString(hParams, 1, szInputName, sizeof(szInputName));

	if (strcmp(szInputName, "test", false) == 0)
	{
		PrintToChatAll("NEWLIGHT TEST TESTED");

		CBaseEntity pEntity = CBaseEntity(_this);
		pEntity.edictFlags &= ~FL_EDICT_ALWAYS;
		pEntity.edictFlags |= FL_EDICT_PVSCHECK;

		SDKHook(_this, SDKHook_SetTransmit, Hook_NewLightPointTransmit);

		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}
	else
	{
		PrintToChatAll("NEWLIGHT TEST 42");
	}

	if (strcmp(szInputName, "test2", false) == 0)
	{
		CreateTimer(1.0, test2, _this);
	}

    return MRES_Ignored;
}

public Action test2(Handle timer, int _this)
{
	PrintToChatAll("m_bEnableGodRays %d", GetEntProp(_this, Prop_Send, "m_bEnableGodRays"));	
	PrintToChatAll("m_fDecay %.2f", GetEntPropFloat(_this, Prop_Send, "m_fDecay"));	
	CreateTimer(1.0, test2, _this);
	return Plugin_Stop;
}