#pragma newdecls required
#pragma semicolon 1

//Purpose: adds spawn override for certain items
public void Item_Path(int iEntIndex)
{
	CItem pItem = CItem(iEntIndex);

	char iszResponseContext[MAX_KEY];
	GetEntPropString(iEntIndex, Prop_Data, "m_iszResponseContext", iszResponseContext, sizeof(iszResponseContext));

	//check if feature is enabled
	if (GetConVarBool(g_ConvarNecroItemSpawnOverride))
	{
		//set sp item version if wanted
		if (StrEqual(iszResponseContext, "item_sp:1", false))
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

//Purpose: adds spawn post override for certain items
public void Item_PathPost(int iEntIndex)
{
	char iszResponseContext[MAX_KEY];
	GetEntPropString(iEntIndex, Prop_Data, "m_iszResponseContext", iszResponseContext, sizeof(iszResponseContext));

	if (GetConVarBool(g_ConvarNecroItemSpawnOverride))
	{
		if (StrEqual(iszResponseContext, "item_sp:1", false))
		{
			g_bIsMultiplayerOverride = true;
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