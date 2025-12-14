#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// item_weapon_snark
// Once the snark nest has exploded, don't do it again. This used to cause chain reaction because we set zero item respawn time.
//------------------------------------------------------
public void Hook_OnItemSnarkDamagePost(int victim, int attacker, int inflictor, float damage, int damagetype)
{
	CItem pItem = CItem(victim);

	char iszResponseContext[MAX_KEY];
	GetEntPropString(pItem.entindex, Prop_Data, "m_iszResponseContext", iszResponseContext, sizeof(iszResponseContext));

	

	if (GetConVarBool(g_ConvarNecroItemSpawnOverride) && StrEqual(iszResponseContext, "item_sp:1", false))
	{
		SDKHook(victim, SDKHook_OnTakeDamage, Hook_PreventItemSnarkDamage);
		SDKUnhook(victim, SDKHook_OnTakeDamagePost, Hook_OnItemSnarkDamagePost);
	}
}

public Action Hook_PreventItemSnarkDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon,
		float damageForce[3], float damagePosition[3], int damagecustom)
{
	return Plugin_Stop;
}