//
float4x4		g_matPalette[80];
int			g_iNumBoneInfluences = 4;
//

matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_DiffuseTexture;
texture			g_NormalTexture;

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

struct VS_IN
{
	float3		vPosition	: POSITION;
	float3		vNormal		: NORMAL;
	//float3		vTangent	: TANGENT;
	//float2		vTexUV		: TEXCOORD0;

	float4		vWeights	: BLENDWEIGHT0;
	int4		iBoneIndeices : BLENDINDICES0;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	//float2		vTexUV		: TEXCOORD0;
	float3		vNormal		: TEXCOORD1;
	//float3		vTangent	: TEXCOORD2;
	//float3		vBinormal	: TEXCOORD3;
	//vector		vProjPos	: TEXCOORD4;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	float4 vPosition = float4(0.f, 0.f, 0.f, 1.f);
	float3 vNormal = float3(0.f, 0.f, 0.f);
	float fLastWeight = 0.f;
	int n = g_iNumBoneInfluences - 1;
	In.vNormal = normalize(In.vNormal);

	// 정점의 포지션과 노말을 블렌딩한다.
	for (int i = 0; i < n; ++i)
	{
		fLastWeight += In.vWeights[i];
		vPosition += In.vWeights[i] * mul(In.vPosition, g_matPalette[In.iBoneIndeices[i]]);
		vNormal += In.vWeights[i] * mul(In.vNormal, g_matPalette[In.iBoneIndeices[i]]);
	}

	fLastWeight = 1.f - fLastWeight;
	vPosition += fLastWeight * mul(In.vPosition, g_matPalette[In.iBoneIndeices[n]]);
	vNormal += fLastWeight * mul(In.vNormal, g_matPalette[In.iBoneIndeices[n]]);

	vPosition.w = 1.f;

	// 정점 스크린좌표로 변환
	matrix	matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = mul(vNormal, matWVP);
	
	
	
	//Out.vTexUV = In.vTexUV;
	//Out.vTangent = float3(0.f, 0.f, 0.f);
	//Out.vBinormal = float3(0.f, 0.f, 0.f);
	//Out.vProjPos = Out.vPosition;

	////////////////////////////////////////////////////////////////
	//matrix	matWV, matWVP;
	//matWV = mul(g_matWorld, g_matView);
	//matWVP = mul(matWV, g_matProj);

	//Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	//Out.vTexUV = In.vTexUV;
	//
	//// Normal과 Tangent를 외적하여 Binormal 산출
	//Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	//Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_matWorld));
	//Out.vBinormal = normalize(mul(vector(cross(Out.vNormal, Out.vTangent), 0.f), g_matWorld));
	//
	//Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float2		vTexUV		: TEXCOORD0;
	float3		vNormal		: TEXCOORD1;
	float3		vTangent	: TEXCOORD2;
	float3		vBinormal	: TEXCOORD3;
	vector		vProjPos	: TEXCOORD4;
};

struct PS_OUT
{
	vector		vColor			: COLOR0;
	vector		vDepth			: COLOR1;
	vector		vNormalResult	: COLOR2;
};

float3 Gamma_LinearSpace(float3 _rgb)
{
	return pow(abs(_rgb), 2.2f);
}

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

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

	return Out;
}

technique		Default_Device
{
	pass // Pass 0 : Diffuse / Depth / Normal
	{
		//fillmode = wireframe;
		indexedvertexblendenable = true;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
}
