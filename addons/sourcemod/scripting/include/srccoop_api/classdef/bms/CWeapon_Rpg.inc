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
	
	public void SetLaserOn(const bool bLaserOn)
	{
		SetEntProp(this.entindex, Prop_Send, "m_bLaserOn", bLaserOn);
	}

	public float GetNextReload()
	{
		return GetEntPropFloat(this.entindex, Prop_Data, "m_flNextReload");
	}
	
	public float SetNextReload(const float flNextReload)
	{
		SetEntPropFloat(this.entindex, Prop_Data, "m_flNextReload", flNextReload);
	}

	public float GetHomingGrenade()
	{
		return GetEntPropEnt(this.entindex, Prop_Data, "m_hHomingGrenade");
	}

	public float SetHomingGrenade(const int iEntIndex)
	{
		SetEntPropEnt(this.entindex, Prop_Data, "m_hHomingGrenade", iEntIndex);
	}

	public bool Reload()
	{
		return SDKCall(g_pThrownReload, this.entindex);
	}
}
