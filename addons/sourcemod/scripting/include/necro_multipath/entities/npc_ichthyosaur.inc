#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CNPC_Ichthyosaur - npc_ichthyosaur
// multiplayer crashfix
//------------------------------------------------------
public MRESReturn Hook_IchthyosaurIdleSound(int _this)
{
	AddSinglePlayerOverride(CBaseEntity(_this));
	return MRES_Ignored;
}

public MRESReturn Hook_IchthyosaurIdleSoundPost(int _this)
{
	PopSinglePlayerOverride();
	return MRES_Ignored;
}