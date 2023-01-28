matrix			g_matView;
matrix			g_matProj;

matrix			g_matLightViewProj;
matrix			g_matScaleBias;

texture			g_DiffuseTexture;
texture			g_Diffuse1Texture;
texture			g_NormalTexture;
texture			g_DissolveTexture;
texture			g_ShadowMapTexture;

float			g_fSlopeScaleDepthBias = 1.0f;
float			g_fDepthBias = -0.0005f;
float			g_fDefaultDepthBias = 0.0f;

sampler			DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;

	minfilter = linear;
	magfilter = linear;
};

sampler			Diffuse1Sampler = sampler_state
{
	texture = g_Diffuse1Texture;

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
	
	AddressU = Clamp;
	AddressV = Clamp;
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
	float4		vValues		: TEXCOORD5;	//XYZ 그림자방향
	float4		vSetColor	: COLOR0;		//A 플래그 //RGB 객체별컬러
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	vector		vSetColor	: COLOR0;
	float2		vTexUV		: TEXCOORD0;
	float3		vNormal		: TEXCOORD1;
	float3		vTangent	: TEXCOORD2;
	float3		vBinormal	: TEXCOORD3;
	vector		vProjPos	: TEXCOORD4;
	float4		vValues		: TEXCOORD5;
	vector		vSh_Diffuse : COLOR1;
	//vector		vSh_Ambient	: COLOR2;
	vector		vSh_Depth	: TEXCOORD6;	//matWLV
	vector		ShadowMapUV	: TEXCOORD7;	//matWLVB
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

	//각종 행렬 연산
	matWV = mul(matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	matWLVP = mul(matWorld, g_matLightViewProj);
	matWLVPB = mul(matWLVP, g_matScaleBias);

	//float3 포지션을 float4 포지션으로 변환
	vector vPosition = vector(In.vPosition.xyz, 1.f);
	Out.vPosition = mul(vPosition, matWVP);

	//컬러, 텍스처UV
	Out.vSetColor = In.vSetColor;
	Out.vTexUV = In.vTexUV;

	// Normal과 Tangent를 외적하여 Binormal 산출
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), matWorld));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), matWorld));
	Out.vBinormal = normalize(mul(vector(cross(Out.vNormal, Out.vTangent), 0.f), matWorld));

	Out.vProjPos = Out.vPosition;
	Out.vValues = In.vValues;

	//그림자
	Out.vSh_Diffuse = In.vSetColor * max(dot(In.vValues, In.vNormal), 0);
	//Out.vSh_Ambient = In.vColor * 0.8f;
	Out.vSh_Depth = mul(vPosition, matWLVP);
	Out.ShadowMapUV = mul(vPosition, matWLVPB);

	return Out;
}

struct PS_IN
{
	vector		vSetColor	: COLOR0;
	float2		vTexUV		: TEXCOORD0;
	float3		vNormal		: TEXCOORD1;
	float3		vTangent	: TEXCOORD2;
	float3		vBinormal	: TEXCOORD3;
	vector		vProjPos	: TEXCOORD4;
	float4		vValues		: TEXCOORD5;
	vector		vSh_Diffuse	: COLOR1;
	//vector		vSh_Ambient	: COLOR2;
	vector		vSh_Depth	: TEXCOORD6;	//matWLV
	vector		ShadowMapUV	: TEXCOORD7;	//matWLVB
};

struct PS_OUT
{
	vector		vAlbedo		: COLOR0;
	vector		vDepth		: COLOR1;
	vector		vNormalResult : COLOR2;
	vector		vShadow		: COLOR3;
};

