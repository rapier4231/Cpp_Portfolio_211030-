/////////////////////////////////////////////////
// 상수 테이블
matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_DiffuseTexture;
texture			g_NormalTexture;
texture			g_SpecularTexture;
texture			g_AlphaTexture;

bool			g_bNoneNormalTexture;

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
};
sampler			SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
};
sampler			AlphaSampler = sampler_state
{
	texture = g_AlphaTexture;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 디퓨즈
// 버텍스 쉐이더
struct VS_IN
{
	vector			vPosition	: POSITION;			// 시멘틱
	float2			vTexUV		: TEXCOORD0;
	vector			vNormal		: NORMAL;
};

struct VS_OUT
{
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;		// 디퓨즈
	vector			vNormal		: NORMAL;
	vector			vProjPos	: TEXCOORD1;
};
// 버텍스 쉐이더 메인
VS_OUT VS_D(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV			= mul(g_matWorld, g_matView);		// 월드 * 뷰 
	matWVP			= mul(matWV, g_matProj);			// * 투영

	Out.vPosition	= mul(In.vPosition, matWVP);		// 정점을 투영까지 곱해준 행렬로 다시 곱한다.
	Out.vTexUV		= In.vTexUV;						// 텍스쳐는 그냥 내보낸다.
	Out.vNormal		= normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));	// 노말백터
	Out.vProjPos	= Out.vPosition;

	return Out;											// 변환된 정점 반환
}
// 픽셀 쉐이더
struct PS_IN
{
	float2		vTexUV		: TEXCOORD0;
	vector		vNormal		: NORMAL;
	vector		vProjPos	: TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor		: COLOR0;
	vector		vNormal		: COLOR1;
	vector		vSpecular	: COLOR2;
	vector		vDepth		: COLOR3;
};

// 픽셀 쉐이더 메인
PS_OUT PS_D(PS_IN In)
{
	PS_OUT  Out		= (PS_OUT)0;

	Out.vColor		= tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor.a	= 1.f;

	Out.vNormal		= vector(In.vNormal.xyz * 0.5f + 0.5, 0.f);
	Out.vSpecular	= vector(0.f, 0.f, 0.f, 0.f);
	Out.vDepth		= vector(In.vProjPos.z / In.vProjPos.w,
							In.vProjPos.w / 1000.f,
							0.f,
							0.f);

	//Out.vColor = tex2D(BaseSampler, In.vTexUV); // 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하는 함수(반환타입은 vector 타입)
	//Out.vColor.rg = 0.5f;

	return Out;
}

// 와이어 + Red
PS_OUT PS_D_WIRE_RED(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor.ra = 1.f;
	Out.vColor.gb = 0.f;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5, 0.f);
	Out.vSpecular = vector(0.f, 0.f, 0.f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,
		In.vProjPos.w / 1000.f,
		0.f,
		0.f);

	return Out;
}

// 와이어 + Blue
PS_OUT PS_D_WIRE_BLUE(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor.ba = 1.f;
	Out.vColor.rg = 0.f;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5, 0.f);
	Out.vSpecular = vector(0.f, 0.f, 0.f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,
		In.vProjPos.w / 1000.f,
		0.f,
		0.f);

	return Out;
}

// 와이어 + Yellow
PS_OUT PS_D_WIRE_YELLOW(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor.rga = 1.f;
	Out.vColor.b = 0.f;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5, 0.f);
	Out.vSpecular = vector(0.f, 0.f, 0.f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w,
		In.vProjPos.w / 1000.f,
		0.f,
		0.f);

	return Out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 디퓨즈 노말
// 버텍스 쉐이더
struct VS_IN_DN
{
	vector			vPosition	: POSITION;			// 시멘틱
	float2			vTexUV		: TEXCOORD0;
};

