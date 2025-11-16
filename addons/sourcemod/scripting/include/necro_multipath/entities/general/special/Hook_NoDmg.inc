#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// General hook
// Disable damage - can be used to prevent physics crashes on linux with specific models in BM
//------------------------------------------------------
public Action Hook_NoDmg(int victim, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon, float damageForce[3], float damagePosition[3])
{
	return Plugin_Handled;
}