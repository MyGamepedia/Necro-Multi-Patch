#define BASECOMBATWEAPONPRECACHE_RETURN_CLASSNAME

#define WEAPON_357_SERVER 				"weapon_357_necro"
#define WEAPON_ASSASSIN_GLOCK_SERVER 	"weapon_assassin_glock_necro"
#define WEAPON_CROSSBOW_SERVER 			"weapon_crossbow_necro"
#define WEAPON_CROWBAR_SERVER 			"weapon_crowbar_necro"
#define WEAPON_FRAG_SERVER 				"weapon_frag_necro"
#define WEAPON_GLOCK_SERVER 			"weapon_glock_necro"
#define WEAPON_GLUON_SERVER 			"weapon_gluon_necro"
#define WEAPON_HEADCRAB_SERVER 			"weapon_headcrab_necro"
#define WEAPON_HIVEHAND_SERVER 			"weapon_hivehand_necro"
#define WEAPON_MP5_SERVER 				"weapon_mp5_necro"
#define WEAPON_RPG_SERVER 				"weapon_rpg_necro"
#define WEAPON_SATCHEL_SERVER 			"weapon_satchel_necro"
#define WEAPON_SHOTGUN_SERVER 			"weapon_shotgun_necro"
#define WEAPON_SNARK_SERVER 			"weapon_snark_necro"
#define WEAPON_TAU_SERVER 				"weapon_tau_necro"
#define WEAPON_TRIPMINE_SERVER 			"weapon_tripmine_necro"

//Purpose: Override default weapon classnames to load custom Necro weapon scripts
public MRESReturn Hook_BaseCombatWeaponPrecache(int _this, DHookParam hParams)
{
	if (GetConVarBool(g_ConvarNecroOverrideDefaultWeaponParams))
	{
		char classname[64];
		GetEntityClassname(_this, classname, sizeof(classname));
		
		if(StrEqual(classname, "weapon_357"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_357_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_assassin_glock"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_ASSASSIN_GLOCK_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_crossbow"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_CROSSBOW_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_crowbar"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_CROWBAR_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_frag"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_FRAG_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_glock"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_GLOCK_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_gluon"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_GLUON_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_headcrab"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_HEADCRAB_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_hivehand"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_HIVEHAND_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_mp5"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_MP5_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_rpg"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_RPG_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_satchel"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_SATCHEL_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_shotgun"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_SHOTGUN_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_snark"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_SNARK_SERVER); 
		}
		
		if(StrEqual(classname, "weapon_tau"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_TAU_SERVER); 
		}

		if(StrEqual(classname, "weapon_tripmine"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", WEAPON_TRIPMINE_SERVER); 
		}
	}

	return MRES_Ignored;
}

//Purpose: Restore default weapon classnames after precache is done so the weapon can spawn
public MRESReturn Hook_BaseCombatWeaponPrecachePost(int _this, DHookParam hParams)
{
	#if defined BASECOMBATWEAPONPRECACHE_RETURN_CLASSNAME
	if (GetConVarBool(g_ConvarNecroOverrideDefaultWeaponParams))
	{
		char classname[64];
		GetEntityClassname(_this, classname, sizeof(classname));
		
		if(StrEqual(classname, WEAPON_357_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_357"); 
		}
		
		if(StrEqual(classname, WEAPON_ASSASSIN_GLOCK_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_assassin_glock"); 
		}
		
		if(StrEqual(classname, WEAPON_CROSSBOW_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_crossbow"); 
		}
		
		if(StrEqual(classname, WEAPON_CROWBAR_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_crowbar"); 
		}
		
		if(StrEqual(classname, WEAPON_FRAG_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_frag"); 
		}
		
		if(StrEqual(classname, WEAPON_GLOCK_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_glock"); 
		}
		
		if(StrEqual(classname, WEAPON_GLUON_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_gluon"); 
		}
		
		if(StrEqual(classname, WEAPON_HEADCRAB_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_headcrab"); 
		}
		
		if(StrEqual(classname, WEAPON_HIVEHAND_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_hivehand"); 
		}
		
		if(StrEqual(classname, WEAPON_MP5_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_mp5"); 
		}
		
		if(StrEqual(classname, WEAPON_RPG_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_rpg"); 
		}
		
		if(StrEqual(classname, WEAPON_SATCHEL_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_satchel"); 
		}
		
		if(StrEqual(classname, WEAPON_SHOTGUN_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_shotgun"); 
		}
		
		if(StrEqual(classname, WEAPON_SNARK_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_snark"); 
		}
		
		if(StrEqual(classname, WEAPON_TAU_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_tau"); 
		}

		if(StrEqual(classname, WEAPON_TRIPMINE_SERVER))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_tripmine"); 
		}
	}
	#endif

	return MRES_Ignored;
}

public MRESReturn Hook_BaseCombatWeaponHolsterPost(int _this)
{
	CBaseCombatWeapon pWeapon = CBaseCombatWeapon(_this);
	pWeapon.SetHolsterTime(GetTickedTime());
	return MRES_Ignored;
}

//------------------------------------------------------
// weapons
// use sp weapon models
//------------------------------------------------------
public MRESReturn Hook_WeaponSetModel(int _this, DHookParam hParams)
{
	if (!DHookIsNullParam(hParams, 1))
	{
		CBaseCombatWeapon pWeapon = CBaseCombatWeapon(_this);
		if (pWeapon != NULL_CBASEENTITY)
		{
			CBaseCombatCharacter pOwner = pWeapon.GetOwner();
			if (pOwner != NULL_CBASEENTITY && !pOwner.IsPlayer())
			{
				static const char szWeaponModel[][][] =
				{
					{ "models/weapons/w_glock_mp.mdl", "models/weapons/w_glock.mdl" },
					{ "models/weapons/w_357_mp.mdl", "models/weapons/w_357.mdl" },
					{ "models/weapons/w_mp5_mp.mdl", "models/weapons/w_mp5.mdl" },
					{ "models/weapons/w_shotgun_mp.mdl", "models/weapons/w_shotgun.mdl" },
					{ "models/weapons/w_rpg_mp.mdl", "models/weapons/w_rpg.mdl" },
				};

				char szModelName[MAX_MODELNAME];
				DHookGetParamString(hParams, 1, szModelName, sizeof(szModelName));

				for (int i = 0; i < sizeof(szWeaponModel); i++)
				{
					if (strcmp(szModelName, szWeaponModel[i][0], false) == 0)
					{
						if (PrecacheModel(szWeaponModel[i][1], false))
						{
							hParams.SetString(1, szWeaponModel[i][1]);
							return MRES_ChangedHandled;
						}

						break;
					}
				}
			}
		}
	}
	return MRES_Ignored;
}

//Purpose: Auto reload weapon when holster time exceeds defined convar time
void Hook_BaseCombatWeaponItemHolsterFramePost(const int iWeaponEntIndex, const float flWeaponAutoReloadTime)
{
	CBaseCombatWeapon pWeapon = CBaseCombatWeapon(iWeaponEntIndex);
	
	if (GetConVarBool(g_ConvarNecroAllowWeaponAutoReload) && 
        (GetTickedTime() - pWeapon.GetHolsterTime()) > flWeaponAutoReloadTime)
	{
		pWeapon.SetHolsterTime(GetTickedTime());
		pWeapon.FinishReload();
	}
	
	return;
}