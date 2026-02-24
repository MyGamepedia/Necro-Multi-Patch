#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// env_cascade_light
// Block CSMVolume inputs to prevent console spam.
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