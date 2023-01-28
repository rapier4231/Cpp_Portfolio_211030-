vector			g_EdgeSize;
float			g_PercentStat;

matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_Bg_Tex;
texture			g_Stat_Tex;
texture			g_StatFill_Tex;

sampler			BgSampler = sampler_state
{
	texture = g_Bg_Tex;
	AddressU = clamp;
	AddressV = clamp;
	minfilter = linear;
	magfilter = linear;
};

sampler			StatSampler = sampler_state
{
	texture = g_Stat_Tex;
	AddressU = clamp;
	AddressV = clamp;
	minfilter = linear;
	magfilter = linear;
};

sampler			StatFillSampler = sampler_state
{
	texture = g_StatFill_Tex;
AddressU = clamp;
AddressV = clamp;
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

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	float2 FixUV = In.vTexUV;
	FixUV.r = In.vTexUV.r * 0.8f + 0.1f;
	FixUV.g = In.vTexUV.g * 0.8f + 0.1f;
	Out.vColor = tex2D(BgSampler, FixUV);

	Out.vColor.a = 0.1f;
	return Out;
}

PS_OUT PS_Stat(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	In.vTexUV.g = In.vTexUV.g + 0.01f;
	Out.vColor = tex2D(StatSampler, In.vTexUV);

	if (In.vTexUV.g < g_PercentStat)
	{
		if (Out.vColor.a)
		{
			Out.vColor.r = 0.8f;
			Out.vColor.g = 0.9f;
			Out.vColor.b = 1.f;
			Out.vColor.a = 0.6f;
			return Out;
		}
		else
		{
			Out.vColor.a = 0.f;

			return Out;
		}
	}
	else
	{
		if (Out.vColor.a)
		{
			Out.vColor.r = 0.35f;
			Out.vColor.g = 0.8f;
			Out.vColor.b = 1.f;
			Out.vColor.a = 0.6f;
			return Out;
		}
		else
		{
			Out.vColor.a = 0.f;
			return Out;
		}
	}


	return Out;
}

PS_OUT PS_JoBar(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	if (In.vTexUV.r > g_PercentStat)
	{
		Out.vColor.r = 0.8f;
		Out.vColor.g = 0.8f;
		Out.vColor.b = 0.8f;
		Out.vColor.a = 0.3f;
		return Out;
	}
	else
	{
		Out.vColor.r = 0.9f;
		Out.vColor.g = 0.9f;
		Out.vColor.b = 0.4f;
		Out.vColor.a = 0.6f;
		return Out;
	}
	return Out;
}

PS_OUT PS_DinoHead(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	Out.vColor = tex2D(StatSampler, In.vTexUV);
	Out.vColor.a = Out.vColor.a * 0.65f;
	return Out;
}

PS_OUT PS_horizontal_line(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	Out.vColor = tex2D(StatSampler, In.vTexUV);
	Out.vColor.a = Out.vColor.a * 0.46f;
	return Out;
}

PS_OUT PS_vertical_line(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	float2 FixUV = In.vTexUV;
	FixUV.r = In.vTexUV.r * 0.5f;
	FixUV.g = In.vTexUV.g * 0.5f;
	//Out.vColor = tex2D(StatSampler, FixUV);
	vector      vBase	= tex2D(StatSampler, FixUV);
	vector		vFilter = tex2D(StatFillSampler, FixUV);
	vFilter.g = vFilter.r;
	vFilter.b = vFilter.r;

	//g_PercentStat 범위는 0~2.f //오른쪽 부터 만들자
	if ((In.vTexUV.r < ((g_PercentStat*0.25f+0.5f) + 0.01f) && In.vTexUV.r > ((g_PercentStat*0.25f + 0.5f) - 0.01f)		)	 ||
		(In.vTexUV.r < ((0.5f -g_PercentStat*0.25f) + 0.01f) && In.vTexUV.r >((0.5f - g_PercentStat*0.25f) - 0.01f)))
	{
		Out.vColor = (vBase*0.9f) + (vFilter);
		Out.vColor.a = 0.3f;
	}
	else if (( In.vTexUV.r < ((g_PercentStat*0.25f + 0.5f) + 0.02f) &&  In.vTexUV.r > ((g_PercentStat*0.25f + 0.5f) - 0.02f))	  ||
		    (  In.vTexUV.r < ((0.5f - g_PercentStat*0.25f) + 0.02f) &&  In.vTexUV.r >((0.5f - g_PercentStat*0.25f) - 0.02f)))
	{
		Out.vColor = (vBase*0.9f) + (vFilter * 0.7f);
		Out.vColor.a = 0.3f;
	}
	else if (( In.vTexUV.r < ((g_PercentStat*0.25f + 0.5f) + 0.04f) &&  In.vTexUV.r > ((g_PercentStat*0.25f + 0.5f) - 0.04f)	)   ||
			 ( In.vTexUV.r < ((0.5f - g_PercentStat*0.25f) + 0.04f) &&  In.vTexUV.r >((0.5f - g_PercentStat*0.25f)) - 0.04f))
	{
		Out.vColor = (vBase*0.9f) + (vFilter * 0.4f);
		Out.vColor.a = 0.3f;
	}
	else
	{
		Out.vColor = (vBase*0.9f);
		Out.vColor.a = 0.1f;
	}

	return Out;
}

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

	pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_Stat();
	}

		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_JoBar();
	}

		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_DinoHead();
	}

		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_horizontal_line();
	}

		pass
	{
		alphablendenable = true;
		srcblend = one;
		destblend = one;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_vertical_line();
	}
}

/////////////////////////////////////////////////