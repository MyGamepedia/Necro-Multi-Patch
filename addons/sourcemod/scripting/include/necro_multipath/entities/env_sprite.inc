public Sprite_PathCanister(int entity)
{
	int parent = GetEntPropEnt(entity, Prop_Data, "m_hMoveParent");
	
	if (!IsValidEntity(parent)) 
	{
		return;
	}
	
	char classname[64];
	GetEntityClassname(parent, classname, sizeof(classname));
	
	if(StrEqual(classname, "item_ammo_canister"))
	{
		if(GetEntProp(entity, Prop_Data, "m_iHammerID") == 0 && GetEntPropFloat(entity, Prop_Data, "m_flSpriteScale") == 0.15
		&& GetEntProp(entity, Prop_Data, "m_nRenderMode") == 7)
		{
			char model[PLATFORM_MAX_PATH];
			GetEntPropString(entity, Prop_Data, "m_ModelName", model, sizeof(model));
			
			if(StrEqual(model, "sprites/glow.vmt"))
			{
				SetEntProp(entity, Prop_Data, "m_nRenderMode", 9); //todo: maybe make it optional
				DHookEntity(hkAcceptInput, false, parent, _, Hook_CanisterInputRespawn);
				
				char name[64];
				Format(name, sizeof(name), "sprite_ammocanister_%d", parent);
				DispatchKeyValue(entity, "targetname", name);
				
				AddOutput(parent, "OnUser4", name, "ShowSprite");
				AddOutput(parent, "OnPlayerPickup", name, "HideSprite");
			}
		}
	}
}