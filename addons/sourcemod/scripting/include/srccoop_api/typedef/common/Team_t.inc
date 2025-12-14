#pragma newdecls required
#pragma semicolon 1

enum
{
	TEAM_ANY = -2, //auto assign
	TEAM_INVALID = -1, //invalid team
	TEAM_UNASSIGNED = 0, //unassigned team (dm mode) or spectator (teamplay)
	TEAM_SPECTATOR = 1, //spectator
	TEAM_ONE = 2, // Marine, Combine
	TEAM_TWO = 3, // Scientist, Rebels
}
