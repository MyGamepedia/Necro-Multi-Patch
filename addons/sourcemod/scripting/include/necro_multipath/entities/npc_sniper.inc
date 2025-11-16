#pragma newdecls required
#pragma semicolon 1

// `SelectSchedule` crashes because a null local player pointer was accessed.
// The entire function is reconstructed to remove the check.
// 
// Class: `CProtoSniper` - npc_sniper
//
// ## Citations
//
// - [Source SDK 2013](https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/mp/src/game/server/hl2/proto_sniper.cpp#L1385)
//
public MRESReturn Hook_ProtoSniperSelectSchedule(int _this, DHookReturn hReturn)
{
	CProtoSniper pSniper = CProtoSniper(_this);
	
	if (pSniper.HasCondition(view_as<int>(COND_ENEMY_DEAD)))
	{
		// unsure why this is crashing; unneeded
		/*
		if (PrecacheSound("NPC_Sniper.TargetDestroyed", true))
		{
			EmitGameSoundToAll("NPC_Sniper.TargetDestroyed", pSniper.entindex);
		}
		*/
	}

	if (!pSniper.IsWeaponLoaded())
	{
		DHookSetReturn(hReturn, SCHED_RELOAD);
		return MRES_Supercede;
	}
	
	// skipped SCHED_PSNIPER_PLAYER_DEAD
	// this schedule is pretty useless as it only turns on the laser to show the dead player where he got sniped from
	
	if (pSniper.HasCondition(view_as<int>(COND_HEAR_DANGER)))
	{
		// Hear Danger; add glint, ect. here
		DHookSetReturn(hReturn, SCHED_PSNIPER_SUPPRESSED);
		return MRES_Supercede;
	}
	
	if (!pSniper.IsEnabled())
	{
		DHookSetReturn(hReturn, SCHED_PSNIPER_DISABLEDWAIT);
		return MRES_Supercede;
	}
	
	if (pSniper.HasCondition(view_as<int>(COND_SNIPER_SWEEP_TARGET)))
	{
		CSniperTarget pSweepTarget = pSniper.GetSweepTarget();
		if ((pSweepTarget.IsValid() && (pSweepTarget.m_spawnflags & SF_SNIPERTARGET_NOINTERRUPT)) || pSniper.IsSweepHighestPriority())
		{
			DHookSetReturn(hReturn, SCHED_PSNIPER_SWEEP_TARGET_NOINTERRUPT);
		}
		else
		{
			DHookSetReturn(hReturn, SCHED_PSNIPER_SWEEP_TARGET);
		}
		return MRES_Supercede;
	}
	
	CBaseEntity pEnemy = pSniper.GetEnemy();
	if (!pEnemy.IsValid() || pSniper.HasCondition(view_as<int>(COND_ENEMY_DEAD)))
	{
		pSniper.SetEnemy(CBaseEntity());
		DHookSetReturn(hReturn, SCHED_PSNIPER_SCAN);
		return MRES_Supercede;
	}
	
	if (pSniper.HasCondition(view_as<int>(COND_SNIPER_FRUSTRATED)))
	{
		DHookSetReturn(hReturn, SCHED_PSNIPER_FRUSTRATED_ATTACK);
		return MRES_Supercede;
	}
	
	if (pSniper.HasCondition(view_as<int>(COND_SNIPER_CANATTACKDECOY)))
	{
		DHookSetReturn(hReturn, SCHED_RANGE_ATTACK2);
		return MRES_Supercede;
	}
	
	if (pSniper.HasCondition(view_as<int>(COND_SNIPER_NO_SHOT)))
	{
		DHookSetReturn(hReturn, SCHED_PSNIPER_NO_CLEAR_SHOT);
		return MRES_Supercede;
	}
	
	if (pSniper.HasCondition(view_as<int>(COND_CAN_RANGE_ATTACK1)))
	{
		DHookSetReturn(hReturn, SCHED_RANGE_ATTACK1);
		return MRES_Supercede;
	}

	DHookSetReturn(hReturn, SCHED_PSNIPER_CAMP);
	return MRES_Supercede;
}
