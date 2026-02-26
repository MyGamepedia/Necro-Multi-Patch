#pragma newdecls required
#pragma semicolon 1

#if defined SRCCOOP_BLACKMESA
MRESReturn Hook_EventQueueAddEvent_GetAddress(Address _this, DHookParam hParams)
{
    g_EventQueue = _this;
    return MRES_Ignored;
}


/* //didn't find out how to disable it properly in our case
public Action Hook_EventQueueAddEvent_DisableGetAddress(Handle hTimer)
{
    hkEventQueueAddEvent.Disable(Hook_Pre, Hook_EventQueueAddEvent_GetAddress);
    return Plugin_Stop;
}
*/
#endif