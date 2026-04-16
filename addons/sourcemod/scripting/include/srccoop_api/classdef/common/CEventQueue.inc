#pragma newdecls required
#pragma semicolon 1

static Handle g_pAddEvent;

methodmap CEventQueue
{
	public static void InitClassdef(const GameData hGameConfig)
	{
		char szAddEvent[] = "CEventQueue::AddEvent";
		StartPrepSDKCall(SDKCall_Raw);
		if (!PrepSDKCall_SetFromConf(hGameConfig, SDKConf_Signature, szAddEvent))
			LogMessage("Could not obtain gamedata signature %s", szAddEvent);
		else
		{
			PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Pointer);
			PrepSDKCall_AddParameter(SDKType_String, SDKPass_Pointer);
			PrepSDKCall_AddParameter(SDKType_Float, SDKPass_Plain);
			PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Pointer);
			PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Pointer);
			PrepSDKCall_AddParameter(SDKType_PlainOldData, SDKPass_Plain);
			if (!(g_pAddEvent = EndPrepSDKCall())) SetFailState("Could not prep SDK call %s", szAddEvent);
		}
	}
	
	//Calls CEventQueue::AddEvent.
	public static void AddEvent(const CBaseEntity pEntity, const char[] szInput, const float flDelay = 0.0, const CBaseEntity pActivator = NULL_CBASEENTITY, const CBaseEntity pCaller = NULL_CBASEENTITY, const int iOutputID = 0)
	{
		if (g_EventQueue != Address_Null)
		{
			SDKCall(g_pAddEvent, g_EventQueue, pEntity.entindex, szInput, flDelay, pActivator.entindex, pCaller.entindex, iOutputID);
		}
		else
			PrintToServer("[SourceCoop API]: NULL ADDRESS FOR g_EventQueue!!!");
	}
}