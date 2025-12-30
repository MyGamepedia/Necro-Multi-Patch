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
    
    //trigger the first snark explosion and clean from the array
    if (g_hClientSnarks[pPlayer.entindex][19] != 0)
    {
        CNpc_Snark pOldSnark = CNpc_Snark(g_hClientSnarks[pPlayer.entindex][19]);
        if (pOldSnark.IsValid())
        {
            pOldSnark.AcceptInputInt("SetHealth", 0);
        }

        g_hClientSnarks[pPlayer.entindex][19] = 0;
    }

    //shift to right everyone
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

    //add new snark
    g_hClientSnarks[pPlayer.entindex][0] = pSnark.entindex;
    pSnark.SetUserData("m_iSnarkNum", 0);

    //snark appears in owners head for a while, fix it by hidding snark for a while
    pSnark.SetRenderMode(RENDER_NONE);
    pSnark.AcceptInput("DisableShadow");
    pSnark.AddOutput("OnUser4", "!self", "Addoutput", "rendermode 0", 0.02, 1);
    pSnark.AddOutput("OnUser4", "!self", "Addoutput", "EnableShadow", 0.02, 1);
    pSnark.FireOutput("OnUser4");
}

public void Hook_Snark_OnDeleted(CNpc_Snark pSnark)
{
	CBlackMesaPlayer pPlayer = CBlackMesaPlayer(pSnark.GetSnarkOwner());
	if (!pPlayer.IsValid())
		return;

	int iSnarkNum = pSnark.GetUserData("m_iSnarkNum");

	if (iSnarkNum < 0 || iSnarkNum >= 20)
		return;

	//shift to left
	for (int i = iSnarkNum; i < 19; i++)
	{
		int nextEnt = g_hClientSnarks[pPlayer.entindex][i + 1];
		g_hClientSnarks[pPlayer.entindex][i] = nextEnt;

		if (nextEnt != 0)
		{
			CNpc_Snark pNextSnark = CNpc_Snark(nextEnt);
			if (pNextSnark.IsValid())
			{
				pNextSnark.SetUserData("m_iSnarkNum", i);
			}
		}
	}

	//clean the last element
	g_hClientSnarks[pPlayer.entindex][19] = 0;
}