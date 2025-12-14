public Action Listener_Chooseteam(int client, const char[] command, int args)
{
    if(!g_iTeamplay)
    {
        FakeClientCommandEx(GetClientUserId(client), "spawnchooseteam");
        return Plugin_Handled;
    }

    return Plugin_Continue;
}