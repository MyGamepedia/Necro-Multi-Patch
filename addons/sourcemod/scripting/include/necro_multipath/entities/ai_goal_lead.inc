#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CAI_LeadBehavior
// multiplayer support for ai_goal_lead
//------------------------------------------------------
public MRESReturn Hook_CanSelectSchedule(DHookReturn hReturn)
{
	// ToDo: acutally check conditions as in original code
	DHookSetReturn(hReturn, true);
	return MRES_Supercede;
}