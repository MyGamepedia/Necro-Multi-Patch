#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_FlashlightOn(int iClient, DHookReturn hReturn, DHookParam hParams)
{
    if(GetConVarBool(g_ConvarNecroOtherPlayersFlashlight))
    {
        int beam_flashlight = CreateEntityByName("beam");
        int spotlightend_flashlight = CreateEntityByName("spotlight_end");

        int spritetexture = PrecacheModel("sprites/glow01.vmt"); //sprite texture used by both halo and beam

        if(!IsValidEntity(beam_flashlight) || !IsValidEntity(spotlightend_flashlight))
        {
            return MRES_Ignored;
        }

        DispatchSpawn(spotlightend_flashlight);

        //Mainly repeats ent from C_HL2MP_Player::AddEntity
        //(https://github.com/ValveSoftware/source-sdk-2013/blob/b2705ba55b3b802b86ef2b2dbf97939c9d4fb685/src/game/client/hl2mp/c_hl2mp_player.cpp#L616)
        SetEntProp(beam_flashlight, Prop_Data, "m_nHaloIndex", spritetexture);
        SetEntProp(beam_flashlight, Prop_Data, "m_nModelIndex", spritetexture);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_fHaloScale", 3.0);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_fWidth", 8.0);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_fEndWidth", 35.0);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_fFadeLength", 300.0);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_fAmplitude", 0.0);
        SetEntPropFloat(beam_flashlight, Prop_Send, "m_fSpeed", 0.0);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_fStartFrame", 0.0);
        SetEntPropFloat(beam_flashlight, Prop_Data, "m_flFrameRate", 0.0);
        SetEntProp(beam_flashlight, Prop_Data, "m_nBeamFlags", FBEAM_FOREVER | FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM);

        //set up ents, use client for point A and use spotlight_end for point B, set 2 beam ents, set beam type 2
        SetEntPropEnt(beam_flashlight, Prop_Send, "m_hAttachEntity", EntIndexToEntRef(iClient));
        SetEntPropEnt(beam_flashlight, Prop_Send, "m_hAttachEntity", EntIndexToEntRef(spotlightend_flashlight), 1);
        SetEntProp(beam_flashlight, Prop_Send, "m_nNumBeamEnts", 2);
        SetEntProp(beam_flashlight, Prop_Send, "m_nBeamType", 2);

        DispatchKeyValue(beam_flashlight, "rendercolor", "245 240 225"); //the same as flashlight color

        DispatchSpawn(beam_flashlight);

        char name[64];
        Format(name, sizeof(name), "beam_otherplayerflashlight_%d", iClient);
        SetEntPropString(beam_flashlight, Prop_Data, "m_iClassname", name); //HACK! I always get 0 with targetname, use classname to write and check the name
    
        float vecPos[3], vecAng[3];

        //teleport the beam to proper pos on the first frame to workaround beam at worldspawn on the first frame
        GetEntityAttachment(iClient, 4, vecPos, vecAng);
        TeleportEntity(beam_flashlight, vecPos, vecAng);

        //make the beam following the player
        SetVariantString("!activator");
        AcceptEntityInput(beam_flashlight, "SetParent", iClient);

        SetEntProp(beam_flashlight, Prop_Data, "m_nAttachIndex", 4, 0); //to make beam working with attachment point, used this field, 4 is attach num, 0 is the A beam point ent

        SetEntPropEnt(spotlightend_flashlight, Prop_Data, "m_hDamageFilter", beam_flashlight); //store beam in this field for quick access
        SetEntPropEnt(spotlightend_flashlight, Prop_Data, "m_hOwnerEntity", iClient); //store client

        SetEntPropEnt(beam_flashlight, Prop_Data, "m_hDamageFilter", spotlightend_flashlight); //store spotlight_end in this field for quick access
        SetEntPropEnt(spotlightend_flashlight, Prop_Data, "m_hOwnerEntity", iClient); //store client

        //hide for owner
        SDKHook(beam_flashlight, SDKHook_SetTransmit, Hook_Player_Flashlight3rdPersonEffects_Transmit);
        SDKHook(spotlightend_flashlight, SDKHook_SetTransmit, Hook_Player_Flashlight3rdPersonEffects_Transmit);

        Player_OtherPlayersFlashlightUpdateBeamLength(spotlightend_flashlight); //calc pos
    }

	return MRES_Ignored;
}

//Purpose: Hide "other players flashlight" effect ents for owner
public Action Hook_Player_Flashlight3rdPersonEffects_Transmit(int entity, int client) 
{
	int owner = GetEntPropEnt(entity, Prop_Data, "m_hOwnerEntity"); //get owner
	
	if(!IsValidEntity(owner)) //remove entity if no valid owner
	{
		AcceptEntityInput(entity, "Kill");
		return Plugin_Continue;
	}

	if (client == owner) //hide for owner
	{	
		return Plugin_Stop;
	}

	if (IsClientObserver(client)) //check if client is in observer mode
	{
		int observertarget = GetEntPropEnt(client, Prop_Data, "m_hObserverTarget"); //get target being observed
		int observermode = GetEntProp(client, Prop_Data, "m_iObserverMode"); //get observer mode
		
		if (observertarget == owner && observermode == 4) //if target is owner and in first person mode - hide
		{
			return Plugin_Stop;
		}
	}

	return Plugin_Continue;
}

//Purpose: Update flashlight's spotlight_end position every frame by tracing beam from player's eyes
public void Player_OtherPlayersFlashlightUpdateBeamLength(int entity)
{
	if(!IsValidEntity(entity)) //it can be removed after the past frame, make sure it's valid
	{
		return;
	}

	int iClient = GetEntPropEnt(entity, Prop_Data, "m_hOwnerEntity"); //get client the spotlight_end belongs

	//no valid client somehow ? remove this spotlight_end, we don't need it anymore
	if(!IsValidEntity(iClient))
	{
		AcceptEntityInput(entity, "Kill");
		return;
	}

	//NOTE: The new code seems to work but not tested too much
	float vecPos[3], vecAng[3], vecEnd[3], vecDir[3], vecEndGuess[3];

	//use eyes so the player can highlight something for the other player
	GetClientEyeAngles(iClient, vecAng);
	GetClientEyePosition(iClient, vecPos);

	GetAngleVectors(vecAng, vecDir, NULL_VECTOR, NULL_VECTOR);
	
	AddVectors(vecPos, vecDir, vecEndGuess);
	ScaleVector(vecDir, 200.0); //limit max range by 200

	AddVectors(vecPos, vecDir, vecEndGuess);
	Handle trace = TR_TraceRayFilterEx(vecPos, vecEndGuess, MASK_SHOT, RayType_EndPoint, TraceEntityFilterPlayers, iClient); //trace ray with owner ignore
	TR_GetEndPosition(vecEnd, trace); //get end pos
	CloseHandle(trace);

	MakeVectorFromPoints(vecEnd, vecPos, vecDir);
	NormalizeVector(vecDir, vecDir);
	ScaleVector(vecDir, 16.0); //make the end result shorter to make spotlight_end display properly
	AddVectors(vecEnd, vecDir, vecEnd);

	TeleportEntity(entity, vecEnd, NULL_VECTOR);

	RequestFrame(Player_OtherPlayersFlashlightUpdateBeamLength, entity); //to the same on next frame
}

//Purpose: Make tracer ignore a client
public bool TraceEntityFilterPlayers(int entity, int mask, any data)
{
    int client = data;
    if (entity == client)
        return false;
	
    return true;
}