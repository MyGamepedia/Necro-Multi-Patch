//Purpose: Prevent setting invalid values for host_timescale convar so the admin (lets not point) don't break the server again
public void OnHostTimeScaleChanged(ConVar convar, const char[] oldValue, const char[] newValue)
{
	float flNew = StringToFloat(newValue);

    // Block zero or negative values
    if (flNew <= 0.0)
    {
        convar.SetString(oldValue);
    }
}