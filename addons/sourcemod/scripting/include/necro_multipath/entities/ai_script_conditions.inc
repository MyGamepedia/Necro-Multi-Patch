#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CAI_ScriptConditions - ai_script_conditions
// Sets the nearest player to this entity's actor(s)/target into context for UTIL_GetLocalPlayer override to use.
//------------------------------------------------------
public MRESReturn Hook_AIConditionsThink(int _this)
{
	CAI_ScriptConditions pThis = CAI_ScriptConditions(_this);
	CBasePlayer pLocalPlayerOverride = NULL_CBASEENTITY;

	if (pThis.IsEnabled())
	{
		int count;
		float vecMidpoint[3], vecOrigin[3];

		CUtlVector pElementsList = CUtlVector(pThis.address + FindDataMapInfo(_this, "m_ElementList"));
		int len = pElementsList.Count();

		for (int i = 0; i < len; i++)
		{
			// 0 - EHANDLE			m_hActor
			// 4 - CSimTimer		m_Timer
			// 12 - CSimTimer		m_Timeout
			// == 20
			Address conditionsElement = pElementsList.Get(i, 20);
			if (!conditionsElement)
				continue;
			
			CBaseEntity pActor = CBaseEntity(LoadEntityFromHandleAddress(conditionsElement));
			if (!pActor.IsValid())
				continue;
			
			count++;
			pActor.GetAbsOrigin(vecOrigin);
			AddVectors(vecMidpoint, vecOrigin, vecMidpoint);
		}

		if (count)
		{
			ScaleVector(vecMidpoint, 1.0 / count);
		}
		else
		{
			// secondary valid usage is to specify target only
			CBaseEntity pTarget = pThis.GetTargetEnt();
			if (pTarget.IsValid())
			{
				pTarget.GetAbsOrigin(vecMidpoint);
			}
		}

		pLocalPlayerOverride = GetNearestPlayerPreferAliveEx(vecMidpoint);
		if (pLocalPlayerOverride == NULL_CBASEENTITY)
		{
			AddSinglePlayerPlaceholder();
			return MRES_Supercede;
		}
	}
	AddSinglePlayerOverride(pLocalPlayerOverride);
	return MRES_Ignored;
}

public MRESReturn Hook_AIConditionsThinkPost(int _this)
{
	PopSinglePlayerOverride();
	return MRES_Ignored;
}