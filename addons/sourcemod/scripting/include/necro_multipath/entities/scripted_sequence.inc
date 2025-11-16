#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CAI_ScriptedSequence::StartScript
// Attempts to fix null reference on m_hTargetEnt inside CAI_ScriptedSequence::ScriptThink
// by preventing CAI_ScriptedSequence::StartScript from nullifying its own target entity.
//------------------------------------------------------
public MRESReturn Hook_ScriptedSequenceStartScript(int _this)
{
	CAI_ScriptedSequence pThis = CAI_ScriptedSequence(_this);
	CBaseEntity pTarget = pThis.GetTargetEnt();
	if (pTarget != NULL_CBASEENTITY && pTarget.IsNPC())
	{
		CAI_BaseNPC pTargetNpc = view_as<CAI_BaseNPC>(pTarget);
		if (pThis == pTargetNpc.GetCine())
		{
			pTargetNpc.SetCine(NULL_CBASEENTITY);
		}
	}
	return MRES_Ignored;
}