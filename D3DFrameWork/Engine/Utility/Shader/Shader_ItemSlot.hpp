float			g_Alpha;
bool			g_bNoTex;
bool			g_bDrag;
bool			g_bDrop;
float			g_fDrawCutRatio;
int				g_iDrawCutUp;

matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture		g_BaseTexture;
texture		g_ItemTexture;

texture		g_Engram_BackGround;
texture		g_CraftingBroken;
bool		g_bPossibleMaking;
bool		g_bPossibleMakingLv;


sampler			ItemSampler = sampler_state
{
	texture = g_ItemTexture;
AddressU = clamp;
AddressV = clamp;
	minfilter = linear;
	magfilter = linear;
};

sampler			BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

sampler			BackGroundSampler = sampler_state
{
	texture = g_Engram_BackGround;
	minfilter = linear;
	magfilter = linear;
};

sampler			CraftingBrokenSampler = sampler_state
{
	texture = g_CraftingBroken;
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

	//스크롤 제어
	if (1 == g_iDrawCutUp)
	{
		if (g_fDrawCutRatio > In.vTexUV.g)
		{
			Out.vColor.a = 0.f;
			return Out;
		}
	}
	else if(2 == g_iDrawCutUp)
	{
		if ((1.f - g_fDrawCutRatio) < In.vTexUV.g)
		{
			Out.vColor.a = 0.f;
			return Out;
		}
	}
	/////////////////////////////////////

	float2 FixUV = In.vTexUV;
	FixUV.r = In.vTexUV.r * 1.2f - 0.1f;
	FixUV.g = In.vTexUV.g * 1.2f - 0.1f;
	Out.vColor = tex2D(ItemSampler, FixUV);
	//Out.vColor = tex2D(ItemSampler, In.vTexUV);

	if (g_bNoTex)
	{
		Out.vColor = tex2D(BaseSampler, In.vTexUV);
		Out.vColor.a = g_Alpha;
		return Out;
	}
	if (g_bDrag)
	{
		if (Out.vColor.a)
		{
			Out.vColor.a = 0.5f;
			return Out;
		}
		else
		{
			Out.vColor.a = g_Alpha;
			return Out;
		}
	}
	if (g_bDrop)
	{
		if (Out.vColor.a)
		{
			Out.vColor.a = 1.f;
			return Out;
		}
		else
		{
			Out.vColor.a = g_Alpha;
			return Out;
		}
	}

	if (Out.vColor.a)
	{
		return Out;
	}
	else
	{
		Out.vColor.a = g_Alpha;
		return Out;
	}
	return Out;
}

PS_OUT PS_Making(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;

	//스크롤 제어
	if (1 == g_iDrawCutUp)
	{
		if (g_fDrawCutRatio > In.vTexUV.g)
		{
			Out.vColor.a = 0.f;
			return Out;
		}
	}
	else if (2 == g_iDrawCutUp)
	{
		if ((1.f - g_fDrawCutRatio) < In.vTexUV.g)
		{
			Out.vColor.a = 0.f;
			return Out;
		}
	}
	/////////////////////////////////////

	vector vBackGround = tex2D(BackGroundSampler, In.vTexUV);
	vector vCraftingBroken = tex2D(CraftingBrokenSampler, In.vTexUV);
	float2 FixUV = In.vTexUV;
	FixUV.r = In.vTexUV.r * 1.2f - 0.1f;
	FixUV.g = In.vTexUV.g * 1.2f - 0.1f;
	vector vItem = tex2D(ItemSampler, FixUV);
	//vector vItem = tex2D(ItemSampler, In.vTexUV);

	if (g_bPossibleMakingLv) // 제대로 불이 들어옴
	{
		if (g_bPossibleMaking) //제작이 가능하면 초록색?이 들어온다
		{
			if (vItem.a)
			{
				Out.vColor.r = vItem.r *0.9f;
				Out.vColor.g = (vItem.g + 0.2f) *0.9f;
				Out.vColor.b = vItem.b *0.9f;
				Out.vColor.a = 0.5f;
				return Out;
			}
			else
			{
				if (In.vTexUV.r < 0.01f)
				{
					Out.vColor = vBackGround*0.9f;
					Out.vColor.g = Out.vColor.g + 0.3f;
					Out.vColor.a = 0.4f;
					return Out;
				}

				if (In.vTexUV.r > 0.99f)
				{
					Out.vColor = vBackGround*0.9f;
					Out.vColor.g = Out.vColor.g + 0.3f;
					Out.vColor.a = 0.4f;
					return Out;
				}

				if (In.vTexUV.g < 0.01f)
				{
					Out.vColor = vBackGround*0.9f;
					Out.vColor.g = Out.vColor.g + 0.3f;
					Out.vColor.a = 0.4f;
					return Out;
				}

				if (In.vTexUV.g > 0.99f)
				{
					Out.vColor = vBackGround*0.9f;
					Out.vColor.g = Out.vColor.g + 0.3f;
					Out.vColor.a = 0.4f;
					return Out;
				}
				Out.vColor = vBackGround *0.9f;
				Out.vColor.g = Out.vColor.g + 0.2f;
				return Out;
			}
		}
		else//빨간색, 제작 불가능 표시
		{
			if (vItem.a)
			{
				Out.vColor.r = (vItem.r + 0.3f) *0.7f;
				Out.vColor.g = vItem.g *0.7f;
				Out.vColor.b = vItem.b *0.7f;
				Out.vColor.a = 0.5f;
				return Out;
			}
			else
			{
				if (In.vTexUV.r < 0.01f)
				{
					Out.vColor.r = 1.f;
					Out.vColor = vBackGround*0.8f;
					Out.vColor.a = 0.4f;
					return Out;
				}

				if (In.vTexUV.r > 0.99f)
				{
					Out.vColor.r = 1.f;
					Out.vColor = vBackGround*0.8f;
					Out.vColor.a = 0.4f;
					return Out;
				}

				if (In.vTexUV.g < 0.01f)
				{
					Out.vColor.r = 1.f;
					Out.vColor = vBackGround*0.8f;
					Out.vColor.a = 0.4f;
					return Out;
				}

				if (In.vTexUV.g > 0.99f)
				{
					Out.vColor.r = 1.f;
					Out.vColor = vBackGround*0.8f;
					Out.vColor.a = 0.4f;
					return Out;
				}
				Out.vColor = vBackGround*0.8f;
				Out.vColor.r = 1.f;
				return Out;
			}
		}
	}
	else //흑색으로 나옴
	{
		if (vItem.a)
		{
			Out.vColor = vItem*0.4f;
			Out.vColor.a = 0.8f;
			return Out;
		}
		else
		{
			Out.vColor = vBackGround*0.4f;
			Out.vColor.a = vBackGround.a;
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
		pixelshader = compile ps_3_0 PS_Making();
	}
}

/////////////////////////////////////////////////