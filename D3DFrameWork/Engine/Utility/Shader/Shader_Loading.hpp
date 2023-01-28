float			g_fAlpha;

matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_Bg_Tex;
texture			g_Bg2_Tex;

sampler			BgSampler = sampler_state
{
	texture = g_Bg_Tex;
	AddressU = clamp;
	AddressV = clamp;
	minfilter = linear;
	magfilter = linear;
};
sampler			Bg2Sampler = sampler_state
{
	texture = g_Bg2_Tex;
	AddressU = clamp;
	AddressV = clamp;
	minfilter = linear;
	magfilter = linear;
};

static const float W[13] = { 0.0561,0.1353,0.278,0.4868,0.7261,0.9231,1,0.9231 ,0.7261 ,0.4868 ,0.278 ,0.1353 ,0.0561 };
static const float Total = 6.2108;

float fWinCX = 800.f;
float fWinCY = 600.f;

struct VS_IN
{
	vector			vPosition : POSITION;
	float2			vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector			vPosition : POSITION;
	float2			vTexUV : TEXCOORD0;
};

VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(In.vPosition, matWVP);
	Out.vTexUV = In.vTexUV;
	return Out;
}

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
	Out.vColor = tex2D(BgSampler, In.vTexUV);
	//float2 t = In.vTexUV;
	//float2 uv = 0;
	//float tu = 1.f / fWinCX;
	//vector Tex2D;
	//for (int i = -6; i < 6; ++i)
	//{
	//	uv = t + float2(tu * i, 0);
	//	Tex2D = tex2D(BgSampler, uv);
	//	Out.vColor.r += W[6 + i] * Tex2D.r;
	//	Out.vColor.g += W[6 + i] * Tex2D.g;
	//	Out.vColor.b += W[6 + i] * Tex2D.b;
	//}
	//Out.vColor.r /= Total;
	//Out.vColor.g /= Total;
	//Out.vColor.b /= Total;
	/////////Y 블러
	//uv = 0;
	//tu = 1.f / fWinCY;
	//for (int i = -6; i < 6; ++i)
	//{
	//	uv = t + float2(0, tu * i);
	//	Tex2D = tex2D(BgSampler, uv);
	//	Out.vColor.r += W[6 + i] * Tex2D.r;
	//	Out.vColor.g += W[6 + i] * Tex2D.g;
	//	Out.vColor.b += W[6 + i] * Tex2D.b;
	//}
	//Out.vColor.r /= Total;
	//Out.vColor.g /= Total;
	//Out.vColor.b /= Total;
	//Out.vColor.a = 1.f;
	return Out;
}

PS_OUT PS_PlayerBlood(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	////g_fSpread는 0~0.5f 까지 줌.
	//if(g_fSpread < In.vTexUV.r && 0.5f > In.vTexUV.r)
	//	return Out;
	//if((1.f - g_fSpread) > In.vTexUV.r && 0.5f < In.vTexUV.r)
	//	return Out;

	//if (g_fSpread < In.vTexUV.g && 0.5f > In.vTexUV.g)
	//	return Out;

	//if ((1.f - g_fSpread) > In.vTexUV.g && 0.5f < In.vTexUV.g)
	//	return Out;

	Out.vColor = tex2D(BgSampler, In.vTexUV);
	if (0.f > Out.vColor.r)
	{
		Out.vColor.a = 0.f;
		return Out;
	}
	//if(0.5f > Out.vColor.a)
	//	return Out;

	Out.vColor.a = g_fAlpha;
	return Out;
}


PS_OUT PS_MiniMap(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	Out.vColor = tex2D(BgSampler, In.vTexUV);

	if(Out.vColor.a)
		Out.vColor.a = g_fAlpha;

	return Out;
}

PS_OUT PS_MiniMapBg(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	Out.vColor = tex2D(Bg2Sampler, In.vTexUV);

	if (Out.vColor.a)
		Out.vColor.a = g_fAlpha;

	return Out;
}

technique		Default_Device
{
		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_PlayerBlood();
	}

		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MiniMap();
	}

		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MiniMapBg();
	}
}

/////////////////////////////////////////////////