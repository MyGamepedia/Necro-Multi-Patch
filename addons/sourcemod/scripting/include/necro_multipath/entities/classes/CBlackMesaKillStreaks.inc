#pragma newdecls required
#pragma semicolon 1

//Purpose: Translate killstreak messages
public MRESReturn Hook_PostChatMessage(DHookParam hParams)
{
    #if defined DEBUG
    PrintToChatAll("Hook_PostChatMessage called!");
    #endif

    char szMsg[MAX_NAME_LENGTH + 24];
    DHookGetParamString(hParams, 1, szMsg, sizeof(szMsg));

    #if defined DEBUG
    PrintToChatAll("Killstreak message: %s", szMsg);
    #endif

    if(StrContains(szMsg, "FIRST BLOOD", false) != -1)
    {
        CBlackMesaPlayer pPlayer = DHookGetParam(hParams, 2);
        char szPlayerName[MAX_NAME_LENGTH];
        pPlayer.GetName(szPlayerName, sizeof(szPlayerName));

        PrintToChatAll("%t", "#Necro_FIRST_BLOOD", szPlayerName);
        return MRES_Supercede;
    }

    if(StrContains(szMsg, "KILLING SPREE", false) != -1)
    {
        CBlackMesaPlayer pPlayer = DHookGetParam(hParams, 2);
        char szPlayerName[MAX_NAME_LENGTH];
        pPlayer.GetName(szPlayerName, sizeof(szPlayerName));

        PrintToChatAll("%t", "#Necro_KillingSpree", szPlayerName);
        return MRES_Supercede;
    }

    if(StrContains(szMsg, "RAMPAGE", false) != -1)
    {
        CBlackMesaPlayer pPlayer = DHookGetParam(hParams, 2);
        char szPlayerName[MAX_NAME_LENGTH];
        pPlayer.GetName(szPlayerName, sizeof(szPlayerName));

        PrintToChatAll("%t", "#Necro_RAMPAGE", szPlayerName);
        return MRES_Supercede;
    }

    if(StrContains(szMsg, "DOMINATING", false) != -1)
    {
        CBlackMesaPlayer pPlayer = DHookGetParam(hParams, 2);
        char szPlayerName[MAX_NAME_LENGTH];
        pPlayer.GetName(szPlayerName, sizeof(szPlayerName));

        PrintToChatAll("%t", "#Necro_DOMINATING", szPlayerName);
        return MRES_Supercede;
    }

    if(StrContains(szMsg, "UNSTOPPABLE", false) != -1)
    {
        CBlackMesaPlayer pPlayer = DHookGetParam(hParams, 2);
        char szPlayerName[MAX_NAME_LENGTH];
        pPlayer.GetName(szPlayerName, sizeof(szPlayerName));

        PrintToChatAll("%t", "#Necro_UNSTOPPABLE", szPlayerName);
        return MRES_Supercede;
    }

    if(StrContains(szMsg, "GODLIKE", false) != -1)
    {
        CBlackMesaPlayer pPlayer = DHookGetParam(hParams, 2);
        char szPlayerName[MAX_NAME_LENGTH];
        pPlayer.GetName(szPlayerName, sizeof(szPlayerName));

        PrintToChatAll("%t", "#Necro_GODLIKE", szPlayerName);
        return MRES_Supercede;
    }

    if(StrContains(szMsg, "WICKED SICK", false) != -1)
    {
        CBlackMesaPlayer pPlayer = DHookGetParam(hParams, 2);
        char szPlayerName[MAX_NAME_LENGTH];
        pPlayer.GetName(szPlayerName, sizeof(szPlayerName));

        PrintToChatAll("%t", "#Necro_WICKED_SICK", szPlayerName);
        return MRES_Supercede;
    }

    return MRES_Ignored;
}