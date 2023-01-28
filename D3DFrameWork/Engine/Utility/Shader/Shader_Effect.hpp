matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

float3			g_vObjColor;
float2			g_vTexUV;

float			g_fAlpha;
float			g_fTime;

int				g_iTextureX;
int				g_iTextureY;

texture			g_DiffuseTexture;
texture			g_DepthTexture;
texture			g_NoiseTexture;
texture			g_SubTexture;
texture			g_NormalTexture;

sampler			DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler			DepthSampler = sampler_state
{
	texture = g_DepthTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler			NoiseSampler = sampler_state
{
	texture = g_NoiseTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler			SubSampler = sampler_state
{
	texture = g_SubTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
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
	vector			vPosition	: POSITION;			// Ω√∏‡∆Ω
	float2			vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector			vPosition	: POSITION;
	float2			vTexUV		: TEXCOORD0;		// µ«ª¡Ó
	vector			vProjPos	: TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(In.vPosition, matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float2		vTexUV		: TEXCOORD0;
	vector		vProjPos	: TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor	: COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, float2(In.vTexUV.x / g_iTextureX, In.vTexUV.y / g_iTextureY) + g_vTexUV);

	float2		vDepthUV = (float2)0.f;

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float fViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;

	Out.vColor.rgb *= g_vObjColor;

	if (0 < fViewZ - In.vProjPos.w)
		Out.vColor.a = Out.vColor.a * g_fAlpha;
	else
		Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w) * g_fAlpha;
	return Out;
}

PS_OUT PS_RED(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, float2(In.vTexUV.x / g_iTextureX, In.vTexUV.y / g_iTextureY) + g_vTexUV);

	float2		vDepthUV = (float2)0.f;

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float fViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;

	Out.vColor.rgb *= g_vObjColor;
	Out.vColor.r = 1.f;

	if (0 < fViewZ - In.vProjPos.w)
		Out.vColor.a = Out.vColor.a * g_fAlpha;
	else
		Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w) * g_fAlpha;

	return Out;
}

PS_OUT PS_NOISE(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector  vNoise = tex2D(NoiseSampler, float2(In.vTexUV.x, In.vTexUV.y + g_vTexUV.y)) * 0.5f;
	vector	vSubTex = tex2D(SubSampler, saturate(In.vTexUV + vNoise.xy));

	Out.vColor = tex2D(DiffuseSampler, saturate(In.vTexUV + vNoise.xy)) * vSubTex;

	float2		vDepthUV = (float2)0.f;

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float fViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;

	Out.vColor.rgb *= g_vObjColor;

	if (0 < fViewZ - In.vProjPos.w)
		Out.vColor.a = Out.vColor.a * g_fAlpha;
	else
		Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w) * g_fAlpha;

	return Out;
}

PS_OUT PS_NORMAL(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, float2(In.vTexUV.x / g_iTextureX, In.vTexUV.y / g_iTextureY) + g_vTexUV);

	vector vNormal = tex2D(NormalSampler, float2(In.vTexUV.x / g_iTextureX, In.vTexUV.y / g_iTextureY) + g_vTexUV);

	float2		vDepthUV = (float2)0.f;

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float fViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;

	Out.vColor.rgb *= g_vObjColor * vNormal.rgb;
	//Out.vColor.r = 1.f;

	if(0 < fViewZ - In.vProjPos.w)
		Out.vColor.a = Out.vColor.a * g_fAlpha;
	else
		Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w) * g_fAlpha;

	return Out;
}

PS_OUT PS_NONEALPHA(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, g_vTexUV);

	float2		vDepthUV = (float2)0.f;

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float fViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;

	Out.vColor.rgb *= g_vObjColor;
	Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w) * g_fAlpha;

	return Out;
}


PS_OUT PS_BUFF(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector  vNoise = tex2D(NoiseSampler, In.vTexUV /*+ g_fTime*/) * 0.5f;

	Out.vColor = tex2D(DiffuseSampler, saturate(In.vTexUV/* + vNoise.xy*/));

	float2		vDepthUV = (float2)0.f;

	vDepthUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vDepthUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float fViewZ = tex2D(DepthSampler, vDepthUV).y * 1000.f;

	Out.vColor.rgb *= g_vObjColor;

	if (0 < fViewZ - In.vProjPos.w)
		Out.vColor.a = Out.vColor.a * g_fAlpha;
	else
		Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w) * g_fAlpha;

	return Out;
}

technique		Default_Device
{
	pass	// Pass 0 : Effect
	{
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	// Pass 1 : Effect - RedColor
	{
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_RED();
	}

	pass	// Pass 2 : Effect - Use Noise
	{
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_NOISE();
	}

	pass	// Pass 3 : Effect - Use Normal
	{
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_NORMAL();
	}

	pass	// Pass 4 : Effect - NoneAlpha
	{
		//zwriteenable = true;
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_NONEALPHA();
	}

	pass	// Pass 5 : Effect - Buff
	{
		zwriteenable = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_BUFF();
	}
}