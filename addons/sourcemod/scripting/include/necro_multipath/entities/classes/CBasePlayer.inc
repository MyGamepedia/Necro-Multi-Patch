#pragma newdecls required
#pragma semicolon 1

static float g_vec3PlayerShootPosition[MAX_PLAYERS + 1][3];
static float g_flPlayerMaxSpeed[MAX_PLAYERS + 1];

#if defined PLAYERPATCH_BM_CLIENT_PREDICTION
static bool g_bSetJumpModuleActive[MAX_PLAYERS + 1];
#endif

//------------------------------------------------------
// Local Player override
//------------------------------------------------------

/**
 * Pushes a player or an entity, for which a player will be searched on demand, to the stack.
 * Must be followed by PopLocalPlayerOverride - use AddLocalPlayerPlaceholder for pairing without adding an override.
 */
void AddLocalPlayerOverride(CBaseEntity pEntity)
{
	if (pEntity == NULL_CBASEENTITY)
	{
		g_iLocalPlayerStackSkips[g_iLocalPlayerStackPointer]++;
		return;
	}
	if (++g_iLocalPlayerStackPointer >= sizeof(g_pLocalPlayerEntity))
	{
		g_iLocalPlayerStackPointer--;
		ThrowError("LocalPlayer override stack overflow (max: %d)", sizeof(g_pLocalPlayerEntity) - 1);
	}
	g_pLocalPlayerEntity[g_iLocalPlayerStackPointer] = pEntity;
}

void AddLocalPlayerOverrideEx(int iEntIndex)
{
	AddLocalPlayerOverride(CBaseEntity(iEntIndex));
}

void AddLocalPlayerPlaceholder()
{
	AddLocalPlayerOverride(NULL_CBASEENTITY);
}

/* 
 * Undo's current local player override entity by popping the stack.
 * Must be preceded by a call to AddLocalPlayerOverride or AddLocalPlayerPlaceholder.
 */
void PopLocalPlayerOverride()
{
	if (g_iLocalPlayerStackSkips[g_iLocalPlayerStackPointer] > 0)
	{
		g_iLocalPlayerStackSkips[g_iLocalPlayerStackPointer]--;
		return;
	}
	if (g_iLocalPlayerStackPointer <= 0)
	{
		ThrowError("Tried to pop LocalPlayer override, but none was set.");
	}
	g_iLocalPlayerStackPointer--;
}

//------------------------------------------------------
// MaxClients override
//------------------------------------------------------

/**
 * Overrides MaxClients to 1 and increments internal override counter. 
 * Must be followed by PopMaxClientsOverride - use AddMaxClientsPlaceholder for pairing without adding an override.
 */
void AddMaxClientsOverride()
{
	#define MAX_MAXPLAYERS_OVERRIDES 100
	if (g_iMaxClientsOverrides >= MAX_MAXPLAYERS_OVERRIDES)
	{
		ThrowError("MaxClients override overflow (max: %d)", MAX_MAXPLAYERS_OVERRIDES);
	}
	CGlobalVars.Get().SetMaxPlayers(1);
	g_iMaxClientsOverrides++;
}

void AddMaxClientsPlaceholder()
{
	g_iMaxClientsOverrides++;
}

/* 
 * Decrements internal MaxClients override counter and undo's the override when reaching 0.
 * Must be preceded by a call to AddMaxClientsOverride or AddMaxClientsPlaceholder.
 */
void PopMaxClientsOverride()
{
	if (g_iMaxClientsOverrides <= 0)
	{
		ThrowError("Tried to pop MaxClients override, but none was set.");
	}
	if (!--g_iMaxClientsOverrides)
	{
		CGlobalVars.Get().SetMaxPlayers(MaxClients);
	}
}

//------------------------------------------------------
// Convenience methods
//------------------------------------------------------

/**
 * Convenience method that adds both LocalPlayer and MaxClients overrides.
 */
void AddSinglePlayerOverride(CBaseEntity pLocalPlayerEntity)
{
	AddLocalPlayerOverride(pLocalPlayerEntity);
	AddMaxClientsOverride();
}

/**
 * Convenience method that adds placeholder entries to both LocalPlayer and MaxClients overrides.
 */
void AddSinglePlayerPlaceholder()
{
	AddLocalPlayerPlaceholder();
	AddMaxClientsPlaceholder();
}

/**
 * Convenience method that pops both LocalPlayer and MaxClients overrides.
 */
void PopSinglePlayerOverride()
{
	PopLocalPlayerOverride();
	PopMaxClientsOverride();
}


