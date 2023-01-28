float			g_Alpha;

matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture		g_BaseTexture;
sampler			BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

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

// ¹öÅØ½º ½¦ÀÌ´õ
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

// ÇÈ¼¿ ½¦ÀÌ´õ ¸ÞÀÎ
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	vector		vBase = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = vBase;

	Out.vColor.a = g_Alpha *vBase.a;

	return Out;
}

// Å×Å©´ÏÄü
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