matrix			g_matLightViewProj;

texture			g_DiffuseTexture;
texture			g_DissolveTexture;

sampler			DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;

	minfilter = linear;
	magfilter = linear;
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
	// stream 0    
	float3		vPosition	: POSITION;
	float3		vNormal		: NORMAL;
	float3		vTangent	: TANGENT;
	float2		vTexUV		: TEXCOORD0;

	// stream 1
	float4		matWorld0	: TEXCOORD1;
	float4		matWorld1	: TEXCOORD2;
	float4		matWorld2	: TEXCOORD3;
	float4		matWorld3	: TEXCOORD4;
	float4		vValues		: TEXCOORD5;	//XYZ //W 플래그
	float4		vSetColor	: COLOR0;		//A 디졸브 //RGB 객체별컬러
};

struct VS_OUT
{
	vector		vPos		: POSITION;
	float4		vSetColor	: COLOR0;
	float2		vTexUV		: TEXCOORD0;
	vector		ShadowMapUV : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	matrix	matWorld, matWLVP;

	//월드매트릭스 조립   
	matWorld = matrix(In.matWorld0.xyzw
		, In.matWorld1.xyzw
		, In.matWorld2.xyzw
		, In.matWorld3.xyzw);

	//위치좌표
	vector vPosition = vector(In.vPosition.xyz, 1.f);
	matWLVP = mul(matWorld, g_matLightViewProj);
	Out.vPos = mul(vPosition, matWLVP);

	Out.vSetColor = In.vSetColor;

	// 카메라 좌표계에서의 깊이를 텍스처에 넣는다
	Out.ShadowMapUV = Out.vPos.zzzw;

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4		vSetColor	: COLOR0;
	float2		vTexUV		: TEXCOORD0;
	vector		ShadowMapUV : TEXCOORD1;
};

struct PS_OUT
{
	vector		vShadowMap : COLOR0;
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

	//디졸브
	float vDissolve = tex2D(DissolveSampler, In.vTexUV).r;
	float vClipAmount = vDissolve - In.vSetColor.a;
	if (0.f > vClipAmount)
		return Out;

	Out.vShadowMap = In.ShadowMapUV.z / In.ShadowMapUV.w;

	float	fAlpha = tex2D(DiffuseSampler, In.vTexUV).a;
	Out.vShadowMap.a = fAlpha;

	return Out;
}

technique		Default_Device
{
	pass
	{
		/*alphatestenable = false;
		cullmode = ccw;*/
		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	// Pass 1 : AlphaTest
	{
		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
		cullmode = none;
		
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	// Pass 2 : Dissolve
	{
		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
		cullmode = none;
		
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN2();
	}
}