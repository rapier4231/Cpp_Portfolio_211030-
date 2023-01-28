float2		g_avSampleOffsets[16];

texture		g_HDRSourceTexture;

sampler HDRSourceSampler = sampler_state
{
	texture = g_HDRSourceTexture;
};

struct PS_IN
{
	float2		vTexUV		: TEXCOORD0;
};

struct PS_OUT
{
	vector		vHDRSource	: COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out			= (PS_OUT)0;

	vector	vHDRSource = tex2D(HDRSourceSampler, In.vTexUV);
	
	float fHDRScalar = 1.f;

	Out.vHDRSource = vector(vHDRSource.x * fHDRScalar, vHDRSource.y * fHDRScalar, vHDRSource.z * fHDRScalar, 1.f);

	return Out;
}

PS_OUT PS_MAIN2(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDownSample = 0.0f;

	for (int i = 0; i < 16; i++) {
		vDownSample += tex2D(HDRSourceSampler, In.vTexUV + g_avSampleOffsets[i]);
	}

	Out.vHDRSource = vDownSample / 16;

	return Out;
}

PS_OUT PS_MAIN3(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vHDRSource = tex2D(HDRSourceSampler, In.vTexUV);

	// 어두운 부분 제외
	Out.vHDRSource.rgb -= 1.5f;

	// 하한값을 0으로
	Out.vHDRSource = 3.0f * max(Out.vHDRSource, 0.0f);

	return Out;
}

technique		Default_Device
{
	pass	// Pass 0
	{
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	// Pass 1
	{
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN2();
	}

	pass	// Pass 2
	{
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN3();
	}
}