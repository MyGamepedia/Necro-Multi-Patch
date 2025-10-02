#pragma newdecls required
#pragma semicolon 1

bool AddOutput(const int entity, const char[] szOutput, const char[] szTarget, const char[] szTargetInput, 
					  const char[] szParameter = "",const float flDelay = 0.0, const int iTimesToFire = -1)
{
	char szBuffer[256];
	Format(szBuffer, sizeof(szBuffer), "%s %s:%s:%s:%f:%d", szOutput, szTarget, szTargetInput, szParameter, flDelay, iTimesToFire);
	SetVariantString(szBuffer);
	
	return AcceptEntityInput(entity, "AddOutput");
}