struct VS_OUT_DN
{
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;		// 디퓨즈
	vector			vProjPos	: TEXCOORD1;
};
// 버텍스 쉐이더 메인
VS_OUT_DN VS_DN(VS_IN_DN In)
{
	VS_OUT_DN  Out	= (VS_OUT_DN)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	Out.vProjPos = Out.vPosition;

	return Out;
}

// 픽셀 쉐이더
struct PS_IN_DN
{
	float2		vTexUV		: TEXCOORD0;
	vector		vProjPos	: TEXCOORD1;
};

struct PS_OUT_DN
{
	vector		vColor		: COLOR0;
	vector		vDepth		: COLOR1;
};

float3 Gamma_LinearSpace(float3 _rgb)
{
	return pow(abs(_rgb), 2.2f);
}

// 픽셀 쉐이더 메인
PS_OUT_DN PS_DN(PS_IN_DN In)
{
	PS_OUT_DN  Out = (PS_OUT_DN)0;

	Out.vColor		= tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor.rgb	= Gamma_LinearSpace(Out.vColor.rgb);
	Out.vColor.a	= 1.f;

	Out.vDepth		= vector(In.vProjPos.z / In.vProjPos.w,	In.vProjPos.w / 1000.f,	0.f, 0.f);

	return Out;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 디퓨즈 스페큘러
// 버텍스 쉐이더
struct VS_IN_DS
{
	vector			vPosition	: POSITION;			// 시멘틱
	float2			vTexUV		: TEXCOORD0;
	vector			vNormal		: NORMAL;
};

struct VS_OUT_DS
{
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;		// 디퓨즈
	vector			vNormal		: NORMAL;
	vector			vProjPos	: TEXCOORD1;
};
// 버텍스 쉐이더 메인
VS_OUT_DS VS_DS(VS_IN_DS In)
{
	VS_OUT_DS  Out = (VS_OUT_DS)0;

	matrix matWV, matWVP;

	matWV			= mul(g_matWorld, g_matView);		// 월드 * 뷰 
	matWVP			= mul(matWV, g_matProj);			// * 투영

	Out.vPosition	= mul(In.vPosition, matWVP);		// 정점을 투영까지 곱해준 행렬로 다시 곱한다.
	Out.vTexUV		= In.vTexUV;						// 텍스쳐는 그냥 내보낸다.
	Out.vNormal		= normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));	// 노말백터
	Out.vProjPos	= Out.vPosition;

	return Out;											// 변환된 정점 반환
}

// 픽셀 쉐이더
struct PS_IN_DS
{
	float2		vTexUV		: TEXCOORD0;
	vector		vNormal		: NORMAL;
	vector		vProjPos	: TEXCOORD1;
};

struct PS_OUT_DS
{
	vector		vColor		: COLOR0;
	vector		vNormal		: COLOR1;
	vector		vSpecular	: COLOR2;
	vector		vDepth		: COLOR3;
};

// 픽셀 쉐이더 메인
PS_OUT_DS PS_DS(PS_IN_DS In)
{
	PS_OUT_DS  Out	= (PS_OUT_DS)0;

	Out.vColor		= tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor.a	= 1.f;

	Out.vNormal		= vector(In.vNormal.xyz * 0.5f + 0.5, 0.f);
	Out.vSpecular	= tex2D(SpecularSampler, In.vTexUV);
	Out.vDepth		= vector(In.vProjPos.z / In.vProjPos.w,
							In.vProjPos.w / 1000.f,
							0.f,
							0.f);

	return Out;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 디퓨즈 노말 스펙큘러
struct VS_IN_DNS
{
	vector			vPosition	: POSITION;			// 시멘틱
	float2			vTexUV		: TEXCOORD0;
	vector			vNormal		: NORMAL;
};

struct VS_OUT_DNS
{
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;		// 디퓨즈
	vector			vNormal		: NORMAL;
	vector			vProjPos	: TEXCOORD1;
};
VS_OUT_DNS VS_DNS(VS_IN_DNS In)
{
	VS_OUT_DNS  Out = (VS_OUT_DNS)0;

	matrix matWV, matWVP;

	matWV			= mul(g_matWorld, g_matView);		// 월드 * 뷰 
	matWVP			= mul(matWV, g_matProj);			// * 투영

	Out.vPosition	= mul(In.vPosition, matWVP);		// 정점을 투영까지 곱해준 행렬로 다시 곱한다.
	Out.vTexUV		= In.vTexUV;						// 텍스쳐는 그냥 내보낸다.
	Out.vNormal		= normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));	// 노말백터
	Out.vProjPos	= Out.vPosition;

	return Out;											// 변환된 정점 반환
}

