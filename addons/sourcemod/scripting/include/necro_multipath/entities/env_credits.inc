#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// env_credits
// multiplayer support
//------------------------------------------------------
public MRESReturn Hook_EnvCreditsAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!DHookIsNullParam(hParams, 1))
	{
		char szInputType[MAX_FORMAT];
		DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));
		if (strcmp(szInputType, "ShowLogo", false) == 0)
		{
			float flLogoLength = GetEntPropFloat(_this, Prop_Data, "m_flLogoLength");
			if (flLogoLength)
			{
				Handle pMsg = StartMessageAll("LogoTimeMsg", USERMSG_RELIABLE);
				if (pMsg != null)
				{
					BfWriteFloat(pMsg, flLogoLength);
					EndMessage();
				}
			}
			else
			{
				Handle pMsg = StartMessageAll("CreditsMsg", USERMSG_RELIABLE);
				if (pMsg != null)
				{
					BfWriteByte(pMsg, 1);
					EndMessage();
				}
			}
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}
		if (strcmp(szInputType, "RollCredits", false) == 0)
		{
			Handle pMsg = StartMessageAll("CreditsMsg", USERMSG_RELIABLE);
			if (pMsg != null)
			{
				BfWriteByte(pMsg, 2);
				EndMessage();
			}
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}
		if (strcmp(szInputType, "RollEndCredits", false) == 0 || strcmp(szInputType, "RollOutroCredits", false) == 0)
		{
			Handle pMsg = StartMessageAll("CreditsMsg", USERMSG_RELIABLE);
			if (pMsg != null)
			{
				BfWriteByte(pMsg, 3);
				EndMessage();
			}
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}
	}
	return MRES_Ignored;
}