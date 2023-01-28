#include "..\Headers\Light.h"
#include "Export_Utility.h"

USING(Engine)

CLight::CLight(LPDIRECT3DDEVICE9 _pDevice)
	: m_pDevice(_pDevice)
	, m_iIndex(0)
{
	m_pDevice->AddRef();
}

HRESULT CLight::Ready_Light(const D3DLIGHT9 * pLightInfo, const _uint & iIndex)
{
	memcpy(&m_tLightInfo, pLightInfo, sizeof(D3DLIGHT9));

	m_iIndex = iIndex;

	D3DVIEWPORT9		ViewPort;
	m_pDevice->GetViewport(&ViewPort);

	// 버텍스 버퍼 생성 함수
	FAILED_CHECK_RETURN(m_pDevice->CreateVertexBuffer(sizeof(VTX_SCREEN) * 4,
		0,  // 숫자 0인 경우 정적 버퍼를 사용
		FVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_pVB, NULL), E_FAIL);

	// 인덱스 버퍼 생성 함수
	FAILED_CHECK_RETURN(m_pDevice->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,  // 숫자 0인 경우 정적 버퍼를 사용
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB, NULL), E_FAIL);

	VTX_SCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(0.f - 0.5f, 0.f - 0.5f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec4(_float(ViewPort.Width - 0.5f), 0.f - 0.5f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4(_float(ViewPort.Width - 0.5f), _float(ViewPort.Height - 0.5f), 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec4(0.f - 0.5f, _float(ViewPort.Height - 0.5f), 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*	pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	//m_pDevice->SetLight(iIndex, pLightInfo);
	//m_pDevice->LightEnable(iIndex, TRUE);

	return S_OK;
}

void CLight::Render_Light(LPD3DXEFFECT & pEffect, _bool _bBlur)
{
	// Shade Pass 0 : DirectionLight, 1 : PointLight, 2 : Direction + Shadow
	_uint iPass = 0;

	if (D3DLIGHT_DIRECTIONAL == m_tLightInfo.Type)
	{
		pEffect->SetVector("g_vLightDir", &_vec4(m_tLightInfo.Direction, 0.f));

		if (!Engine::Get_ShadowOnOff())
			iPass = 2;
		else
			iPass = 0;
	}
	else if (D3DLIGHT_POINT == m_tLightInfo.Type)
	{
		pEffect->SetVector("g_vLightPos", &_vec4(m_tLightInfo.Position, 1.f));
		pEffect->SetFloat("g_fRange", m_tLightInfo.Range);
		iPass = 1;
	}

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&m_tLightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&m_tLightInfo.Ambient);

	_mat matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	pEffect->SetVector("g_vCamPos", (_vec4*)&matView._41);
	pEffect->SetFloat("g_fPower", 1.f);
	pEffect->SetMatrix("g_matInvView", &matView);
	pEffect->SetMatrix("g_matInvProj", &matProj);

	pEffect->CommitChanges();

	pEffect->BeginPass(iPass);
	
	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTX_SCREEN));

	m_pDevice->SetFVF(FVF_SCREEN);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
}

void CLight::Set_LightStat(SETLIGHTSTAT _eSetLightStat, _float _fPower)
{
	switch (_eSetLightStat)
	{
	case Engine::LIGHTPOWERUP:
		if (_fPower > m_tLightInfo.Diffuse.r)
			m_tLightInfo.Diffuse.r += 0.1f;
		else
			m_tLightInfo.Diffuse.r = _fPower;	
		break;

	case Engine::LIGHTPOWERDOWN:
		if (_fPower < m_tLightInfo.Diffuse.r)
			m_tLightInfo.Diffuse.r -= 0.1f;
		else
			m_tLightInfo.Diffuse.r = _fPower;
		break;

	case Engine::LIGHTRANGEUP:
		if (_fPower > m_tLightInfo.Range)
			m_tLightInfo.Range += 0.2f;
		else
			m_tLightInfo.Range = _fPower;
		break;

	case Engine::LIGHTRANGEDOWN:
		if (_fPower < m_tLightInfo.Range)
			m_tLightInfo.Range -= 0.2f;
		else
			m_tLightInfo.Range = _fPower;
		break;

	case Engine::LIGHTEND:
		break;

	default:
		break;
	}
}

CLight * CLight::Create(LPDIRECT3DDEVICE9 _pDevice, const D3DLIGHT9* _pLightInfo, const _uint & _iIndex)
{
	CLight*	pInstance = new CLight(_pDevice);

	if (FAILED(pInstance->Ready_Light(_pLightInfo, _iIndex)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	m_pDevice->LightEnable(m_iIndex, FALSE);

	Safe_Release(m_pDevice);
}
