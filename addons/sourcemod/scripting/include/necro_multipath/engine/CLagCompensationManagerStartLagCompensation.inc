#pragma newdecls required
#pragma semicolon 1

//Purpose: Prevents engine error if "const CUserCmd *cmd = player->GetCurrentUserCommand() == nullprt", we need it to fix tracerbullet crash when it hits something
MRESReturn Hook_StartLagCompensation(Address pThis, DHookParam hParams)
{
	int iClient = hParams.Get(1); //get the client
	
	if (iClient > 0 && iClient <= MaxClients && IsClientInGame(iClient) && !IsFakeClient(iClient)) //make sure client is valid
	{
		Address pClient = hParams.GetAddress(1); //get the player pointer
		Address pCUserCmd = LoadFromAddress(pClient + view_as<Address>(g_iUserCmdOffset), NumberType_Int32); //get the CUserCmd pointer from player structure
		
		if (pCUserCmd == Address_Null) //if CUserCmd is nullptr - prevent engine from using this func to avoid crash
		{
			return MRES_Supercede;
		}
	}
	
	return MRES_Ignored;
}