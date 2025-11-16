#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// Physics_RunThinkFunctions - The main physics simulation loop against all entities
// This hook is only active when there are no real players on the server!
// Its main purpose is to ensure NPCs can always find a valid player pointer when thinking. 
//------------------------------------------------------
public MRESReturn Hook_Physics_RunThinkFunctions(DHookParam hParams)
{
	return MRES_Supercede;
}