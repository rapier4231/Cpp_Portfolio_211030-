/////////////////////////////////////////////////
// ��� ���̺�
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
/// ��ǻ��
// ���ؽ� ���̴�
struct VS_IN
{
	vector			vPosition	: POSITION;			// �ø�ƽ
	float2			vTexUV		: TEXCOORD0;
	vector			vNormal		: NORMAL;
};

struct VS_OUT
{
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;		// ��ǻ��
	vector			vNormal		: NORMAL;
	vector			vProjPos	: TEXCOORD1;
};
// ���ؽ� ���̴� ����
VS_OUT VS_D(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV			= mul(g_matWorld, g_matView);		// ���� * �� 
	matWVP			= mul(matWV, g_matProj);			// * ����

	Out.vPosition	= mul(In.vPosition, matWVP);		// ������ �������� ������ ��ķ� �ٽ� ���Ѵ�.
	Out.vTexUV		= In.vTexUV;						// �ؽ��Ĵ� �׳� ��������.
	Out.vNormal		= normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));	// �븻����
	Out.vProjPos	= Out.vPosition;

	return Out;											// ��ȯ�� ���� ��ȯ
}
// �ȼ� ���̴�
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

// �ȼ� ���̴� ����
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

	//Out.vColor = tex2D(BaseSampler, In.vTexUV); // 2���� �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� �����ϴ� �Լ�(��ȯŸ���� vector Ÿ��)
	//Out.vColor.rg = 0.5f;

	return Out;
}

// ���̾� + Red
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

// ���̾� + Blue
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

// ���̾� + Yellow
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
/// ��ǻ�� �븻
// ���ؽ� ���̴�
struct VS_IN_DN
{
	vector			vPosition	: POSITION;			// �ø�ƽ
	float2			vTexUV		: TEXCOORD0;
};

struct VS_OUT_DN
{
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;		// ��ǻ��
	vector			vProjPos	: TEXCOORD1;
};
// ���ؽ� ���̴� ����
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

// �ȼ� ���̴�
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

// �ȼ� ���̴� ����
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
/// ��ǻ�� ����ŧ��
// ���ؽ� ���̴�
struct VS_IN_DS
{
	vector			vPosition	: POSITION;			// �ø�ƽ
	float2			vTexUV		: TEXCOORD0;
	vector			vNormal		: NORMAL;
};

struct VS_OUT_DS
{
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;		// ��ǻ��
	vector			vNormal		: NORMAL;
	vector			vProjPos	: TEXCOORD1;
};
// ���ؽ� ���̴� ����
VS_OUT_DS VS_DS(VS_IN_DS In)
{
	VS_OUT_DS  Out = (VS_OUT_DS)0;

	matrix matWV, matWVP;

	matWV			= mul(g_matWorld, g_matView);		// ���� * �� 
	matWVP			= mul(matWV, g_matProj);			// * ����

	Out.vPosition	= mul(In.vPosition, matWVP);		// ������ �������� ������ ��ķ� �ٽ� ���Ѵ�.
	Out.vTexUV		= In.vTexUV;						// �ؽ��Ĵ� �׳� ��������.
	Out.vNormal		= normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));	// �븻����
	Out.vProjPos	= Out.vPosition;

	return Out;											// ��ȯ�� ���� ��ȯ
}

// �ȼ� ���̴�
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

// �ȼ� ���̴� ����
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
/// ��ǻ�� �븻 ����ŧ��
struct VS_IN_DNS
{
	vector			vPosition	: POSITION;			// �ø�ƽ
	float2			vTexUV		: TEXCOORD0;
	vector			vNormal		: NORMAL;
};

struct VS_OUT_DNS
{
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;		// ��ǻ��
	vector			vNormal		: NORMAL;
	vector			vProjPos	: TEXCOORD1;
};
VS_OUT_DNS VS_DNS(VS_IN_DNS In)
{
	VS_OUT_DNS  Out = (VS_OUT_DNS)0;

	matrix matWV, matWVP;

	matWV			= mul(g_matWorld, g_matView);		// ���� * �� 
	matWVP			= mul(matWV, g_matProj);			// * ����

	Out.vPosition	= mul(In.vPosition, matWVP);		// ������ �������� ������ ��ķ� �ٽ� ���Ѵ�.
	Out.vTexUV		= In.vTexUV;						// �ؽ��Ĵ� �׳� ��������.
	Out.vNormal		= normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));	// �븻����
	Out.vProjPos	= Out.vPosition;

	return Out;											// ��ȯ�� ���� ��ȯ
}

// �ȼ� ���̴�
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

// �ȼ� ���̴� ����
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
// ��ũ��ũ
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