// ��� ���̺�
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
// ���ؽ� ���̴�
struct VS_IN
{
	vector			vPosition	: POSITION;			// �ø�ƽ
	float2			vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;
};


// ���ؽ� ���̴� ����
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV			= mul(g_matWorld, g_matView);		// ���� * �� 
	matWVP			= mul(matWV, g_matProj);			// * ����

	Out.vPosition	= mul(In.vPosition, matWVP);		// ������ �������� ������ ��ķ� �ٽ� ���Ѵ�.
	Out.vTexUV		= In.vTexUV;						// �ؽ��Ĵ� �׳� ��������.

	return Out;											// ��ȯ�� ���� ��ȯ
}
/////////////////////////////////////////////////
// �ȼ� ���̴�
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

// �ȼ� ���̴� ����
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

	//Out.vColor = tex2D(BaseSampler, In.vTexUV); // 2���� �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� �����ϴ� �Լ�(��ȯŸ���� vector Ÿ��)
	
	//Out.vColor.rg = 0.5f;

	return Out;
}

PS_OUT		PS_LOADINGBACKGROUND(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� �����ϴ� �Լ�(��ȯŸ���� vectorŸ��)

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

	pass // Pass 1, �ε� ��׶��� ����
	{
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		cullmode = none;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_LOADINGBACKGROUND();
	}

	pass // Pass 2, �ε� ��׶��� ������ ����
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