#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// Fixes `nullptr` being returned due to the function only checking one new client every function call.
// - NPC dialogue should always work on `+use` even if other players are out of PVS of the NPC.
// - NPC pose parameters should not ever reset anymore due out of PVS player.
//------------------------------------------------------
public MRESReturn Hook_UTIL_FindClient(DHookReturn hReturn, DHookParam hParams)
{
	// Naive solution with fast execution.
	// The original implementation below this implementation causes issues with NPCs doing nothing.
	CBaseEntity pEntity = CBaseEntity(hParams.Get(1));
	DHookSetReturn(hReturn, (pEntity != NULL_CBASEENTITY) ? GetNearestPlayerPreferAlive(pEntity).entindex : -1);
	return MRES_Supercede;

	/*
	CBaseEntity pNPC = CBaseEntity(hParams.Get(1));

	float vec3Origin[3];
	pNPC.GetAbsOrigin(vec3Origin);

	for (int i = 1; i <= MaxClients; ++i)
	{
		CBasePlayer pPlayer = CBasePlayer(i);
		if (pPlayer != NULL_CBASEENTITY && !pPlayer.HasFlags(FL_NOTARGET))
		{
			float vec3PlayerPosition[3];
			pPlayer.GetAbsOrigin(vec3PlayerPosition);

			int iCluster = CVEngineServer.GetClusterForOrigin(vec3PlayerPosition);
			if (iCluster != -1)
			{
				MemoryBlock pPVS = CVEngineServer.GetPVSForCluster(iCluster);
				bool bFound = CVEngineServer.CheckOriginInPVS(vec3Origin, pPVS);
				delete pPVS;

				if (bFound)
				{
					DHookSetReturn(hReturn, i);
					return MRES_Supercede;
				}
			}
		}
	}
	DHookSetReturn(hReturn, -1);
	return MRES_Supercede;
	*/
}