//------------------------------------------------------
// NPCs that use the response system https://developer.valvesoftware.com/wiki/Response_System
// The original function calls `AI_GetSinglePlayer()` which will end up returning null.
//------------------------------------------------------
public MRESReturn Hook_ExpresserHost_DoModifyOrAppendCriteriaPost(DHookParam hParams)
{
	CBaseEntity pSpeaker = CBaseEntity(DHookGetParam(hParams, 1));
	CBasePlayer pPlayer = GetNearestPlayerPreferAlive(pSpeaker, true);
	if (pPlayer != NULL_CBASEENTITY)
	{
		AI_CriteriaSet pCriteria = AI_CriteriaSet(DHookGetParam(hParams, 2));

		float vec3SpeakerPosition[3];
		pSpeaker.GetAbsOrigin(vec3SpeakerPosition);

		float vec3PlayerPosition[3];
		pPlayer.GetAbsOrigin(vec3PlayerPosition);

		int iLength = RoundFloat(GetVectorDistance(vec3SpeakerPosition, vec3PlayerPosition));
		
		// This is rounded from a float to string to save stack space for string formatting.
		char szLength[MAX_CHARS_INT(INT_MIN)];
		IntToString(iLength, szLength, sizeof(szLength));
		pCriteria.AppendCriteria("distancetoplayer", szLength);

		// Original SDK code checks if the player is looking at the speaker.
		// This is not used as the closest player that is alive may not be looking at the speaker while other players are.
		if (/*pPlayer.FInViewConeEntity(pSpeaker) &&*/ pPlayer.FVisibleEntity(pSpeaker))
		{
			pCriteria.AppendCriteria("seenbyplayer", "1");
		}
	}
	return MRES_Ignored;
}

//------------------------------------------------------
// The singleplayer override for PreThink handles our
// prop pickup, sprinting, vm anims, suit queue updates, etc
//
// We also add some additional features here
//------------------------------------------------------
public void Hook_PlayerPreThink(int iClient)
{	
	CBasePlayer pPlayer = CBasePlayer(iClient);
	if (pPlayer.IsAlive())
	{
		//g_bIsMultiplayerOverride = false;
	}

	g_flPlayerMaxSpeed[iClient] = pPlayer.GetMaxSpeed();
}

public void Hook_PlayerPreThinkPost(int iClient)
{
	//g_bIsMultiplayerOverride = true;
	
	CBasePlayer pPlayer = CBasePlayer(iClient);
	if (pPlayer.IsAlive())
	{
		#if defined PLAYERPATCH_BM_CLIENT_PREDICTION
		CBlackMesaPlayer pBlackMesaPlayer = view_as<CBlackMesaPlayer>(pPlayer);

		if (!sv_always_run.BoolValue)	// If `sv_always_run` is set to `true`, then client prediction is working as intended.
		{
			// In multiplayer, max speed on both the client and the server is set when `IN_SPEED` is held.
			// While in singleplayer, it is set when `IN_SPEED` is held and the player is holding any directional movement key.
			bool bIsPressingSprint = pBlackMesaPlayer.IsPressingButton(IN_SPEED);
			bool bIsDucked = pBlackMesaPlayer.IsDucked();
			if (bIsPressingSprint && (!bIsDucked || (bIsDucked && g_flPlayerMaxSpeed[iClient] == 320.0)))
			{
				pBlackMesaPlayer.SetMaxSpeed(320.0);		// Controlled by `sv_speed_sprint` in game code.
			}
			else
			{
				pBlackMesaPlayer.SetMaxSpeed(190.0);		// Controlled by `sv_speed_mp` since `sv_speed_walk` is set to `0` only on the client.
			}
		}

		// Manually re-enable the jump module.
		// This introduces one rare client prediction bug where the client can jump multiple times without
		// enough mana but this only happens if the client has extremely high ping.
		if (pBlackMesaPlayer.GetJumpModuleMana() < sv_long_jump_manacost.FloatValue)
		{
			if (g_bSetJumpModuleActive[iClient])
			{
        		pBlackMesaPlayer.SendConVarValueInt(sv_jump_long_enabled, false);
				g_bSetJumpModuleActive[iClient] = false;
			}
		}
		else
		{
			if (!g_bSetJumpModuleActive[iClient])
			{
        		pBlackMesaPlayer.SendConVarValueInt(sv_jump_long_enabled, sv_jump_long_enabled.BoolValue);
				g_bSetJumpModuleActive[iClient] = true;
			}
		}
		#endif
		
		#if defined SRCCOOP_BLACKMESA
		CBlackMesaBaseCombatWeapon pWeapon = view_as<CBlackMesaBaseCombatWeapon>(pPlayer.GetActiveWeapon());
		if (pWeapon != NULL_CBASEENTITY)
		{
			static bool g_bDisableAttack[MAX_PLAYERS + 1];

			// Fixes prediction errors with weapons.
			bool bIsZooming = pPlayer.IsZooming();
			bool bDisableAttack = bIsZooming || pPlayer.IsCarryingObject();
			if (bDisableAttack)
			{
				pPlayer.SetNextAttack(FLT_MAX);
			}
			else
			{
				if (g_bDisableAttack[iClient])
				{
					pPlayer.SetNextAttack(GetGameTime() + 0.35);
				}
			}
			g_bDisableAttack[iClient] = bDisableAttack;

			// Fix gluon laser still emitting while zoomed.
			if (bIsZooming && pWeapon.IsClassname("weapon_gluon"))
			{
				CWeapon_Gluon pGluon = view_as<CWeapon_Gluon>(pWeapon);
				if (pGluon.GetFiringState() != 0)
				{
					pGluon.SetFiringState(0);
					StopSound(iClient, SNDCHAN_WEAPON, ")weapons/gluon/special1.wav");
				}
			}

			// TODO: Remove this when new beneathticle AI is added.
			// The player is parented to the beneathticle which causes this bug.
			// This solution will still cause prediction issues when the players ping is high enough.
			if (pPlayer.GetWaterLevel() == WL_Eyes)
			{
				// fix clientside firing weapons when dragged by beneathticle
				CBaseEntity pParent = pPlayer.GetParent();
				if (pParent != NULL_CBASEENTITY)
				{
					if (HasEntProp(pParent.entindex, Prop_Data, "m_hPrey"))
					{
						if (GetEntPropEnt(pParent.entindex, Prop_Data, "m_hPrey") == pPlayer.entindex)
						{
							if (!pWeapon.CanFireUnderwater() && pWeapon.GetPrimaryAmmoType() != -1)
							{
								pWeapon.DelayNextAttack(0.2);
							}
						}
					}
				}
			}
		}
		#endif // SRCCOOP_BLACKMESA
	}
}

