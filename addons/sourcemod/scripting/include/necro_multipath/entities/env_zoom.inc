#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CEnvZoom - env_zoom
// multiplayer support
//------------------------------------------------------
public MRESReturn Hook_EnvZoomAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	static bool bHookSkip = false;
	if (!bHookSkip)
	{
		if (!DHookIsNullParam(hParams, 1))
		{
			char szInput[MAX_FORMAT];
			DHookGetParamString(hParams, 1, szInput, sizeof(szInput));
		
			CBaseEntity pActivator;
			if (!DHookIsNullParam(hParams, 2))
			{
				pActivator = CBaseEntity(DHookGetParam(hParams, 2));
			}
			
			char szNewInput[8];
			int iNewClient;
			
			if (strcmp(szInput, "Zoom", false) == 0)
			{
				szNewInput = "Zoom";
				if (pActivator.IsPlayer())
				{
					iNewClient = pActivator.entindex;
				}
			}
			else if (strcmp(szInput, "ZoomAll", false) == 0)
			{
				szNewInput = "Zoom";
			}
			else if (strcmp(szInput, "UnZoom", false) == 0)
			{
				szNewInput = "UnZoom";
			}
			else
			{
				return MRES_Ignored;
			}
			
			bHookSkip = true;
			if (iNewClient > 0)
			{
				AddLocalPlayerOverrideEx(iNewClient);
				AcceptEntityInput(_this, szNewInput);
				PopLocalPlayerOverride();
			}
			else
			{
				for (int i = 1; i <= MaxClients; i++)
				{
					if (IsClientInGame(i))
					{
						AddLocalPlayerOverrideEx(i);
						AcceptEntityInput(_this, szNewInput);
						PopLocalPlayerOverride();
					}
				}
			}
			bHookSkip = false;
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}
	}
	return MRES_Ignored;
}
