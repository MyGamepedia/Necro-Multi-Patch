#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// env_explosion
// fix !player ignore entity
//------------------------------------------------------
public void Hook_ExplosionSpawn(int iEntIndex)
{
	char buffer[MAX_VALUE];
	GetEntPropString(iEntIndex, Prop_Data, "m_strEntityNameToIgnore", buffer, sizeof(buffer)); // this is entity handle m_hEntityIgnore in other games
	if (StrEqual(buffer, "!player", false))
	{
		SetEntPropString(iEntIndex, Prop_Data, "m_strEntityNameToIgnore", "");
		SetEntProp(iEntIndex, Prop_Data, "m_iClassIgnore", CLASS_PLAYER);
	}
}