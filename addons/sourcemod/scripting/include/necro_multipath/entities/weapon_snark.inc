#pragma newdecls required
#pragma semicolon 1

#define EVENT_WEAPON_SNARK_THROW 3005 //main attack
#define EVENT_WEAPON_SNARK_LOB 3013 //secondary attack


public MRESReturn Hook_WeaponSnarkOperatorHandleAnimEvent(int _this, DHookParam hParams)
{
    Address pEvent = hParams.GetAddress(1);

    if (pEvent == Address_Null)
        return MRES_Ignored;

    int iEventId = LoadFromAddress(pEvent, NumberType_Int32);

    PrintToServer("Snark anim event: 0x%X (%d)", iEventId, iEventId);
    
    if(iEventId != EVENT_WEAPON_SNARK_THROW || iEventId != EVENT_WEAPON_SNARK_LOB)
        return MRES_Ignored;

    CBlackMesaPlayer pPlayer = hParams.Get(2);
    if (pPlayer.IsValid() == false)
        return MRES_Ignored;

    int iUpSpeed, iForwardSpeed;
    if(iEventId == EVENT_WEAPON_SNARK_THROW)
    {
        iUpSpeed = GetConVarInt(sk_weapon_snark_throw_uspeed);
        iForwardSpeed = GetConVarInt(sk_weapon_snark_throw_fspeed);
    }
    else
    {
        iUpSpeed = GetConVarInt(sk_weapon_snark_lob_uspeed);
        iForwardSpeed = GetConVarInt(sk_weapon_snark_lob_fspeed);
    }

    float vecAngles[3], vecEyePos[3], vecPlayerVel[3], vecForward[3], vecUp[3], vecThrowVel[3], vecAngularVel[3];

    pPlayer.GetEyeAngles(vecAngles);
    pPlayer.GetEyePosition(vecEyePos);
    pPlayer.GetAbsVelocity(vecPlayerVel);

    GetAngleVectors(vecAngles, vecForward, NULL_VECTOR, vecUp);

    vecThrowVel[0] = vecUp[0] * iUpSpeed + vecForward[0] * iForwardSpeed + vecPlayerVel[0] * 1.2;
    vecThrowVel[1] = vecUp[1] * iUpSpeed + vecForward[1] * iForwardSpeed + vecPlayerVel[1] * 1.2;
    vecThrowVel[2] = vecUp[2] * iUpSpeed + vecForward[2] * iForwardSpeed + vecPlayerVel[2] * 1.2;

    vecAngularVel[0] = 750.0;
    vecAngularVel[1] = GetRandomFloat(-750.0, 750.0);
    vecAngularVel[2] = 0.0;

    pPlayer.SetAmmoFromIndex(AMMO_SNARKS, pPlayer.GetAmmoFromIndex(AMMO_SNARKS) - 1);

    /*CNpc_Snark pSnark = SDKCall(g_pCreateWeaponTracedBoneMergeEntity, "npc_snark", pPlayer.entindex);
    if (pSnark.IsValid() == false)
        return MRES_Supercede;

    pSnark.ApplyLocalVelocityImpulse(vecThrowVel);
    pSnark.ApplyLocalAngularVelocityImpulse(vecAngularVel);

    if(MaxClients < 1 && CMultiplayRules.IsTeamplay())
    {
        pSnark.SetTeam(pPlayer.GetTeam());
    }*/

    return MRES_Supercede;
}