//------------------------------------------------------
// Player AcceptInput hook
// Prevent input of kill to player as this will put the player in a hanging state
// This can occur in some maps where a trigger is set to kill the !activator and the player so happens to be there
//------------------------------------------------------
public MRESReturn Hook_PlayerAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!DHookIsNullParam(hParams, 1))
	{
		char szInputType[MAX_FORMAT];
		DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));

		if (strcmp(szInputType, "kill", false) == 0)
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}

		if (strcmp(szInputType, "GiveGunGameResources", false) == 0)
		{
			if(g_iNecroGunGameCurrentState != -1)
			{
				if(GetConVarInt(g_ConvarNecroGunGamePunishingWeaponState) == g_iNecroGunGameClientLevel[_this])
    			{
        			PrintToChat(_this, "%t", "#GunGame_GotPunishingWeapon");
    			}

				if(CBlackMesaPlayer(_this).IsValid())
					GunGameGiveResources(g_iNecroGunGameClientLevel[_this], CBlackMesaPlayer(_this));
			}

			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}
	}
	return MRES_Ignored;
}

public MRESReturn Hook_PlayerKilledPost(int _this, DHookParam hParams)
{
	CBasePlayer pPlayer = CBasePlayer(_this);
	ClearNpcMemoryForPlayer(pPlayer);

	g_fClientFastRespawnDelay[pPlayer.entindex] = GetGameTime() + g_fClientFastRespawnDelay[0];
		
	#if defined DEBUG
	PrintToServer("Hook_PlayerKilled: g_fClientFastRespawnDelay[%d]: %f", client, g_fClientFastRespawnDelay[client]);
	#endif

	return MRES_Ignored;
}


// Fixes hit registration being off by 1 tick.
// Credits to Xutax_Kamay (https://forums.alliedmods.net/showthread.php?t=315405)
public MRESReturn Hook_PlayerWeaponShootPosition_Post(int iClient, DHookReturn hReturn)
{
	CBasePlayer pPlayer = CBasePlayer(iClient);
	// Objects that are carried will call this hook and lag further behind the desired position.
	if (pPlayer.IsCarryingObject())
		return MRES_Ignored;

	DHookSetReturnVector(hReturn, g_vec3PlayerShootPosition[iClient]);
	return MRES_Supercede;
}

//------------------------------------------------------
// Pickup_ForcePlayerToDropThisObject
// reconstructed to avoid crash on UTIL_GetLocalPlayer nullptr
//------------------------------------------------------
public MRESReturn Hook_ForcePlayerToDropThisObject(DHookParam hParams)
{
	if (!DHookIsNullParam(hParams, 1))
	{
		// singleplayer doesn't care what's passed in so we have to check for both player or item
		CBaseEntity pParam = CBaseEntity(DHookGetParam(hParams, 1));
		if (pParam.IsPlayer())
		{
			view_as<CBasePlayer>(pParam).ForceDropOfCarriedPhysObjects();
		}
		else
		{
			for (int i = 1; i <= MaxClients; i++)
			{
				if (IsClientInGame(i))
				{
					// is this player carrying the required item?
					CBasePlayer pPlayer = CBasePlayer(i);
					if (pPlayer.GetCarriedObject() == pParam)
					{
						pPlayer.ForceDropOfCarriedPhysObjects();
						break;
					}
				}
			}
		}
	}
	return MRES_Supercede;
}

