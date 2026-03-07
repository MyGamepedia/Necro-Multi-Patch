#pragma newdecls required
#pragma semicolon 1

#if defined SRCCOOP_BLACKMESA
MRESReturn Hook_EventQueueAddEvent_GetAddress(Address _this, DHookParam hParams)
{
    if (g_EventQueue != Address_Null)
    {
        CreateTimer(0.0, Hook_EventQueueAddEvent_DisableGetAddress, _, TIMER_FLAG_NO_MAPCHANGE);
        return MRES_Ignored;
    }
    else
    {
        g_EventQueue = _this;
    }

    return MRES_Ignored;
}

public Action Hook_EventQueueAddEvent_DisableGetAddress(Handle hTimer)
{
    hkEventQueueAddEvent.Disable(Hook_Pre, Hook_EventQueueAddEvent_GetAddress);
    return Plugin_Stop;
}
#endif