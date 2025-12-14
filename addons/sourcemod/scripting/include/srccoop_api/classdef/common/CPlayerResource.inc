#pragma newdecls required
#pragma semicolon 1


Handle g_pUpdateConnectedPlayer;
Handle g_pUpdateDisconnectedPlayer;

/**
 * Note: This entity updates its values in 0.1s thinks.
 */
methodmap CPlayerResource < CBaseEntity
{
	public static void InitClassdef(const GameData hGameConfig)
	{
		char szConnectedPlayer[] = "CPlayerResource::UpdateConnectedPlayer";
		StartPrepSDKCall(SDKCall_Entity);

		if (!PrepSDKCall_SetFromConf(hGameConfig, SDKConf_Virtual, szConnectedPlayer))
		{
			LogMessage("Could not obtain gamedata offset %s", szConnectedPlayer);
		}
		else
		{
			PrepSDKCall_AddParameter(SDKType_PlainOldData, SDKPass_Plain);
			PrepSDKCall_AddParameter(SDKType_CBasePlayer, SDKPass_Pointer); 

			if (!(g_pUpdateConnectedPlayer = EndPrepSDKCall()))
				SetFailState("Could not prep SDK call %s", szConnectedPlayer);
		}

		char szDisconnectedPlayer[] = "CPlayerResource::UpdateDisconnectedPlayer";
		StartPrepSDKCall(SDKCall_Entity);

		if (!PrepSDKCall_SetFromConf(hGameConfig, SDKConf_Virtual, szDisconnectedPlayer))
			LogMessage("Could not obtain gamedata offset %s", szDisconnectedPlayer);
		else
		{
			PrepSDKCall_SetReturnInfo(SDKType_PlainOldData, SDKPass_Plain);

			if (!(g_pUpdateDisconnectedPlayer = EndPrepSDKCall()))
				SetFailState("Could not prep SDK call %s", szDisconnectedPlayer);
		}
	}

	public CPlayerResource(const int iEntIndex = -1)
	{
		return view_as<CPlayerResource>(CBaseEntity(iEntIndex));
	}

	public static CPlayerResource Get()
	{
		return CPlayerResource(GetPlayerResourceEntity());
	}

	public int GetPing(const CBasePlayer pPlayer)
	{
		return GetEntProp(this.entindex, Prop_Send, "m_iPing", _, pPlayer.entindex);
	}

	public void SetPing(const CBasePlayer pPlayer, const int iPing)
	{
		SetEntProp(this.entindex, Prop_Send, "m_iPing", iPing, _, pPlayer.entindex);
	}

	public int GetScore(const CBasePlayer pPlayer)
	{
		return GetEntProp(this.entindex, Prop_Send, "m_iScore", _, pPlayer.entindex);
	}

	public void SetScore(const CBasePlayer pPlayer, const int iScore)
	{
		SetEntProp(this.entindex, Prop_Send, "m_iScore", iScore, _, pPlayer.entindex);
	}

	public int GetDeaths(const CBasePlayer pPlayer)
	{
		return GetEntProp(this.entindex, Prop_Send, "m_iDeaths", _, pPlayer.entindex);
	}

	public void SetDeaths(const CBasePlayer pPlayer, const int iDeaths)
	{
		SetEntProp(this.entindex, Prop_Send, "m_iDeaths", iDeaths, _, pPlayer.entindex);
	}
	
	public bool IsConnected(const CBasePlayer pPlayer)
	{
		return view_as<bool>(GetEntProp(this.entindex, Prop_Send, "m_bConnected", _, pPlayer.entindex));
	}

	public void SetConnected(const CBasePlayer pPlayer, const bool bConnected)
	{
		SetEntProp(this.entindex, Prop_Send, "m_bConnected", bConnected, _, pPlayer.entindex);
	}

	public int GetTeam(const CBasePlayer pPlayer)
	{
		return GetEntProp(this.entindex, Prop_Send, "m_iTeam", _, pPlayer.entindex);
	}

	public void SetTeam(const CBasePlayer pPlayer, const int iTeam)
	{
		SetEntProp(this.entindex, Prop_Send, "m_iTeam", iTeam, _, pPlayer.entindex);
	}

	public bool IsAlive(const CBasePlayer pPlayer)
	{
		return GetEntProp(this.entindex, Prop_Send, "m_bAlive", _, pPlayer.entindex) != 0;
	}

	public void SetAlive(const CBasePlayer pPlayer, const bool bAlive)
	{
		SetEntProp(this.entindex, Prop_Send, "m_bAlive", bAlive, _, pPlayer.entindex);
	}

	public int GetHealth(const CBasePlayer pPlayer)
	{
		return GetEntProp(this.entindex, Prop_Send, "m_iHealth", _, pPlayer.entindex);
	}

	public void SetHealth(const CBasePlayer pPlayer, const int iHealth)
	{
		SetEntProp(this.entindex, Prop_Send, "m_iHealth", iHealth, _, pPlayer.entindex);
	}

	#if defined SRCCOOP_BLACKMESA
	public int GetAccountId(const CBasePlayer pPlayer)
	{
		return GetEntProp(this.entindex, Prop_Send, "m_iAccountID", _, pPlayer.entindex);
	}

	public void SetAccountId(const CBasePlayer pPlayer, const int iAccountID)
	{
		SetEntProp(this.entindex, Prop_Send, "m_iAccountID", iAccountID, _, pPlayer.entindex);
	}

	public bool IsValidPlayer(const CBasePlayer pPlayer)
	{
		return GetEntProp(this.entindex, Prop_Send, "m_bValid", _, pPlayer.entindex) != 0;
	}

	public void SetValidPlayer(const CBasePlayer pPlayer, const bool bValid)
	{
		SetEntProp(this.entindex, Prop_Send, "m_bValid", bValid, _, pPlayer.entindex);
	}

	public int GetFrags(const CBasePlayer pPlayer)
	{
		return GetEntProp(this.entindex, Prop_Send, "m_iFrags", _, pPlayer.entindex);
	}

	public void SetFrags(const CBasePlayer pPlayer, const int iFrags)
	{
		SetEntProp(this.entindex, Prop_Send, "m_iFrags", iFrags, _, pPlayer.entindex);
	}

	public void UpdateConnectedPlayer(const int iIndex, const int pPlayer)
	{
		SDKCall(g_pUpdateConnectedPlayer, this, iIndex, pPlayer);
	}

	public void UpdateDisconnectedPlayer(const int iIndex)
	{
		SDKCall(g_pUpdateDisconnectedPlayer, iIndex);
	}

	#endif // SRCCOOP_BLACKMESA
}
