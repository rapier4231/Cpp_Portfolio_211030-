matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	if (Out.r)
	{
		Out.r = 0.f;
	}

	return Out;
}

technique		Default_Device
{
	pass
	{		
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = true;
		pixelshader = compile ps_3_0 PS_MAIN();
	}
}

/////////////////////////////////////////////////