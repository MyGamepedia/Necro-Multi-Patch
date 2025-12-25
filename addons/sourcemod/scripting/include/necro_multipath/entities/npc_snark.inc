#pragma newdecls required
#pragma semicolon 1

public void Snark_PathPost(const int iEntIndex)
{
    CNpc_Snark pSnark = CNpc_Snark(iEntIndex);
    if (!pSnark.IsValid())
        return;

    CBlackMesaPlayer pPlayer = CBlackMesaPlayer(pSnark.GetOwner());
    if (!pPlayer.IsValid())
    return;

    pSnark.SetSnarkOwner(pPlayer.entindex); //store here if we lost owner, because it does somehow, right before it should be removed, idk why
    
    if (g_hClientSnarks[pPlayer.entindex][19] != 0)
    {
        CNpc_Snark pOldSnark = CNpc_Snark(g_hClientSnarks[pPlayer.entindex][19]);
        if (pOldSnark.IsValid())
        {
            g_hClientSnarks[pPlayer.entindex][19] = 0;
            pOldSnark.AcceptInputInt("SetHealth", 0);
        }
    }

    for (int i = 19; i > 0; i--)
    {
        int iOldEnt = g_hClientSnarks[pPlayer.entindex][i - 1];
        g_hClientSnarks[pPlayer.entindex][i] = iOldEnt;

        if (iOldEnt != 0)
        {
            CNpc_Snark pOldSnark = CNpc_Snark(iOldEnt);
            if (pOldSnark.IsValid())
                pOldSnark.SetUserData("m_iSnarkNum", i);
        }
    }

    g_hClientSnarks[pPlayer.entindex][0] = pSnark.entindex;
}