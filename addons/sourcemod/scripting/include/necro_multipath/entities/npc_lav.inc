#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CAI_BaseNPC - npc_lav
// fixes the client crash seen in bm_c2a5g related to LAV npc
//------------------------------------------------------
public void Hook_LAVSpawnPost(int iEntIndex)
{
	CBaseEntity pEntity = CBaseEntity(iEntIndex);
	pEntity.edictFlags |= FL_EDICT_ALWAYS;
}