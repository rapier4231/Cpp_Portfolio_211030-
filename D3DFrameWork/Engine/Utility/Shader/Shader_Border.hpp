float			g_Alpha;
float			g_fDrawCutRatio;
int				g_iDrawCutUp;

vector			g_EdgeSize;

float			g_fOrder;

matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_OrderTex;

sampler			OrderTexSampler = sampler_state
{
	texture = g_OrderTex;
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

	//胶农费 力绢
	if (1 == g_iDrawCutUp)
	{
		if (g_fDrawCutRatio > In.vTexUV.g)
		{
			Out.vColor.a = 0.f;
			return Out;
		}
	}
	else if (2 == g_iDrawCutUp)
	{
		if ((1.f - g_fDrawCutRatio) < In.vTexUV.g)
		{
			Out.vColor.a = 0.f;
			return Out;
		}
	}
	/////////////////////////////////////

	if (g_EdgeSize.r >= In.vTexUV.r || g_EdgeSize.g >= In.vTexUV.g || g_EdgeSize.b <= In.vTexUV.r || g_EdgeSize.a <= In.vTexUV.g)
	{
		Out.vColor.r = 0.2f;
		Out.vColor.g = 1.f;
		Out.vColor.b = 1.f;
		Out.vColor.a = g_Alpha;
	}
	else
	{
		Out.vColor = (0.f, 0.f, 0.f, 0.f);
	}

	return Out;
}


PS_OUT PS_Making(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	//胶农费 力绢
	if (1 == g_iDrawCutUp)
	{
		if (g_fDrawCutRatio > In.vTexUV.g)
		{
			Out.vColor.a = 0.f;
			return Out;
		}
	}
	else if (2 == g_iDrawCutUp)
	{
		if ((1.f - g_fDrawCutRatio) < In.vTexUV.g)
		{
			Out.vColor.a = 0.f;
			return Out;
		}
	}
	/////////////////////////////////////

	float2 FixUV = In.vTexUV;
	FixUV.r = In.vTexUV.r * 1.3f - 0.13f;
	FixUV.g = In.vTexUV.g * 1.3f - 0.13f;
	Out.vColor = tex2D(OrderTexSampler, FixUV);


	if (g_EdgeSize.r >= In.vTexUV.r || g_EdgeSize.g >= In.vTexUV.g || g_EdgeSize.b <= In.vTexUV.r || g_EdgeSize.a <= In.vTexUV.g)
	{
		if (In.vTexUV.r > g_fOrder && g_EdgeSize.a <= In.vTexUV.g)
		{
			Out.vColor = (0.f, 0.f, 0.f, 0.f);
			return Out;
		}
		Out.vColor.r = 0.2f;
		Out.vColor.g = 1.f;
		Out.vColor.b = 1.f;
		Out.vColor.a = g_Alpha;
		return Out;
	}
	else
	{
		if (Out.vColor.a)
		{
			Out.vColor.a = Out.vColor.a*0.7f;
			return Out;
		}
		else
		{
			Out.vColor = (0.f, 0.f, 0.f, 0.f);
			return Out;
		}
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
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	Making
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_Making();
	}
}

/////////////////////////////////////////////////