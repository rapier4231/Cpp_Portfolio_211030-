float			g_FilterPosY;
float			g_Alpha;
float			g_RatioX;
float			g_RatioY;

matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture		g_BaseTexture;
texture		g_FilterTexture;

sampler			BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

sampler			FilterSampler = sampler_state
{
	texture = g_FilterTexture;

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

// ¹öÅØ½º ½¦ÀÌ´õ
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

// ÇÈ¼¿ ½¦ÀÌ´õ ¸ÞÀÎ
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	float2		baseUV = In.vTexUV;
	baseUV.r = baseUV.r * g_RatioX;
	baseUV.g = baseUV.g * g_RatioY;
	vector		vBase = tex2D(BaseSampler, baseUV);

	vector		vFilter = tex2D(FilterSampler, baseUV);
	vFilter.g = vFilter.r;
	vFilter.b = vFilter.r;
	if (baseUV.g <= (g_FilterPosY + 0.01f) && baseUV.g >= (g_FilterPosY - 0.01f))
	{
		Out.vColor = (vBase*0.9f)+(vFilter); 
		Out.vColor.a = g_Alpha;
	}
	else if (baseUV.g <= (g_FilterPosY + 0.03f) && baseUV.g >= (g_FilterPosY - 0.03f))
	{
		Out.vColor = (vBase*0.9f) + (vFilter * 0.8f); 
		Out.vColor.a = g_Alpha;
	}
	else if (baseUV.g <= (g_FilterPosY + 0.05f) && baseUV.g >= (g_FilterPosY - 0.05f))
	{
		Out.vColor = (vBase*0.9f) + (vFilter * 0.6f);
		Out.vColor.a = g_Alpha;
	}
	else
	{
		Out.vColor = (vBase*0.9f);
		Out.vColor.a = g_Alpha;
	}

	return Out;
}

// Å×Å©´ÏÄü
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
}

/////////////////////////////////////////////////