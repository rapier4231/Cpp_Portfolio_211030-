matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;
matrix			g_matLightViewProj;
matrix			g_matScaleBias;

texture			g_DiffuseTexture;
texture			g_DiffuseTextureCover;
texture			g_DiffuseTextureSlope;
texture			g_DiffuseTextureTop;

texture			g_NormalTexture;
texture			g_NormalTextureCover;
texture			g_NormalTextureSlope;
texture			g_NormalTextureTop;

texture			g_ShadowMapTexture;

float			g_fDetail;	//디테일맵
float			g_fDetailCover;

float3			g_vLightDir;
vector			g_vSetColor = vector(1.f, 1.f, 1.f, 1.f);

sampler			DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	
	minfilter = linear;
	magfilter = linear;
};

sampler			NormalSampler = sampler_state
{
	texture = g_NormalTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler			DiffuseSamplerCover = sampler_state
{
	texture = g_DiffuseTextureCover;

	minfilter = linear;
	magfilter = linear;
};

sampler			NormalSamplerCover = sampler_state
{
	texture = g_NormalTextureCover;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler			DiffuseSamplerSlope = sampler_state
{
	texture = g_DiffuseTextureSlope;

	minfilter = linear;
	magfilter = linear;
};

sampler			NormalSamplerSlope = sampler_state
{
	texture = g_NormalTextureSlope;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler			DiffuseSamplerTop = sampler_state
{
	texture = g_DiffuseTextureTop;

	minfilter = linear;
	magfilter = linear;
};

sampler			NormalSamplerTop = sampler_state
{
	texture = g_NormalTextureTop;

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
	
	AddressU = Clamp;
	AddressV = Clamp;
};

////노이즈 텍스처
//texture			g_NoiseTexture;
//sampler			NoiseSampler = sampler_state
//{
//	texture = g_NoiseTexture;
//
//	minfilter = linear;
//	magfilter = linear;
//};

//스플래팅 필터 (알파맵)
texture			g_FilterTexture;
sampler			FilterSampler = sampler_state
{
	texture = g_FilterTexture;

	minfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	float3		vPosition	: POSITION;
	float3		vNormal		: NORMAL;
	float3		vTangent	: TANGENT;
	float2		vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
	float3		vNormal		: TEXCOORD1;
	float3		vTangent	: TEXCOORD2;
	float3		vBinormal	: TEXCOORD3;
	vector		vProjPos	: TEXCOORD4;
	vector		vSh_Diffuse : TEXCOORD5;
	vector		vSh_Depth	: TEXCOORD6;
	vector		ShadowMapUV : TEXCOORD7;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	matrix	matWV, matWVP, matWLVP, matWLVPB;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	matWLVP = mul(g_matWorld, g_matLightViewProj);
	matWLVPB = mul(matWLVP, g_matScaleBias);

	vector vPosition = vector(In.vPosition.xyz, 1.f);
	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

	// Normal과 Tangent를 외적하여 Binormal 산출
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld)).xyz;
	Out.vTangent = normalize(mul(vector(In.vTangent.xyz, 0.f), g_matWorld)).xyz;
	Out.vBinormal = normalize(mul(vector(cross(Out.vNormal, Out.vTangent).xyz, 0.f), g_matWorld)).xyz;

	Out.vProjPos = Out.vPosition;
	
	//그림자
	Out.vSh_Diffuse = g_vSetColor * max(dot(g_vLightDir, In.vNormal), 0);
	//Out.vSh_Ambient = In.vColor * 0.8f;
	Out.vSh_Depth = mul(vPosition, matWLVP);
	Out.ShadowMapUV = mul(vPosition, matWLVPB);

	return Out;
}

struct PS_IN
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
	float3		vNormal		: TEXCOORD1;
	float3		vTangent	: TEXCOORD2;
	float3		vBinormal	: TEXCOORD3;
	vector		vProjPos	: TEXCOORD4;
	vector		vSh_Diffuse	: TEXCOORD5;
	vector		vSh_Depth	: TEXCOORD6;
	vector		ShadowMapUV : TEXCOORD7;
};

struct PS_OUT
{
	vector		vColor			: COLOR0;
	vector		vDepth			: COLOR1;
	vector		vNormalResult	: COLOR2;
	vector		vShadow			: COLOR3;
};

