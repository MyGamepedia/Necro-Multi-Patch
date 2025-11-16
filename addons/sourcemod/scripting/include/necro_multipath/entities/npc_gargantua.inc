#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// phys_bone_follower
// This hook deletes selected NPC's bone followers to increase multiplayer server perf.
// Using VPhysicsUpdate since Spawn isn't called for this ent.
//------------------------------------------------------
public void Hook_BoneFollowerVPhysicsUpdatePost(int iEntIndex)
{
	CBaseEntity pEntity = CBaseEntity(iEntIndex);
	CBaseEntity pOwner = pEntity.GetOwner();
	if (pOwner != NULL_CBASEENTITY)
	{
		char szOwnerClass[MAX_CLASSNAME];
		pOwner.GetClassname(szOwnerClass, sizeof(szOwnerClass));
		if (StrEqual(szOwnerClass, "npc_gargantua"))
		{
			RemoveEntity(iEntIndex);
		}
	}
	SDKUnhook(iEntIndex, SDKHook_VPhysicsUpdatePost, Hook_BoneFollowerVPhysicsUpdatePost);
}

//------------------------------------------------------
// CAI_BaseNPC - npc_gargantua
// Use SOLID_BBOX instead of bone followers for collisions
//------------------------------------------------------
public void Hook_GargSpawnPost(int iEntIndex)
{
	CBaseEntity pEntity = CBaseEntity(iEntIndex);
	pEntity.SetSolidType(SOLID_BBOX);
}

//------------------------------------------------------
// CAI_BaseNPC - npc_gargantua
// fix the 'ChasePlayer' input by setting a target ent.
//------------------------------------------------------
public MRESReturn Hook_GargAcceptInputPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
	if (!DHookIsNullParam(hParams, 1))
	{
		char szInputType[MAX_FORMAT];
		DHookGetParamString(hParams, 1, szInputType, sizeof(szInputType));
		if (strcmp(szInputType, "chaseplayer", false) == 0)
		{
			SDKHook(_this, SDKHook_ThinkPost, GargantuaChaseThink);
			GargantuaChaseThink(_this);
		}
		else if (strcmp(szInputType, "chaseend", false) == 0)
		{
			SDKUnhook(_this, SDKHook_ThinkPost, GargantuaChaseThink);
		}
	}
	return MRES_Ignored;
}

public void GargantuaChaseThink(int _this)
{
	CAI_BaseNPC pThis = CAI_BaseNPC(_this);
	pThis.SetTargetEnt(GetNearestPlayer(pThis));
}