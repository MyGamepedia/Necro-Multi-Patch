#pragma newdecls required
#pragma semicolon 1

//Purpose: adds spawn override for certain items
public void Item_Path(int iEntIndex)
{
	CItem pItem = CItem(iEntIndex);
	
	//check if feature is enabled
	if (GetConVarBool(g_ConvarNecroItemSpawnOverride))
	{
		//set sp item version if wanted
		if (pItem.GetUserData("m_iItemType") == 1)
		{
			g_bIsMultiplayerOverride = false; //disable for sp physics
			pItem.HookOutput("OnPlayerPickup", Hook_ItemSP_OnPlayerPickup, 1); //remove sp items
			pItem.SetCollisionGroup(COLLISION_GROUP_WEAPON); //set proper collision group
			
			//get current model
			char szModel[PLATFORM_MAX_PATH];
			pItem.GetModelName(szModel, sizeof(szModel));

			int iModelLenght = strlen(szModel);

			//check if contains _mp.mdl at the end
			if (iModelLenght > 7 && StrEqual(szModel[iModelLenght - 7], "_mp.mdl", false))
			{
				szModel[iModelLenght - 7] = '\0'; //remove "_mp.mdl"
				StrCat(szModel, sizeof(szModel), ".mdl"); //set ".mdl"

				PrecacheModel(szModel);
				pItem.SetModel(szModel); //use default model path from sp
			}
		}
	}
}

public MRESReturn Hook_BasePickupKeyValue(int _this, DHookReturn hReturn, DHookParam hParams)
{
	CItem pItem = CItem(_this);
	
	static char szKeyName[MAX_FORMAT];
	static char szValue[MAX_VALUE];
	DHookGetParamString(hParams, 1, szKeyName, sizeof(szKeyName));

	if (StrEqual(szKeyName, "ItemType", false))
	{
		DHookGetParamString(hParams, 2, szValue, sizeof(szValue));

		if(StrEqual(szValue, "1", false)) //singleplayer item type
		{
			pItem.SetUserData("m_iItemType", 1);
		}

		if(StrEqual(szValue, "2", false)) //coop (instancing) item type from SourceCoop
		{
			pItem.SetUserData("m_iItemType", 2);
		}

		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}


	char szClassname[MAX_CLASSNAME];
	GetEntityClassname(_this, szClassname, sizeof(szClassname));

	if (strcmp(szClassname, "item_battery") == 0)
	{
		//we may don't want to add dlights on spawn (on pro_crossfire for example)
		if (StrEqual(szKeyName, "nonewlight", false))
		{
			DHookGetParamString(hParams, 2, szValue, sizeof(szValue));

			if(StrEqual(szValue, "1", false))
			{
				pItem.SetUserData("m_bNoNewLight", 1);

				DHookSetReturn(hReturn, true);
				return MRES_Supercede;
			}
		}
	}

	return MRES_Ignored;
}


//Purpose: adds spawn post override for certain items
public void Item_PathPost(int iEntIndex)
{
	if (GetConVarBool(g_ConvarNecroItemSpawnOverride))
	{
		if (CBaseEntity(iEntIndex).GetUserData("m_iItemType") == 1)
		{
			g_bIsMultiplayerOverride = true; //re-enable multiplayer for non-sp items
		}
		else if (CBaseEntity(iEntIndex).GetUserData("m_iItemType") == 2)
		{
			//do nothing until needed
		}
	}
}
	
//Purpose: Kill sp flaged item after pick up
static Action Hook_ItemSP_OnPlayerPickup(const char[] szOutput, int iCaller, int iActivator, float flDelay)
{
	if(IsValidEntity(iCaller))
	{
		AcceptEntityInput(iCaller, "Kill");
	}

	return Plugin_Continue;
}