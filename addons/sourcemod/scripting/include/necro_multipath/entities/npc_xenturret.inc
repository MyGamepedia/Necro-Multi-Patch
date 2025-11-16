#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CNPC_XenTurret - npc_xenturret
//------------------------------------------------------
public void Hook_XenTurretSpawnPost(int iEntIndex)
{
	// Use our fixed model, which doesn't have the clientside particles which affected everyone's screens even when they shouldn't.
	static char szNewModel[] = "models/props_xen/xen_turret_mpfix.mdl";
	if (PrecacheModel(szNewModel))
	{
		SetEntityModel(iEntIndex, szNewModel);
	}
}