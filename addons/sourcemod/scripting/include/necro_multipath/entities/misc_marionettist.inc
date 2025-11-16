#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CMiscMarionettist - misc_marionettist
// Prevent a crash due to reactivation before finishing or when used by multiple players at once
//------------------------------------------------------
CBaseEntity g_pActiveMarionettist[MAXPLAYERS+1] = {view_as<CBaseEntity>(-1), ...};

public MRESReturn Hook_MarionettistAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	//TODO: Replace with convar logic
	/*
	if (CoopManager.IsCoopModeEnabled())
	{
	*/
		if (!DHookIsNullParam(hParams, 1))
		{
			char szInputType[MAX_FORMAT];
			DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));
			if (strcmp(szInputType, "Manipulate", false) == 0)
			{
				char szParam[MAX_FORMAT];
				DHookGetParamObjectPtrString(hParams, 4, 0, ObjectValueType_String, szParam, sizeof(szParam));
				if (strcmp(szParam, "!activator", false) == 0)
				{
					if (!DHookIsNullParam(hParams, 2))
					{
						int iActivator = DHookGetParam(hParams, 2);
						CBaseEntity pActivator = CBaseEntity(iActivator);
						if (pActivator != NULL_CBASEENTITY && pActivator.IsPlayer())
						{
							MarionettistManipulate(_this, iActivator, view_as<CBasePlayer>(pActivator));
							DHookSetReturn(hReturn, true);
							return MRES_Supercede;
						}
					}
				}
				else if (strcmp(szParam, "!player", false) == 0 || strcmp(szParam, "player", false) == 0)
				{
					for (int i = 1; i <= MaxClients; i++)
					{
						if (IsClientInGame(i) && !IsFakeClient(i) && IsPlayerAlive(i))
						{
							MarionettistManipulate(_this, i, CBasePlayer(i));
						}
					}
					DHookSetReturn(hReturn, true);
					return MRES_Supercede;
				}
			}
		}
	/*
	}
	*/
	return MRES_Ignored;
}

void MarionettistManipulate(int iMarionettist, int iClient, CBasePlayer pPlayer)
{
	// cancel previous
	if (g_pActiveMarionettist[iClient].IsValid())
	{
		g_pActiveMarionettist[iClient].Kill();
	}
	
	// create a puppet from the master, without hooking it
	g_bTempDontHookEnts = true;
	CBaseEntity pPuppet = CloneMarionettist(iMarionettist);
	g_bTempDontHookEnts = false;
	
	// start it
	pPuppet.AcceptInputStr("Manipulate", "!activator", pPlayer);
	hkThink.HookEntity(Hook_Post, pPuppet.entindex, Hook_MarionettistPuppetThinkPost);
	hkUpdateOnRemove.HookEntity(Hook_Post, pPuppet.entindex, Hook_MarionettistPuppetRemoved);
	g_pActiveMarionettist[iClient] = pPuppet;
}

CBaseEntity CloneMarionettist(int iEnt)
{
	int iClone = CreateEntityByName("misc_marionettist");
	char buff[MAX_VALUE];
	GetEntPropString(iEnt, Prop_Data, "m_iszSoundScriptStart", buff, sizeof(buff));
	SetEntPropString(iClone, Prop_Data, "m_iszSoundScriptStart", buff);
	GetEntPropString(iEnt, Prop_Data, "m_iszSoundScriptLoop", buff, sizeof(buff));
	SetEntPropString(iClone, Prop_Data, "m_iszSoundScriptLoop", buff);
	GetEntPropString(iEnt, Prop_Data, "m_iszSoundScriptEnd", buff, sizeof(buff));
	SetEntPropString(iClone, Prop_Data, "m_iszSoundScriptEnd", buff);
	char szDestNamesProp[29];
	for (int i = 0; i < 16; i++)
	{
		FormatEx(szDestNamesProp, sizeof(szDestNamesProp), "m_iszDestinationsNames[ %d ]", i);
		GetEntPropString(iEnt, Prop_Data, szDestNamesProp, buff, sizeof(buff));
		SetEntPropString(iClone, Prop_Data, szDestNamesProp, buff);
	}
	SetEntPropFloat(iClone, Prop_Data, "m_flInnerDestinationRadiusPrevious", GetEntPropFloat(iEnt, Prop_Data, "m_flInnerDestinationRadiusPrevious"));
	SetEntPropFloat(iClone, Prop_Data, "m_flInnerPullSpeedPrevious", GetEntPropFloat(iEnt, Prop_Data, "m_flInnerPullSpeedPrevious"));
	SetEntPropFloat(iClone, Prop_Data, "m_flOuterDestinationRadiusPrevious", GetEntPropFloat(iEnt, Prop_Data, "m_flOuterDestinationRadiusPrevious"));
	SetEntPropFloat(iClone, Prop_Data, "m_flOuterPullSpeedPrevious", GetEntPropFloat(iEnt, Prop_Data, "m_flOuterPullSpeedPrevious"));
	SetEntProp(iClone, Prop_Data, "m_bIgnoreCollisions", GetEntProp(iEnt, Prop_Data, "m_bIgnoreCollisions"));
	ActivateEntity(iClone);
	DispatchSpawn(iClone);
	
	CBaseEntity pClone = CBaseEntity(iClone);
	pClone.SetParent(CBaseEntity(iEnt));
	return pClone;
}

public MRESReturn Hook_MarionettistPuppetThinkPost(int _this)
{
	CBaseEntity pThis = CBaseEntity(_this);
	if (pThis.GetNextThinkTick() == -1)
	{
		// last think, our puppet has finished
		pThis.Kill();
		return MRES_Ignored;
	}
	for (int i = 1; i <= MaxClients; i++)
	{
		if (g_pActiveMarionettist[i] == pThis)
		{
			if (!IsClientInGame(i) || !IsPlayerAlive(i))
			{
				// player left / died while manipulating
				pThis.Kill();
				break;
			}
		}
	}
	return MRES_Ignored;
}

// Catches all sources of removal, potentially resets player noclip.
MRESReturn Hook_MarionettistPuppetRemoved(int iEntIndex)
{
	CBaseEntity pPuppet = CBaseEntity(iEntIndex);
	for (int i = 1; i <= MaxClients; i++)
	{
		if (g_pActiveMarionettist[i] == pPuppet)
		{
			if (GetEntProp(g_pActiveMarionettist[i].entindex, Prop_Data, "m_bIgnoreCollisions"))
			{
				if (IsClientInGame(i))
				{
					CBasePlayer pPlayer = CBasePlayer(i);
					pPlayer.SetMoveType(MOVETYPE_WALK);
				}
			}
			g_pActiveMarionettist[i] = NULL_CBASEENTITY;
			break;
		}
	}
	return MRES_Ignored;
}