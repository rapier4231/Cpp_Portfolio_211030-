//////////////////////////////////////////////////////////////////////////
// 더이상 사용하지 않음 : Shader_Instance에 병합됨							//
//////////////////////////////////////////////////////////////////////////

matrix			g_matView;
matrix			g_matProj;

matrix			g_matLightViewProj;
matrix			g_matScaleBias;

texture			g_DiffuseTexture;
texture			g_DissolveTexture;		
texture			g_ShadowMapTexture;
texture			g_ShadowBlurTexture;

//vector			g_vLightDir;

float			g_fSlopeScaleDepthBias = 1.0f;
float			g_fDepthBias = -0.0005f;
float			g_fDefaultDepthBias = 0.0f;

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

sampler			ShadowMapSampler = sampler_state
{
	texture = g_ShadowMapTexture;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	
	//AddressU = Clamp;
	//AddressV = Clamp;
};

/*sampler			ShadowBlurSampler = sampler_state
{
	texture = g_ShadowBlurTexture;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	
	AddressU = Clamp;
	AddressV = Clamp;
};*/

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
	float4		vValues		: TEXCOORD5;	//XYZ 그림자방향 //W 플래그
	float4		vSetColor	: COLOR0;		//A 디졸브 //RGB 객체별컬러
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	vector		vDiffuse	: COLOR0;
	vector		vAmbient	: COLOR1;
	vector		vSetColor	: COLOR2;
	vector		ShadowMapUV	: TEXCOORD0;
	vector		vDepth		: TEXCOORD1;
	float2		vTexUV		: TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	matrix	matWorld, matWV, matWVP, matWLVP, matWLVPB;

	//월드매트릭스 조립   
	matWorld = matrix(In.matWorld0.xyzw
		, In.matWorld1.xyzw
		, In.matWorld2.xyzw
		, In.matWorld3.xyzw);

	//행렬 연산
	matWV = mul(matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	matWLVP = mul(matWorld, g_matLightViewProj);
	matWLVPB = mul(matWLVP, g_matScaleBias);

	//위치값 출력
	vector vPosition = vector(In.vPosition.xyz, 1.f);
	Out.vPosition = mul(vPosition, matWVP);

	//그림자 색
	Out.vDiffuse = In.vSetColor * max(dot(In.vValues, In.vNormal), 0);
	Out.vAmbient = In.vSetColor * 0.8f;
	
	//버텍스컬러 (디졸브값)
	Out.vSetColor = In.vSetColor;

	// 그림자 맵
	Out.ShadowMapUV = mul(vPosition, matWLVPB);

	// 비교를 위한 깊이값
	Out.vDepth = mul(vPosition, matWLVP);

	//텍스처UV
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector		vDiffuse	: COLOR0;
	vector		vAmbient	: COLOR1;
	vector		vSetColor	: COLOR2;
	vector		ShadowMapUV	: TEXCOORD0;
	vector		vDepth		: TEXCOORD1;
	float2		vTexUV		: TEXCOORD2;
};

struct PS_OUT
{
	vector		vColor		: COLOR0;
};

float3 Gamma_LinearSpace(float3 _rgb)
{
	return pow(abs(_rgb), 2.2f);
}

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

PS_OUT		PS_MAIN2(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	float vDissolve = tex2D(DissolveSampler, In.vTexUV).r;
	float vClipAmount = vDissolve - In.vSetColor.a;
	if (0.f > vClipAmount)
		return Out;

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

technique		Default_Device
{
	pass
	{
		slopescaledepthbias = g_fDefaultDepthBias;
		depthbias = g_fDepthBias;
		zenable = true;

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

	pass	// Pass 2 : Dissolve
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

}
