#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CAI_BaseNPC - KeyValue(char)
// Lets mappers use custom npc models
// (Using keyvalues proves most reliable against BM overriding models with its character manifest file before Spawn())
//------------------------------------------------------
public MRESReturn Hook_BaseNPCKeyValuePost(int _this, DHookReturn hReturn, DHookParam hParams)
{
	CBaseEntity pEntity = CBaseEntity(_this);
	
	static char szKey[MAX_FORMAT];
	static char szVal[MAX_VALUE];
	DHookGetParamString(hParams, 1, szKey, sizeof(szKey));
	if (StrEqual(szKey, "custommodel"))
	{
		DHookGetParamString(hParams, 2, szVal, sizeof(szVal));
		int iModelIndex = PrecacheModel(szVal);
		if (iModelIndex)
		{
			pEntity.SetModel(szVal);
			pEntity.SetModelIndex(iModelIndex);
			DHookEntity(hkSetModel, false, _this, _, BaseNPCSetModelBlock);
		}
	}
	return MRES_Ignored;
}

public MRESReturn BaseNPCSetModelBlock(int _this, DHookParam hParams)
{
	return MRES_Supercede;
}

//------------------------------------------------------
// CAI_BaseNPC::Think
// Universal GetLocalPlayer pre-setter, ran before NPC thinks
//------------------------------------------------------
public MRESReturn Hook_BaseNPCThink(int _this)
{
	// Set this entity into context
	AddLocalPlayerOverrideEx(_this);
	return MRES_Ignored;
}

public MRESReturn Hook_BaseNPCThinkPost(int _this)
{
	PopLocalPlayerOverride();
	return MRES_Ignored;
}

//------------------------------------------------------
// CAI_BaseNPC
// Fix UpdateEnemyMemory input when player string is passed as parameter
//------------------------------------------------------
public MRESReturn Hook_BaseNPCAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!DHookIsNullParam(hParams, 1))
	{
		char szInputType[MAX_FORMAT];
		DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));
		if (strcmp(szInputType, "UpdateEnemyMemory", false) == 0)
		{
			char szParam[32];
			DHookGetParamObjectPtrString(hParams, 4, 0, ObjectValueType_String, szParam, sizeof(szParam));
			if (StrEqual(szParam, "!player", false) || StrEqual(szParam, "!pvsplayer", false))
			{
				CAI_BaseNPC pThis = CAI_BaseNPC(_this);
				CBasePlayer pPlayer;
				float vecAbsOrigin[3];
				for (int i = 1; i <= MaxClients; i++)
				{
					if (IsClientInGame(i) && !IsFakeClient(i) && IsPlayerAlive(i))
					{
						pPlayer = CBasePlayer(i);
						pPlayer.GetAbsOrigin(vecAbsOrigin);
						pThis.UpdateEnemyMemory(pPlayer, vecAbsOrigin, pThis);
					}
				}
				DHookSetReturn(hReturn, true);
				return MRES_Supercede;
			}
		}
	}
	return MRES_Ignored;
}

