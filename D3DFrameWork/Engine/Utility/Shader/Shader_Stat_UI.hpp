float			g_Alpha;
float			g_PercentStat;
vector			g_EdgeSize;
int				g_iMouse;
float			g_fStatUpCollTime;
bool			g_bCanRiseStat;

matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_StatTexture;
texture			g_GlowTexture;
texture			g_ArrowUpTexture;
sampler			StatSampler = sampler_state
{
	texture = g_StatTexture;
	AddressU = clamp;
	AddressV = clamp;
	minfilter = linear;
	magfilter = linear;
};

sampler			GlowSampler = sampler_state
{
	texture = g_GlowTexture;
	AddressU = clamp;
	AddressV = clamp;
	minfilter = linear;
	magfilter = linear;
};

sampler			ArrowUpSampler = sampler_state
{
	texture = g_ArrowUpTexture;
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
	if (In.vTexUV.r < g_PercentStat)
	{
		Out.vColor.r = 0.2f;
		Out.vColor.g = 1.f;
		Out.vColor.b = 1.f;
		Out.vColor.a = 0.6f;
		return Out;
	}
	else
	{
	
		Out.vColor.r = 1.f;
		Out.vColor.g = 1.f;
		Out.vColor.b = 1.f;
		Out.vColor.a = 0.1f;
		return Out;
	}
}

PS_OUT PS_Stat(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	Out.vColor = tex2D(StatSampler, In.vTexUV);

	return Out;
}


PS_OUT PS_Plus(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	float2 FixUV = In.vTexUV;
	FixUV.r = In.vTexUV.r + 0.03f;
	FixUV.g = In.vTexUV.g + 0.03f;
	Out.vColor = tex2D(StatSampler, FixUV);

	if (g_EdgeSize.r >= In.vTexUV.r || g_EdgeSize.g >= In.vTexUV.g || g_EdgeSize.b <= In.vTexUV.r || g_EdgeSize.a <= In.vTexUV.g)
	{
		if (1 == g_iMouse)//마우스 올라왔으면
		{
			Out.vColor.r = 0.7f;
			Out.vColor.g = 0.7f;
			Out.vColor.b = 0.7f;
			Out.vColor.a = 0.6f;
		}
		else if (2 == g_iMouse)//클릭했으면
		{
			Out.vColor.r = 1.f;
			Out.vColor.g = 1.f;
			Out.vColor.b = 1.f;
			Out.vColor.a = 0.8f;
		}
		else if(0 == g_iMouse)//걍 두면
		{
			Out.vColor.r = 0.1f;
			Out.vColor.g = 0.7f;
			Out.vColor.b = 0.7f;
			Out.vColor.a = 0.4f;
		}
		return Out;
	}
	else
	{
		if (2 == g_iMouse)
		{
			FixUV.g = In.vTexUV.g + (g_fStatUpCollTime * 4.f) - 1.f;
			vector ArrowUp = tex2D(ArrowUpSampler, FixUV);
			if (ArrowUp.a)
			{
				ArrowUp.r = 0.9f;
				ArrowUp.g = 0.9f;
				ArrowUp.b = 0.9f;
				ArrowUp.a = 1.f;
			}
			Out.vColor.r = (Out.vColor.r*0.5f) + (ArrowUp.r*0.5f);
			Out.vColor.g = (Out.vColor.g*0.5f) + (ArrowUp.g*0.5f);
			Out.vColor.b = (Out.vColor.b*0.5f) + (ArrowUp.b*0.5f);
			Out.vColor.a = (Out.vColor.a*0.5f) + (ArrowUp.a*0.5f);
			return Out;
		}
		else
		{
			if (Out.vColor.a)
			{
				if(g_bCanRiseStat)
					return Out;
				else
				{
					Out.vColor.a = 0.5f;
					return Out;
				}
			}
			else
			{
				Out.vColor.r = 0.1f;
				Out.vColor.g = 0.1f;
				Out.vColor.b = 0.1f;
				Out.vColor.a = 0.7f;
				return Out;
			}
		}
	}
}

PS_OUT PS_Glow(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	Out.vColor = tex2D(GlowSampler, In.vTexUV);
	Out.vColor.a = Out.vColor.a * 0.4f;
	return Out;
}

PS_OUT PS_Loading_Gage(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	if (1.f != g_PercentStat)
	{
		//노이즈 넣어서 물 처럼 촤라라락
	}

	if (In.vTexUV.r < g_PercentStat)
	{
		Out.vColor.r = 0.8f*(In.vTexUV.r*0.6f ) + 0.1f;
		Out.vColor.g = 0.9f*(In.vTexUV.r*0.6f) + 0.1f;
		Out.vColor.b = 1.f*(In.vTexUV.r*0.6f) + 0.1f;
		Out.vColor.a = 0.7f;
		return Out;
	}
	else
	{
		Out.vColor.r = 1.f;
		Out.vColor.g = 1.f;
		Out.vColor.b = 1.f;
		Out.vColor.a = 0.1f;
		return Out;
	}
}

technique		Default_Device
{
		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = true;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = true;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_Stat();
	}

		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = true;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_Plus();
	}

		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = true;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_Glow();
	}

		pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		zwriteenable = true;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_Loading_Gage();
	}
}

/////////////////////////////////////////////////