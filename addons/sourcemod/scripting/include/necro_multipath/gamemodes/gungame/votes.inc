#pragma newdecls required
#pragma semicolon 1
 
public Action GunGameStartVote(Handle timer, int iPlayerIndex)
{
	Menu gungamemenu = new Menu(GunGameVoteMenuHandler, MenuAction_DisplayItem | MenuAction_Display);

    gungamemenu.SetTitle("%t", "#GunGame_VoteMenu");

    //TODO: replace with methodmap, something like "MenuTranslateable" 
    char iszGungamemenutext[64];

    if(g_iNecroGunGameCurrentState == -1)
    {
        Format(iszGungamemenutext, sizeof(iszGungamemenutext), "%t", "#GunGame_Enable");
        gungamemenu.AddItem("GunGameVote_Enable", iszGungamemenutext);
    }

    else
    {
        Format(iszGungamemenutext, sizeof(iszGungamemenutext), "%t", "#GunGame_Disable");
        gungamemenu.AddItem("GunGameVote_Disable", iszGungamemenutext);
    }

    Format(iszGungamemenutext, sizeof(iszGungamemenutext), "%t", "#GunGame_Skip");
    gungamemenu.AddItem("GunGameVote_Skip", iszGungamemenutext);

    gungamemenu.ExitButton = false;

    gungamemenu.DisplayVoteToAll(20);

    return Plugin_Stop;
}

public void GunGameVoteMenuHandler(Menu menu, MenuAction action, int iParam1, int iParam2)
{
    switch(action)
    {
        case MenuAction_Select:
        {
            char iszOption[32];
            menu.GetItem(iParam2, iszOption, sizeof(iszOption));

            if(StrEqual(iszOption, "GunGameVote_Enable"))
            {
                g_iGunGameVoteEnable++;
            }

            if(StrEqual(iszOption, "GunGameVote_Disable"))
            {
                g_iGunGameVoteDisable++;
            }

            if(StrEqual(iszOption, "GunGameVote_Skip"))
            {
                g_iGunGameVoteSkip++;
            }

        }

        case MenuAction_End:
        {
            //most players want to make it enabled
            if((g_iGunGameVoteEnable > g_iGunGameVoteDisable) && (g_iGunGameVoteEnable > g_iGunGameVoteSkip))
            {
                PrintToChatAll("%t", "#GunGame_VoteEndEnable");
                SetConVarBool(g_ConvarNecroGunGameEnabled, true);

                CreateTimer(4.0, GunGameVoteNextMap);
            }

            //most players want to disable it
            else if((g_iGunGameVoteDisable > g_iGunGameVoteEnable) && (g_iGunGameVoteDisable > g_iGunGameVoteSkip))
            {
                PrintToChatAll("%t", "#GunGame_VoteEndDisable");
                SetConVarBool(g_ConvarNecroGunGameEnabled, false);

                CreateTimer(4.0, GunGameVoteNextMap);
            }

            //most players want to skip the vote
            else if((g_iGunGameVoteSkip > g_iGunGameVoteEnable) && (g_iGunGameVoteSkip > g_iGunGameVoteDisable))
            {
                PrintToChatAll("%t", "#GunGame_VoteEndSkip");
            }

            //in case if 2 or 3 of these have the same amount of votes
            else
            {
                PrintToChatAll("%t", "#GunGame_VoteEndFail");
            }

            g_iGunGameVoteEnable = 0;
            g_iGunGameVoteDisable = 0;
            g_iGunGameVoteSkip = 0;

            delete menu;
        }
    }
}

public void GunGameVoteNextMap(Handle timer)
{
    char iszCurrentMapName[MAX_MAPNAME];

    GetCurrentMap(iszCurrentMapName, sizeof(iszCurrentMapName));

    ForceChangeLevel(iszCurrentMapName, "\"Gun Game\" vote.");
}

public Action Timer_GunGameServerMessage(Handle timer)
{
    PrintToChatAll("%t", "#GunGame_VoteInfo");
    return Plugin_Continue;
}