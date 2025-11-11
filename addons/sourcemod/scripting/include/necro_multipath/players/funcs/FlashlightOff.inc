#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_FlashlightOff(int iClient, DHookReturn hReturn, DHookParam hParams)
{
	if(GetConVarBool(g_ConvarNecroOtherPlayersFlashlight))
		Player_Flashlight3rdPersonOff(iClient); //removed "other players flashlight" effects

	return MRES_Ignored;
}

//Purpose: Removes "other players flashlight" effects (beam and spotlight_end)
public bool Player_Flashlight3rdPersonOff(int iClient)
{
	char name[64];
	Format(name, sizeof(name), "beam_otherplayerflashlight_%d", iClient);

	//get beam entity by searching child ents 
	int beam_flashlight = GetChildByClassname(iClient, name); //TODO: replace with GetChildByTargetname when we will have working code

	if(!IsValidEntity(beam_flashlight))
	{
		return false; //false if one of the ents is not valid
	}

	int spotlightend_flashlight = GetEntPropEnt(beam_flashlight, Prop_Data, "m_hDamageFilter"); //get spotlight_end by getting it from beam's dmg filter

	AcceptEntityInput(beam_flashlight, "Kill");

	if(!IsValidEntity(spotlightend_flashlight))
	{
		return false;
	}

	AcceptEntityInput(spotlightend_flashlight, "Kill"); 

	return true; //true if everything is succeeded
}