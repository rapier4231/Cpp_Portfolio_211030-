texture		g_BlendTexture;

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
	float2		vTexUV		: TEXCOORD0;
};

struct PS_OUT
{
	vector		vBloom		: COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out			= (PS_OUT)0;

	vector	vBlend = tex2D(BlendSampler, In.vTexUV);

	// Bloom
	float brightness = dot(vBlend.rgb, float3(0.2126f, 0.7152f, 0.0722f));

	if (0.6f < brightness)
		Out.vBloom = float4(vBlend.rgb * 1.f, 1.f);

	return Out;
}

technique		Default_Device
{
	pass	// Pass 0
	{
		alphablendenable = true;
		srcblend = one;
		destblend = one;
		
		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}
}