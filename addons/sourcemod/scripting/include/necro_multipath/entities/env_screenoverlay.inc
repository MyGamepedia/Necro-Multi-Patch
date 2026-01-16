#pragma newdecls required
#pragma semicolon 1

#if defined ENTPATCH_ENV_SCREENOVERLAY
//------------------------------------------------------
// CEnvScreenOverlay - env_screenoverlay
// Inputs hook for custom logic.
//------------------------------------------------------
public MRESReturn Hook_EnvScreenoverlayAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	//skip in case if input name is nullprt
	if (DHookIsNullParam(hParams, 1))
	{
		DHookSetReturn(hReturn, true);
		return MRES_Ignored;
	}

	//get input name
	char szInputName[MAX_FORMAT];
	DHookGetParamString(hParams, 1, szInputName, sizeof(szInputName));



	//==================================================
	//StartOverlays input
	//==================================================
	if (strcmp(szInputName, "StartOverlays", false) == 0)
	{
		//we don't have rights to override global overlay entity, ignore this input
		if (g_iActiveScreenOverlayEntity[0] != 0)
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;			
		}

		//don't do anything in case if player is nullprt for some reason
		if (DHookIsNullParam(hParams, 2))
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}

		CBasePlayer	pPlayer = DHookGetParam(hParams, 2);
		CEnvScreenOverlay pScreenOverlay = CEnvScreenOverlay(_this);

		//don't active me if i'm already active for this player
		if (g_iActiveScreenOverlayEntity[pPlayer.entindex] == pScreenOverlay.entindex)
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}
		
		int iTextureIndex = pScreenOverlay.GetUserData("m_iOverlayIndex");

		//get material that we will apply for screen by getting it from starting overlay
		char szOverlayName[MAX_FORMAT];
		pScreenOverlay.GetOverlayName(iTextureIndex, szOverlayName, sizeof(szOverlayName));

		//print warning and do nothing if the first overlay keyvalue is empty, just like in source sdk
		//(https://github.com/ValveSoftware/source-sdk-2013/blob/2d3a6efb50bba856a44e73d4f0098ed4a726699c/src/game/server/env_screenoverlay.cpp#L134C12-L134C62)
		if (szOverlayName[0] == '\0')
		{
			//don't use this code unless debugging or you just want to have a very heavy spam in server console
			/*
			//note: added entindex so we can define "invalid" entity by index if no targetname for it
			char szTargetname[MAX_FORMAT];
			pScreenOverlay.GetTargetname(szTargetname, sizeof(szTargetname));
			PrintToServer("WARNING: env_screenoverlay (%d) with targetname \"%s\" has no overlays to display.\n", pScreenOverlay.entindex, szTargetname);
			*/
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}

		//if other local screenoverlay is active - stop overlay for it before we use this entity
		if (g_iActiveScreenOverlayEntity[pPlayer.entindex] != 0)
		{
			AcceptEntityInput(g_iActiveScreenOverlayEntity[pPlayer.entindex], "StopOverlays", pPlayer.entindex);
		}

		//send command to apply screen overlay
		char szPlayerCommand[MAX_FORMAT];
		Format(szPlayerCommand, sizeof(szPlayerCommand), "r_screenoverlay %s", szOverlayName);
		pPlayer.SendCommand(szPlayerCommand); //apply overlay

		g_iActiveScreenOverlayEntity[pPlayer.entindex] = pScreenOverlay.entindex; //store active overlay entindex for this player

		int iNextOverlay = iTextureIndex + 1;

		if (iNextOverlay < 0)
			iNextOverlay = 0;
		
		if (iNextOverlay > 9)
			iNextOverlay = 9;

		g_iNextScreenOverlayIndex[pPlayer.entindex] = iNextOverlay; //set next overlay we want to play

		float flOverlayDuration = pScreenOverlay.GetOverlayTimes(0);

		//don't do anything anymore if used infinite time for the first overlay
		if (flOverlayDuration < 0)
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}

		g_flNextOverlayTime[pPlayer.entindex] = GetTickedTime() + flOverlayDuration;

		//don't create more timers if already active
		if (pScreenOverlay.GetUserData("m_bIsActive") == false)
		{
			pScreenOverlay.SetUserData("m_bIsActive", true);

			//Think() doesn't seem to work on this entity at all, use i/o
			pScreenOverlay.AddOutput("OnUser4", "!self", "Think", "", 0.01, 1);
			pScreenOverlay.FireOutput("OnUser4"); //TODO: replace with AddEvent to event queue when we will have everything needed
		}

		//prevent orig code
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}
	


	//==================================================
	//StopOverlays input, needs !activator or param to work
	//==================================================
	if (strcmp(szInputName, "StopOverlays", false) == 0)
	{
		CBasePlayer pPlayer = NULL_CBASEENTITY;

		//param 4 is empty, check if param 2 isn't nullprt
		if (!DHookIsNullParam(hParams, 2))
		{
			pPlayer = DHookGetParam(hParams, 2);
		}
		else
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}

		CEnvScreenOverlay pScreenOverlay = CEnvScreenOverlay(_this);

		//don't if player's active overlay isn't me
		if (g_iActiveScreenOverlayEntity[pPlayer.entindex] != pScreenOverlay.entindex)
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}

		pPlayer.SendCommand("r_screenoverlay off"); //used by source source sdk code as disabled overlay val

		g_iActiveScreenOverlayEntity[pPlayer.entindex] = 0; //mark that this player has no active ent overlays
		pScreenOverlay.SetUserData("m_bIsActive", false);

		//prevent orig code
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}



	//==================================================
	//StartOverlaysAll input (CUSTOM INPUT), starts overlays for all players
	//==================================================
	if (strcmp(szInputName, "StartOverlaysAll", false) == 0)
	{
		g_iActiveScreenOverlayEntity[0] = _this; //mark that global overlay entity is active
		CEnvScreenOverlay pScreenOverlay = CEnvScreenOverlay(_this);
		int iTextureIndex = pScreenOverlay.GetUserData("m_iOverlayIndex");
		char szOverlayName[MAX_FORMAT];
		pScreenOverlay.GetOverlayName(iTextureIndex, szOverlayName, sizeof(szOverlayName));

		// If the first overlay is empty, do nothing for all
		if (szOverlayName[0] == '\0')
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}

		float flOverlayDuration = pScreenOverlay.GetOverlayTimes(0);

		for (int i = 1; i <= MaxClients; i++)
		{
			CBasePlayer pPlayer = CBasePlayer(i);
			if (!pPlayer.IsValid() || !IsClientInGame(i) || IsFakeClient(i) || !IsValidEntity(i))
				continue;

			// Don't activate if already active for this player
			if (g_iActiveScreenOverlayEntity[pPlayer.entindex] == pScreenOverlay.entindex)
				continue;

			// If another overlay is active, stop it first
			if (g_iActiveScreenOverlayEntity[pPlayer.entindex] != 0)
			{
				AcceptEntityInput(g_iActiveScreenOverlayEntity[pPlayer.entindex], "StopOverlaysAll", pPlayer.entindex);
			}

			// Send overlay command
			char szPlayerCommand[MAX_FORMAT];
			Format(szPlayerCommand, sizeof(szPlayerCommand), "r_screenoverlay %s", szOverlayName);
			pPlayer.SendCommand(szPlayerCommand);

			g_iActiveScreenOverlayEntity[pPlayer.entindex] = pScreenOverlay.entindex; //store active overlay entindex for this player

			int iNextOverlay = iTextureIndex + 1;

			if (iNextOverlay < 0)
				iNextOverlay = 0;
		
			if (iNextOverlay > 9)
				iNextOverlay = 9;

			g_iNextScreenOverlayIndex[pPlayer.entindex] = iNextOverlay;

			// Only set timer if overlay is not infinite
			if (flOverlayDuration >= 0)
			{
				g_flNextOverlayTime[pPlayer.entindex] = GetTickedTime() + flOverlayDuration;
			}
		}

		// Only create timer if not already active
		if (pScreenOverlay.GetUserData("m_bIsActive") == false && flOverlayDuration >= 0)
		{
			pScreenOverlay.SetUserData("m_bIsActive", true);
			pScreenOverlay.AddOutput("OnUser4", "!self", "Think", "", 0.01, 1);
			pScreenOverlay.FireOutput("OnUser4"); //TODO: replace with AddEvent to event queue when we will have everything needed
		}

		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}



	//==================================================
	//StopOverlaysAll input (CUSTOM INPUT), stops overlays for all players
	//==================================================
	if (strcmp(szInputName, "StopOverlaysAll", false) == 0)
	{
		//don't do anything if i'm not active the active global overlay entity
		if (g_iActiveScreenOverlayEntity[0] != _this)
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}

		g_iActiveScreenOverlayEntity[0] = 0; //mark that no global overlay entity is active
		CEnvScreenOverlay pScreenOverlay = CEnvScreenOverlay(_this);
		for(int i = 1; i <= MaxClients; i++)
		{
			CBasePlayer pPlayer = CBasePlayer(i);
			int entidx = pPlayer.entindex;
			if (entidx > 0 && entidx <= MAX_PLAYERS && IsClientInGame(i) && !IsFakeClient(i) && IsValidEntity(i))
			{
				if (g_iActiveScreenOverlayEntity[pPlayer.entindex] == pScreenOverlay.entindex)
				{
					pPlayer.SendCommand("r_screenoverlay off");
					g_iActiveScreenOverlayEntity[pPlayer.entindex] = 0;
					g_flNextOverlayTime[pPlayer.entindex] = 0.0;
					g_iNextScreenOverlayIndex[pPlayer.entindex] = 0;
				}
			}
		}
		
		pScreenOverlay.SetUserData("m_bIsActive", false);
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}



	//==================================================
	//SwitchOverlay input
	//==================================================
	if (strcmp(szInputName, "SwitchOverlay", false) == 0)
	{
		CEnvScreenOverlay pScreenOverlay = CEnvScreenOverlay(_this);

		//get input param to define the texture overlay entity will start from
		char szParam[MAX_FORMAT];
		DHookGetParamObjectPtrString(hParams, 4, 0, ObjectValueType_String, szParam, sizeof(szParam));
		TrimString(szParam);

		//-1 is also in orig code, cuz for Hammer user everything counts from 1 to 10, while 0 to 9 for programmers
		int iSwitchOverlayVal = StringToInt(szParam) - 1;

		//following 2 if() mimics orig code behavior
		if (iSwitchOverlayVal < 0)
		{
    		iSwitchOverlayVal = -iSwitchOverlayVal;
		}

		if (iSwitchOverlayVal > 9)
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}

		pScreenOverlay.SetUserData("m_iOverlayIndex", iSwitchOverlayVal); //set new starting overlay texture num

		//prevent orig code
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}



	//==================================================
	//Think input (CUSTOM INPUT), used as workaround for missing Think() calls on this entity
	//unlike timers, this also works with time scaling (host_timescale, host_framerate, etc)
	//==================================================
	if (strcmp(szInputName, "Think", false) == 0)
	{
		CEnvScreenOverlay pScreenOverlay = CEnvScreenOverlay(_this);

		if (pScreenOverlay.IsValid() == false || pScreenOverlay.GetUserData("m_bIsActive") == false)
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}

		float flTickedTime = GetTickedTime(); //store ingame time in this var so we don't call func every time

		int iNextOverlayIndex = -1;
		int iNextOverlay[MAX_PLAYERS + 1] = {-1}; //store next overlays to set for certain players
		float flNextOverlayStartTime[MAX_PLAYERS + 1] = {-1.0};

		for(int i = 1; i <= MaxClients; i++)
		{
			CBasePlayer pPlayer = CBasePlayer(i);
			int entidx = pPlayer.entindex;
			// Only proceed if entidx is in valid range (1..MAX_PLAYERS)
			if (entidx > 0 && entidx <= MAX_PLAYERS && IsClientInGame(i) && !IsFakeClient(i) && IsValidEntity(i))
			{
				if (g_iActiveScreenOverlayEntity[pPlayer.entindex] == pScreenOverlay.entindex)
				{
					iNextOverlayIndex = g_iNextScreenOverlayIndex[pPlayer.entindex];
					if (iNextOverlayIndex <= 9 && iNextOverlayIndex >= 0)
					{
						iNextOverlay[i] = g_iNextScreenOverlayIndex[pPlayer.entindex];
					}
					else
					{
						iNextOverlay[i] = -1;
					}

					flNextOverlayStartTime[i] = g_flNextOverlayTime[pPlayer.entindex];
					continue;
				}
			}
			iNextOverlay[i] = -1;
			flNextOverlayStartTime[i] = -1.0;
		}

		for(int i = 1; i <= MaxClients; i++)
		{
			CBasePlayer pPlayer = CBasePlayer(i);
			if (!pPlayer.IsValid())
				continue;

			// Only switch overlay if the delay has finished
			if (flNextOverlayStartTime[i] <= flTickedTime && iNextOverlay[i] != -1)
			{
				//PrintToChatAll("iNextOverlay[%d] == %d", i, iNextOverlay[i]);

				//get material that we will apply for screen
				char szOverlayName[MAX_FORMAT];
				pScreenOverlay.GetOverlayName(iNextOverlay[i], szOverlayName, sizeof(szOverlayName));

				//print warning and do nothing if the first overlay keyvalue is empty, just like in source sdk
				if (szOverlayName[0] == '\0')
				{
					continue;
				}

				//send command to apply screen overlay
				char szPlayerCommand[MAX_FORMAT];
				Format(szPlayerCommand, sizeof(szPlayerCommand), "r_screenoverlay %s", szOverlayName);
				pPlayer.SendCommand(szPlayerCommand);

				// If at last overlay (9), do not increment further, but still set timer for this overlay
				if (iNextOverlayIndex <= 9 && iNextOverlayIndex >= 0)
				{
					int iTheNextOverlay = iNextOverlay[i] + 1;

					if (iTheNextOverlay < 0)
						iTheNextOverlay = 0;
					
					if (iTheNextOverlay > 9)
						iTheNextOverlay = 9;

					g_iNextScreenOverlayIndex[pPlayer.entindex] = iTheNextOverlay;
					g_flNextOverlayTime[pPlayer.entindex] = flTickedTime + pScreenOverlay.GetOverlayTimes(iNextOverlay[i]);
				}
			}
		}
		pScreenOverlay.AddOutput("OnUser4", "!self", "Think", "", 0.01, 1);
		pScreenOverlay.FireOutput("OnUser4");

		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}

	return MRES_Ignored; //ignore if the input is not one of those we should override
}