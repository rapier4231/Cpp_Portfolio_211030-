#include "stdafx.h"
#include "..\Headers\Get_UI.h"

#include "Export_Function.h"
USING(Engine)

CGet_UI::CGet_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CGet_UI::CGet_UI(const CGet_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CGet_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CGet_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fSizeX = WINCX;
	m_tORTHOUI_INFO.fSizeY = WINCY;
	m_tORTHOUI_INFO.fPosX = WINCX * 0.5f;
	m_tORTHOUI_INFO.fPosY = WINCY* 0.5f;

	return S_OK;
}

_int CGet_UI::Update_GameObject(const _double & _dTimeDelta)
{
	if (0 > m_fHit_UI_Time)
		return NO_EVENT;

	COrthoUI::Update_GameObject(_dTimeDelta);

	m_fHit_UI_Time -= (_float)_dTimeDelta;
	if (m_fHit_UI_Time > 0.87f)
	{
		m_tORTHOUI_INFO.fAlpha += (_float)_dTimeDelta*6.f;
		if (0.63f < m_tORTHOUI_INFO.fAlpha)
			m_tORTHOUI_INFO.fAlpha = 0.63f;
	}
	else if(m_fHit_UI_Time < 0.3f)
	{
		m_tORTHOUI_INFO.fAlpha -= (_float)_dTimeDelta*1.2f;
		if (0.2f > m_tORTHOUI_INFO.fAlpha)
			m_tORTHOUI_INFO.fAlpha = 0.2f;
	}

	//if (0.5f > m_fHitspread)
	//	m_fHitspread += (_float)_dTimeDelta*5.f;
	//else

	return NO_EVENT;
}

_int CGet_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (0 > m_fHit_UI_Time)
		return NO_EVENT;

	COrthoUI::LateUpdate_GameObject(_dTimeDelta);


	Engine::Add_RenderGroup(RENDER_ORTHOBACKUI, this);
	return NO_EVENT;
}

void CGet_UI::Render_GameObject(_mat _matOrtho)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 6;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(1); /////

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CGet_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pHitTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Player_Hit_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Loading"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CGet_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = WINCX;
	matWorld._22 = WINCY;
	matWorld._33 = FTRUE;

	matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
	matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
	matWorld._43 = FTRUE;

	m_pHitTexCom->Set_Texture(_pEffect, "g_Bg_Tex");
	_pEffect->SetFloat("g_fAlpha", m_tORTHOUI_INFO.fAlpha);
	_pEffect->SetFloat("g_fSpread", m_fHitspread);
		
	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);
	return S_OK;
}

void CGet_UI::Player_Hit_On()
{
	m_fHit_UI_Time = 1.f;
	//m_fHitspread = 0.f;
	m_tORTHOUI_INFO.fAlpha = 0.3f;
}

CGet_UI * CGet_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CGet_UI*	pInstance = new CGet_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CGet_UI::Clone(void * _pArg)
{
	CGet_UI*	pClone = new CGet_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CGet_UI::Free()
{
	COrthoUI::Free();
}