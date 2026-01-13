#pragma newdecls required
#pragma semicolon 1

static Handle g_pSetDetonateTimer;
static Handle g_pExplodeTouch;

methodmap CBlackMesaBaseDetonator < CBaseAnimating
{
	public static void InitClassdef(const GameData hGameConfig)
	{
		char szSetDetonateTimer[] = "CBlackMesaBaseDetonator::SetDetonateTimer";
		StartPrepSDKCall(SDKCall_Entity);
		if (!PrepSDKCall_SetFromConf(hGameConfig, SDKConf_Signature, szSetDetonateTimer))
			LogMessage("Could not obtain gamedata signature %s", szSetDetonateTimer);
		else
		{
			PrepSDKCall_AddParameter(SDKType_Float, SDKPass_Plain);
			if (!(g_pSetDetonateTimer = EndPrepSDKCall())) SetFailState("Could not prep SDK call %s", szSetDetonateTimer);
		}

		char szExplodeTouch[] = "CBlackMesaBaseDetonator::ExplodeTouch";
		StartPrepSDKCall(SDKCall_Entity);
		if (!PrepSDKCall_SetFromConf(hGameConfig, SDKConf_Virtual, szExplodeTouch))
			LogMessage("Could not obtain gamedata offset %s", szExplodeTouch);
		else
		{
			PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Pointer);
			if (!(g_pExplodeTouch = EndPrepSDKCall())) SetFailState("Could not prep SDK call %s", szExplodeTouch);
		}
	}
	public CBlackMesaBaseDetonator(const int iEntIndex = -1)
	{
		return view_as<CBlackMesaBaseDetonator>(CBaseAnimating(iEntIndex));
	}

	public static CBlackMesaBaseDetonator Create()
	{
		return CBlackMesaBaseDetonator(CreateEntityByName("grenade_tripmine"));
	}

	public bool IsPlacedByLevelDesigner()
	{
		return GetEntProp(this.entindex, Prop_Data, "m_bPlacedByLevelDesigner") != 0;
	}

	public void SetPlacedByLevelDesigner(const bool bIsPlacedByDesigner)
	{
		SetEntProp(this.entindex, Prop_Data, "m_bPlacedByLevelDesigner", bIsPlacedByDesigner);
	}

	public CEnvBeamTeam GetBeam()
	{
		return CEnvBeamTeam(GetEntPropEnt(this.entindex, Prop_Data, "m_hBeam"));
	}

	public CSpriteTeam GetSprite()
	{
		return CSpriteTeam(GetEntPropEnt(this.entindex, Prop_Data, "m_hLight"));
	}

	public CBlackMesaPlayer GetOwner()
	{
		return CBlackMesaPlayer(GetEntPropEnt(this.entindex, Prop_Data, "m_hThrower"));
	}

	public void SetThrower(const CBlackMesaPlayer pPlayer)
	{
		SetEntPropEnt(this.entindex, Prop_Data, "m_hThrower", pPlayer.entindex);
	}

	public CBlackMesaPlayer GetOriginalThrower()
	{
		return CBlackMesaPlayer(GetEntPropEnt(this.entindex, Prop_Data, "m_hOriginalThrower"));
	}

	public void SetOriginalThrower(const CBlackMesaPlayer pPlayer)
	{
		SetEntPropEnt(this.entindex, Prop_Data, "m_hOriginalThrower", pPlayer.entindex);
	}

	public void SetDetonateTimer(const float flDetonateTimer)
	{
		SDKCall(g_pSetDetonateTimer, this.entindex, flDetonateTimer);
	}

	public void ExplodeTouch(const CBaseEntity pEntity)
	{
		SDKCall(g_pExplodeTouch, this.entindex, pEntity.entindex);
	}
}

