#pragma newdecls required
#pragma semicolon 1

#define SF_SPEED_MOD_SUPPRESS_WEAPONS	(1<<0)	// Take away weapons
#define SF_SPEED_MOD_SUPPRESS_HUD		(1<<1)	// Take away the HUD
#define SF_SPEED_MOD_SUPPRESS_JUMP		(1<<2)
#define SF_SPEED_MOD_SUPPRESS_DUCK		(1<<3)
#define SF_SPEED_MOD_SUPPRESS_USE		(1<<4)
#define SF_SPEED_MOD_SUPPRESS_SPEED		(1<<5)
#define SF_SPEED_MOD_SUPPRESS_ATTACK	(1<<6)
#define SF_SPEED_MOD_SUPPRESS_ZOOM		(1<<7)


CMovementSpeedMod g_pPlayerSpeed[MAX_PLAYERS + 1] = { NULL_CBASEENTITY, ... };

//------------------------------------------------------
// CMovementSpeedMod - player_speedmod
// remove flags ''Suppress HUD'' with ''Suppress attack'' and replace with custom user data that we'll use for the input
//------------------------------------------------------
public void Hook_PlayerSpeedmodSpawnPost(int iEntIndex)
{
	CBaseEntity pEntity = CBaseEntity(iEntIndex);

	if (pEntity.m_spawnflags & SF_SPEED_MOD_SUPPRESS_HUD)
	{
		pEntity.m_spawnflags &= ~SF_SPEED_MOD_SUPPRESS_HUD; //remove the flag so it can't hide the parts we don't want
		pEntity.SetUserData("m_bSuppressHud", true);
	}

	if (pEntity.m_spawnflags & SF_SPEED_MOD_SUPPRESS_ATTACK)
	{
		//pEntity.m_spawnflags &= ~SF_SPEED_MOD_SUPPRESS_ATTACK;
		pEntity.SetUserData("m_bSuppressAttack", true);
	}
}

