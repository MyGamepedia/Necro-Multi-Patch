#pragma newdecls required
#pragma semicolon 1

methodmap CBlackMesaPlayer < CBasePlayer
{
	public CBlackMesaPlayer(const int iEntIndex = -1)
	{
		return view_as<CBlackMesaPlayer>(CBasePlayer(iEntIndex));
	}
	
	public bool HasJumpModule()
	{
		return GetEntProp(this.entindex, Prop_Send, "m_bHasJumpModule") != 0;
	}
	public void SetJumpModule(const bool bShouldHaveJumpModule)
	{
		SetEntProp(this.entindex, Prop_Send, "m_bHasJumpModule", bShouldHaveJumpModule);
	}
	public void PerformForcedWeaponDeploy(const char[] szWeaponClassname)
	{
		this.AcceptInputStr("PerformForcedWeaponDeploy", szWeaponClassname);
	}
	public float GetJumpModuleMana()
	{
		return GetEntPropFloat(this.entindex, Prop_Send, "m_flLongJumpMana");
	}
	public float SetJumpModuleMana(const float flManaCost)
	{
		SetEntPropFloat(this.entindex, Prop_Send, "m_flLongJumpMana", flManaCost);
	}
	public bool IsSpectator()
	{
		int iTeam = this.GetTeam();

		if((CMultiplayRules.IsTeamplay() && (iTeam == 1 || iTeam == 0)) || iTeam == 1)
			return true;

		return false;
	}
	public bool IsInProtection(ConVar pProtectionAlphaConVar)
	{
		if (!this.IsSpectator() && this.GetRenderMode() == RENDER_TRANSCOLOR && this.GetRenderAlpha() == pProtectionAlphaConVar.IntValue)
			return true;

		return false;
	}
}