float3 Gamma_LinearSpace(float3 _rgb)
{
	return pow(abs(_rgb), 2.2f);
}

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	//해당 픽셀의 경사도
	float fSlope = 1.f - In.vNormal.y;
	float2 vSlopeUV = In.vTexUV;
	//vSlopeUV *= (1.f + fSlope);

	//각 Diffuse텍스처 샘플링
	vector vColorField = tex2D(DiffuseSampler, In.vTexUV * g_fDetail);
	vector vColorCover = tex2D(DiffuseSamplerCover, In.vTexUV * g_fDetailCover);
	vector vColorFilter = tex2D(FilterSampler, In.vTexUV);
	vColorField = (vColorField * vColorFilter) + (vColorCover * (1.f - vColorFilter));

	vector vColorSlope	= tex2D(DiffuseSamplerSlope, In.vTexUV * g_fDetail);
	vector vColorTop	= tex2D(DiffuseSamplerTop, In.vTexUV * g_fDetail);

	// Normal - CTile의 기본 노멀맵(ANR) 샘플링 : DerivedNormalZ
	vector		vNormalTexField = tex2D(NormalSampler, In.vTexUV * g_fDetail);
	float3		vNormalField;
	vNormalField.x = vNormalTexField.a * 2.f - 1.f;
	vNormalField.y = vNormalTexField.g * 2.f - 1.f;
	vNormalField.z = sqrt(1 - saturate(vNormalField.x*vNormalField.x + vNormalField.y*vNormalField.y));
	vNormalField = normalize(vNormalField.xyz);

	// NormalCover 샘플링
	vector		vNormalTexCover = tex2D(NormalSamplerCover, In.vTexUV * g_fDetailCover);
	float3		vNormalCover = normalize(vNormalTexCover.rgb * 2.f - 1.f);
	vNormalField = (vNormalField * vColorFilter.xyz) + (vNormalCover * (1.f - vColorFilter.xyz));

	// NormalSlope 샘플링
	vector		vNormalTexSlope = tex2D(NormalSamplerSlope, In.vTexUV * g_fDetail);
	float3		vNormalSlope = normalize(vNormalTexSlope.rgb * 2.f - 1.f);

	// NormalTop 샘플링
	vector		vNormalTexTop = tex2D(NormalSamplerTop, In.vTexUV * g_fDetail);
	float3		vNormalTop = normalize(vNormalTexTop.rgb * 2.f - 1.f);

	//출력할 컬러 결정
	float fBlendAmount;
	vector vColorFinal;
	float3 vNormalResult;
	if (fSlope < 0.2f)
	{
		fBlendAmount = fSlope / 0.2f;
		Out.vColor = lerp(vColorField, vColorSlope, fBlendAmount);
		vNormalResult = lerp(vNormalField, vNormalSlope, fBlendAmount);
	}
	if (0.2f <= fSlope && fSlope < 0.5f)
	{
		fBlendAmount = (fSlope - 0.2f) * (1.f / (0.5f - 0.2f));
		Out.vColor = lerp(vColorSlope, vColorTop, fBlendAmount);
		vNormalResult = lerp(vNormalSlope, vNormalTop, fBlendAmount);
	}
	if (0.5f <= fSlope)
	{
		Out.vColor = vColorTop;
		vNormalResult = vNormalTop;
	}

	// Diffuse
	Out.vColor.rgb = Gamma_LinearSpace(Out.vColor.rgb);
	Out.vColor.a = 1.f;

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	//노멀에 월드행렬 곱해서 출력
	float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));
	vNormalResult = mul(vNormalResult, matWorld);
	Out.vNormalResult = vector(vNormalResult * 0.5f + 0.5f, 0.f);
	
	//Shadow
	float  fShadow = tex2Dproj(ShadowMapSampler, In.ShadowMapUV).x;
	vector vSh_Ambient = g_vSetColor * 0.8f;

	if (fShadow * In.vSh_Depth.w < In.vSh_Depth.z - 0.00001f)
	{
		//Out.vShadow = vSh_Ambient  * 0.5f;
		Out.vShadow = vector(0.f, 0.f, 0.f, 0.f);
	}

	else
		Out.vShadow = vSh_Ambient + In.vSh_Diffuse;

	Out.vShadow.a = tex2D(DiffuseSampler, In.vTexUV).a;
	
	return Out;
}

