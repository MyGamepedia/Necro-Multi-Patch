public void OnHostTimeScaleChanged(ConVar convar, const char[] oldValue, const char[] newValue)
{
	float flNew = StringToFloat(newValue);

    // Block zero or negative values
    if (flNew <= 0.0)
    {
        convar.SetString(oldValue);
    }
}