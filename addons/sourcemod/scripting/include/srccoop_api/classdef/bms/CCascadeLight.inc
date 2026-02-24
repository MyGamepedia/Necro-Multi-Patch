#pragma newdecls required
#pragma semicolon 1

methodmap CCascadeLight < CBaseEntity
{
	public CCascadeLight(const int iEntIndex = -1)
	{
		return view_as<CCascadeLight>(CBaseEntity(iEntIndex));
	}
	
	public static CCascadeLight Create()
	{
		return CCascadeLight(CreateEntityByName("env_cascade_light"));
	}
	
	public float GetLightRadius(int iIndex)
	{
        iIndex = Clamp(iIndex, 1, 3);
		switch (iIndex)
		{
			case 1: return GetEntPropFloat(this.entindex, Prop_Send, "m_fLightRadius1");
			case 2: return GetEntPropFloat(this.entindex, Prop_Send, "m_fLightRadius2");
			case 3: return GetEntPropFloat(this.entindex, Prop_Send, "m_fLightRadius3");
		}
	}

    public void SetLightRadius(int iIndex, const float fValue)
	{
        iIndex = Clamp(iIndex, 1, 3);
		switch (iIndex)
		{
			case 1: SetEntPropFloat(this.entindex, Prop_Send, "m_fLightRadius1", fValue);
			case 2: SetEntPropFloat(this.entindex, Prop_Send, "m_fLightRadius2", fValue);
			case 3: SetEntPropFloat(this.entindex, Prop_Send, "m_fLightRadius3", fValue);
		}
	}

    public float GetDepthbias(int iIndex)
	{
        iIndex = Clamp(iIndex, 1, 3);
		switch (iIndex)
		{
			case 1: return GetEntPropFloat(this.entindex, Prop_Send, "m_fDepthbias1");
			case 2: return GetEntPropFloat(this.entindex, Prop_Send, "m_fDepthbias2");
			case 3: return GetEntPropFloat(this.entindex, Prop_Send, "m_fDepthbias3");
		}
	}

    public void SetDepthbias(int iIndex, const float fValue)
	{
        iIndex = Clamp(iIndex, 1, 3);
		switch (iIndex)
		{
			case 1: SetEntPropFloat(this.entindex, Prop_Send, "m_fDepthbias1", fValue);
			case 2: SetEntPropFloat(this.entindex, Prop_Send, "m_fDepthbias2", fValue);
			case 3: SetEntPropFloat(this.entindex, Prop_Send, "m_fDepthbias3", fValue);
		}
	}

    public float GetSlopescaledepthbias(int iIndex)
	{
        iIndex = Clamp(iIndex, 1, 3);
		switch (iIndex)
		{
			case 1: return GetEntPropFloat(this.entindex, Prop_Send, "m_fSlopescaledepthbias1");
			case 2: return GetEntPropFloat(this.entindex, Prop_Send, "m_fSlopescaledepthbias2");
			case 3: return GetEntPropFloat(this.entindex, Prop_Send, "m_fSlopescaledepthbias3");
		}
	}

    public void SetSlopescaledepthbias(int iIndex, const float fValue)
	{
        iIndex = Clamp(iIndex, 1, 3);
		switch (iIndex)
		{
			case 1: SetEntPropFloat(this.entindex, Prop_Send, "m_fSlopescaledepthbias1", fValue);
			case 2: SetEntPropFloat(this.entindex, Prop_Send, "m_fSlopescaledepthbias2", fValue);
			case 3: SetEntPropFloat(this.entindex, Prop_Send, "m_fSlopescaledepthbias3", fValue);
		}
	}
	
	public void SetEnabled(const bool bEnabled)
	{
		SetEntProp(this.entindex, Prop_Send, "m_bEnabled", bEnabled);
	}

    public bool IsEnabled()
    {
        return GetEntProp(this.entindex, Prop_Send, "m_bEnabled") != 0;
    }

    public void SetViewModelDepthbias(const float fValue)
    {
        SetEntPropFloat(this.entindex, Prop_Send, "m_fViewModelDepthbias", fValue);
    }

    public float GetViewModelDepthbias()
    {
        return GetEntPropFloat(this.entindex, Prop_Send, "m_fViewModelDepthbias");
    }

    public void SetViewModelSlopescaledepthbias(const float fValue)
    {
        SetEntPropFloat(this.entindex, Prop_Send, "m_fViewModelSlopescaledepthbias", fValue);
    }

    public float GetViewModelSlopescaledepthbias()
    {
        return GetEntPropFloat(this.entindex, Prop_Send, "m_fViewModelSlopescaledepthbias");
    }

    public void SetCSMVolumeManualMode(const bool bManualMode)
    {
        SetEntProp(this.entindex, Prop_Send, "m_bCSMVolumeManualMode", bManualMode);
    }

    public bool IsCSMVolumeManualMode()
    {
        return GetEntProp(this.entindex, Prop_Send, "m_bCSMVolumeManualMode") != 0;
    }

    public void SetStartDisabled(const bool bStartDisabled)
    {
        SetEntProp(this.entindex, Prop_Data, "m_bStartDisabled", bStartDisabled);
    }

    public bool IsStartDisabled()
    {
        return GetEntProp(this.entindex, Prop_Data, "m_bStartDisabled") != 0;
    }

    public bool GetUseLightEnvAngles()
    {
        return GetEntProp(this.entindex, Prop_Send, "m_bUseLightEnvAngles") != 0;
    }
    
    public void SetUseLightEnvAngles(const bool bUseLightEnvAngles)
    {
        SetEntProp(this.entindex, Prop_Send, "m_bUseLightEnvAngles", bUseLightEnvAngles);
    }

    public void SetMaxShadowDistance(const float fMaxShadowDistance)
    {
        char szBuffer[256];
		FloatToString(fMaxShadowDistance, szBuffer, sizeof(szBuffer));
        this.KeyValue("maxshadowdistance", szBuffer); //doesn't exists as field
    }

    public float GetMaxShadowDistance()
    {
        return GetEntPropFloat(this.entindex, Prop_Send, "m_fLightRadius2"); //seems to be stored here
    }

    public void GetLightColor(int vecBuffer[3])
    {
        int iColorVal = GetEntProp(this.entindex, Prop_Send, "m_LightColor");
            
        vecBuffer[0] = (iColorVal & 0xFF); // red
        vecBuffer[1] = ((iColorVal >> 8) & 0xFF); // green
        vecBuffer[2] = ((iColorVal >> 16) & 0xFF); // blue
    }

    public int GetLightColorInt()
    {
        return GetEntProp(this.entindex, Prop_Send, "m_LightColor");
    }

    public void SetLightColor(const int vecColor[3])
    {
        int iColorVal = (vecColor[0] & 0xFF) | ((vecColor[1] & 0xFF) << 8) | ((vecColor[2] & 0xFF) << 16);
        SetEntProp(this.entindex, Prop_Send, "m_LightColor", iColorVal);
    }

    public void SetLightColorInt(const int iLightColor)
    {
        SetEntProp(this.entindex, Prop_Send, "m_LightColor", iLightColor);
    }

    public int GetLightColorScale()
    {
        return GetEntProp(this.entindex, Prop_Send, "m_LightColorScale");
    }

    public void SetLightColorScale(const int iLightColorScale)
    {
        SetEntProp(this.entindex, Prop_Send, "m_LightColorScale", iLightColorScale);
    }
}