#pragma newdecls required
#pragma semicolon 1

//Purpose: Add item_ammo_canister's sprite on/off state control depending on canister's state
public void Sprite_PathCanister(int entity)
{
	int parent = GetEntPropEnt(entity, Prop_Data, "m_hMoveParent"); //get parent entity
	
	if (!IsValidEntity(parent)) //rerurn if no valid parent
	{
		return;
	}
	
	char classname[64];
	GetEntityClassname(parent, classname, sizeof(classname));
	
	if(StrEqual(classname, "item_ammo_canister"))
	{
		if(GetEntProp(entity, Prop_Data, "m_iHammerID") == 0 && GetEntPropFloat(entity, Prop_Data, "m_flSpriteScale") == 0.15
		&& GetEntProp(entity, Prop_Data, "m_nRenderMode") == 7) //we need exctra steps to identify if the sprite is what we need
		{
			char model[PLATFORM_MAX_PATH];
			GetEntPropString(entity, Prop_Data, "m_ModelName", model, sizeof(model));
			
			if(StrEqual(model, "sprites/glow.vmt")) //also check model
			{
				SetEntProp(entity, Prop_Data, "m_nRenderMode", 9); //use world space glow to the sprite don't clip
																   //todo: make optional ?

				DHookEntity(hkAcceptInput, false, parent, _, Hook_CanisterInputRespawn); //hook canister's inputs so we surely know that this canister is respawned 
				
				char name[64];
				Format(name, sizeof(name), "sprite_ammocanister_%d", parent); //set sprite to control with entity IO
				DispatchKeyValue(entity, "targetname", name);
				
				AddOutput(parent, "OnUser4", name, "ShowSprite"); //once canister is respawned - it fires "FireUser4" on self, this allows us to enable the sprite
				AddOutput(parent, "OnPlayerPickup", name, "HideSprite"); //hide sprite on pickup
			}
		}
	}
}