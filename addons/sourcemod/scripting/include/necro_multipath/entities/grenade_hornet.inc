#pragma newdecls required
#pragma semicolon 1

//NOTE:All the code in this file is not used right now due to tick and expls issues
/*
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
    if (g_hClientHornets[pPlayer.entindex][19] != 0)
    {
        CBlackMesaBaseDetonator pOldHornet =
            CBlackMesaBaseDetonator(g_hClientHornets[pPlayer.entindex][19]);

        if (pOldHornet.IsValid())
        {
            PrintToChatAll("Removing");
            pOldHornet.SetDetonateTimer(-1.0);
        }
    }

    //shift to right
    for (int i = 19; i > 0; i--)
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

public void Hook_Hornet_OnDeleted(const CBlackMesaBaseDetonator pHornet)
{    
	CBlackMesaPlayer pPlayer = pHornet.GetOriginalThrower();
	if (!pPlayer.IsValid())
		return;

	int iHornetNum = pHornet.GetUserData("m_iHornetNum");

	if (iHornetNum < 0 || iHornetNum >= 20)
		return;

	//shift to left
	for (int i = iHornetNum; i < 19; i++)
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
	g_hClientHornets[pPlayer.entindex][19] = 0;
}
*/