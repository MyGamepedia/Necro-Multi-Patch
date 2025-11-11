#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_PlayerSpawnPost(int iClient, DHookReturn hReturn, DHookParam hParams)
{
	//BUG: sometimes SourceTV can slip through even though we never hook it?
	if (IsFakeClient(iClient))
		return MRES_Ignored;

	if(IsPlayerAlive(iClient))
	{
		//disable "other players flashlight"
		SendConVarValue(iClient, sv_cheats, "1"); //HACK! This thing needs cheats ON
		ClientCommand(iClient, "r_flashlight_3rd_draw 0");

		//Fix for if player died on a ladder
		SetEntPropEnt(iClient, Prop_Data, "m_hLadder", -1);

		//TODO: Finish this code that cleans blood for spawned player
		/*int blackmesaviewmodel = GetEntPropEnt(iClient, Prop_Data, "m_hViewModel");

		if(IsValidEntity(blackmesaviewmodel))
		{
			int materialmodifycontrol_weapon = CreateEntityByName("material_modify_control");
			int materialmodifycontrol_arms = CreateEntityByName("material_modify_control");

			if(IsValidEntity(material_modify_control))
			{
				DispatchKeyValue(material_modify_control,"materialVar","$detailframe");
			}
		}*/
	}

	else
	{
		//Initialize clientside convars from skill.cfg, which may have not executed. 
		//Todo: a better way to execute all this, used 7 func due to compile error
		ClientCommand(iClient, "gb_flashlight_PosX -5; gb_flashlight_PosY -28.0; gb_flashlight_Intensity 0.8; gb_flashlight_Col 245 240 225");
		ClientCommand(iClient, "gb_flashlight_Range 1000; gb_flashlight_oldfalloff_quad 5; gb_flashlight_oldfalloff_linear 6000; cl_xort_light_col_R 170");
		ClientCommand(iClient, "cl_xort_light_col_B 80; cl_xort_light_intensity 750; cl_xort_light_range 256; cl_xort_light_ttl_Zap 0.5; cl_xort_light_ttl_dispel 0.5");
		ClientCommand(iClient, "cl_envportal_light_col_R 170; cl_envportal_light_col_B 80; r_bloomtintr_nextgen 0.25; r_bloomtintg_nextgen 0.25; r_bloomtintb_nextgen 0.25");
		ClientCommand(iClient, "r_bloomtintexponent_nextgen 2.2; r_bloom_nextgen_threshold 0.1; mat_bloom_nextgen 1; cl_gargflame_spotL_col_R 35");
		ClientCommand(iClient, "cl_gargflame_spotL_col_G 165; cl_gargflame_spotL_intensity 96000; cl_gargflame_spotL_range 768; cl_gargflame_pointL_col_R 35");
		ClientCommand(iClient, "cl_gargflame_pointL_col_G 165; cl_gargflame_pointL_intensity 32000; cl_garg_eyeL_light_intensity 100; cl_garg_eyeL_light_range 100");
	}

	return MRES_Ignored;
}