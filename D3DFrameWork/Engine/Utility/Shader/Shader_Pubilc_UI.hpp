float			g_Alpha;
float			g_Ratio_SizeX;
float			g_Ratio_SizeY;
vector			g_vecColor;

matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_Bg_Tex;

sampler			BgSampler = sampler_state
{
	texture = g_Bg_Tex;
	AddressU = clamp;
	AddressV = clamp;
	minfilter = linear;
	magfilter = linear;
};


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
	float2 FixUV = In.vTexUV;
	FixUV.r = In.vTexUV.r * g_Ratio_SizeX + 0.05f;
	FixUV.g = In.vTexUV.g * g_Ratio_SizeY + 0.05f;
	Out.vColor = tex2D(BgSampler, FixUV);

	Out.vColor.a = g_Alpha;
	return Out;
}

PS_OUT PS_Draw(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	Out.vColor.r = g_vecColor.r;
	Out.vColor.g = g_vecColor.g;
	Out.vColor.b = g_vecColor.b;
	Out.vColor.a = g_Alpha;
	return Out;
}

PS_OUT PS_NamalAlphaTex(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	Out.vColor = tex2D(BgSampler, In.vTexUV);

	if (Out.vColor.a > 0.1f)
		Out.vColor.a = g_Alpha;

	return Out;
}

PS_OUT PS_NomalAlphaColorTex(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	Out.vColor = tex2D(BgSampler, In.vTexUV);

	if (Out.vColor.a > 0.1f)
		Out.vColor.a = g_Alpha;
	else 
		return Out;

	if (Out.vColor.r || Out.vColor.g || Out.vColor.b)
	{
		Out.vColor.r = g_vecColor.r;
		Out.vColor.g = g_vecColor.g;
		Out.vColor.b = g_vecColor.b;
	}
	return Out;
}

PS_OUT PS_NamalMoveTex(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	Out.vColor = tex2D(BgSampler, In.vTexUV);

	if (Out.vColor.a > 0.1f)
		Out.vColor.a = g_Alpha;

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
		pixelshader = compile ps_3_0 PS_Draw();
	}

		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_NamalAlphaTex();
	}

		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_NomalAlphaColorTex();
	}

		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_NamalMoveTex();
	}
}

/////////////////////////////////////////////////