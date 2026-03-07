#pragma newdecls required
#pragma semicolon 1

public void Hornet_PathPost(const int iEntIndex)
{
    CBlackMesaBaseDetonator pHornet = CBlackMesaBaseDetonator(iEntIndex);
    if (!pHornet.IsValid())
        return;

    CBlackMesaPlayer pPlayer = CBlackMesaPlayer(GetEntPropEnt(pHornet.entindex, Prop_Data, "m_hOwnerEntity"));
    if (!pPlayer.IsValid())
        return;

    pHornet.SetOriginalThrower(pPlayer); //?doesn't seem to set for hornets by the game itself?

    //trigger the first hornet explosion and clean from the array
    if (g_hClientHornets[pPlayer.entindex][MAX_HORNETS - 1] != 0)
    {
        CBlackMesaBaseDetonator pOldHornet = CBlackMesaBaseDetonator(g_hClientHornets[pPlayer.entindex][MAX_HORNETS - 1]);

        if (pOldHornet.IsValid())
        {
            pOldHornet.Kill(); //NOTE: this is good enough for the goal, but but ideally it should explode, unfortunately calling methods didn't work for me
        }
    }

    //shift to right
    for (int i = MAX_HORNETS - 1; i > 0; i--)
    {
        int oldEnt = g_hClientHornets[pPlayer.entindex][i - 1];
        g_hClientHornets[pPlayer.entindex][i] = oldEnt;

        if (oldEnt != 0)
        {
            CBlackMesaBaseDetonator pOld = CBlackMesaBaseDetonator(oldEnt);
            if (pOld.IsValid())
                pOld.SetUserData("m_iHornetNum", i);
        }
    }

    //add new
    g_hClientHornets[pPlayer.entindex][0] = pHornet.entindex;
    pHornet.SetUserData("m_iHornetNum", 0);
}

MRESReturn Hook_HornetUpdateOnRemove(int _this)
{
	CBlackMesaBaseDetonator pHornet = CBlackMesaBaseDetonator(_this);

	CBlackMesaPlayer pPlayer = pHornet.GetOriginalThrower();
	if (!pPlayer.IsValid())
		return MRES_Ignored;

	int iHornetNum = pHornet.GetUserData("m_iHornetNum");

	if (iHornetNum < 0 || iHornetNum >= MAX_HORNETS)
		return MRES_Ignored;

	//shift to left
	for (int i = iHornetNum; i < MAX_HORNETS - 1; i++)
	{
		int nextEnt = g_hClientHornets[pPlayer.entindex][i + 1];
		g_hClientHornets[pPlayer.entindex][i] = nextEnt;

		if (nextEnt != 0)
		{
			CBlackMesaBaseDetonator pNextHornet = CBlackMesaBaseDetonator(nextEnt);
			if (pNextHornet.IsValid())
			{
				pNextHornet.SetUserData("m_iHornetNum", i);
			}
		}
	}

	//clean the last element
	g_hClientHornets[pPlayer.entindex][MAX_HORNETS - 1] = 0;

    return MRES_Ignored;
}