float			g_FillAlpha;
float			g_Left_Radian;
float			g_Right_Radian;
float			g_Dis_Fill;
float			g_Dis_Near;
float			g_Dis_Far;
vector			g_vecColor;
matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_Bg_Tex;

sampler			BgSampler = sampler_state
{
	texture = g_Bg_Tex;
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

VS_OUT		VS_Ef_Rotate(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matScale , matRotate, matTrans, matWV, matWVP;

	matScale = matRotate = matTrans = g_matWorld;
	matScale._11 = g_vecColor.r;
	matScale._22 = g_vecColor.g;

	matRotate._11 = cos(g_Left_Radian);
	matRotate._12 = -sin(g_Left_Radian);
	matRotate._21 = sin(g_Left_Radian);
	matRotate._22 = cos(g_Left_Radian);

	matTrans._41 = g_vecColor.b - g_Dis_Near;
	matTrans._42 = -g_vecColor.a + g_Dis_Far;
	matTrans._43 = 1.f;

	matWV = mul(matScale, matRotate);
	matWV = mul(matWV, matTrans);

//	matWV = mul(matW, g_matView); 어짜피 항등행렬
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

	//g_FillAlpha;
	//g_Left_Angle;
	//g_Right_Angle;
	//g_Dis_Fill;
	//g_vecColor;
	//g_Dis_Near;
	//g_Dis_Far;
	float FixPosX;
	float FixPosY;
	FixPosX = In.vTexUV.r - 0.5f;
	FixPosY = In.vTexUV.g - 0.5f;

	float fPSRadian;
	fPSRadian = atan2(FixPosY, FixPosX);
	if (0 > fPSRadian)
		fPSRadian += 6.2831f;

	///////////각도 끝. 이제 거리

	float fDis;
	fDis = sqrt((FixPosX*FixPosX) + (FixPosY*FixPosY));

	if (g_Right_Radian < g_Left_Radian)//왼쪽이 더 값이 크다면
	{
		////////////////////////////////////360도에 0도 해야하는데 약간 더 준거 ////////////////////////////////
		if ((((g_Left_Radian < fPSRadian) && (6.3f > fPSRadian)) || ((g_Right_Radian > fPSRadian) && (-0.1f < fPSRadian)))
			&& (fDis > g_Dis_Near) && (fDis < g_Dis_Far))
		{
			Out.vColor = g_vecColor;
			if (fDis < g_Dis_Fill)
				Out.vColor.a = g_FillAlpha;
		}
		else
			Out.vColor.a = 0.f;
	}
	else
	{
		if ((g_Left_Radian < fPSRadian) && (g_Right_Radian > fPSRadian)
			&& (fDis > g_Dis_Near) && (fDis < g_Dis_Far))
		{
			Out.vColor = g_vecColor;
			if (fDis < g_Dis_Fill)
				Out.vColor.a = g_FillAlpha;
		}
		else
			Out.vColor.a = 0.f;

	}
	return Out;
}

PS_OUT PS_Ef_UI(PS_IN In)
{
	PS_OUT  Out = (PS_OUT)0;
	Out.vColor = tex2D(BgSampler, In.vTexUV);

	if(Out.vColor.a)
		Out.vColor.a = g_FillAlpha;
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
		vertexshader = compile vs_3_0 VS_Ef_Rotate();
		pixelshader = compile ps_3_0 PS_Ef_UI();
	}
}

/////////////////////////////////////////////////