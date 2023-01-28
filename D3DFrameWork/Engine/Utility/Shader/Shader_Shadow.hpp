//////////////////////////////////////////////////////////////////////////
// 더이상 사용하지 않음 : Shader_Normal_Shadow에 병합됨					//
//////////////////////////////////////////////////////////////////////////
matrix			g_matWorldViewProj;
matrix			g_matWorldLightViewProj;
matrix			g_matWorldLightViewProjBias;
vector			g_vCol;
vector			g_vLightDir;

float			 g_fSlopeScaleDepthBias = 1.0f;
float			 g_fDepthBias = -0.0005f;
float			 g_fDefaultDepthBias = 0.0f;

bool			g_bTerrain = false;

texture			g_ShadowMapTexture;
texture			g_DissolveTexture;
texture			g_DiffuseTexture;

float			g_fAmount;

sampler			ShadowMapSampler = sampler_state
{
	texture = g_ShadowMapTexture;

	//MinFilter = LINEAR;
	//MagFilter = LINEAR;
	//MipFilter = NONE;

	//AddressU = Clamp;
	//AddressV = Clamp;
};

sampler			DissolveSampler = sampler_state
{
	texture = g_DissolveTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler			DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VS_IN
{
	vector			vPosition	: POSITION;
	float3			vNormal		: NORMAL;
	float2			vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector			vPosition		: POSITION;
	vector			vDiffuse		: COLOR0;
	vector			vAmbient		: COLOR1;
	vector			ShadowMapUV		: TEXCOORD0;
	vector			vDepth			: TEXCOORD1;
	float2			vTexUV			: TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	Out.vPosition = mul(In.vPosition, g_matWorldViewProj);

	// 색
	Out.vDiffuse = g_vCol * max(dot(g_vLightDir, In.vNormal), 0);
	Out.vAmbient = g_vCol * 0.8f;

	// 그림자 맵
	Out.ShadowMapUV = mul(In.vPosition, g_matWorldLightViewProjBias);

	// 비교를 위한 깊이값
	Out.vDepth = mul(In.vPosition, g_matWorldLightViewProj);

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector			vDiffuse		: COLOR0;
	vector			vAmbient		: COLOR1;
	vector			ShadowMapUV		: TEXCOORD0;
	vector			vDepth			: TEXCOORD1;
	float2			vTexUV			: TEXCOORD2;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	vector vColor;
	float  fShadow = tex2Dproj(ShadowMapSampler, In.ShadowMapUV).x;

	if (fShadow * In.vDepth.w < In.vDepth.z - 0.00001f)
		vColor = In.vAmbient  * 0.5f;
	else
		vColor = In.vAmbient + In.vDiffuse;
	
	Out.vColor = vColor;
	float	fAlpha = tex2D(DiffuseSampler, In.vTexUV).a;
	Out.vColor.a = fAlpha;

	return Out;
}

PS_OUT PS_MAIN2(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	float vDissolve = tex2D(DissolveSampler, In.vTexUV).r;
	float vClipAmount = vDissolve - g_fAmount;

	if (0.f > vClipAmount)
		return Out;

	vector vColor;
	float  fShadow = tex2Dproj(ShadowMapSampler, In.ShadowMapUV).x;

	if (fShadow * In.vDepth.w < In.vDepth.z - 0.00001f)
	{
		vColor = In.vAmbient  * 0.5f;
	}

	else
		vColor = In.vAmbient + In.vDiffuse;

	Out.vColor = vColor;
	float	fAlpha = tex2D(DiffuseSampler, In.vTexUV).a;
	Out.vColor.a = fAlpha;

	return Out;
}

technique		Default_Device
{
	pass
	{
		// Z Fighting 해결?
		slopescaledepthbias = g_fDefaultDepthBias;
		depthbias = g_fDepthBias;
		zenable = true;
		
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	// Pass 1 : Dissolve
	{
		slopescaledepthbias = g_fDefaultDepthBias;
		depthbias = g_fDepthBias;
		zenable = true;

		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN2();
	}

	pass	// Pass 2 : AlphaTest
	{
		slopescaledepthbias = g_fDefaultDepthBias;
		depthbias = g_fDepthBias;
		zenable = true;

		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
}
