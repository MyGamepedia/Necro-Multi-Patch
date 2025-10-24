#define BASECOMBATWEAPONPRECACHE_RETURN_CLASSNAME

//Purpose: Override default weapon classnames to load custom Necro weapon scripts
public MRESReturn Hook_BaseCombatWeaponPrecache(int _this, DHookParam hParams)
{
	if (GetConVarBool(g_ConvarNecroOverrideDefaultWeaponParams))
	{
		char classname[64];
		GetEntityClassname(_this, classname, sizeof(classname));
		
		if(StrEqual(classname, "weapon_357"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_357_necro"); 
		}
		
		if(StrEqual(classname, "weapon_assassin_glock"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_assassin_glock_necro"); 
		}
		
		if(StrEqual(classname, "weapon_crossbow"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_crossbow_necro"); 
		}
		
		if(StrEqual(classname, "weapon_crowbar"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_crowbar_necro"); 
		}
		
		if(StrEqual(classname, "weapon_frag"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_frag_necro"); 
		}
		
		if(StrEqual(classname, "weapon_glock"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_glock_necro"); 
		}
		
		if(StrEqual(classname, "weapon_gluon"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_gluon_necro"); 
		}
		
		if(StrEqual(classname, "weapon_headcrab"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_headcrab_necro"); 
		}
		
		if(StrEqual(classname, "weapon_hivehand"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_hivehand_necro"); 
		}
		
		if(StrEqual(classname, "weapon_mp5"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_mp5_necro"); 
		}
		
		if(StrEqual(classname, "weapon_rpg"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_rpg_necro"); 
		}
		
		if(StrEqual(classname, "weapon_satchel"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_satchel_necro"); 
		}
		
		if(StrEqual(classname, "weapon_shotgun"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_shotgun_necro"); 
		}
		
		if(StrEqual(classname, "weapon_snark"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_snark_necro"); 
		}
		
		if(StrEqual(classname, "weapon_tau"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_tau_necro"); 
		}

		if(StrEqual(classname, "weapon_tripmine"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_tripmine_necro"); 
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
		
		if(StrEqual(classname, "weapon_357_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_357"); 
		}
		
		if(StrEqual(classname, "weapon_assassin_glock_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_assassin_glock"); 
		}
		
		if(StrEqual(classname, "weapon_crossbow_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_crossbow"); 
		}
		
		if(StrEqual(classname, "weapon_crowbar_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_crowbar"); 
		}
		
		if(StrEqual(classname, "weapon_frag_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_frag"); 
		}
		
		if(StrEqual(classname, "weapon_glock_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_glock"); 
		}
		
		if(StrEqual(classname, "weapon_gluon_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_gluon"); 
		}
		
		if(StrEqual(classname, "weapon_headcrab_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_headcrab"); 
		}
		
		if(StrEqual(classname, "weapon_hivehand_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_hivehand"); 
		}
		
		if(StrEqual(classname, "weapon_mp5_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_mp5"); 
		}
		
		if(StrEqual(classname, "weapon_rpg_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_rpg"); 
		}
		
		if(StrEqual(classname, "weapon_satchel_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_satchel"); 
		}
		
		if(StrEqual(classname, "weapon_shotgun_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_shotgun"); 
		}
		
		if(StrEqual(classname, "weapon_snark_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_snark"); 
		}
		
		if(StrEqual(classname, "weapon_tau_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_tau"); 
		}

		if(StrEqual(classname, "weapon_tripmine_necro"))
		{
			SetEntPropString(_this, Prop_Data, "m_iClassname", "weapon_tripmine"); 
		}
	}
	#endif

	return MRES_Ignored;
}