float3 Gamma_LinearSpace(float3 _rgb)
{
	return pow(abs(_rgb), 2.2f);
}

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	// Albedo
	Out.vAlbedo = tex2D(DiffuseSampler, In.vTexUV);
	Out.vAlbedo.rgb = Gamma_LinearSpace(Out.vAlbedo.rgb);
	Out.vAlbedo.a = 1.f;

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	// Normal
	vector		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	float3		vNormal = normalize(vNormalTex.rgb * 2.f - 1.f);
	float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));

	vNormal = mul(vNormal, matWorld);

	Out.vNormalResult = vector(vNormal * 0.5f + 0.5f, 0.f);

	//Shadow
	float  fShadow = tex2Dproj(ShadowMapSampler, In.ShadowMapUV).x;
	vector vSh_Ambient = In.vSetColor * 0.8f;

	if (fShadow * In.vSh_Depth.w < In.vSh_Depth.z - 0.00001f)
	{
		Out.vShadow = vSh_Ambient  * 0.5f;
		Out.vShadow = vector(0.f, 0.f, 0.f, 0.f);
	}
	else
		Out.vShadow = vSh_Ambient + In.vSh_Diffuse;

	Out.vShadow.a = tex2D(DiffuseSampler, In.vTexUV).a;
	
	return Out;
}

PS_OUT		PS_ALPHATEST(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	// Albedo
	Out.vAlbedo = tex2D(DiffuseSampler, In.vTexUV);
	Out.vAlbedo.rgb = Gamma_LinearSpace(Out.vAlbedo.rgb);

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	// Normal
	vector		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	float3		vNormal = normalize(vNormalTex.rgb * 2.f - 1.f);
	float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));

	vNormal = mul(vNormal, matWorld);

	Out.vNormalResult = vector(vNormal * 0.5f + 0.5f, 0.f);

	//Shadow
	float  fShadow = tex2Dproj(ShadowMapSampler, In.ShadowMapUV).x;
	vector vSh_Ambient = In.vSetColor * 0.8f;

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

PS_OUT PS_DISSOLVE(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	// Dissolve
	float fDissolve = tex2D(DissolveSampler, In.vTexUV).r;
	float fClipAmount = fDissolve - In.vSetColor.a;
	if (0.f > fClipAmount)
		return Out;

	// Albedo
	Out.vAlbedo = tex2D(DiffuseSampler, In.vTexUV);
	Out.vAlbedo.rgb = Gamma_LinearSpace(Out.vAlbedo.rgb);
	Out.vAlbedo.a = 1.f;

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	// Normal
	vector		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	float3		vNormal = normalize(vNormalTex.rgb * 2.f - 1.f);
	float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));

	vNormal = mul(vNormal, matWorld);

	Out.vNormalResult = vector(vNormal * 0.5f + 0.5f, 0.f);
	
	//Shadow
	float  fShadow = tex2Dproj(ShadowMapSampler, In.ShadowMapUV).x;
	vector vSh_Ambient = In.vSetColor * 0.8f;

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

PS_OUT PS_DISSOLVE_A(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	// Dissolve
	float fDissolve = tex2D(DissolveSampler, In.vTexUV).r;
	float fClipAmount = fDissolve - In.vSetColor.a;
	if (0.f > fClipAmount)
		return Out;

	// Albedo
	Out.vAlbedo = tex2D(DiffuseSampler, In.vTexUV);
	Out.vAlbedo.rgb = Gamma_LinearSpace(Out.vAlbedo.rgb);

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	// Normal
	vector		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	float3		vNormal = normalize(vNormalTex.rgb * 2.f - 1.f);
	float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));

	vNormal = mul(vNormal, matWorld);

	Out.vNormalResult = vector(vNormal * 0.5f + 0.5f, 0.f);
	
	//Shadow
	float  fShadow = tex2Dproj(ShadowMapSampler, In.ShadowMapUV).x;
	vector vSh_Ambient = In.vSetColor * 0.8f;

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

PS_OUT PS_BLEND(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	// Albedo
	vector vColor0 = tex2D(DiffuseSampler, In.vTexUV);
	vector vColor1 = tex2D(Diffuse1Sampler, In.vTexUV);
	float fBlendAmount = tex2D(DissolveSampler, In.vTexUV).r;
	Out.vAlbedo = lerp(vColor0, vColor1, fBlendAmount);
	Out.vAlbedo.rgb = Gamma_LinearSpace(Out.vAlbedo.rgb);
	Out.vAlbedo.a = 1.f;

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	// Normal
	vector		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	float3		vNormal = normalize(vNormalTex.rgb * 2.f - 1.f);
	float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));

	vNormal = mul(vNormal, matWorld);

	Out.vNormalResult = vector(vNormal * 0.5f + 0.5f, 0.f);

	//Shadow
	float  fShadow = tex2Dproj(ShadowMapSampler, In.ShadowMapUV).x;
	vector vSh_Ambient = In.vSetColor * 0.8f;

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