// Clears the player out of NPC memories and clears flags.
//
static void ClearNpcMemoryForPlayer(const CBasePlayer pPlayer)
{
	int iEntIndex = -1;
	while ((iEntIndex = FindEntityByClassname(iEntIndex, "*")) != -1)
	{
		CBaseEntity pEntity = CBaseEntity(iEntIndex);
		if (pEntity.IsNPC())
		{
			#if defined SRCCOOP_BLACKMESA
			if (pEntity.IsClassname("npc_abrams"))
			{
				// causes a crash in CNPC_Abrams::AimMiniTurret2
				continue;
			}
			#endif

			CAI_BaseNPC pNPC = view_as<CAI_BaseNPC>(pEntity);
			pNPC.ForgetEntity(pPlayer);
		}
	}

	pPlayer.m_iEFlags &= ~EFL_IS_BEING_LIFTED_BY_BARNACLE;
}

public MRESReturn Hook_FlashlightOn(int iClient, DHookReturn hReturn, DHookParam hParams)
{
    if(GetConVarBool(g_ConvarNecroOtherPlayersFlashlight))
    {
        int beam_flashlight = CreateEntityByName("beam");
        int spotlightend_flashlight = CreateEntityByName("spotlight_end");

        int spritetexture = PrecacheModel("sprites/glow01.vmt"); //sprite texture used by both halo and beam

        if(!IsValidEntity(beam_flashlight) || !IsValidEntity(spotlightend_flashlight))
        {
            return MRES_Ignored;
        }

        DispatchSpawn(spotlightend_flashlight);
        SetEntPropFloat(spotlightend_flashlight, Prop_Data, "m_flLightScale", 8.0); //light radius we need so it doesn't leak most of the time

        //Mainly repeats ent from C_HL2MP_Player::AddEntity
        //(https://github.com/ValveSoftware/source-sdk-2013/blob/b2705ba55b3b802b86ef2b2dbf97939c9d4fb685/src/game/client/hl2mp/c_hl2mp_player.cpp#L616)
        SetEntProp(beam_flashlight, Prop_Data, "m_nHaloIndex", spritetexture);
        SetEntProp(beam_flashlight, Prop_Data, "m_nModelIndex", spritetexture);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_fHaloScale", 3.0);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_fWidth", 8.0);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_fEndWidth", 35.0);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_fFadeLength", 300.0);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_fAmplitude", 0.0);
        SetEntPropFloat(beam_flashlight, Prop_Send, "m_fSpeed", 0.0);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_fStartFrame", 0.0);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_flFrameRate", 0.0);
        SetEntProp(beam_flashlight, Prop_Data, "m_nBeamFlags", FBEAM_FOREVER | FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM);

        //set up ents, use client for point A and use spotlight_end for point B, set 2 beam ents, set beam type 2
        SetEntPropEnt(beam_flashlight, Prop_Send, "m_hAttachEntity", EntIndexToEntRef(iClient));
        SetEntPropEnt(beam_flashlight, Prop_Send, "m_hAttachEntity", EntIndexToEntRef(spotlightend_flashlight), 1);
        SetEntProp(beam_flashlight, Prop_Send, "m_nNumBeamEnts", 2);
        SetEntProp(beam_flashlight, Prop_Send, "m_nBeamType", 2);

        DispatchKeyValue(beam_flashlight, "rendercolor", "0 0 0");

        char name[64];
        Format(name, sizeof(name), "beam_otherplayerflashlight_%d", iClient);
        SetEntPropString(beam_flashlight, Prop_Data, "m_iClassname", name); //HACK! I always get 0 with targetname, use classname to write and check the name
    
        float vecPos[3], vecAng[3];

        //teleport the beam to proper pos on the first frame to workaround beam at worldspawn on the first frame
        GetEntityAttachment(iClient, 4, vecPos, vecAng);
        TeleportEntity(beam_flashlight, vecPos, vecAng);

        //make the beam following the player
        SetVariantString("!activator");
        AcceptEntityInput(beam_flashlight, "SetParent", iClient);

        SetEntProp(beam_flashlight, Prop_Data, "m_nAttachIndex", 3, 0); //to make beam working with attachment point, used this field, 4 is attach num, 0 is the A beam point ent

        SetEntPropEnt(spotlightend_flashlight, Prop_Data, "m_hDamageFilter", beam_flashlight); //store beam in this field for quick access
        SetEntPropEnt(spotlightend_flashlight, Prop_Data, "m_hOwnerEntity", iClient); //store client

        SetEntPropEnt(beam_flashlight, Prop_Data, "m_hDamageFilter", spotlightend_flashlight); //store spotlight_end in this field for quick access
        SetEntPropEnt(spotlightend_flashlight, Prop_Data, "m_hOwnerEntity", iClient); //store client

        //hide for owner
        if(GetConVarBool(g_ConvarNecroOtherPlayersFlashlightTransmit))
        {
            SDKHook(beam_flashlight, SDKHook_SetTransmit, Hook_Player_Flashlight3rdPersonEffects_Transmit);
            SDKHook(spotlightend_flashlight, SDKHook_SetTransmit, Hook_Player_Flashlight3rdPersonEffects_Transmit);
        }

		DispatchSpawn(beam_flashlight);

        Player_OtherPlayersFlashlightUpdateBeamLength(spotlightend_flashlight); //calc pos

		RequestFrame(Hook_Player_Flashlight3rdPersonEffects_SetColor, beam_flashlight);
    }

	return MRES_Ignored;
}