//------------------------------------------------------
// CAI_BaseNPC - Recreation of SetPlayerAvoidState for MP compatibility
// From https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/mp/src/game/server/ai_basenpc.cpp#L3822
//------------------------------------------------------
public MRESReturn Hook_SetPlayerAvoidState(int _this)
{
	CAI_BaseNPC pThis = CAI_BaseNPC(_this);
	
	bool bShouldPlayerAvoid;
	float vNothing[3];
	
	pThis.GetSequenceLinearMotion(pThis.GetSequence(), vNothing);
	bool bIsMoving = (pThis.IsMoving() || !VecEqual(vNothing, vec3_origin));
	
	// If we are coming out of a script, check if we are stuck inside the player.
	if (pThis.GetPerformAvoidance() || (pThis.ShouldPlayerAvoid() && bIsMoving))
	{
		float vMins[3], vMaxs[3], vOrigin[3], vPlrMins[3], vPlrMaxs[3], vPlrOrigin[3];
		pThis.GetMins(vMins);
		pThis.GetMaxs(vMaxs);
		pThis.GetAbsOrigin(vOrigin);
		AddVectors(vMins, vOrigin, vMins);
		AddVectors(vMaxs, vOrigin, vMaxs);
		
		for (int i = 1; i <= MaxClients; i++)
		{
			if (IsClientInGame(i) && !IsFakeClient(i) && IsPlayerAlive(i))
			{
				CBasePlayer pPlayer = CBasePlayer(i);
				pPlayer.GetMins(vPlrMins);
				pPlayer.GetMaxs(vPlrMaxs);
				pPlayer.GetAbsOrigin(vPlrOrigin);
				AddVectors(vPlrMins, vPlrOrigin, vPlrMins);
				AddVectors(vPlrMaxs, vPlrOrigin, vPlrMaxs);
				
				if (IsBoxIntersectingBox(vMins, vMaxs, vPlrMins, vPlrMaxs))
				{
					bShouldPlayerAvoid = true;
					break;
				}
			}
		}
	}
	
	pThis.SetPlayerAvoidState(pThis.ShouldPlayerAvoid());
	pThis.SetPerformAvoidance(bShouldPlayerAvoid);
	
	Collision_Group_t iCollGroup = pThis.GetCollisionGroup();
	if (iCollGroup == COLLISION_GROUP_NPC || iCollGroup == COLLISION_GROUP_NPC_ACTOR)
	{
		if (bShouldPlayerAvoid)
		{
			pThis.SetCollisionGroup(COLLISION_GROUP_NPC_ACTOR);
		}
		else
		{
			pThis.SetCollisionGroup(COLLISION_GROUP_NPC);
		}
	}
	return MRES_Supercede;
}

