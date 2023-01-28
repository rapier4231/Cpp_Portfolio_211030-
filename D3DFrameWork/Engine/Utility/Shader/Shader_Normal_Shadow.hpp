matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;
matrix			g_matLightViewProj;
matrix			g_matScaleBias;

texture			g_DiffuseTexture;
texture			g_NormalTexture;
texture			g_DissolveTexture;
texture			g_ShadowMapTexture;
texture			g_MaskTexture;
texture			g_NoiseTexture;
texture			g_GradientTexture;
texture			g_DiffuseCubeTexture;

bool			g_bNoneNormalTexture;
bool			g_bMaskTexture;
float			g_fShadowBias;
float			g_fAmount;
float			g_fNoiseAcc;

float3			g_vLightDir;
float3			g_vecCameraPos;

vector			g_DinoColor1;
vector			g_DinoColor2;
vector			g_DinoColor3;

vector			g_vSetColor = vector(1.f, 1.f, 1.f, 1.f);

sampler			DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
};

sampler			NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

sampler			MaskSampler = sampler_state
{
	texture = g_MaskTexture;
};

sampler			DissolveSampler = sampler_state
{
	texture = g_DissolveTexture;
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

sampler			NoiseSampler = sampler_state
{
	texture = g_NoiseTexture;
};

sampler			GradientSampler = sampler_state
{
	texture = g_GradientTexture;
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
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_matWorld));
	Out.vBinormal = normalize(mul(vector(cross(Out.vNormal, Out.vTangent), 0.f), g_matWorld));

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
	float2		vTexUV		: TEXCOORD0;
	float3		vNormal		: TEXCOORD1;
	float3		vTangent	: TEXCOORD2;
	float3		vBinormal	: TEXCOORD3;
	vector		vProjPos	: TEXCOORD4;
	vector		vSh_Diffuse	: TEXCOORD5;
	vector		vSh_Depth	: TEXCOORD6;
	vector		ShadowMapUV	: TEXCOORD7;
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

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	// Mask
	vector vMask = tex2D(MaskSampler, In.vTexUV);

	// Diffuse
	vector Color = tex2D(DiffuseSampler, In.vTexUV) * g_vSetColor;

	if (g_bMaskTexture)
	{
		//if (vMask.r)
		//	Color += g_DinoColor1 * 0.33f;
		//if (vMask.g)
		//	Color += g_DinoColor2 * 0.33f;
		//if (vMask.b)
		//	Color += g_DinoColor3 * 0.33f;

		vector add1 = lerp(Color, g_DinoColor1, vMask.r);
		vector add2 = lerp(Color, g_DinoColor2, vMask.b);
		vector add3 = lerp(Color, g_DinoColor2, vMask.g);

		Color = lerp(add1, add2, 0.5f);
		Color = lerp(Color, add3, 0.5f);
	}

	Out.vColor.rgb = Gamma_LinearSpace(Color.rgb);
	Out.vColor.a = 1.f;

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
	vector vSh_Ambient = g_vSetColor * 0.8f;

	if (fShadow * In.vSh_Depth.w < In.vSh_Depth.z - g_fShadowBias)
	{
		Out.vShadow = vSh_Ambient  * 0.5f;
		//Out.vShadow = vector(0.f, 0.f, 0.f, 0.f);
	}

	else
		Out.vShadow = vSh_Ambient + In.vSh_Diffuse;

	Out.vShadow.a = tex2D(DiffuseSampler, In.vTexUV).a;

	return Out;
}

PS_OUT		PS_ALPHATEST(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;
	
	// Diffuse
	Out.vColor = tex2D(DiffuseSampler, In.vTexUV) * g_vSetColor;
	Out.vColor.rgb = Gamma_LinearSpace(Out.vColor.rgb);

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
	vector vSh_Ambient = g_vSetColor * 0.8f;

	if (fShadow * In.vSh_Depth.w < In.vSh_Depth.z - g_fShadowBias)
	{
		Out.vShadow = vSh_Ambient  * 0.5f;
		//Out.vShadow = vector(0.f, 0.f, 0.f, 0.f);
	}

	else
		Out.vShadow = vSh_Ambient + In.vSh_Diffuse;

	Out.vShadow.a = tex2D(DiffuseSampler, In.vTexUV).a;

	return Out;
}

