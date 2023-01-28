matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;
matrix			g_matLastWorldView;

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
	vector		vDir		: TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	vector	vNewPos = Out.vPosition;
	vector	vOldPos = mul(vector(In.vPosition, 1.f), g_matLastWorldView);
	vOldPos = mul(vOldPos, g_matProj);

	float3	vDir = vNewPos.xyz - vOldPos.xyz;

	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), matWV));
	Out.vTexUV = In.vTexUV;

	float	fA = dot(normalize(vDir), normalize(Out.vNormal));
	if (0.f > fA)
		Out.vPosition = vOldPos;
	else
		Out.vPosition = vNewPos;

	float2	vVelocity = (vNewPos.xy / vNewPos.w) - (vOldPos.xy / vOldPos.w);
	Out.vDir.xy = vVelocity * 0.5f;
	Out.vDir.y *= -1.f;
	Out.vDir.z = Out.vPosition.z;
	Out.vDir.w = Out.vPosition.w;

	return Out;
}

VS_OUT VS_TEST(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	// World * View
	//matrix	matWV = mul(g_matWorld, g_matView);

	//vector	x1 = mul(In.vPosition, matWV);					// 현재 뷰좌표
	//vector	x0 = mul(In.vPosition, g_matLastWorldView);		// 1프레임 직전 뷰좌표
	//vector	v = x1 - x0;									// 속도
	//float3	n = mul(In.vNormal, matWV);						// 뷰좌표계에서의 법선

	//bool	bFront = (0 <= dot(n, v.xyz));					// 속도방향으로 향하고 있는지?
	//vector	x = bFront ? x1 : x0;							// 방향에따라 위치결정
	//x.w = 1.f;

	//Out.vPosition = mul(x, g_matProj);						// 투영공간으로

	//// 투영공간에서 텍스처공간으로 변환
	//Out.vTex.x = Out.vPosition.x + Out.vPosition.w;
	//Out.vTex.y = -Out.vPosition.y + Out.vPosition.w;
	//Out.vTex.w = 2.f * Out.vPosition.w;

	//// 텍스처좌표로 속도를 구한다
	//vector s0 = mul(x0, g_matProj);
	//s0 /= s0.w;

	//vector s1 = mul(x1, g_matProj);
	//s1 /= s1.w;

	//Out.vVelocity = s1 - s0;
	//Out.vVelocity.x *= +0.5f;
	//Out.vVelocity.y *= -0.5f;

	//Out.vTexUV = In.vTexUV;

	//// Normal과 Tangent를 외적하여 Binormal 산출
	//Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));

	//Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float2		vTexUV		: TEXCOORD0;
	float3		vNormal		: TEXCOORD1;
	vector		vDir		: TEXCOORD2;
};

struct PS_OUT
{
	vector		vMotionBlur		: COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	// MotionBlur
	Out.vMotionBlur.xy = In.vDir.xy;
	Out.vMotionBlur.z = 1.f;
	Out.vMotionBlur.w = In.vDir.z / In.vDir.w;

	return Out;
}

PS_OUT PS_TEST(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	//int		iSample = 26;
	//float	fSample = (float)iSample;

	//for (int i = 0; i < iSample; i++)
	//{
	//	float t = (float)(i + 1) / fSample;
	//	Out.vMotionBlur += tex2D(DiffuseSampler, In.vTex / In.vTex.w + t * In.vVelocity);
	//}

	//Out.vMotionBlur /= fSample;

	//// Diffuse
	//vector Color = tex2D(DiffuseSampler, In.vTexUV) * g_vSetColor;
	//Out.vColor.rgb = Gamma_LinearSpace(Color.rgb);
	//Out.vColor.a = 1.f;

	//// Depth
	//Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);

	//// Normal
	//vector		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	//float3		vNormal = normalize(vNormalTex.rgb * 2.f - 1.f);
	//float3x3	matWorld = float3x3(normalize(In.vTangent), normalize(In.vBinormal), normalize(In.vNormal));

	//vNormal = mul(vNormal, matWorld);

	//Out.vNormalResult = vector(vNormal * 0.5f + 0.5f, 0.f);

	return Out;
}

technique		Default_Device
{
	pass	// Pass 0 : MotionBlur
	{
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	// Pass 1 : MotionBlur
	{
		alphablendenable = true;
		srcblend = one;
		destblend = invsrcalpha;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	// Pass 2 : Test
	{
		vertexshader = compile vs_3_0 VS_TEST();
		pixelshader = compile ps_3_0 PS_TEST();
	}
}
