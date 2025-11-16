#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// `env_xen_portal_effect`
// Fixes xen portals not pushing players.
//------------------------------------------------------
public MRESReturn Hook_XenPortalEffect_AcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	char szInputType[MAX_FORMAT];
	DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));
	if (strcmp(szInputType, "PushAway", false) == 0 || strcmp(szInputType, "Burst", false) == 0)
	{
		CEnv_XenPortalEffect pPortalEffect = CEnv_XenPortalEffect(_this);

		float flSize = pPortalEffect.GetSize();
		float vec3PortalEffectPosition[3];
		pPortalEffect.GetAbsOrigin(vec3PortalEffectPosition);

		for (int i = 1; i <= MaxClients; ++i)
		{
			CBasePlayer pPlayer = CBasePlayer(i);
			if (pPlayer != NULL_CBASEENTITY && pPlayer.IsAlive())
			{
				float vec3PlayerPosition[3];
				pPlayer.GetAbsOrigin(vec3PlayerPosition);
				
				if (GetVectorDistance(vec3PortalEffectPosition, vec3PlayerPosition) < (flSize * 2))
				{
					float vec3Distance[3];
					SubtractVectors(vec3PlayerPosition, vec3PortalEffectPosition, vec3Distance);
					NormalizeVector(vec3Distance, vec3Distance);

					float vec3Angles[3];
					GetVectorAngles(vec3Distance, vec3Angles);

					float vec3Forward[3];
					GetAngleVectors(vec3Angles, vec3Forward, NULL_VECTOR, NULL_VECTOR);
					ScaleVector(vec3Forward, 1024.0);	// TODO: Add better implementation matching original game.

					pPlayer.SetAbsVelocity(vec3Forward);
				}
			}
		}
	}
	return MRES_Ignored;
}