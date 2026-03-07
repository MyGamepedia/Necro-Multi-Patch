#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// npc_barnacle
// Fixes prediction and view jitter while being grabbed by a barnacle.
//------------------------------------------------------
public void Hook_Barnacle_OnGrab(const char[] szOutput, const int iCaller, const int iActivator, const float flDelay)
{
	CNPC_Barnacle pBarnacle = CNPC_Barnacle(iCaller);

	// The enemy of the barnacle is not set until after `CNPC_Barnacle::AttachTongueToTarget` is finished executing.
	// https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/game/server/hl2/npc_barnacle.cpp#L1359
	CreateTimer(0.0, Timer_Barnacle_OnGrab, pBarnacle, TIMER_FLAG_NO_MAPCHANGE);
}

//------------------------------------------------------
// npc_barnacle
// Fixes prediction and view jitter while being grabbed by a barnacle.
//------------------------------------------------------
public void Timer_Barnacle_OnGrab(Handle hTimer, const CNPC_Barnacle pBarnacle)
{
	if (pBarnacle.IsValid() && pBarnacle.IsAlive())
	{
		CBaseEntity pEnemy = pBarnacle.GetEnemy();
		if (pEnemy != NULL_CBASEENTITY && pEnemy.IsPlayer())
		{
			CBasePlayer pPlayer = view_as<CBasePlayer>(pEnemy);

			// Prevents client-side prediction errors for movement while being pulled up.
			pPlayer.m_fFlags |= FL_ATCONTROLS;
			// Removes the initial view jitter when being pulled up.
			pPlayer.SetMoveType(MOVETYPE_FLY);
			// Remove the view jitter while being pulled up.
			pPlayer.SetLaggedMovement(0.0);
		}
	}
}

//------------------------------------------------------
// npc_barnacle
// Fixes prediction and view jitter while being grabbed by a barnacle.
//------------------------------------------------------
public void Hook_Barnacle_OnRelease(const char[] szOutput, const int iCaller, const int iActivator, const float flDelay)
{
	CNPC_Barnacle pBarnacle = CNPC_Barnacle(iCaller);
	CBaseEntity pEnemy = pBarnacle.GetEnemy();
	if (pEnemy != NULL_CBASEENTITY && pEnemy.IsPlayer())
	{
		CBasePlayer pPlayer = view_as<CBasePlayer>(pEnemy);
		pPlayer.m_fFlags &= ~FL_ATCONTROLS;
		pPlayer.SetMoveType(MOVETYPE_WALK);
		pPlayer.SetLaggedMovement(1.0);
	}
}

//------------------------------------------------------
// npc_barnacle
// fix death animation not played sometimes
//------------------------------------------------------
public void Barnacle_Path(int iEntIndex)
{
	CAI_BaseNPC pBarnacle = CAI_BaseNPC(iEntIndex);
	pBarnacle.AddOutput("OnDeath", "!self", "AddOutput", "sequence 4");
}

//------------------------------------------------------
// npc_barnacle
// fix damage animation not played sometimes
//------------------------------------------------------
public Action Hook_BarnacleOnTakeDamagePost(int victim, int &attacker, int &inflictor, float &damage, int &damagetype, int &weapon, float damageForce[3], float damagePosition[3])
{
	/*
	if (weapon != NULL_CBASEENTITY)
	{
		char szClassname[64];
		GetEntityClassname(weapon, szClassname, sizeof(szClassname));
		if (strcmp(szClassname, "weapon_357") == 0)
		{
			damage *= sk_barnacle_357_dmg_scale.FloatValue;
		}

		if (strcmp(szClassname, "weapon_glock") == 0 || strcmp(szClassname, "weapon_mp5") == 0)
		{
			damage *= sk_barnacle_bullet_dmg_scale.FloatValue;
		}

		if (strcmp(szClassname, "weapon_shotgun") == 0)
		{
			damage *= sk_barnacle_buckshot_dmg_scale.FloatValue;
		}
	}

	
	attacker = 0;
	weapon = -1;
	damagetype = 0;
	inflictor = 0;
	damage = 0.0;

	for (int i = 0; i <= 2; i++)
	{
		damageForce[i] = 0.0;
		damageForce[i] = 0.0;
	}
	*/

	CAI_BaseNPC pEntity = CAI_BaseNPC(victim);
	pEntity.SetNextThink(GetTickedTime() + 0.1);

	//PrintToChatAll("victim %d, attacker %d, inflictor %d, damage %.2f, type %d, weapon %d", victim, attacker, inflictor, damage, damagetype, weapon);

	return Plugin_Continue;
}