// 상수 테이블
texture		g_AlbedoTexture;
texture		g_ShadeTexture;
texture		g_SpecularTexture;
texture		g_BloomTexture;
texture		g_BlurTexture;
texture		g_BlendTexture;
texture		g_BlendBlurXTexture;
texture		g_BlendBlurYTexture;

bool	g_bBlurY = false;

sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;
};

sampler ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;
};


sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
};

sampler BloomSampler = sampler_state
{
	texture = g_BloomTexture;
};

sampler BlurSampler = sampler_state
{
	texture = g_BlurTexture;
};

sampler BlendSampler = sampler_state
{
	texture = g_BlendTexture;

	AddressU = clamp;
	AddressV = clamp;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

sampler BlendBlurXSampler = sampler_state
{
	texture = g_BlendBlurXTexture;

	AddressU = clamp;
	AddressV = clamp;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

sampler BlendBlurYSampler = sampler_state
{
	texture = g_BlendBlurYTexture;

AddressU = clamp;
AddressV = clamp;

MinFilter = LINEAR;
MagFilter = LINEAR;
MipFilter = LINEAR;
};

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
	vector		vBlurX : COLOR1;
	vector		vBlurY : COLOR2;
};

float3 ToneMapACES(float3 hdr)
{
	const float A = 2.51f, B = 0.03f, C = 2.43f, D = 0.59f, E = 0.14f;
	return saturate((hdr * (A * hdr + B)) / (hdr * (C * hdr + D) + E));
}

float3 Gamma_ForMonitor(float3 _rgb, float _f = 0.45454545f)
{
	return pow(_rgb, _f); // 0.45454545 = 1/2.2
}

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);
	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector		vBloom = tex2D(BloomSampler, In.vTexUV);
	vector		vBlur = tex2D(BlurSampler, In.vTexUV);

	vAlbedo.rgb = ToneMapACES(vAlbedo.rgb);
	vAlbedo.rgb = Gamma_ForMonitor(vAlbedo.rgb);

	Out.vColor = vAlbedo/* * vShade*/ /*+ vBlur*/;

	//// Bloom
	//float brightness = dot(Out.vColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));

	//if (0.99f < brightness)
	//	Out.vColor = float4(Out.vColor.rgb, 1.f);

	return Out;
}

float m_TexW = 150.f;
float m_TexH = 150.f;

static const float Weight[13] =
{
	0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f, 1.f, 0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f
};

static const float Total = 6.2108f;

PS_OUT PS_BLENDBLUR(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	//Out.vColor = tex2D(BlendSampler, In.vTexUV);

	// Blur X
	float4	vBlurX = 0;

	float2	tX = In.vTexUV;
	float2	uvX = 0;

	float	tuX = 1.f / m_TexW;

	for (int i = -6; i < 6; ++i)
	{
		uvX = tX + float2(tuX * i, 0);
		vBlurX += Weight[6 + i] * tex2D(BlendSampler, uvX);
	}

	vBlurX /= Total;

	Out.vBlurX = vBlurX;

	return Out;
}

PS_OUT PS_BLENDBLURY(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	//Out.vColor = tex2D(BlendSampler, In.vTexUV);

	// Blur Y
	float4	vBlurY = 0;

	float2	tY = In.vTexUV;
	float2	uvY = 0;

	float	tuY = 1.f / m_TexH;

	for (int i = -6; i < 6; ++i)
	{
		uvY = tY + float2(0, tuY * i);
		if (g_bBlurY)
			vBlurY += Weight[6 + i] * tex2D(BlendBlurYSampler, uvY);
		else
			vBlurY += Weight[6 + i] * tex2D(BlendBlurXSampler, uvY);
	}

	vBlurY /= Total;

	Out.vBlurY = vBlurY;
	Out.vBlurY.a = 1.f;

	return Out;
}

technique		Default_Device
{
	pass Blend
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass Blend
	{
		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_BLENDBLUR();
	}

	pass Blend
	{
		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_BLENDBLURY();
	}
}