#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// point_clientcommand
// multiplayer support
//------------------------------------------------------
public MRESReturn Hook_ClientCommandAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	static bool bHookSkip = false;
	if (!bHookSkip)
	{
		if (!DHookIsNullParam(hParams, 1))
		{
			char szInput[MAX_FORMAT];
			DHookGetParamString(hParams, 1, szInput, sizeof(szInput));
			
			if (!StrEqual(szInput, "CommandAll", false) && !StrEqual(szInput, "Command", false))
			{
				return MRES_Ignored;
			}
			
			char szParam[MAX_FORMAT];
			DHookGetParamObjectPtrString(hParams, 4, 0, ObjectValueType_String, szParam, sizeof(szParam));
			TrimString(szParam);
			
			if (strcmp(szInput, "CommandAll", false) == 0) // new input
			{
				// do special handling for the give command: give it to all and add to equipment for later spawns
				int iStart = 5;
				if (strncmp(szParam, "give ", iStart, false) == 0)
				{
					if (strncmp(szParam[iStart], "item_weapon_", 12, false) == 0)
					{
						iStart += 5; // start after item_
					}
					for (int i = 1; i <= MaxClients; i++)
					{
						if (IsClientInGame(i) && !IsFakeClient(i) && IsPlayerAlive(i))
						{
							CBasePlayer pPlayer = CBasePlayer(i);
							if (!pPlayer.HasWeapon(szParam[iStart]))
							{
								pPlayer.GiveItem(szParam[iStart]);
							}
						}
					}
				}
				else
				{
					bHookSkip = true;
					for (int i = 1; i <= MaxClients; i++)
					{
						if (IsClientInGame(i))
						{
							SetVariantString(szParam);
							AcceptEntityInput(_this, "Command", i);
						}
					}
					bHookSkip = false;
				}
				DHookSetReturn(hReturn, true);
				return MRES_Supercede;
			}
		}
	}
	return MRES_Ignored;
}