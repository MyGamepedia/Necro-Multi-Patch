#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_GetUserSettings(int _this, DHookReturn hReturn, DHookParam hParams)
{
    PrintToChatAll("Hook_GetUserSettings");
    /*
	char szClientCommand[MAX_FORMAT];
	DHookGetParamString(hParams, 2, szClientCommand, sizeof(szClientCommand));

    int iCBasePlayer = DHookGetParam(hParams, 1);

    if(IsFakeClient(iCBasePlayer) && StrEqual(szClientCommand, "cl_toggle_duck"))
    {
        DHookSetReturn(hReturn, "");
        return MRES_Supercede;
    }*/

    return MRES_Ignored;
}