#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_GetUserSettings(int _this, DHookReturn hReturn, DHookParam hParams)
{
    char szCvar[MAX_FORMAT];
    DHookGetParamString(hParams, 1, szCvar, sizeof(szCvar));

    int iClient = _this;

    if(IsFakeClient(iClient) && StrEqual(szCvar, "cl_toggle_duck"))
    {
        DHookSetReturnString(hReturn, "");
        return MRES_Supercede;
    }

    return MRES_Ignored;
}