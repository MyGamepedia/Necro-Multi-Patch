#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CNPC_Human_Medic - npc_human_medic
// implementation of medic healthkit drop on death if one or more players have health below 50
//------------------------------------------------------
public MRESReturn Hook_HumanMedicKilled(int _this, DHookParam hParams)
{
    CBaseEntity pMedic = CBaseEntity(_this);
    int iHealth;

    //the orig code doesn't take care by what and how the medic was killed, 
    //the medic will drop healthkit even if killed by a zombie,
    //the only condition checked is if a player has health below 50,
    //to recreate the same behavior, we need to check health for all players
    for (int i = 1; i <= MaxClients; i++)
    {
        if (!IsClientInGame(i) || IsFakeClient(i) || !IsPlayerAlive(i))
            continue;


        iHealth = CBasePlayer(i).GetHealth();

        if (iHealth < 50) //if below 50 health - create healthkit under the legs of the medic
        {
            CItem pHealthKit = CItem(CreateEntityByName("item_healthkit"));

            if (pHealthKit == NULL_CBASEENTITY)
                return MRES_Ignored;

            float vecMedicOrigin[3], vecMedicAngles[3];
            pMedic.GetAbsOrigin(vecMedicOrigin);
            pMedic.GetAbsAngles(vecMedicAngles); //NOTE: it looks like the orig code cares only about X angle

            pHealthKit.Teleport(vecMedicOrigin, vecMedicAngles);
            pHealthKit.KeyValue("ItemType", "1"); //filter this item as sp to spawn sp version of healthkit
            pHealthKit.Spawn();

            break;
        }
    }

    return MRES_Ignored;
}