PS_OUT PS_ANR(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	// Albedo
	Out.vAlbedo = tex2D(DiffuseSampler, In.vTexUV);
	Out.vAlbedo.rgb = Gamma_LinearSpace(Out.vAlbedo.rgb);
	Out.vAlbedo.a = 1.f;

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	// Normal : 노멀맵(ANR) 샘플링 (DerivedNormalZ)
	vector		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	float3		vNormal;
	vNormal.x = vNormalTex.a * 2.f - 1.f;
	vNormal.y = vNormalTex.g * 2.f - 1.f;
	vNormal.z = sqrt(1 - saturate(vNormal.x*vNormal.x + vNormal.y*vNormal.y));
	float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));
	vNormal = mul(vNormal, matWorld);

	Out.vNormalResult = vector(vNormal * 0.5f + 0.5f, 0.f);

	//Shadow
	float  fShadow = tex2Dproj(ShadowMapSampler, In.ShadowMapUV).x;
	vector vSh_Ambient = In.vSetColor * 0.8f;

	if (fShadow * In.vSh_Depth.w < In.vSh_Depth.z - 0.00001f)
	{
		Out.vShadow = vSh_Ambient  * 0.5f;
		Out.vShadow = vector(0.f, 0.f, 0.f, 0.f);
	}
	else
		Out.vShadow = vSh_Ambient + In.vSh_Diffuse;

	Out.vShadow.a = tex2D(DiffuseSampler, In.vTexUV).a;

	return Out;
}

PS_OUT PS_METALIC(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	// Albedo
	Out.vAlbedo = tex2D(DiffuseSampler, In.vTexUV);
	Out.vAlbedo.rgb = Gamma_LinearSpace(Out.vAlbedo.rgb);
	Out.vAlbedo.a = 1.f;

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	// Normal
	vector		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	float3		vNormal = normalize(vNormalTex.rgb * 2.f - 1.f);
	float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));

	vNormal = mul(vNormal, matWorld);

	Out.vNormalResult = vector(vNormal * 0.5f + 0.5f, 0.f);

	//Shadow
	float  fShadow = tex2Dproj(ShadowMapSampler, In.ShadowMapUV).x;
	vector vSh_Ambient = In.vSetColor * 0.8f;

	if (fShadow * In.vSh_Depth.w < In.vSh_Depth.z - 0.00001f)
	{
		Out.vShadow = vSh_Ambient  * 0.5f;
		Out.vShadow = vector(0.f, 0.f, 0.f, 0.f);
	}
	else
		Out.vShadow = vSh_Ambient + In.vSh_Diffuse;

	Out.vShadow.a = tex2D(DiffuseSampler, In.vTexUV).a;

	return Out;
}

technique		Default_Device
{
	pass	// Pass 0 : Base (Diffuse / Depth / Normal)
	{
		alphatestenable = false;
		cullmode = ccw;//fillmode = wireframe;

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
		pixelshader = compile ps_3_0 PS_ALPHATEST();
	}
	
	pass	// Pass 2 : Dissolve
	{
		alphatestenable = false;
		cullmode = ccw;
		
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_DISSOLVE();
	}
	
	pass	// Pass 3 : Dissolve + AlphaTest
	{
		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
		cullmode = none;
		
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_DISSOLVE_A();
	}

		pass	// Pass 4 : Splatting
	{
		alphatestenable = false;
		cullmode = ccw;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_BLEND();
	}

		pass	// Pass 5 : ANR Normalmap
	{
		alphatestenable = false;
		cullmode = ccw;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_ALPHATEST();
	}

		pass	// Pass 6 : ANR Normalmap + AlphaTest
	{
		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
		cullmode = none;
		
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_METALIC();
	}

		pass	// Pass 7 : Metalic
	{
		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_METALIC();
	}
}