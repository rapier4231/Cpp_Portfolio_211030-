texture		g_ShadowTexture;
texture		g_BlurXTexture;
texture		g_BlurYTexture;

sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;

	AddressU = clamp;
	AddressV = clamp;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

sampler BlurXSampler = sampler_state
{
	texture = g_BlurXTexture;

	AddressU = clamp;
	AddressV = clamp;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};


sampler BlurYSampler = sampler_state
{
	texture = g_BlurYTexture;

	AddressU = clamp;
	AddressV = clamp;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

float m_TexW = 300.f;
float m_TexH = 300.f;

static const float Weight[13] =
{
	0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f, 1.f, 0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f
};

static const float Total = 6.2108f;

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vBlurX : COLOR0;
	vector		vBlurY : COLOR1;
};

PS_OUT		PS_BLURX(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	if (0.9f < tex2D(ShadowSampler, In.vTexUV).r)
	{
		Out.vBlurX = vector(1.f, 1.f, 1.f, 1.f);

		return Out;
	}

	// Blur X
	float4	vBlurX = 0;

	float2	tX = In.vTexUV;
	float2	uvX = 0;

	float	tuX = 1.f / m_TexW;

	for (int i = -6; i < 6; ++i)
	{
		uvX = tX + float2(tuX * i, 0);
		vBlurX += Weight[6 + i] * tex2D(ShadowSampler, uvX);
	}

	vBlurX /= Total;

	Out.vBlurX = vBlurX;

	return Out;
}

PS_OUT		PS_BLURY(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vBlurX = tex2D(BlurXSampler, In.vTexUV);

	// Blur Y
	float4	vBlurY = 0;

	float2	tY = In.vTexUV;
	float2	uvY = 0;

	float	tuY = 1.f / m_TexH;

	for (int i = -6; i < 6; ++i)
	{
		uvY = tY + float2(0, tuY * i);
		vBlurY += Weight[6 + i] * tex2D(BlurXSampler, uvY);
	}

	vBlurY /= Total;

	Out.vBlurY = vBlurY;
	Out.vBlurY.a = 1.f;

	return Out;
}

PS_OUT		PS_BLURYACC(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vBlurX = tex2D(BlurXSampler, In.vTexUV);

	// Blur Y
	float4	vBlurY = 0;

	float2	tY = In.vTexUV;
	float2	uvY = 0;

	float	tuY = 1.f / m_TexH;

	for (int j = -6; j < 6; ++j)
	{
		uvY = tY + float2(0, tuY * j);
		vBlurY += Weight[6 + j] * tex2D(BlurYSampler, uvY);
	}

	vBlurY /= Total;

	Out.vBlurY = vBlurY;
	Out.vBlurY.a = 1.f;

	return Out;
}

technique		Default_Device
{
	pass // Pass 0 : Blur X
	{
		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_BLURX();
	}

	pass // Pass 1 : Blur Y
	{
		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_BLURY();
	}

	pass // Pass 2 : Blur Y Accumulate
	{
		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_BLURYACC();
	}
}