//------------------------------------------------------
// CMovementSpeedMod - player_speedmod
// Inputs hook for custom logic.
//------------------------------------------------------
public MRESReturn Hook_PlayerSpeedmodAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (DHookIsNullParam(hParams, 1))
		return MRES_Ignored;

	char szInputType[MAX_FORMAT];
	DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));
	


	//==================================================
	//ModifySpeedAll input (CUSTOM INPUT), apply speed modify for all clients
	//==================================================
	if (strcmp(szInputType, "ModifySpeedAll", false) == 0) // new input
	{
		char szParam[32];
		DHookGetParamObjectPtrString(hParams, 4, 0, ObjectValueType_String, szParam, sizeof(szParam));
		float flSpeed = StringToFloat(szParam);

		CMovementSpeedMod pPlayerSpeed = CMovementSpeedMod(_this);
		CBasePlayer pPlayer;

		for (int i = 1; i <= MaxClients; i++)
		{
			pPlayer = CBasePlayer(i);
			if (pPlayer.IsValid())
			{
				pPlayerSpeed.ModifySpeed(flSpeed);
			}
		}
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}
		


	//==================================================
	//ModifySpeed input
	//==================================================
	else if (strcmp(szInputType, "ModifySpeed", false) == 0)
	{
		//don't do anything in case if player is nullprt for some reason
		if (DHookIsNullParam(hParams, 2))
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}

		CBasePlayer	pPlayer = CBasePlayer(DHookGetParam(hParams, 2));

		//only valid players
		if (!pPlayer.IsValid())
		{
			DHookSetReturn(hReturn, true);
			return MRES_Supercede;
		}

		CMovementSpeedMod pEntity = CMovementSpeedMod(_this);
		int iPlrIndex = pPlayer.entindex;

		char szParam[32];
		DHookGetParamObjectPtrString(hParams, 4, 0, ObjectValueType_String, szParam, sizeof(szParam));

		bool bEnable = (StringToFloat(szParam) != 1.0); //1.0 means the speed modify will be disabled

		CMovementSpeedMod pActive = g_pPlayerSpeed[iPlrIndex];
		bool bHasActive = pActive.IsValid();
		bool bInternalDisable = (pEntity.GetUserData("m_bInternalDisable") == true);

		
		//==================================================
		// DISABLE
		//==================================================
		if (!bEnable)
		{
			// internal disable: allow it even if active already points to another mod
			if (!bInternalDisable)
			{
				// if no active - do nothing
				if (!bHasActive)
				{
					DHookSetReturn(hReturn, true);
					return MRES_Supercede;
				}

				// if active isn't this - do nothing
				if (pActive != pEntity)
				{
					DHookSetReturn(hReturn, true);
					return MRES_Supercede;
				}

				// disable this
				g_pPlayerSpeed[iPlrIndex] = NULL_CBASEENTITY;
			}
			else
			{
				// consume the internal flag
				pEntity.SetUserData("m_bInternalDisable", false);
			}
		}
		//==================================================
		// ENABLE
		//==================================================
		else
		{
			// if this is already active - do nothing
			if (bHasActive && pActive == pEntity)
			{
				DHookSetReturn(hReturn, true);
				return MRES_Supercede;
			}

			// if other is active - disable it first
			if (bHasActive && pActive != pEntity)
			{
				pActive.SetUserData("m_bInternalDisable", true);
				pActive.AcceptInputFloat("ModifySpeed", 1.0, pPlayer, pPlayer);

				// set new active before recursive disable, so old one won't clear it
				g_pPlayerSpeed[iPlrIndex] = pEntity;

				// safety clear
				pActive.SetUserData("m_bInternalDisable", false);
			}
			else
			{
				// no active - just enable this
				g_pPlayerSpeed[iPlrIndex] = pEntity;
			}
		}

		//i contain Suppress Attack flag
		//prevents wpn fire prediction glitch by setting a very long attack delay 
		if (pEntity.GetUserData("m_bSuppressAttack") == true)
		{
			CBaseCombatWeapon pWeapon = pPlayer.GetActiveWeapon();
			if (pWeapon.IsValid())
			{
				if (bEnable)
				{
					pWeapon.SetNextPrimaryAttack(FLT_MAX);
					pWeapon.SetNextSecondaryAttack(FLT_MAX);
				}
				else
				{
					pWeapon.SetNextPrimaryAttack(GetGameTime());
					pWeapon.SetNextSecondaryAttack(GetGameTime());			
				}
			}
		}

		//i contain Suppress Hud flag
		//orig code hides most of the elements, including chat
		//we hide only what needed by removing the flag in spawn hook and using other here
		//bots not allowed (no need)
		if (pEntity.GetUserData("m_bSuppressHud") == true && !pPlayer.IsBot())
		{
			HideHudFlags_t hdtFlags = pPlayer.GetHideHud();

			if (bEnable)
			{
				hdtFlags |= HIDEHUD_PLAYERDEAD;	
			}
			else
				hdtFlags &= ~HIDEHUD_PLAYERDEAD;

			pPlayer.SetHideHud(hdtFlags);
		}
	}
	
	return MRES_Ignored;
}

//------------------------------------------------------
// CMovementSpeedMod - player_speedmod
// Disable entity before remove
//------------------------------------------------------
MRESReturn Hook_PlayerSpeedmodUpdateOnRemove(int _this)
{
	CMovementSpeedMod pEntity = CMovementSpeedMod(_this);
	CBasePlayer pPlayer;
	for (int i = 1; i <= MaxClients; i++)
	{
		pPlayer = CBasePlayer(i);
		if (pPlayer.IsValid() && g_pPlayerSpeed[pPlayer.entindex] == pEntity)
		{
			pEntity.AcceptInputFloat("ModifySpeed", 1.0, pPlayer, pPlayer);
		}
	}

	return MRES_Ignored;
}