#pragma newdecls required
#pragma semicolon 1

static Handle g_pThrownReload;

methodmap CWeapon_Rpg < CBlackMesaBaseCombatWeapon
{
	public static void InitClassdef(const GameData hGameConfig)
	{
		char szThrownReload[] = "CBlackMesaBaseWeaponThrown::Reload";
		StartPrepSDKCall(SDKCall_Entity);
		if (!PrepSDKCall_SetFromConf(hGameConfig, SDKConf_Virtual, szThrownReload))
			LogMessage("Could not obtain gamedata offset %s", szThrownReload);
		else
		{
			if (!(g_pThrownReload = EndPrepSDKCall())) SetFailState("Could not prep SDK call %s", szThrownReload);
		}
	}
	public CWeapon_Rpg(const int iEntIndex = -1)
	{
		return view_as<CWeapon_Rpg>(CBlackMesaBaseCombatWeapon(iEntIndex));
	}
	
	public static CWeapon_Rpg Create()
	{
		return CWeapon_Rpg(CreateEntityByName("weapon_rpg"));
	}

	public bool HasLaserOn()
	{
		return GetEntProp(this.entindex, Prop_Send, "m_bLaserOn") != 0;
	}
	
	public void SetLaser(const bool bLaser)
	{
		SetEntProp(this.entindex, Prop_Send, "m_bLaserOn", bLaser);
	}

	public float GetNextReload()
	{
		return GetEntPropFloat(this.entindex, Prop_Data, "m_flNextReload");
	}
	
	public float SetNextReload(const float flNextReload)
	{
		SetEntPropFloat(this.entindex, Prop_Data, "m_flNextReload", flNextReload);
	}

	public CBaseEntity GetHomingGrenade()
	{
		return CBaseEntity(GetEntPropEnt(this.entindex, Prop_Data, "m_hHomingGrenade"));
	}

	public void SetHomingGrenade(CBaseEntity pEntity)
	{
		SetEntPropEnt(this.entindex, Prop_Data, "m_hHomingGrenade", pEntity.entindex);
	}

	public bool Reload()
	{
		return SDKCall(g_pThrownReload, this.entindex);
	}
}
