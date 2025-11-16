#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CAI_MoveProbe::TestGroundMove
// Linux crash fix (Xen grunts?)
//------------------------------------------------------
public MRESReturn Hook_TestGroundMove(Address _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!hParams.IsNull(6))
	{
		float flTotalDist = hParams.GetObjectVar(6, 32, ObjectValueType_Float);
		if (flTotalDist == 1.0 / 0.0) // infinity check
		{
			hReturn.Value = false;
			return MRES_Supercede;
		}
	}
	return MRES_Ignored;
}