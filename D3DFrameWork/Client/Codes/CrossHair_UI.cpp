#include "stdafx.h"
#include "..\Headers\CrossHair_UI.h"

#include "Export_Function.h"

#define CrossHairColor _vec4(0.3f,0.8f,1.f,0.f)
#define HitMakerColor _vec4(0.9f,0.3f,0.4f,0.f)
#define CrossHairAlpha 0.7f
#define HitMakerAlphaDownSpeed 2.f

USING(Engine)

HRESULT CCrossHair_UI::Set_Screen_UI()
{
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = CrossHairAlpha;
	return S_OK;
}

CCrossHair_UI::CCrossHair_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CCrossHair_UI::CCrossHair_UI(const CCrossHair_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CCrossHair_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CCrossHair_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = CrossHairAlpha;
	return S_OK;
}

_int CCrossHair_UI::Update_GameObject(const _double & _dTimeDelta)
{
	// 크로스 헤어 On
	//if (Engine::KeyDown(KeyCheck_Lbutton))
	//	static_cast<CCrossHair_UI*>(Engine::Get_GameObject(L"CrossHair_UI"))->Render_CrossHair(true);

	// 크로스 헤어 Off
	//if (Engine::KeyDown(KeyCheck_Rbutton))
	//	static_cast<CCrossHair_UI*>(Engine::Get_GameObject(L"CrossHair_UI"))->Render_CrossHair(false);

	//Hit On
	//if (Engine::KeyDown(KeyCheck_I))
	//	static_cast<CCrossHair_UI*>(Engine::Get_GameObject(L"CrossHair_UI"))->HitRender_On();

	if(!m_tORTHOUI_INFO.bRender)
		return NO_EVENT;

	COrthoUI::Update_GameObject(_dTimeDelta);

	m_fHitMakerAlpha -= (_float)_dTimeDelta *HitMakerAlphaDownSpeed;
	if (0.f > m_fHitMakerAlpha)
		m_fHitMakerAlpha = 0.f;

	return NO_EVENT;
}

_int CCrossHair_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_tORTHOUI_INFO.bRender)
		return NO_EVENT;

	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	Engine::Add_RenderGroup(RENDER_ORTHOBACKUI, this);
	return NO_EVENT;
}

void CCrossHair_UI::Render_GameObject(_mat _matOrtho)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 4;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(3); /////

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	/////////////////////////////////////힛 메이커
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, 1);
	pEffect->BeginPass(3);    ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	///////////////////////////////////
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CCrossHair_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pCrossHair1_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_CrossHair"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pCrossHair2_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_HitMarker"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture2", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Pubilc_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CCrossHair_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._33 = FTRUE;

	matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
	matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
	matWorld._43 = FTRUE;

	if (1 == _bTwo)
	{
		matWorld._11 = matWorld._22 = 1.f*ScrnRatio;
		m_pCrossHair2_Com->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_Alpha", m_fHitMakerAlpha);
		_pEffect->SetVector("g_vecColor", &HitMakerColor);
	}
	else
	{
		matWorld._11 = matWorld._22 = stURI.fSizeXY[EScrnResolution];
		m_pCrossHair1_Com->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_Alpha", m_tORTHOUI_INFO.fAlpha);
		_pEffect->SetVector("g_vecColor", &CrossHairColor);
	}

	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);
	return S_OK;
}

CCrossHair_UI * CCrossHair_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CCrossHair_UI*	pInstance = new CCrossHair_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CCrossHair_UI::Clone(void * _pArg)
{
	CCrossHair_UI*	pClone = new CCrossHair_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CCrossHair_UI::Free()
{
	COrthoUI::Free();
}