// 상수 테이블
texture		g_AlbedoTexture;
texture		g_DepthTexture;
texture		g_ShadeTexture;
texture		g_SpecularTexture;
texture		g_ShadowTexture;

float		g_fFade;

sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;
};

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

sampler ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;
};

sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
};

sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;
};

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor	: COLOR0;
};

float3 ToneMapACES(float3 hdr)
{
	const float A = 2.51f, B = 0.03f, C = 2.43f, D = 0.59f, E = 0.14f;
	return saturate((hdr * (A * hdr + B)) / (hdr * (C * hdr + D) + E));
	//return hdr / (hdr + 1.f);
}

float3 Gamma_ForMonitor(float3 _rgb, float _f = 0.45454545f)
{
	return pow(abs(_rgb), _f); // 0.45454545 = 1/2.2
}

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);
	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);

	Out.vColor = vAlbedo * vShade /*+ vSpecular*/;

	return Out;
}

PS_OUT PS_HDR(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);
	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);
	vector		vShadow = tex2D(ShadowSampler, In.vTexUV);
	vector		vFog = tex2D(DepthSampler, In.vTexUV);

	float fFog = saturate((vFog.r + vFog.g) * 0.5f - 0.35f);
	vAlbedo.rgb = Gamma_ForMonitor(vAlbedo.rgb);
	vector vResult;

	vResult = (1 - fFog) * (vAlbedo * vShade) + fFog * vector(fFog, fFog, fFog, 1.f);
	//vResult = (1 - fFog) * (vAlbedo * vShade + vBlur) + fFog * vector(fFog, fFog, fFog, 1.f);

	Out.vColor = vResult;
	Out.vColor.rgb = ToneMapACES(vResult.rgb) * g_fFade;
	Out.vColor.a = 1.f;

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

	pass Blend_HDR
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_HDR();
	}
}