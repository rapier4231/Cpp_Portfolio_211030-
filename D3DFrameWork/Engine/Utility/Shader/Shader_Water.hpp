matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_DiffuseTexture;
texture			g_NormalTexture;
texture			g_NoiseTexture;
texture			g_GradientTexture;
texture			g_DiffuseCubeTexture;

float			g_fNoiseAcc;
vector			g_vSetColor = vector(1.f, 1.f, 1.f, 1.f);

sampler			DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
};

sampler			NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

sampler			NoiseSampler = sampler_state
{
	texture = g_NoiseTexture;
};

sampler			GradientSampler = sampler_state
{
	texture = g_GradientTexture;
};

samplerCUBE			DiffuseCubeSampler = sampler_state
{
	texture = g_DiffuseCubeTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
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
	float3		vEye		: TEXCOORD5;
	float2		Ds1			: TEXCOORD6;
	float2		Ds2			: TEXCOORD7;
	float3		vViewPos	: TEXCOORD8;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_matWorld));
	Out.vBinormal = normalize(mul(vector(cross(Out.vNormal, Out.vTangent), 0.f), g_matWorld));

	Out.vProjPos = Out.vPosition;

	return Out;
}


static float4 wvK = float4(0.002f, 0.001f, 0.01f, 0.004f);
static float4 wvSpeed = float4(4.f, 3.5f, 1.4f, 2.8f);
static float4 wvOmega = float4(0.35f, 0.2f, 0.4f, 0.3f);

float3	WavePos(float3 Pos, float3 eps = float3(0.f, 0.f, 0.f))
{
	float3	vecPos;
	float	fR;
	float3	fP;

	Pos += eps;

	vecPos = Pos;

	fP = Pos - float3(-256.f, 0.f, 0.f);
	fR = sqrt(fP.x * fP.x + fP.z * fP.z);
	vecPos.y += exp(-wvK.x * fR) * sin(fR * wvOmega.x - g_fNoiseAcc * wvSpeed.x);


	fP = Pos - float3(256.f, 0.f, 0.f);
	fR = sqrt(fP.x * fP.x + fP.z * fP.z);
	vecPos.y += exp(-wvK.y * fR) * sin(fR * wvOmega.y - g_fNoiseAcc * wvSpeed.y);


	fP = Pos - float3(0.f, 0.f, 256.f);
	fR = sqrt(fP.x * fP.x + fP.z * fP.z);
	vecPos.y += exp(-wvK.z * fR) * sin(fR * wvOmega.z - g_fNoiseAcc * wvSpeed.z);


	fP = Pos - float3(0.f, 0.f, -256.f);
	fR = sqrt(fP.x * fP.x + fP.z * fP.z);
	vecPos.y += exp(-wvK.w * fR) * sin(fR * wvOmega.w - g_fNoiseAcc * wvSpeed.w);

	return vecPos;
}

VS_OUT VS_WATER(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	float4	P = 0;
	float3	E = 1;
	float3	N = 0;

	float3	tPos;
	float3	tPosX;
	float3	tPosZ;

	tPos = WavePos(In.vPosition);

	P = float4(tPos, 1.f);
	P = mul(P, g_matWorld);
	P = mul(P, g_matView);

	E = -normalize(P);

	P = mul(P, g_matProj);

	tPosX = WavePos(In.vPosition, float3(1.f, 0.f, 0.1f));
	tPosX = normalize(tPosX - tPos);

	tPosZ = WavePos(In.vPosition, float3(0.f, 0.f, 0.1f));
	tPosZ = normalize(tPosZ - tPos);

	N = cross(tPosX, tPosZ);
	N.x *= 0.1f;
	N.z *= 0.1f;
	N = normalize(N);

	N = mul(N, g_matWorld);
	N = mul(N, g_matView);
	N = normalize(N);

	//In.vTexUV.x = In.vPosition.x / 16.;
	//In.vTexUV.y = 1.f - In.vPosition.z / 16.;

	Out.vPosition = P;
	Out.vEye = E;
	Out.vNormal = N;
	//Out.vNormal = normalize(In.vNormal * 2.f - 1.f);

	Out.vTexUV = In.vTexUV;

	Out.vProjPos = Out.vPosition;

	float2	wvSpdU = float2(0.02f, +0.02f);
	float2	wvSpdV = float2(0.02f, -0.02f);
	float	fTime = g_fNoiseAcc;
	float2	Tex = 1;
	float2	Ds1;
	float2	Ds2;

	Tex.x = In.vPosition.x / 16.f;
	Tex.y = 1.f - In.vPosition.z / 16.f;

	Ds1 = Tex.xy + wvSpdU * fTime;
	Ds2 = Tex.yx + wvSpdV * fTime;

	Out.Ds1 = Ds1;
	Out.Ds2 = Ds2;

	return Out;
}

struct PS_IN
{
	float2		vTexUV		: TEXCOORD0;
	float3		vNormal		: TEXCOORD1;
	float3		vTangent	: TEXCOORD2;
	float3		vBinormal	: TEXCOORD3;
	vector		vProjPos	: TEXCOORD4;
	float3		vEye		: TEXCOORD5;
	float2		Ds1			: TEXCOORD6;
	float2		Ds2			: TEXCOORD7;
};

struct PS_OUT
{
	vector		vColor			: COLOR0;
	vector		vDepth			: COLOR1;
	vector		vNormalResult	: COLOR2;
	//vector		vShadow			: COLOR3;	//물에서는 사용하지 않음
};

float3 Gamma_LinearSpace(float3 _rgb)
{
	return pow(abs(_rgb), 2.2f);
}

float3 NorPrc(float3 B, float3 T, float3 N, float2 Tex)
{
	float3 O = 0;
	float3 C = 2.f * tex2D(NormalSampler, Tex).xyz - 1.f;	// Extraction Bump Vector from N Map Texture

	O.x = B.x * C.x + T.x * C.y + N.x * C.z;
	O.y = B.y * C.x + T.y * C.y + N.y * C.z;
	O.z = B.z * C.x + T.z * C.y + N.z * C.z;

	return normalize(O);
}

PS_OUT PS_WATER(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	float3 E = normalize(In.vEye);
	float3 B = { 1,0,0 };							// Base X direction Vector for Binormal vector
	float3 T = { 0,1,0 };							// Base Y direction Vector for Tangent vector
	float3 N = { 0,0,1 };							// Base Z direction Vector for N vector
	float3 N1;
	float3 N2;

	float3 R;
	float  F;

	float	n1 = 1.0;
	float	n2 = 1.333;

	F = n2 / n1;

	N = normalize(In.vNormal);
	T = (cross(N, B));
	B = (cross(T, N));

	N1 = NorPrc(B, T, N, In.Ds1);
	N2 = NorPrc(B, T, N, In.Ds2);

	N = normalize(N1 + N2);
	R = reflect(-E, N);						// Reflection Vector
	F = pow(F - dot(-E, N), 2);				// Calcuate Simple Fresnel = (F - I.N)^2

	Out.vColor = texCUBE(DiffuseCubeSampler, R);
	Out.vColor.a = F;
	Out.vColor *= g_vSetColor;

	// Depth
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	// Normal
	Out.vNormalResult = vector(N, 1.f);

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

technique		Default_Device
{
	pass	// Pass 0 : Water
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_WATER();
		pixelshader = compile ps_3_0 PS_WATER();
	}

	pass	// Pass 1 : Noise
	{
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_NOISE();
	}
}