// 픽셀 쉐이더
struct PS_IN_DNS
{
	float2		vTexUV		: TEXCOORD0;
	vector		vNormal		: NORMAL;
	vector		vProjPos	: TEXCOORD1;
};

struct PS_OUT_DNS
{
	vector		vColor		: COLOR0;
	vector		vNormal		: COLOR1;
	vector		vSpecular	: COLOR2;
	vector		vDepth		: COLOR3;
};

// 픽셀 쉐이더 메인
PS_OUT_DNS PS_DNS(PS_IN_DNS In)
{
	PS_OUT_DNS  Out = (PS_OUT_DNS)0;

	Out.vColor		= tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor.a	= 1.f;

	//Out.vNormal = tex2D(NormalSampler, In.vTexUV);
	Out.vNormal		= saturate(tex2D(NormalSampler, In.vTexUV) * In.vNormal);
	Out.vSpecular	= tex2D(SpecularSampler, In.vTexUV);

	Out.vDepth		= vector(In.vProjPos.z / In.vProjPos.w,
							In.vProjPos.w / 1000.f,
							0.f,
							0.f);
	return Out;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
PS_OUT		PS_ALPHATEST(PS_IN In)
{
	PS_OUT	Out		= (PS_OUT)0;

	Out.vColor		= tex2D(DiffuseSampler, In.vTexUV);
	vector Alpha	= tex2D(AlphaSampler, In.vTexUV);

	Out.vColor.a	= Alpha.x;

	return Out;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 테크니크
technique		Default_Device
{
	pass Diffuse			// 0 Pass
	{
		vertexshader = compile vs_3_0 VS_D();
		pixelshader = compile ps_3_0 PS_D();
	}

	pass DiffuseNormal		// 1 Pass
	{
		vertexshader = compile vs_3_0 VS_DN();
		pixelshader = compile ps_3_0 PS_DN();
	}

	pass DiffuseSpec		// 2 Pass
	{
		vertexshader = compile vs_3_0 VS_DS();
		pixelshader = compile ps_3_0 PS_DS();
	}

	pass DiffuseNormalSpec	// 3 Pass
	{
		vertexshader = compile vs_3_0 VS_DNS();
		pixelshader = compile ps_3_0 PS_DNS();
	}

	pass	alphatest		// 4 Pass
	{
		alphatestenable = true;
		alpharef = 0xf0;
		alphafunc = greater;
	
		vertexshader = compile vs_3_0 VS_D();
		pixelshader = compile ps_3_0 PS_ALPHATEST();
	}

	pass	Diffuse_Wire_Red	// 5 Pass
	{
		cullmode = none;
		fillmode = wireframe;

		vertexshader = compile vs_3_0 VS_D();
		pixelshader = compile ps_3_0 PS_D_WIRE_RED();
	}

	pass	Diffuse_Wire_Blue	// 6 Pass
	{
		cullmode = none;
		fillmode = wireframe;

		vertexshader = compile vs_3_0 VS_D();
		pixelshader = compile ps_3_0 PS_D_WIRE_BLUE();
	}

	pass	Diffuse_Wire_Yellow	// 7 Pass
	{
		cullmode = none;
		fillmode = wireframe;

		vertexshader = compile vs_3_0 VS_D();
		pixelshader = compile ps_3_0 PS_D_WIRE_YELLOW();
	}
}

/////////////////////////////////////////////////