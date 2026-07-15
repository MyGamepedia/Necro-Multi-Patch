#pragma newdecls required
#pragma semicolon 1

methodmap CMovementSpeedMod < CPointEntity
{
	public CMovementSpeedMod(const int iEntIndex = -1)
	{
		return view_as<CMovementSpeedMod>(CPointEntity(iEntIndex));
	}
	public static CMovementSpeedMod Create()
	{
		return CMovementSpeedMod(CreateEntityByName("player_speedmod"));
	}
	
	public void ModifySpeed(const float flSpeedMultiplier)
	{
        SetVariantFloat(flSpeedMultiplier);
		AcceptEntityInput(this, "ModifySpeed");
	}
}
