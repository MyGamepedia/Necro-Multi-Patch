#pragma newdecls required
#pragma semicolon 1

//Original code by: fdxx
//Edits by: MyGamepedia
MRESReturn Hook_StartLagCompensation(Address pThis, DHookParam hParams)
{
	int iClient = hParams.Get(1);
	
	if (iClient > 0 && iClient <= MaxClients && IsClientInGame(iClient) && !IsFakeClient(iClient))
	{
		Address pClient = hParams.GetAddress(1);
		Address pCUserCmd = LoadFromAddress(pClient + view_as<Address>(g_iUserCmdOffset), NumberType_Int32);
		if (pCUserCmd == Address_Null)
		{
			return MRES_Supercede;
		}
	}
	
	return MRES_Ignored;
}