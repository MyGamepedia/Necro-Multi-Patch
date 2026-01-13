#pragma newdecls required
#pragma semicolon 1

static Handle g_pSendWeaponAnim;
static Handle g_pFinishReload;
static Handle g_pReload;
static Handle g_pHolster;
static Handle g_pBaseCombatWeaponPrecache;

static int g_iOffset_Activity = -1;
static int g_iOffset_IdealSequence = -1;
static int g_iOffset_IdealActivity = -1;

enum CBaseCombatWeapon_SpawnFlags
{
	SF_ITEM_NORESPAWN = (1 << 30),
}

methodmap CBaseCombatWeapon < CBaseAnimating
{
	public static void InitClassdef(const GameData hGameConfig)
	{
		char szSendWeaponAnim[] = "CBaseCombatWeapon::SendWeaponAnim";
		StartPrepSDKCall(SDKCall_Entity);
		if (!PrepSDKCall_SetFromConf(hGameConfig, SDKConf_Virtual, szSendWeaponAnim))
			LogMessage("Could not obtain gamedata offset %s", szSendWeaponAnim);
		else
		{
			PrepSDKCall_SetReturnInfo(SDKType_Bool, SDKPass_Plain);
			PrepSDKCall_AddParameter(SDKType_PlainOldData, SDKPass_Plain);
			if (!(g_pSendWeaponAnim = EndPrepSDKCall())) SetFailState("Could not prep SDK call %s", szSendWeaponAnim);
		}

		char szFinishReload[] = "CBaseCombatWeapon::FinishReload";
		StartPrepSDKCall(SDKCall_Entity);
		if (!PrepSDKCall_SetFromConf(hGameConfig, SDKConf_Virtual, szFinishReload))
			LogMessage("Could not obtain gamedata offset %s", szFinishReload);
		else
		{
			if (!(g_pFinishReload = EndPrepSDKCall())) SetFailState("Could not prep SDK call %s", szFinishReload);
		}

		char szReload[] = "CBaseCombatWeapon::Reload";
		StartPrepSDKCall(SDKCall_Entity);
		if (!PrepSDKCall_SetFromConf(hGameConfig, SDKConf_Virtual, szReload))
			LogMessage("Could not obtain gamedata offset %s", szReload);
		else
		{
			if (!(g_pReload = EndPrepSDKCall())) SetFailState("Could not prep SDK call %s", szReload);
		}

		char szHolster[] = "CBaseCombatWeapon::Holster";
		StartPrepSDKCall(SDKCall_Entity);
		if (!PrepSDKCall_SetFromConf(hGameConfig, SDKConf_Virtual, szHolster))
			LogMessage("Could not obtain gamedata offset %s", szHolster);
		else
		{
			PrepSDKCall_SetReturnInfo(SDKType_Bool, SDKPass_Plain);
			PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Plain);
			if (!(g_pHolster = EndPrepSDKCall())) SetFailState("Could not prep SDK call %s", szHolster);
		}

		char szPrecache[] = "CBaseCombatWeapon::Precache";
				StartPrepSDKCall(SDKCall_Entity);
		if (!PrepSDKCall_SetFromConf(hGameConfig, SDKConf_Signature, szPrecache))
			LogMessage("Could not obtain gamedata offset %s", szPrecache);
		else
		{
			if (!(g_pBaseCombatWeaponPrecache = EndPrepSDKCall())) SetFailState("Could not prep SDK call %s", szPrecache);
		}
	}
	public CBaseCombatWeapon(const int iEntIndex = -1)
	{
		return view_as<CBaseCombatWeapon>(CBaseAnimating(iEntIndex));
	}
	
	property CBaseCombatWeapon_SpawnFlags m_spawnflags
	{
		public get()
		{
			return view_as<CBaseCombatWeapon_SpawnFlags>(view_as<CBaseEntity>(this).m_spawnflags);
		}
		public set(const CBaseCombatWeapon_SpawnFlags eSpawnFlags)
		{
			view_as<CBaseEntity>(this).m_spawnflags = view_as<int>(eSpawnFlags);
		}
	}
	public CBaseCombatCharacter GetOwner()
	{
		return CBaseCombatCharacter(GetEntPropEnt(this.entindex, Prop_Send, "m_hOwner"));
	}
	public int GetPrimaryAmmo()
	{
		return GetEntProp(this.entindex, Prop_Send, "m_iClip1");
	}
	public void SetPrimaryAmmo(const int iPrimaryAmmo)
	{
		SetEntProp(this.entindex, Prop_Send, "m_iClip1", iPrimaryAmmo); 
	}
	public int GetPrimaryAmmoType()
	{
		return GetEntProp(this.entindex, Prop_Send, "m_iPrimaryAmmoType");
	}
	public void SetPrimaryAmmoType(const int iPrimaryAmmoType)
	{
		SetEntProp(this.entindex, Prop_Send, "m_iPrimaryAmmoType", iPrimaryAmmoType); 
	}
	public int GetSecondaryAmmo()
	{
		return GetEntProp(this.entindex, Prop_Send, "m_iClip2");
	}
	public void SetSecondaryAmmo(const int iSecondaryAmmo)
	{
		SetEntProp(this.entindex, Prop_Send, "m_iClip2", iSecondaryAmmo); 
	}
	public int GetSecondaryAmmoType()
	{
		return GetEntProp(this.entindex, Prop_Send, "m_iSecondaryAmmoType");
	}
	public void SetSecondaryAmmoType(const int iSecondaryAmmoType)
	{
		SetEntProp(this.entindex, Prop_Send, "m_iSecondaryAmmoType", iSecondaryAmmoType); 
	}
	public float GetNextPrimaryAttack()
	{
		return GetEntPropFloat(this.entindex, Prop_Send, "m_flNextPrimaryAttack");
	}
	public void SetNextPrimaryAttack(const float flNextPrimaryAttack)
	{
		SetEntPropFloat(this.entindex, Prop_Send, "m_flNextPrimaryAttack", flNextPrimaryAttack); 
	}
	public float GetNextSecondaryAttack()
	{
		return GetEntPropFloat(this.entindex, Prop_Send, "m_flNextSecondaryAttack");
	}
	public void SetNextSecondaryAttack(const float flNextSecondaryAttack)
	{
		SetEntPropFloat(this.entindex, Prop_Send, "m_flNextSecondaryAttack", flNextSecondaryAttack); 
	}

	public void SetHolsterTime(const float flHolsterTime)
	{
		SetEntPropFloat(this.entindex, Prop_Send, "m_flElasticity", flHolsterTime); //HACK! Use UNUSED field to store holster time
	}

	public float GetHolsterTime()
	{
		return GetEntPropFloat(this.entindex, Prop_Send, "m_flElasticity");
	}

	public void DelayNextAttack(float flDelay)
	{
		flDelay += GetGameTime();
		this.SetNextPrimaryAttack(flDelay);
		this.SetNextSecondaryAttack(flDelay);
	}
	public bool CanFireUnderwater()
	{
		return GetEntProp(this.entindex, Prop_Data, "m_bFiresUnderwater") != 0;
	}
	public int GetViewModelIndex()
	{
		return GetEntProp(this.entindex, Prop_Data, "m_nViewModelIndex");
	}
	public void SetViewModelIndex(const int iViewModelIndex = 0)
	{
		SetEntProp(this.entindex, Prop_Data, "m_nViewModelIndex", iViewModelIndex);
	}
	public int GetWeaponModelIndex(const bool bViewModel)
	{
		return GetEntProp(this.entindex, Prop_Send, bViewModel ? "m_iViewModelIndex" : "m_iWorldModelIndex");
	}
	public void SetWeaponModelIndex(const int iViewModelIndex, const bool bViewModel)
	{
		SetEntProp(this.entindex, Prop_Send, bViewModel ? "m_iViewModelIndex" : "m_iWorldModelIndex", iViewModelIndex);
	}
	public float GetWeaponTimeIdle()
	{
		return GetEntPropFloat(this.entindex, Prop_Data, "m_flTimeWeaponIdle");
	}
	public void SetWeaponTimeIdle(const float flTimeIdle)
	{
		SetEntPropFloat(this.entindex, Prop_Data, "m_flTimeWeaponIdle", flTimeIdle);
	}
	public bool SendWeaponAnim(const Activity eActivity)
	{
		return SDKCall(g_pSendWeaponAnim, this.entindex, eActivity);
	}
	public bool FinishReload()
	{
		return SDKCall(g_pFinishReload, this.entindex);
	}
	public bool Holster(CBaseCombatWeapon pSwitchingTo) 
	{
		return SDKCall(g_pHolster, this.entindex, pSwitchingTo.entindex);
	}
	public bool Reload()
	{
		return SDKCall(g_pReload, this.entindex);
	}
	public bool Precache()
	{
		return SDKCall(g_pBaseCombatWeaponPrecache, this.entindex);
	}
	public bool IsReloading()
	{
		return GetEntProp(this.entindex, Prop_Data, "m_bInReload") != 0;
	}
	public Activity GetActivity()
	{
		LazyInitializeOffsets(this);
		return view_as<Activity>(LoadFromAddress(this.address + g_iOffset_Activity, NumberType_Int32));
	}
	public void SetActivity(const Activity eActivity)
	{
		LazyInitializeOffsets(this);
		StoreToAddress(this.address + g_iOffset_Activity, eActivity, NumberType_Int32);
	}
	public int GetIdealSequence()
	{
		LazyInitializeOffsets(this);
		return LoadFromAddress(this.address + g_iOffset_IdealSequence, NumberType_Int32);
	}
	public void SetIdealSequence(const int iIdealSequence)
	{
		LazyInitializeOffsets(this);
		StoreToAddress(this.address + g_iOffset_IdealSequence, iIdealSequence, NumberType_Int32);
	}
	public Activity GetIdealActivity()
	{
		LazyInitializeOffsets(this);
		return view_as<Activity>(LoadFromAddress(this.address + g_iOffset_IdealActivity, NumberType_Int32));
	}
	public void SetIdealActivity(const Activity eActivity)
	{
		LazyInitializeOffsets(this);
		StoreToAddress(this.address + g_iOffset_IdealActivity, eActivity, NumberType_Int32);
	}
}

static void LazyInitializeOffsets(const CBaseCombatWeapon pWeapon)
{
	if (g_iOffset_Activity == -1 || g_iOffset_IdealSequence == -1 || g_iOffset_IdealActivity == -1)
	{
		char szNetworkName[128];
		pWeapon.GetNetClassname(szNetworkName, sizeof(szNetworkName));
		int iOffset = FindSendPropInfo(szNetworkName, "m_iWorldModelIndex");
		if (iOffset == -1)
			ThrowError("Unable to retrieve offset %s", "m_iWorldModelIndex");
		
		g_iOffset_Activity = iOffset + 8;
		g_iOffset_IdealSequence = iOffset + 12;
		g_iOffset_IdealActivity = iOffset + 16;
	}
}