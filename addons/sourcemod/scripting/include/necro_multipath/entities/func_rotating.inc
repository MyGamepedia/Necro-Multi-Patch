#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// func_rotating
// Fix the "Bad SetLocalAngles" error where the rotating entity stops after its angles have reached a limit
//------------------------------------------------------
public Action Timer_FixRotatingAngles(Handle timer, CBaseEntity pEntity)
{
	if (pEntity.IsValid())
	{
		float vecAngles[3];
		pEntity.GetAngles(vecAngles);
		
		for (int i = 0; i < 3; i++)
		{
			vecAngles[i] = FloatFraction(vecAngles[i]) + RoundToFloor(vecAngles[i]) % 360;
		}
		pEntity.SetAngles(vecAngles);
		
		return Plugin_Continue;
	}
	return Plugin_Stop;
}