#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CBoneSetup::AccumulatePose
// fix few random cases where the cycle is NaN
//------------------------------------------------------
public MRESReturn Hook_AccumulatePose(Address _this, DHookParam hParams)
{
	float cycle = hParams.Get(4);
	// NaN Check
	if (cycle != cycle)
	{
		hParams.Set(4, 0.0);
		return MRES_ChangedHandled;
	}
	return MRES_Ignored;
}
