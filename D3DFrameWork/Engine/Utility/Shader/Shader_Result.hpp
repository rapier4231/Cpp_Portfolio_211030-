float		g_fRange = 0.01f;

texture		g_BlurTexture;
texture		g_BlendTexture;

sampler BlurSampler = sampler_state
{
	texture = g_BlurTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler BlendSampler = sampler_state
{
	texture = g_BlendTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	addressu = clamp;
	addressv = clamp;
};

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor	: COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	vector	vBlur = tex2D(BlurSampler, In.vTexUV);
	vector	vBlend = tex2D(BlendSampler, In.vTexUV);

	vector	vResult = vBlend + vBlur;

	Out.vColor = vResult;
	
	return Out;
}

PS_OUT PS_MOTIONBLUR(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	vector	vBlur = tex2D(BlurSampler, In.vTexUV);

	float2	vTexCoord = In.vTexUV;
	float2	vDir = 0.5f - vTexCoord;

	float	fDist = length(vDir);

	vDir /= fDist;

	vector	vColor = tex2D(BlendSampler, vTexCoord);
	vector	vSum = vColor;

	float	fSamples[10] = { -0.08f, -0.05f, -0.03f, -0.02f, -0.01f, 0.01f, 0.02f, 0.03f, 0.05f, 0.08f };

	for (int i = 0; i < 10; i++)
	{
		vSum += tex2D(BlendSampler, vTexCoord + vDir * fSamples[i] * g_fRange);
	}

	vSum *= 1.f / 11.f;

	float	fT = saturate(fDist * 2.2f);

	Out.vColor = lerp(vColor, vSum, fT) + vBlur;

	return Out;
}

technique		Default_Device
{
	pass // Pass 0 : Result
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		
		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass // Pass 1 : Result + MotionBlur
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MOTIONBLUR();
	}
}