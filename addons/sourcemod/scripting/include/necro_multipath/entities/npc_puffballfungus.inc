#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CAI_BaseNPC - npc_puffballfungus
// MP TakeDamage crash fix
//------------------------------------------------------
public Action Hook_PuffballFungusDmg(int victim, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon, float damageForce[3], float damagePosition[3])
{
	attacker = 0;
	inflictor = 0;
	weapon = -1;
	return Plugin_Changed;
}