PS_OUT		PS_MAIN2(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	//해당 픽셀의 경사도
	float fSlope = 1.f - In.vNormal.y;
	float2 vSlopeUV = In.vTexUV;
	//vSlopeUV *= (1.f + fSlope);

	//각 Diffuse텍스처 샘플링
	vector vColorField = tex2D(DiffuseSampler, In.vTexUV/* * g_fDetail*/);	//통맵에 UV 입혀져있으므로 디테일 적용하지 않음
	vector vColorCover = tex2D(DiffuseSamplerCover, In.vTexUV * g_fDetailCover);
	vector vColorFilter = tex2D(FilterSampler, In.vTexUV);

	// ColorField와 ColorCover 섞기
	vColorField = (vColorField * vColorFilter) + (vColorCover * (1.f - vColorFilter));

	vector vColorSlope = tex2D(DiffuseSamplerSlope, In.vTexUV * g_fDetail);
	vector vColorTop = tex2D(DiffuseSamplerTop, In.vTexUV * g_fDetail);

	// Normal - CTile의 기본 노멀이 없으므로 대신 NormalCover(ANR) 사용 : DerivedNormalZ
	//vector		vNormalTexField = tex2D(NormalSamplerCover, In.vTexUV * g_fDetail);
	float3		vNormalField;
	/*vNormalField.x = vNormalTexField.a * 2.f - 1.f;
	vNormalField.y = vNormalTexField.g * 2.f - 1.f;
	vNormalField.z = sqrt(1 - saturate(vNormalField.x*vNormalField.x + vNormalField.y*vNormalField.y));
	vNormalField = normalize(vNormalField.xyz);*/

	// NormalCover(ANR) 샘플링 : DerivedNormalZ
	vector		vNormalTexCover = tex2D(NormalSamplerCover, In.vTexUV * g_fDetailCover);
	float3		vNormalCover;
	vNormalCover.x = vNormalTexCover.a * 2.f - 1.f;
	vNormalCover.y = vNormalTexCover.g * 2.f - 1.f;
	vNormalCover.z = sqrt(1 - saturate(vNormalCover.x*vNormalCover.x + vNormalCover.y*vNormalCover.y));
	vNormalCover = normalize(vNormalCover.xyz);

	// NormalField와 NormalCover 섞기
	vNormalField = vNormalCover; //(vNormalField * vColorFilter.xyz) + (vNormalCover * (1.f - vColorFilter.xyz));

	// NormalSlope 샘플링
	vector		vNormalTexSlope = tex2D(NormalSamplerSlope, In.vTexUV * g_fDetail);
	float3		vNormalSlope = normalize(vNormalTexSlope.rgb * 2.f - 1.f);

	// NormalTop 샘플링
	vector		vNormalTexTop = tex2D(NormalSamplerTop, In.vTexUV * g_fDetail);
	float3		vNormalTop = normalize(vNormalTexTop.rgb * 2.f - 1.f);

	//출력할 컬러 결정
	float fBlendAmount;
	vector vColorFinal;
	float3 vNormalResult;

	float fS1 = 0.6;
	float fS2 = 0.9;
	if (fSlope < fS1)
	{
		fBlendAmount = fSlope / fS1;
		Out.vColor = lerp(vColorField, vColorSlope, fBlendAmount);
		vNormalResult = lerp(vNormalField, vNormalSlope, fBlendAmount);
	}
	if (fS1 <= fSlope && fSlope < fS2)
	{
		fBlendAmount = (fSlope - fS1) * (1.f / (fS2 - fS1));
		Out.vColor = lerp(vColorSlope, vColorTop, fBlendAmount);
		vNormalResult = lerp(vNormalSlope, vNormalTop, fBlendAmount);
	}
	if (fS2 <= fSlope)
	{
		Out.vColor = vColorTop;
		vNormalResult = vNormalTop;
	}

	// Diffuse
	Out.vColor.rgb = Gamma_LinearSpace(Out.vColor.rgb);
	Out.vColor.a = 1.f;

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	//노멀에 월드행렬 곱해서 출력
	float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));
	vNormalResult = mul(vNormalResult, matWorld);
	Out.vNormalResult = vector(vNormalResult * 0.5f + 0.5f, 0.f);

	//Shadow
	float  fShadow = tex2Dproj(ShadowMapSampler, In.ShadowMapUV).x;
	vector vSh_Ambient = g_vSetColor * 0.8f;

	if (fShadow * In.vSh_Depth.w < In.vSh_Depth.z - 0.00001f)
	{
		//Out.vShadow = vSh_Ambient  * 0.5f;
		Out.vShadow = vector(0.f, 0.f, 0.f, 0.f);
	}

	else
		Out.vShadow = vSh_Ambient + In.vSh_Diffuse;

	Out.vShadow.a = tex2D(DiffuseSampler, In.vTexUV).a;

	return Out;
}

technique		Default_Device
{
	pass // Pass 0 : Diffuse / Depth / Normal
	{
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
	pass // Pass 1 : Diffuse / Depth / Normal
	{
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN2();
	}
};