#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CPointTeleport - point_teleport
// multiplayer support
//------------------------------------------------------
public MRESReturn Hook_PointTeleportAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!DHookIsNullParam(hParams, 1))
	{
		char szInputType[MAX_FORMAT];
		DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));
		if (strcmp(szInputType, "Teleport", false) == 0)
		{
			char szTarget[MAX_FORMAT];
			CBaseEntity pTeleport = CBaseEntity(_this);
			pTeleport.GetTarget(szTarget, sizeof(szTarget));
			if (strcmp(szTarget, "!player", false) == 0)
			{
				float origin[3], angles[3];
				GetEntPropVector(_this, Prop_Data, "m_vSaveOrigin", origin);
				GetEntPropVector(_this, Prop_Data, "m_vSaveAngles", angles);
				for (int i = 1; i <= MaxClients; i++)
				{
					if (IsClientInGame(i) && !IsFakeClient(i) && IsPlayerAlive(i))
					{
						TeleportEntity(i, origin, angles, NULL_VECTOR);
					}
				}
				DHookSetReturn(hReturn, true);
				return MRES_Supercede;
			}
		}
	}
	return MRES_Ignored;
}