//Purpose: This is a stupid workaround to make flashlight beam not visible when pointing to 0 0 0 pos on spawn 
public void Hook_Player_Flashlight3rdPersonEffects_SetColor(int iEntity)
{
	if(IsValidEntity(iEntity))
		DispatchKeyValue(iEntity, "rendercolor", "245 240 225");
}

//Purpose: Hide "other players flashlight" effect ents for owner
public Action Hook_Player_Flashlight3rdPersonEffects_Transmit(int entity, int client) 
{
	int owner = GetEntPropEnt(entity, Prop_Data, "m_hOwnerEntity"); //get owner
	
	if(!IsValidEntity(owner)) //remove entity if no valid owner
	{
		AcceptEntityInput(entity, "Kill");
		return Plugin_Continue;
	}

	if (client == owner) //hide for owner
	{	
		return Plugin_Stop;
	}

	if (IsClientObserver(client)) //check if client is in observer mode
	{
		int observertarget = GetEntPropEnt(client, Prop_Data, "m_hObserverTarget"); //get target being observed
		int observermode = GetEntProp(client, Prop_Data, "m_iObserverMode"); //get observer mode
		
		if (observertarget == owner && observermode == 4) //if target is owner and in first person mode - hide
		{
			return Plugin_Stop;
		}
	}

	return Plugin_Continue;
}

//Purpose: Update flashlight's spotlight_end position every frame by tracing beam from player's eyes
public void Player_OtherPlayersFlashlightUpdateBeamLength(int entity)
{
	if(!IsValidEntity(entity)) //it can be removed after the past frame, make sure it's valid
	{
		return;
	}

	int iClient = GetEntPropEnt(entity, Prop_Data, "m_hOwnerEntity"); //get client the spotlight_end belongs

	//no valid client somehow ? remove this spotlight_end, we don't need it anymore
	if(!IsValidEntity(iClient))
	{
		AcceptEntityInput(entity, "Kill");
		return;
	}

	//NOTE: The new code seems to work but not tested too much
	float vecPos[3], vecAng[3], vecEnd[3], vecDir[3], vecEndGuess[3];

	//use eyes so the player can highlight something for the other player
	GetClientEyeAngles(iClient, vecAng);
	GetClientEyePosition(iClient, vecPos);

	GetAngleVectors(vecAng, vecDir, NULL_VECTOR, NULL_VECTOR);
	
	AddVectors(vecPos, vecDir, vecEndGuess);
	ScaleVector(vecDir, 200.0); //limit max range by 200

	AddVectors(vecPos, vecDir, vecEndGuess);
	Handle trace = TR_TraceRayFilterEx(vecPos, vecEndGuess, MASK_SHOT, RayType_EndPoint, TraceEntityFilterPlayers, iClient); //trace ray with owner ignore
	TR_GetEndPosition(vecEnd, trace); //get end pos
	CloseHandle(trace);

	MakeVectorFromPoints(vecEnd, vecPos, vecDir);
	NormalizeVector(vecDir, vecDir);
	ScaleVector(vecDir, 16.0); //make the end result shorter to make spotlight_end display properly
	AddVectors(vecEnd, vecDir, vecEnd);

	TeleportEntity(entity, vecEnd, NULL_VECTOR);

	RequestFrame(Player_OtherPlayersFlashlightUpdateBeamLength, entity); //to the same on next frame
}

//Purpose: Make tracer ignore a client
public bool TraceEntityFilterPlayers(int entity, int mask, any data)
{
    int client = data;
    if (entity == client)
        return false;
	
    return true;
}

public MRESReturn Hook_FlashlightOff(int iClient, DHookReturn hReturn, DHookParam hParams)
{
	if(GetConVarBool(g_ConvarNecroOtherPlayersFlashlight))
		Player_Flashlight3rdPersonOff(iClient); //removed "other players flashlight" effects

	return MRES_Ignored;
}

//Purpose: Removes "other players flashlight" effects (beam and spotlight_end)
public bool Player_Flashlight3rdPersonOff(int iClient)
{
	char name[64];
	Format(name, sizeof(name), "beam_otherplayerflashlight_%d", iClient);

	//get beam entity by searching child ents 
	int beam_flashlight = GetChildByClassname(iClient, name); //TODO: replace with GetChildByTargetname when we will have working code

	if(!IsValidEntity(beam_flashlight))
	{
		return false; //false if one of the ents is not valid
	}

	int spotlightend_flashlight = GetEntPropEnt(beam_flashlight, Prop_Data, "m_hDamageFilter"); //get spotlight_end by getting it from beam's dmg filter

	AcceptEntityInput(beam_flashlight, "Kill");

	if(!IsValidEntity(spotlightend_flashlight))
	{
		return false;
	}

	AcceptEntityInput(spotlightend_flashlight, "Kill"); 

	return true; //true if everything is succeeded
}

