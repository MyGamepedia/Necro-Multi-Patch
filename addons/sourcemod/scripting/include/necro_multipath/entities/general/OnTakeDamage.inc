//Purpose: Modify crossbow bolt hitscan damage
public Action Hook_OnTakeDamage(int victim, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon, float damageForce[3], float damagePosition[3])
{
	#if defined DEBUG
	PrintToServer("Hook_OnTakeDamage: victim=%d, attacker=%d, inflictor=%d, damage=%.2f, damagetype=%d, weapon=%d, damageForce[%.2f, %.2f, %.2f], damagePosition[%.2f, %.2f, %.2f]",
					victim, attacker, inflictor, damage, damagetype, weapon,
					damageForce[0], damageForce[1], damageForce[2],
					damagePosition[0], damagePosition[1], damagePosition[2]);
	#endif
					
	//Check if damage is from crossbow hitscan bolt
	if(IsValidEntity(weapon))
	{
		char classname[64];
		GetEntityClassname(weapon, classname, sizeof(classname));
		
		//Modify damage value if is weapon_crossbow and damage type is 4096 
		if(StrEqual(classname, "weapon_crossbow") && damagetype == 4096)
		{
			if(damage == 125)
			{
				damage = GetConVarFloat(g_ConvarNecroBoltHitscanDamage);
				
				#if defined DEBUG
				PrintToServer("Normal crossbow damage");
				#endif
			}
			
			//Headshot multiplier fix
			if(damage == 125 * GetConVarFloat(FindConVar("sk_player_head")))
			{
				damage = GetConVarFloat(g_ConvarNecroBoltHitscanDamage) * GetConVarFloat(FindConVar("sk_player_head"));
				
				#if defined DEBUG
				PrintToServer("Head crossbow damage");
				#endif
			}
			//TODO: Call the programmer to add more multipliers if needed
			
			return Plugin_Changed;
		}
	}

    return Plugin_Continue;
}