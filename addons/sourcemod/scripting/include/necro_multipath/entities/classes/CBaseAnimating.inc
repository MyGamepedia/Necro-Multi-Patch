#pragma newdecls required
#pragma semicolon 1

public MRESReturn Hook_Dissolve(int _this, DHookReturn hReturn, DHookParam hParams)
{
	EntityDissolveType_t eDissolveType = hParams.Get(4);
	if (eDissolveType == ENTITY_DISSOLVE_FADE || eDissolveType == ENTITY_DISSOLVE_REMOVE_NOW)
	{
		if (CBaseEntity(_this).IsPlayer())
		{
			hParams.Set(4, ENTITY_DISSOLVE_NORMAL);
			return MRES_ChangedHandled;
		}
	}
	return MRES_Ignored;
}