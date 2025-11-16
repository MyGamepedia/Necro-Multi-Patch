#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CMusicTrack - music_track
// crash fix
//------------------------------------------------------
public MRESReturn Hook_MusicTrackThink(int _this)
{
	return MRES_Supercede;
}

public MRESReturn Hook_MusicTrackAcceptInput(int _this, DHookReturn hReturn, DHookParam hParams)
{
	char szInputType[MAX_FORMAT];
	char szBuffer[MAX_VALUE];
	DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));

	if (strcmp(szInputType, "Play", false) == 0)
	{
		GetEntPropString(_this, Prop_Data, "m_TrackScriptSound", szBuffer, sizeof(szBuffer));
		
		Event event = CreateEvent("music_track_register", true);
		event.SetString("soundscriptname", szBuffer);
		event.Fire();
		event = CreateEvent("music_track_play", true);
		event.SetString("soundscriptname", szBuffer);
		event.Fire();
		
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}
	else if (strcmp(szInputType, "Fade", false) == 0)
	{
		GetEntPropString(_this, Prop_Data, "m_TrackScriptSound", szBuffer, sizeof(szBuffer));
		
		Event event = CreateEvent("music_track_fade", true);
		event.SetString("soundscriptname", szBuffer);
		event.Fire();
		
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}
	else if (strcmp(szInputType, "Next", false) == 0)
	{
		GetEntPropString(_this, Prop_Data, "m_strNextTrackEntity", szBuffer, sizeof(szBuffer));
		int iNext = FindEntityByTargetname(-1, szBuffer, "music_track");
		if (iNext != -1)
		{
			AcceptEntityInput(iNext, "Play");
		}
		AcceptEntityInput(_this, "Fade");
		
		DHookSetReturn(hReturn, true);
		return MRES_Supercede;
	}
	return MRES_Ignored;
}