public MRESReturn Hook_GiveDefaultItems(int _this)
{
	CBlackMesaPlayer pPlayer = CBlackMesaPlayer(_this);

	//don't give items to specs
	int team = GetEntProp(pPlayer.entindex, Prop_Data, "m_iTeamNum");

	if((CMultiplayRules.IsTeamplay() && (team == 1 || team == 0)))
		return MRES_Supercede;

	if(team == 1)
		return MRES_Supercede;

	//give items if allowed
	if (!GetConVarBool(g_ConvarNecroGiveDefaultItems))
	{
		return MRES_Supercede;
	}

	//don't give def items in gungame
	if(g_iNecroGunGameCurrentState != -1)
	{
		return MRES_Supercede;
	}

	return MRES_Ignored;
}

public MRESReturn Hook_PlayerForceRespawn(int iClient)
{
	float flDeathTime = GetEntPropFloat(iClient, Prop_Send, "m_flDeathTime");
	if (g_bUnblockRespawn[iClient]
		|| flDeathTime == 0.0
		|| (mp_forcerespawn.BoolValue && (GetGameTime() - flDeathTime) > GetConVarFloat(FindConVar("spec_freeze_time")) + GetConVarFloat(FindConVar("spec_freeze_traveltime"))))
	{
		g_bUnblockRespawn[iClient] = false;
		return MRES_Ignored;
	}
	return MRES_Supercede;
}

public MRESReturn Hook_PlayerStartObserverMode(int iClient, DHookReturn hReturn, DHookParam hParams)
{
	int observermode = hParams.Get(1);
	int observertarget = GetEntPropEnt(iClient, Prop_Data, "m_hObserverTarget");

	//todo: make a better fix by counting the time so we switch before spec_freeze_traveltime is finished
	if(observertarget != iClient) //check if the target is me, if yes - don't do anything, so the camera don't flying "to infinity and being"
	{
		if (observermode == 2)
		{
			hParams.Set(1, 5);
			return MRES_Handled;
		}
	}

	return MRES_Ignored;
}

public MRESReturn Hook_PlayerSpawnPost(int iClient, DHookReturn hReturn, DHookParam hParams)
{
	//BUG: sometimes SourceTV can slip through even though we never hook it?
	if (IsFakeClient(iClient))
		return MRES_Ignored;

	if(IsPlayerAlive(iClient))
	{
		CBasePlayer pPlayer = CBasePlayer(iClient);
		ClearNpcMemoryForPlayer(pPlayer);
		
		//disable "other players flashlight"
		SendConVarValue(iClient, sv_cheats, "1"); //HACK! This thing needs cheats ON
		ClientCommand(iClient, "r_flashlight_3rd_draw 0");

		//Fix for if player died on a ladder
		SetEntPropEnt(iClient, Prop_Data, "m_hLadder", -1);

		//TODO: Finish this code that cleans blood for spawned player
		/*int blackmesaviewmodel = GetEntPropEnt(iClient, Prop_Data, "m_hViewModel");

		if(IsValidEntity(blackmesaviewmodel))
		{
			int materialmodifycontrol_weapon = CreateEntityByName("material_modify_control");
			int materialmodifycontrol_arms = CreateEntityByName("material_modify_control");

			if(IsValidEntity(material_modify_control))
			{
				DispatchKeyValue(material_modify_control,"materialVar","$detailframe");
			}
		}*/
	}

	else
	{
		//Initialize clientside convars from skill.cfg, which may have not executed. 
		//Todo: a better way to execute all this, used 7 func due to compile error
		ClientCommand(iClient, "gb_flashlight_PosX -5; gb_flashlight_PosY -28.0; gb_flashlight_Intensity 0.8; gb_flashlight_Col 245 240 225");
		ClientCommand(iClient, "gb_flashlight_Range 1000; gb_flashlight_oldfalloff_quad 5; gb_flashlight_oldfalloff_linear 6000; cl_xort_light_col_R 170");
		ClientCommand(iClient, "cl_xort_light_col_B 80; cl_xort_light_intensity 750; cl_xort_light_range 256; cl_xort_light_ttl_Zap 0.5; cl_xort_light_ttl_dispel 0.5");
		ClientCommand(iClient, "cl_envportal_light_col_R 170; cl_envportal_light_col_B 80; r_bloomtintr_nextgen 0.25; r_bloomtintg_nextgen 0.25; r_bloomtintb_nextgen 0.25");
		ClientCommand(iClient, "r_bloomtintexponent_nextgen 2.2; r_bloom_nextgen_threshold 0.1; mat_bloom_nextgen 1; cl_gargflame_spotL_col_R 35");
		ClientCommand(iClient, "cl_gargflame_spotL_col_G 165; cl_gargflame_spotL_intensity 96000; cl_gargflame_spotL_range 768; cl_gargflame_pointL_col_R 35");
		ClientCommand(iClient, "cl_gargflame_pointL_col_G 165; cl_gargflame_pointL_intensity 32000; cl_garg_eyeL_light_intensity 100; cl_garg_eyeL_light_range 100");
	}

	return MRES_Ignored;
}

