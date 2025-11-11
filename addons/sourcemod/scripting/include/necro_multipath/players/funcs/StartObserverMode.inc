#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_PlayerStartObserverMode(int iClient, DHookReturn hReturn, DHookParam hParams)
{
	int observermode = hParams.Get(1);
	int observertarget = GetEntPropEnt(iClient, Prop_Data, "m_hObserverTarget");

	//todo: make a better fix by counting the time so we switch before spec_freeze_traveltime is finished
	if(observertarget != iClient) //check if the target is me, if yes - don't do anything, so the camera don't flying "to infinity and being"
	{
		if (observermode == 2 || observermode == 1)
		{
			hParams.Set(1, 5);
			return MRES_Handled;
		}
	}

	return MRES_Ignored;
}