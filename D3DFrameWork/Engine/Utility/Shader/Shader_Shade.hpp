// Shade 전용
texture		g_NormalTexture;
texture		g_DepthTexture;
texture		g_ShadowTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;
};

vector		g_vLightDir;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse = (vector)1.f;
vector		g_vMtrlAmbient = (vector)1.f;

vector		g_vCamPos;
float		g_fPower;

matrix		g_matInvView;
matrix		g_matInvProj;

vector		g_vLightPos;
float		g_fRange;

struct PS_IN
{
	float2		vTexUV		: TEXCOORD0;
};

struct PS_OUT
{
	vector		vShade		: COLOR0;
	vector		vSpecular	: COLOR1;
};

PS_OUT PS_DIRECTIONAL(PS_IN In)
{
	PS_OUT Out			= (PS_OUT)0;

	vector	vTexNormal = tex2D(NormalSampler, In.vTexUV);
	vector	vNormal = vector(vTexNormal.xyz * 2.f - 1.f, 0.f);
	vector	vLightDir = normalize(g_vLightDir);
	vector	vResultColor = max(dot(vLightDir * -1.f, vNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	vector	vShadow = tex2D(ShadowSampler, In.vTexUV);

	Out.vShade = saturate(vResultColor);
	Out.vShade.a = 1.f;

	vector vReflect = reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal);

	vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	float	fViewZ = vDepth.y * 1000.f;		// far값으로 나눴던 z를 다시 far값을 곱해서 원래의 값을 돌려놓기 위한 행동

	vector vPosition;

	// 텍스쳐 uv를 투영 좌표(z나누기가 끝난)로 변환 -> 이후에 fViewZ 곱해줘서 z나누기 이전의 투영 값으로 변환
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;	//x : -1 ~ 1 	
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;	//y : 1 ~ -1
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matInvProj);
	vPosition = mul(vPosition, g_matInvView);

	vector	vLook = normalize(g_vCamPos - vPosition);

	Out.vSpecular = pow(saturate(dot(vLook, normalize(vReflect))), g_fPower);

	return Out;
}

PS_OUT		PS_POINT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	float	fViewZ = vDepth.y * 1000.f;

	vector vPosition;

	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;	//x : -1 ~ 1 	
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;	//y : 1 ~ -1
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matInvProj);
	vPosition = mul(vPosition, g_matInvView);

	vector		vTexNormal = tex2D(NormalSampler, In.vTexUV);
	vector		vNormal = vector(vTexNormal.xyz * 2.f - 1.f, 0.f);

	vector		vLightDir = vPosition - g_vLightPos;

	float		fDistance = length(vLightDir);
	float		fAtt = saturate((g_fRange - fDistance) / g_fRange);

	Out.vShade = (saturate(dot(normalize(vLightDir) * -1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
	Out.vShade.a = 1.f;

	vector	vRelfect = reflect(normalize(vector(vLightDir.xyz, 0.f)), vNormal);

	vector	vLook = normalize(g_vCamPos - vPosition);

	Out.vSpecular = (pow(saturate(dot(vLook, normalize(vRelfect))), g_fPower)) * fAtt;

	return Out;
}


PS_OUT PS_SHADOW(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector	vTexNormal = tex2D(NormalSampler, In.vTexUV);
	vector	vNormal = vector(vTexNormal.xyz * 2.f - 1.f, 0.f);
	vector	vLightDir = normalize(g_vLightDir);
	vector	vResultColor = max(dot(vLightDir * -1.f, vNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	vector	vShadow = tex2D(ShadowSampler, In.vTexUV);

	vResultColor *= vShadow;

	Out.vShade = saturate(vResultColor);
	Out.vShade.a = 1.f;

	vector vReflect = reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal);

	vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	float	fViewZ = vDepth.y * 1000.f;		// far값으로 나눴던 z를 다시 far값을 곱해서 원래의 값을 돌려놓기 위한 행동

	vector vPosition;

	// 텍스쳐 uv를 투영 좌표(z나누기가 끝난)로 변환 -> 이후에 fViewZ 곱해줘서 z나누기 이전의 투영 값으로 변환
	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;	//x : -1 ~ 1 	
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;	//y : 1 ~ -1
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matInvProj);
	vPosition = mul(vPosition, g_matInvView);

	vector	vLook = normalize(g_vCamPos - vPosition);

	Out.vSpecular = pow(saturate(dot(vLook, normalize(vReflect))), g_fPower);

	return Out;
}

technique		Default_Device
{
	pass Directional	// Pass 0
	{
		alphablendenable = true;
		srcblend = one;
		destblend = one;
		
		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_DIRECTIONAL();
	}

	pass Point	// Pass 1
	{
		alphablendenable = true;
		srcblend = one;
		destblend = one;

		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_POINT();
	}

	pass // Pass 2 : NoneShadow
	{
		alphablendenable = true;
		srcblend = one;
		destblend = one;

		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_SHADOW();
	}
}