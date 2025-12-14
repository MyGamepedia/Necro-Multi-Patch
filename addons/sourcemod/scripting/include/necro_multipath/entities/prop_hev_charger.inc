#pragma newdecls required
#pragma semicolon 1

//------------------------------------------------------
// CPropHevCharger - prop_hev_charger
//
// # TODO
//
// - Set "m_bChargingPlayers" on charger to apply screen space effects (currently this will apply to all players)
// - Apply laser particle effect on each individual player within radius
// - Play active sound
//------------------------------------------------------
public MRESReturn Hook_PropHevChargerThink(int _this)
{
	CPropChargerBase pCharger = CPropChargerBase(_this);

	//float flArmorCharge = (pCharger.GetChargerRate() / pCharger.GetChargerAmount()) * GetGameFrameTime();
	
	float flChargerRange = pCharger.GetChargerRange();
	float vec3EntityPosition[3];
	pCharger.GetAbsOrigin(vec3EntityPosition);

	for (int i = 1; i <= MaxClients; ++i)
	{
		CBlackMesaPlayer pPlayer = CBlackMesaPlayer(i);
		if (pPlayer != NULL_CBASEENTITY && pPlayer.IsAlive())
		{
			float vec3PlayerPosition[3];
			pPlayer.GetAbsOrigin(vec3PlayerPosition);

			if (GetVectorDistance(vec3EntityPosition, vec3PlayerPosition) <= flChargerRange)
			{
				// Naive reimplementation of `CPropHevCharger::ShouldApplyEffect` and `CPropHevCharger::ApplyEffect`.
				// The original implementation uses frametime while thinking every 0.1 seconds.
				// As a result, the value will always increment by 1 unless
				// `m_flChargerRate` is set to a extremely high value or `m_flChargerAmt` is set to a extremely low value.
				// The original implementation also will overflow the max amount if the increment value is over `1`.
				int iArmor = pPlayer.GetArmor();
				if (pPlayer.HasSuit() && iArmor < 100)
				{
					pPlayer.SetArmor(iArmor + 1);
					//pPlayer.SendCommand("r_screenoverlay effects/xen_charger_hev_overlay");
				}
			}
		}
	}
	
	pCharger.SetNextThink(GetGameTime() + 0.1);

	return MRES_Supercede;
}