// `UpdateSleepState` ends early due to a null pointer check for the local player.
// The entire function is reconstructed to remove the check and to support multiple players.
// 
// Class: `CAI_BaseNPC`
//
// ## Citations
//
// - [Source SDK 2013](https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/mp/src/game/server/ai_basenpc.cpp#L3353)
//
public MRESReturn Hook_BaseNpcUpdateSleepState(int _this, DHookParam hParams)
{
	// TODO: does bInPVS work for any and all players?
	CAI_BaseNPC pBaseNPC = CAI_BaseNPC(_this);
	bool bInPVS = DHookGetParam(hParams, 1);
	
	AI_SleepState_t eSleepState = pBaseNPC.GetSleepState();
	if (eSleepState > AISS_AWAKE)
	{
		// wake up if any player has walked into radius
		if (HasAnyPlayerWalkedIntoWakeRadius(pBaseNPC))
		{
			pBaseNPC.Wake();
		}
		else if (eSleepState == AISS_WAITING_FOR_PVS)
		{
			if (bInPVS)
			{
				pBaseNPC.Wake();
			}
		}
		else if (eSleepState == AISS_WAITING_FOR_THREAT)
		{
			if (pBaseNPC.HasCondition(view_as<int>(COND_LIGHT_DAMAGE)) || pBaseNPC.HasCondition(view_as<int>(COND_HEAVY_DAMAGE)))
			{
				pBaseNPC.Wake();
			}
			else
			{
				if (bInPVS)
				{
					for (int i = 1; i <= MaxClients; ++i)
					{
						CBasePlayer pPlayer = CBasePlayer(i);
						if (pPlayer.IsValid() && pPlayer.IsAlive() && !(pPlayer.m_fFlags & FL_NOTARGET) && pPlayer.FVisibleEntity(pBaseNPC))
						{
							pBaseNPC.Wake();
							break;
						}
					}
				}
				
				if ((pBaseNPC.GetSoundInterests() & SOUND_DANGER) && !(pBaseNPC.m_spawnflags & SF_NPC_WAIT_TILL_SEEN))
				{
					CSoundEnt pSoundEnt = CSoundEnt.Get();
					if (pSoundEnt.IsValid())
					{
						CSound pSound;
						pSoundEnt.ActiveList(pSound);
						for (; pSound.IsValid(); pSound.Next())
						{
							if ((pSound.GetType() & SOUND_DANGER) && pBaseNPC.CanHearSound(pSound) && pBaseNPC.IsSoundVisible(pSound))
							{
								pBaseNPC.Wake();
								break;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		// NPC is awake
		// Don't let an NPC sleep if they're running a script!
		if (!pBaseNPC.IsInAScript() && pBaseNPC.GetState() != NPC_STATE_SCRIPT)
		{
			if (pBaseNPC.m_SleepFlags & AI_SLEEP_FLAG_AUTO_PVS)
			{
				if (!pBaseNPC.HasCondition(view_as<int>(COND_IN_PVS)))
				{
					pBaseNPC.SetSleepState(AISS_WAITING_FOR_PVS);
					pBaseNPC.Sleep();
				}
			}

			if (pBaseNPC.m_SleepFlags & AI_SLEEP_FLAG_AUTO_PVS_AFTER_PVS)
			{
				if (pBaseNPC.HasCondition(view_as<int>(COND_IN_PVS)))
				{
					// OK, we're in the player's PVS. Now switch to regular old AUTO_PVS sleep rules.
					pBaseNPC.m_SleepFlags |= AI_SLEEP_FLAG_AUTO_PVS;
					pBaseNPC.m_SleepFlags &= ~AI_SLEEP_FLAG_AUTO_PVS_AFTER_PVS;
				}
			}
		}
	}

	return MRES_Supercede;
}

public bool HasAnyPlayerWalkedIntoWakeRadius(CAI_BaseNPC pBaseNPC)
{
	float flWakeRadius = pBaseNPC.GetWakeRadius();
	bool bHasBigEnoughRadius = flWakeRadius > 0.1;
	if (!bHasBigEnoughRadius)
	{
		return false;
	}

	float vec3EntityPosition[3];
	pBaseNPC.GetAbsOrigin(vec3EntityPosition);

	for (int i = 1; i <= MaxClients; ++i)
	{
		CBasePlayer pPlayer = CBasePlayer(i);
		if (pPlayer.IsValid() && pPlayer.IsAlive() && !(pPlayer.m_fFlags & FL_NOTARGET))
		{
			float vec3PlayerPosition[3];
			pPlayer.GetAbsOrigin(vec3PlayerPosition);

			if (GetVectorDistance(vec3EntityPosition, vec3PlayerPosition) <= flWakeRadius)
			{
				return true;
			}
		}
	}

	return false;
}

// Fixes local player task issues.
// `CAI_BaseNPC::RunTask` calls into `UTIL_GetLocalPlayer` and `AI_GetSinglePlayer` which is null in multiplayer.
//
// ## Citations
//
// - [Source SDK 2013](https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/mp/src/game/server/ai_basenpc_schedule.cpp#L3191)
//
public MRESReturn Hook_BaseNPCRunTask(int _this, DHookParam hParams)
{
	CAI_BaseNPC pEntity = CAI_BaseNPC(_this);
	Task_t pTask = DHookGetParam(hParams, 1);
	sharedtasks_e eTask = pTask.GetTask();
	int iTaskInterrupt = pEntity.GetTaskInterrupt();
	
	if ((eTask == TASK_MOVE_AWAY_PATH && iTaskInterrupt == 0) ||
		(eTask == TASK_FACE_PLAYER) ||
		(eTask == view_as<sharedtasks_e>(100000)))	// TASK_FEAR_GET_PATH_TO_SAFETY_HINT
	{
		AddSinglePlayerOverride(pEntity);
	}
	else
	{
		AddSinglePlayerPlaceholder();
	}
	return MRES_Ignored;
}

public MRESReturn Hook_BaseNPCRunTaskPost(int _this, DHookParam hParams)
{
	PopSinglePlayerOverride();
	return MRES_Ignored;
}