//Purpose: Various fixes when player is sending commands
public Action OnPlayerRunCmd(int iClient, int &iButtons, int &iImpulse, float fVel[3], float fAngles[3], int &iWeapon, 
							int& subtype, int& cmdnum, int& tickcount, int& seed, int mouse[2])
{
	if (IsFakeClient(iClient))
		return Plugin_Continue;

	GetClientEyePosition(iClient, g_vec3PlayerShootPosition[iClient]); //record current player eye vector to fix shoot pos
	
	//Hide broken specmenu for spectators
	int observermode = GetEntProp(iClient, Prop_Data, "m_iObserverMode"); //get observer mode
	
	if (mouse[0] || mouse[1])
	{
		g_bPostTeamSelect[iClient] = true;
	}
	
	if (observermode > 1) //hide panel if we are spectator
	{
		if (g_bPostTeamSelect[iClient] && tickcount % 10 == 0)
		{
			ShowVGUIPanel(iClient, "specmenu", _, false);
		}
	}
	
	if(iWeapon != 0) //send use command if we want to use a weapon
	{
		//Client don't use weapon when used custom classname to load custom script, force use with client command by checking classname
		char classname[MAX_CLASSNAME];
		GetEntityClassname(iWeapon, classname, sizeof(classname));

		char command[128];
		Format(command, sizeof(command), "use %s", classname);
		FakeClientCommand(iClient, command);
		
		#if defined DEBUG
		PrintToServer("OnPlayerRunCmd: Client (%d) used %s", iClient, classname);
		#endif
	}
	
	return Plugin_Continue;
}

//Purpose: Allow fast respawn when player presses the buttons
public void OnPlayerRunCmdPost(int client, int buttons, int impulse, const float vel[3], const float angles[3], int weapon, int subtype, int cmdnum, int tickcount, int seed, const int mouse[2])
{
	//Respawn player if: allowed by convar, player pressed any of the buttons, player is dead, player is not on spectator team and the delay time is passed
    if(GetConVarBool(g_ConvarNecroAllowFastRespawn) && buttons & (IN_ATTACK|IN_JUMP|IN_DUCK|IN_FORWARD|IN_BACK|IN_ATTACK2) 
	   && !IsPlayerAlive(client) && GetClientTeam(client) != 1 && GetGameTime() >= g_fClientFastRespawnDelay[client])
	{
		#if defined DEBUG
		PrintToServer("OnPlayerRunCmdPost: Client (%d) respawned without waiting.", client);
		PrintToServer("OnPlayerRunCmdPost: GetGameTime() == %d, g_fClientFastRespawnDelay[%d] == %f", GetGameTime(), client, g_fClientFastRespawnDelay[client]);
		#endif
		
        SetEntPropFloat(client, Prop_Send, "m_flDeathTime", 0.0); //set 0.0 time for death time, this is a proper way to respawn dead player when we need
	}
}

//------------------------------------------------------
// UTIL_GetLocalPlayer
//------------------------------------------------------
public MRESReturn Hook_UTIL_GetLocalPlayer(DHookReturn hReturn)
{
	CBaseEntity pOverrideEntity = g_pLocalPlayerEntity[g_iLocalPlayerStackPointer];
	if (pOverrideEntity.IsValid())
	{
		CBasePlayer pPlayer;
		if (pOverrideEntity.IsPlayer())
		{
			pPlayer = view_as<CBasePlayer>(pOverrideEntity);
		}
		else
		{
			pPlayer = GetNearestPlayerPreferAlive(pOverrideEntity);
			if (pPlayer == NULL_CBASEENTITY)
			{
				// BUGBUG Crash avoidance for Linux SM 1.12 - 7164+
				// return MRES_Ignored;
				DHookSetReturn(hReturn, -1);
				return MRES_Supercede;
			}
		}
		DHookSetReturn(hReturn, pPlayer.entindex);
		return MRES_Supercede;
	}
	// BUGBUG Crash avoidance for Linux SM 1.12 - 7164+
	// return MRES_Ignored;
	DHookSetReturn(hReturn, -1);
	return MRES_Supercede;
	
}

public void OnClientDisconnect(int client)
{
	if (IsFakeClient(client))
		return;

	CBasePlayer pPlayer = CBasePlayer(client);
	PlayerPatch_OnClientDisconnect(pPlayer);
}

