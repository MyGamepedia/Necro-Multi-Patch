public MRESReturn Hook_FAllowFlashlight(DHookReturn hReturn, DHookParam hParams)
{
	DHookSetReturn(hReturn, mp_flashlight.BoolValue);
	return MRES_Supercede;
}