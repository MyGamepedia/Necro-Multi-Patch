#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// player_loadsaved
// multiplayer support
//------------------------------------------------------
public MRESReturn Hook_LoadSavedAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	//TODO: Replace with convar logic
	/*if (!CoopManager.IsCoopModeEnabled())
	{
		return MRES_Ignored;
	}*/

	char szInputType[MAX_FORMAT];
	DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));

	if (strcmp(szInputType, "Reload", false) == 0)
	{
		if (!DHookIsNullParam(hParams, 2))
		{
			int iActivator = DHookGetParam(hParams, 2);
			CBaseEntity pActivator = CBaseEntity(iActivator);
			if (pActivator != NULL_CBASEENTITY && pActivator.IsPlayer())
			{
				CBasePlayer pPlayer = view_as<CBasePlayer>(pActivator);
				CRevertSaved pThis = CRevertSaved(_this);
				pPlayer.ScreenFade(RoundFloat(pThis.GetDuration() * 1000), pThis.GetRenderColor(), FFADE_OUT | FFADE_STAYOUT);
				
				// This breaks the spectator overlay if the player is already dead
				if (pPlayer.IsAlive())
				{
					pPlayer.Suicide();
				}
			}
		}
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}
	
	return MRES_Ignored;
}