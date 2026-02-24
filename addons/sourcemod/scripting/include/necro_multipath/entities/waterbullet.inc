#pragma newdecls required
#pragma semicolon 1

WaterLevel_t g_ePastWaterLevel;

#if defined ENTPATCH_WATERBULLET
public Action Hook_WaterBulletSetTransmit(int entity, int client) 
{
    CBasePlayer pPlayer = CBasePlayer(client);

    if (pPlayer.IsValid() && pPlayer.GetWaterLevel() == WL_Eyes)
    {
        return Plugin_Continue;
    }

    return Plugin_Stop;
}

public MRESReturn Hook_HandleShotImpactingWater(int _this, DHookReturn hReturn, DHookParam hParams)
{
    CBaseEntity pWorldSpawn = CBaseEntity(0); //HACK! Use worldspawn instead of a player to workaround HUD issues
    g_ePastWaterLevel = pWorldSpawn.GetWaterLevel(); //something ovverrided before ? lets save it
    pWorldSpawn.SetWaterLevel(WL_Eyes); //set water level we need to make water bullet appear 
    AddSinglePlayerOverride(pWorldSpawn); //use worldspawn during this func
    return MRES_Ignored;
}

public MRESReturn Hook_HandleShotImpactingWaterPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
    g_pLocalPlayerEntity[g_iLocalPlayerStackPointer].SetWaterLevel(g_ePastWaterLevel); //restore past val
    PopSinglePlayerOverride(); //remove worldspawn override
    return MRES_Ignored;
}

public MRESReturn Hook_FireBullets(int _this, DHookReturn hReturn, DHookParam hParams)
{
    CBaseEntity pWorldSpawn = CBaseEntity(0);
    g_ePastWaterLevel = pWorldSpawn.GetWaterLevel();
    pWorldSpawn.SetWaterLevel(WL_Eyes);
    AddSinglePlayerOverride(pWorldSpawn);
    return MRES_Ignored;
}

public MRESReturn Hook_FireBulletsPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
    g_pLocalPlayerEntity[g_iLocalPlayerStackPointer].SetWaterLevel(g_ePastWaterLevel);
    PopSinglePlayerOverride();
    return MRES_Ignored;
}

//make it properly transmit work properly, otherwise it will be invisible for clients
public MRESReturn Hook_WaterBulletUpdateTransmitState(int _this, DHookReturn hReturn, DHookParam hParams)
{
	DHookSetReturn(hReturn, CBaseEntity(_this).SetTransmitState(0));
	return MRES_Supercede;
}
#endif