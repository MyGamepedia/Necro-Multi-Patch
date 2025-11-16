#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// Scene entities
// fix findnamedentity returning sp player ( nullptr )
//------------------------------------------------------
public MRESReturn Hook_FindNamedEntity(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!DHookIsNullParam(hParams, 1) && !DHookIsNullParam(hParams, 2))
	{
		char szName[MAX_CLASSNAME];
		DHookGetParamString(hParams, 1, szName, sizeof(szName));
		if ((strcmp(szName, "Player", false) == 0)
			|| (strcmp(szName, "!player", false) == 0)
			|| (strcmp(szName, "!nearestfriend", false) == 0)
			|| (strcmp(szName, "!friend", false) == 0))
		{
			CBaseEntity pActor = CBaseEntity(DHookGetParam(hParams, 2));
			if (pActor != NULL_CBASEENTITY)
			{
				CBasePlayer pBestPlayer = GetNearestPlayer(pActor);
				if (pBestPlayer != NULL_CBASEENTITY)
				{
					DHookSetReturn(hReturn, pBestPlayer.entindex);
					return MRES_Supercede;
				}
			}
		}
	}

	return MRES_Ignored;
}