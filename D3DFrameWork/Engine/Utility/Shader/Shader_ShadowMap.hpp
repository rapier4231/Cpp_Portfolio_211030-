matrix			g_matWorldLightViewProj;

texture			g_DiffuseTexture;
texture			g_DissolveTexture;

float			g_fAmount;

sampler			DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler			DissolveSampler = sampler_state
{
	texture = g_DissolveTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VS_IN
{
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector			vPos			: POSITION;
	float2			vTexUV			: TEXCOORD0;
	vector			ShadowMapUV		: TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	vector vPos = mul(In.vPosition, g_matWorldLightViewProj);

	// 위치좌표
	Out.vPos = vPos;

	// 카메라 좌표계에서의 깊이를 텍스처에 넣는다
	Out.ShadowMapUV = vPos.zzzw;

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float2		vTexUV			: TEXCOORD0;
	vector		ShadowMapUV		: TEXCOORD1;
};

struct PS_OUT
{
	vector		vShadowMap		: COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	float	fAlpha = tex2D(DiffuseSampler, In.vTexUV).a;

	Out.vShadowMap = In.ShadowMapUV.z / In.ShadowMapUV.w;
	Out.vShadowMap.a = fAlpha;

	return Out;
}

PS_OUT PS_MAIN2(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	float vDissolve = tex2D(DissolveSampler, In.vTexUV).r;
	float vClipAmount = vDissolve - g_fAmount;

	if (0.f > vClipAmount)
		return Out;

	float	fAlpha = tex2D(DiffuseSampler, In.vTexUV).a;

	Out.vShadowMap = In.ShadowMapUV.z / In.ShadowMapUV.w;
	Out.vShadowMap.a = fAlpha;

	return Out;
}

technique		Default_Device
{
	pass
	{
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	// Pass 1 : Dissolve
	{
		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN2();
	}

	pass	// Pass 2 : AlphaTest
	{
		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
}
