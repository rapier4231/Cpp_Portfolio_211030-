float			g_Alpha;
float			g_Rect_Start_Ratio;
float			g_Rect_End_Ratio;
matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

struct VS_IN
{
	vector			vPosition : POSITION;
	float2			vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector			vPosition : POSITION;
	float2			vTexUV : TEXCOORD0;
};

// 버텍스 쉐이더
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(In.vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

// 픽셀 쉐이더 메인
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	if (g_Rect_Start_Ratio < In.vTexUV.g &&  g_Rect_End_Ratio > In.vTexUV.g) // 스크롤 렉트 그려야 하는 부분
	{
		Out.vColor.r = 0.3f;
		Out.vColor.g = 0.8f;
		Out.vColor.b = 0.9f;
		Out.vColor.a = g_Alpha;
	}
	else
	{
		if (0.1f > In.vTexUV.r || 0.9f < In.vTexUV.r)
			Out.vColor.a = 0.f;
		else
		{
			Out.vColor.r = 0.2f;
			Out.vColor.g = 0.7f;
			Out.vColor.b = 0.8f;
			Out.vColor.a = g_Alpha*0.5f;
		}
	}
	return Out;
}

// 테크니퀵
technique		Default_Device
{
	pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
}

/////////////////////////////////////////////////