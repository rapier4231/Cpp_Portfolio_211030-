matrix			g_matWorld;			// 상수 테이블(클라이언트에 실제하는 값을 대체하는 상수)
matrix			g_matView;
matrix			g_matProj;

struct VS_IN
{
	vector			vPosition : POSITION;		// 시멘틱 : 멤버 변수 고유 속성을 정의하는 지시자
};

struct VS_OUT
{
	vector			vPosition : POSITION;
};

VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = vector(0.f, 0.f, 1.f, 1.f);

	return Out;
}

PS_OUT		PS_MAIN2(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = vector(1.f, 0.f, 0.f, 1.f);

	return Out;
}

PS_OUT		PS_MAIN3(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = vector(0.f, 1.f, 0.f, 1.f);

	return Out;
}

PS_OUT		PS_MAIN4(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = vector(1.f, 1.f, 0.f, 1.f);

	return Out;
}

technique		Default_Device
{
	pass
	{
		fillmode = wireframe;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass
	{
		fillmode = wireframe;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN2();
	}

	pass
	{
		fillmode = wireframe;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN3();
	}

	pass
	{
		fillmode = wireframe;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN4();
	}
};