PS_OUT PS_DISSOLVE(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	float vDissolve = tex2D(DissolveSampler, In.vTexUV).r;
	float vClipAmount = vDissolve - g_fAmount;

	if (0.f > vClipAmount)
		return Out;

	// Diffuse
	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor.rgb = Gamma_LinearSpace(Out.vColor.rgb);
	Out.vColor.a = 1.f;

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
	vector vSh_Ambient = g_vSetColor * 0.8f;

	if (fShadow * In.vSh_Depth.w < In.vSh_Depth.z - g_fShadowBias)
	{
		Out.vShadow = vSh_Ambient  * 0.5f;
		//Out.vShadow = vector(0.f, 0.f, 0.f, 0.f);
	}

	else
		Out.vShadow = vSh_Ambient + In.vSh_Diffuse;

	Out.vShadow.a = tex2D(DiffuseSampler, In.vTexUV).a;

	return Out;
}

PS_OUT PS_DISSOLVE_A(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	float vDissolve = tex2D(DissolveSampler, In.vTexUV).r;
	float vClipAmount = vDissolve - g_fAmount;

	if (0.f > vClipAmount)
		return Out;

	// Diffuse
	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor.rgb = Gamma_LinearSpace(Out.vColor.rgb);

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
	vector vSh_Ambient = g_vSetColor * 0.8f;

	if (fShadow * In.vSh_Depth.w < In.vSh_Depth.z - g_fShadowBias)
	{
		Out.vShadow = vSh_Ambient  * 0.5f;
		//Out.vShadow = vector(0.f, 0.f, 0.f, 0.f);
	}

	else
		Out.vShadow = vSh_Ambient + In.vSh_Diffuse;

	Out.vShadow.a = tex2D(DiffuseSampler, In.vTexUV).a;

	return Out;
}

PS_OUT PS_EFFECT(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	// Diffuse
	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);

	//if (0.1f < Out.vColor.r)
		Out.vColor *= g_vSetColor;

	Out.vColor.rgb = Gamma_LinearSpace(Out.vColor.rgb);
	Out.vColor.a = 1.f;

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	// Normal
	vector		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	float3		vNormal = normalize(vNormalTex.rgb * 2.f - 1.f);
	float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));

	vNormal = mul(vNormal, matWorld);

	//Out.vNormalResult = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vNormalResult = vector(0.8f, 0.8f, 0.8f, 0.f);

	return Out;
}

PS_OUT PS_NOISE(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	// Noise
	vector  vNoise = tex2D(NoiseSampler, float2(In.vTexUV.x + g_fNoiseAcc, In.vTexUV.y));

	// Gradient
	vector	vGradient = tex2D(GradientSampler, In.vTexUV);

	// Diffuse
	Out.vColor = tex2D(DiffuseSampler, float2(In.vTexUV.x, In.vTexUV.y - g_fNoiseAcc));
	//Out.vColor = tex2D(DiffuseSampler, saturate(In.vTexUV + vNoise.xy));

	//if (0.1f < Out.vColor.r)
	Out.vColor *= g_vSetColor;

	Out.vColor.rgb = Gamma_LinearSpace(Out.vColor.rgb);
	Out.vColor.a = (vGradient.r + vGradient.g + vGradient.b) * 0.33f;

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	// Normal
	vector		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	float3		vNormal = normalize(vNormalTex.rgb * 2.f - 1.f);
	float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));

	vNormal = mul(vNormal, matWorld);

	//Out.vNormalResult = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vNormalResult = vector(0.8f, 0.8f, 0.8f, 0.f);

	return Out;
}

PS_OUT PS_SKYBOX(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	// Diffuse
	vector Color = tex2D(DiffuseSampler, In.vTexUV) * g_vSetColor;
	Out.vColor.rgb = Gamma_LinearSpace(Color.rgb);
	Out.vColor.a = 1.f;

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	// Normal
	vector		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	float3		vNormal = normalize(vNormalTex.rgb * 2.f - 1.f);
	float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));

	vNormal = mul(vNormal, matWorld);

	//Out.vNormalResult = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vNormalResult = vector(0.65f, 0.65f, 0.65f, 0.f);

	Out.vShadow = vector(1.f, 1.f, 1.f, 1.f);

	return Out;
}

technique		Default_Device
{
	pass	// Pass 0 : Diffuse / Depth / Normal
	{
		//fillmode = wireframe;
		//cullmode = none;
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
		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_DISSOLVE();
	}

	pass	// Pass 3 : Dissolve, AlphaTest 텍스쳐 전용
	{
		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_DISSOLVE_A();
	}

	pass	// Pass 4 : NoneAlpha Effect
	{
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_EFFECT();
	}

	pass	// Pass 5 : Noise
	{
		//alphablendenable = true;
		//srcblend = srcalpha;
		//destblend = invsrcalpha;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_NOISE();
	}

	pass	// Pass 6 : NORMAL_CULMODE <-??=
	{
		//fillmode = wireframe;
		cullmode = none;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	// Pass 7 : SKYBOX
	{
		zWriteEnable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_SKYBOX();
	}
}
