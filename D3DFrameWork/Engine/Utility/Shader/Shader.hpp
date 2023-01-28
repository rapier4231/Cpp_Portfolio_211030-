// 상수 테이블
matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

sampler			BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

float			g_fAlpha;

/////////////////////////////////////////////////
// 버텍스 쉐이더
struct VS_IN
{
	vector			vPosition	: POSITION;			// 시멘틱
	float2			vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;
};


// 버텍스 쉐이더 메인
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV			= mul(g_matWorld, g_matView);		// 월드 * 뷰 
	matWVP			= mul(matWV, g_matProj);			// * 투영

	Out.vPosition	= mul(In.vPosition, matWVP);		// 정점을 투영까지 곱해준 행렬로 다시 곱한다.
	Out.vTexUV		= In.vTexUV;						// 텍스쳐는 그냥 내보낸다.

	return Out;											// 변환된 정점 반환
}
/////////////////////////////////////////////////
// 픽셀 쉐이더
struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
	//vector		vNormal : COLOR1;
	//vector		vSpecular : COLOR2;
};

// 픽셀 쉐이더 메인
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.a = 1.f;
	//Out.vNormal.a = 0.f; 
	//Out.vNormal = vector(0.f, 1.f, 0.f, 0.f);
	//Out.vSpecular.a = 0.f;

	//if (0.5f > In.vTexUV.x)
	//{
	//	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	//	Out.vColor.r = 0.5f;
	//}
	//else
	//{
	//	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	//	Out.vColor.g = 0.5f;
	//}

	//Out.vColor = tex2D(BaseSampler, In.vTexUV); // 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하는 함수(반환타입은 vector 타입)
	
	//Out.vColor.rg = 0.5f;

	return Out;
}

PS_OUT		PS_LOADINGBACKGROUND(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하는 함수(반환타입은 vector타입)

	Out.vColor.a = Out.vColor.a * g_fAlpha;

	return Out;
}

technique		Default_Device
{
	pass
	{
		lighting = false;
		zwriteenable = false;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass // Pass 1, 로딩 백그라운드 전용
	{
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		cullmode = none;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_LOADINGBACKGROUND();
	}

	pass // Pass 2, 로딩 백그라운드 아이콘 전용
	{
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		cullmode = none;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_LOADINGBACKGROUND();
	}
}

/////////////////////////////////////////////////