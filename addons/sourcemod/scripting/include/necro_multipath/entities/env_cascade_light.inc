#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_CascadeLightKeyValueHACK(int _this, DHookReturn hReturn, DHookParam hParams)
{
	char szKeyName[MAX_FORMAT];
	DHookGetParamString(hParams, 1, szKeyName, sizeof(szKeyName));

	//check if it wants manual mode to prevent broken csm in mp
	//it's important to fix here because resetting or recreating doesn't work and client will always use manual mode even if false for cascade light ent
	if (StrEqual(szKeyName, "CSMVolumeMode", false))
	{
		char szValue[MAX_VALUE];
		DHookGetParamString(hParams, 2, szValue, sizeof(szValue));
		if (StrEqual(szValue, "1", false))
		{
			//HACK! This is illegal (although doesn't cause any issues), but also the only consistent way,
			//returning false/true doesn't work consistently (tones of tests was done)
			return MRES_Supercede;
		}
	}

	return MRES_Ignored;
}


//------------------------------------------------------
// env_cascade_light
// block SetCSMVolume* inputs to prevent heavy console spam "YOU SH ALL BURN IN HELL" msg (while we have no client code XD)
//------------------------------------------------------
public MRESReturn Hook_EnvCascadeLightAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!DHookIsNullParam(hParams, 1))
	{
		char szInputType[MAX_FORMAT];
		DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));
		if (strcmp(szInputType, "SetCSMVolume", false) == 0)
		{
            DHookSetReturn(hReturn, true);
			return MRES_Supercede;
        }

        if (strcmp(szInputType, "SetCSMVolumeToSceneBounds", false) == 0)
		{
            DHookSetReturn(hReturn, true);
			return MRES_Supercede;
        }

        if (strcmp(szInputType, "SetCSMVolumeViaCSMVolumeEntity", false) == 0)
		{
            DHookSetReturn(hReturn, true);
			return MRES_Supercede;
        }
    }

    return MRES_Ignored;
}