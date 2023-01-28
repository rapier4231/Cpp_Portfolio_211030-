float			g_Alpha;
float			g_PercentStat;
bool			g_Blink;

matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture		g_BaseTexture;
texture		g_BoxTexture;

sampler			BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

sampler			BoxSampler = sampler_state
{
	texture = g_BoxTexture;

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
	In.vTexUV.g = In.vTexUV.g + 0.01f;
	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	vector	vBox = tex2D(BoxSampler, In.vTexUV);
	Out.vColor.r = Out.vColor.r + vBox.r;
	Out.vColor.g = Out.vColor.g + vBox.g;
	Out.vColor.b = Out.vColor.b + vBox.b;
	Out.vColor.a = g_Alpha;

	return Out;
}

PS_OUT PS_Second_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	Out.vColor = tex2D(BoxSampler, In.vTexUV);
	Out.vColor.a = g_Alpha;

	return Out;
}

PS_OUT PS_Third_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	In.vTexUV.g = In.vTexUV.g + 0.01f;
	Out.vColor = tex2D(BoxSampler, In.vTexUV);

	if (In.vTexUV.g < g_PercentStat)
	{
		if (Out.vColor.a)
		{
			if (g_Blink)
			{
				Out.vColor.r = 1.f;
				Out.vColor.g = 0.7f;
				Out.vColor.b = 0.8f;
				Out.vColor.a = g_Alpha * 0.5f;
			}
			else
			{
				Out.vColor.r = 0.8f;
				Out.vColor.g = 0.9f;
				Out.vColor.b = 1.f;
				Out.vColor.a = g_Alpha * 0.5f;
			}

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
			if (g_Blink)
			{
				Out.vColor.r = 1.f;
				Out.vColor.g = 0.4f;
				Out.vColor.b = 0.4f;
				Out.vColor.a = g_Alpha;
			}
			else
			{
				Out.vColor.r = 0.2f;
				Out.vColor.g = 0.9f;
				Out.vColor.b = 1.f;
				Out.vColor.a = g_Alpha;
			}
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

PS_OUT PS_Fourth_MAIN(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	In.vTexUV.g = In.vTexUV.g + 0.01f;
	Out.vColor = tex2D(BoxSampler, In.vTexUV);
	//if (Out.vColor.a)
	//{
	//	Out.vColor.r = 1.f;
	//	Out.vColor.g = 0.9f;
	//	Out.vColor.b = 0.4f;
	//	Out.vColor.a = g_Alpha;

	//	return Out;
	//}
	//else
	//{
	//	Out.vColor.a = 0.f;

	//	return Out;
	//}
	//return Out;


	////////////////////////////////////

	if (In.vTexUV.g > g_PercentStat)
	{
		if (Out.vColor.a)
		{
			if (g_Blink)
			{
				Out.vColor.r = 1.f;
				Out.vColor.g = 0.9f;
				Out.vColor.b = 0.4f;
				Out.vColor.a = g_Alpha;
			}
			else
			{
				Out.vColor.r = 1.f;
				Out.vColor.g = 0.9f;
				Out.vColor.b = 0.4f;
				Out.vColor.a = g_Alpha;
			}

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
			Out.vColor.r = 0.8f;
			Out.vColor.g = 0.9f;
			Out.vColor.b = 1.f;
			Out.vColor.a = g_Alpha * 0.5f;
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

	pass	Searchbar
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_Second_MAIN();
	}

	pass	SummaryStat
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_Third_MAIN();
	}

	pass SummaryStatXP
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_Fourth_MAIN();
	}
}

/////////////////////////////////////////////////