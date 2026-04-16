#pragma newdecls required
#pragma semicolon 1

#if defined ENTPATCH_WATERBULLET
void WaterBullet_Path(int iEntIndex)
{
	CBaseEntity pWaterBullet = CBaseEntity(iEntIndex);
	pWaterBullet.edictFlags &= ~FL_EDICT_ALWAYS;
	pWaterBullet.edictFlags |= FL_EDICT_PVSCHECK;
}

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
    pWorldSpawn.SetUserData("m_iPastWaterLevel", pWorldSpawn.GetWaterLevel()); //something ovverrided before ? lets save it
    pWorldSpawn.SetWaterLevel(WL_Eyes); //set water level we need to make water bullet appear 
    AddSinglePlayerOverride(pWorldSpawn); //use worldspawn during this func
    return MRES_Ignored;
}

public MRESReturn Hook_HandleShotImpactingWaterPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
    CBaseEntity pEnt = g_pLocalPlayerEntity[g_iLocalPlayerStackPointer];
    pEnt.SetWaterLevel(pEnt.GetUserData("m_iPastWaterLevel")); //restore past val
    PopSinglePlayerOverride(); //remove worldspawn override
    return MRES_Ignored;
}

public MRESReturn Hook_FireBullets(int _this, DHookReturn hReturn, DHookParam hParams)
{
    CBaseEntity pWorldSpawn = CBaseEntity(0);
    pWorldSpawn.SetUserData("m_iPastWaterLevel", pWorldSpawn.GetWaterLevel());
    pWorldSpawn.SetWaterLevel(WL_Eyes);
    AddSinglePlayerOverride(pWorldSpawn);
    return MRES_Ignored;
}

public MRESReturn Hook_FireBulletsPost(int _this, DHookReturn hReturn, DHookParam hParams)
{
    CBaseEntity pEnt = g_pLocalPlayerEntity[g_iLocalPlayerStackPointer];
    pEnt.SetWaterLevel(pEnt.GetUserData("m_iPastWaterLevel"));
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