//------------------------------------------------------
// OnClientDisconnect
//------------------------------------------------------
public void PlayerPatch_OnClientDisconnect(CBasePlayer pPlayer)
{
	if (pPlayer.IsValid())
	{
		#if defined ENTPATCH_ENV_SCREENOVERLAY
		//disable screenoverlay for this client
		g_iActiveScreenOverlayEntity[pPlayer.entindex] = 0;
		g_iNextScreenOverlayIndex[pPlayer.entindex] = 0;
		g_flNextOverlayTime[pPlayer.entindex] = 0.0;
		#endif
	}
}

public void OnClientDisconnect_Post(int client)
{
	if (g_iPlayerCount)
	{
		g_iPlayerCount = GetRealClientCount(true);
		
		#if defined ENTPATCH_NPC_THINK_LOCALPLAYER
		if (!g_iPlayerCount)
		{
			// pause entity thinking
			hkPhysics_RunThinkFunctions.Enable(Hook_Pre, Hook_Physics_RunThinkFunctions);
		}
		#endif
	}
//	g_szSteamIds[client] = "";
	g_bPostTeamSelect[client] = false;
	g_iAddButtons[client] = 0;
}

//------------------------------------------------------
// ClientPutInServer
//------------------------------------------------------
public void PlayerPatch_OnClientPutInServer(const int iClient)
{
	#if defined PLAYERPATCH_BM_CLIENT_PREDICTION
	g_bSetJumpModuleActive[iClient] = false;
	#endif

	#if defined ENTPATCH_ENV_SCREENOVERLAY
	//if global overlay is active - start it for this client
	if(g_iActiveScreenOverlayEntity[0] != 0)
	{
		AcceptEntityInput(g_iActiveScreenOverlayEntity[0], "StartOverlays", iClient);
	}
	#endif
	
//	SendClientConvars(iClient, false);
}

//------------------------------------------------------
// Player team change hooks
//------------------------------------------------------
public MRESReturn Hook_PlayerChangeTeam(int iClient, DHookParam hParams)
{
	CBasePlayer pPlayer = CBasePlayer(iClient);

	int iTeam = DHookGetParam(hParams, 1);
	int iForceTeam = GetConVarInt(g_ConvarNecroForceTeam);

	//force team if enabled
	if (iForceTeam != TEAM_INVALID && iClient != -1)
	{
		hParams.Set(1, iForceTeam);
		iTeam = iForceTeam;
	}

	bool bJoinedSpectators = (!CMultiplayRules.IsTeamplay() && iTeam == TEAM_SPECTATOR);
	bool bJoinedSpectatorsTeamplay = (CMultiplayRules.IsTeamplay() && iTeam == TEAM_UNASSIGNED || iTeam == TEAM_SPECTATOR);

	//if joined spectators, force player to drop the carried phys objects or dismount tank entity
	if(bJoinedSpectators || bJoinedSpectatorsTeamplay)
	{
		CBaseEntity pCarriedObject = pPlayer.GetUseEntity();
		if (pCarriedObject != NULL_CBASEENTITY)
		{
			pPlayer.ForceDropOfCarriedPhysObjects();
		}

		//POWER SALO said that sometimes orig code doesn't remove items itself
		pPlayer.RemoveAllItems();

		if(GetConVarBool(g_ConvarNecroOtherPlayersFlashlight))
			Player_Flashlight3rdPersonOff(pPlayer.entindex);
	}

	if (iForceTeam != TEAM_INVALID && iClient != -1)
	{
		return MRES_ChangedHandled;
	}

	return MRES_Ignored;
}

public MRESReturn Hook_PlayerChangeTeamPost(int iClient, DHookParam hParams)
{
	CBasePlayer pPlayer = CBasePlayer(iClient);
	int iTeam = DHookGetParam(hParams, 1);

	//check if joined spectators depending on teamplay state
	bool bJoinedSpectatorsTeamplay = (CMultiplayRules.IsTeamplay() && iTeam == 1 || iTeam == 0); //teamplay enabled
	bool bJoinedSpectators = (!CMultiplayRules.IsTeamplay() && iTeam == 1); //templay disabled

	if(bJoinedSpectators || bJoinedSpectatorsTeamplay)
	{
		ClearNpcMemoryForPlayer(pPlayer);
	}

	//TODO: move to "jointeam"
	//fix specgui showing if player tried join specs while forced to be in a team
	if(g_ConvarNecroForceTeam && !bJoinedSpectators || !bJoinedSpectatorsTeamplay)
	{
		ShowVGUIPanel(iClient, "specgui", _, false);
	}

	return MRES_Ignored;
}

public MRESReturn Hook_PlayerShouldCollide(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if(g_ConvarNecroPlayersCollide.BoolValue)
		return MRES_Ignored;
	
	Collision_Group_t eCollisionGroup = DHookGetParam(hParams, 1);
	if (eCollisionGroup == COLLISION_GROUP_PLAYER || eCollisionGroup == COLLISION_GROUP_PLAYER_MOVEMENT)
	{
		DHookSetReturn(hReturn, false);
		return MRES_Supercede;
	}
	return MRES_Ignored;
}
