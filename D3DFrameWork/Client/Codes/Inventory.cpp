#include "stdafx.h"
#include "..\Headers\Inventory.h"
#include "Export_Function.h"

#include "Border.h"

//#define INVENPOSX		180.f
//#define INVENPOSY		290.f
//#define INVENSIZEX	230.f
//#define INVENSIZEY	450.f

//#define INVENEDGESIZELEFT 0.004f
//#define INVENEDGESIZERIGHT 0.9914f
//#define INVENEDGESIZETOP 0.002f
//#define INVENEDGESIZEBOTTOM 0.996f

#define INVENALPHA	0.4f
#define INVENTEXRATIOX	0.3f
#define INVENTEXRATIOY	0.8f

#define FILTERTEXSPEEDCORRECTION 0.15f
#define OPENEDGEANISPEEDCORRECTION 1.f

#define INVENEDGEALPHA	0.4f

USING(Engine)

HRESULT CInventory::Set_Screen_UI()
{
	STInventoryURI stURI;

	if (m_bLeftInventory)
		m_tORTHOUI_INFO.fPosX = stURI.fLeftPosX[EScrnResolution];
	else
		m_tORTHOUI_INFO.fPosX = (WINCX - stURI.fLeftPosX[EScrnResolution]);

	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	return S_OK;
}

CInventory::CInventory(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CInventory::CInventory(const CInventory & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CInventory::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CInventory::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	if (_pArg)
	{
		m_bLeftInventory = *static_cast<_bool*>(_pArg);

		if (m_bLeftInventory)
			m_tORTHOUI_INFO.fPosX = stURI.fLeftPosX[EScrnResolution] - 0.04f*ScrnRatio;
		else
			m_tORTHOUI_INFO.fPosX = (WINCX - stURI.fLeftPosX[EScrnResolution]);
	}

	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = INVENALPHA;
	m_tORTHOUI_INFO.bUpdate = true;

	m_fTexRatioX = INVENTEXRATIOX;
	m_fTexRatioY = INVENTEXRATIOY;

	return S_OK;
}

_int CInventory::Update_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::Update_GameObject(_dTimeDelta);

	m_fFilterTex_PosY += (_float)_dTimeDelta * FILTERTEXSPEEDCORRECTION;
	if (FTRUE <= m_fFilterTex_PosY)
		m_fFilterTex_PosY = FNULL;

	InvenOpenAni(_dTimeDelta);

	return S_OK;
}

_int CInventory::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	if (m_tORTHOUI_INFO.bRender)
	{
		Engine::Add_RenderGroup(RENDER_ORTHOBACKUI,this);
	}

	return _int();
}

void CInventory::Render_GameObject(_mat _matOrtho)
{
	if (!m_tORTHOUI_INFO.bRender)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = NULL;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(NULL);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}


CInventory * CInventory::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CInventory*	pInstance = new CInventory(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CInventory::Clone(void * _pArg/* = nullptr*/)
{
	CInventory*	pClone = new CInventory(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CInventory::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent= m_pTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Inventory"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pFilter_TextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Inventory_Filter"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_FilterTexture", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Inventory"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);
	
	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CInventory::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect , _mat _matOrtho)
{
	m_pTextureCom->Set_Texture(_pEffect, "g_BaseTexture");
	m_pFilter_TextureCom->Set_Texture(_pEffect, "g_FilterTexture");

	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = m_tORTHOUI_INFO.fSizeX;
	matWorld._22 = m_tORTHOUI_INFO.fSizeY;
	matWorld._33 = FTRUE;

	matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
	matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
	matWorld._43 = FTRUE;

	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);

	_pEffect->SetFloat("g_FilterPosY", m_fFilterTex_PosY);
	_pEffect->SetFloat("g_Alpha", m_tORTHOUI_INFO.fAlpha);
	
	_pEffect->SetFloat("g_RatioX", m_fTexRatioX);
	_pEffect->SetFloat("g_RatioY", m_fTexRatioY);

	return S_OK;
}

void CInventory::InvenOpenAni(const _double & _dTimeDelta)
{
	if (m_bInvenOpenAni)
	{
		m_tORTHOUI_INFO.v4EdgeSize.x -= (_float)(_dTimeDelta * OPENEDGEANISPEEDCORRECTION);
		m_tORTHOUI_INFO.v4EdgeSize.y -= (_float)(_dTimeDelta * OPENEDGEANISPEEDCORRECTION);
		m_tORTHOUI_INFO.v4EdgeSize.z += (_float)(_dTimeDelta * OPENEDGEANISPEEDCORRECTION);
		m_tORTHOUI_INFO.v4EdgeSize.w += (_float)(_dTimeDelta * OPENEDGEANISPEEDCORRECTION);
		m_tORTHOUI_INFO.fEdgeAlpha += (_float)(_dTimeDelta) * FHALF;

		if (stURI.v4EdgeSize[EScrnResolution].x >= m_tORTHOUI_INFO.v4EdgeSize.x)
			m_tORTHOUI_INFO.v4EdgeSize.x = stURI.v4EdgeSize[EScrnResolution].x;
		if (stURI.v4EdgeSize[EScrnResolution].y >= m_tORTHOUI_INFO.v4EdgeSize.y)
			m_tORTHOUI_INFO.v4EdgeSize.y = stURI.v4EdgeSize[EScrnResolution].y;
		if (stURI.v4EdgeSize[EScrnResolution].z <= m_tORTHOUI_INFO.v4EdgeSize.z)
			m_tORTHOUI_INFO.v4EdgeSize.z = stURI.v4EdgeSize[EScrnResolution].z;
		if (stURI.v4EdgeSize[EScrnResolution].w <= m_tORTHOUI_INFO.v4EdgeSize.w)
			m_tORTHOUI_INFO.v4EdgeSize.w = stURI.v4EdgeSize[EScrnResolution].w;
		if (INVENEDGEALPHA <= m_tORTHOUI_INFO.fEdgeAlpha)
			m_tORTHOUI_INFO.fEdgeAlpha = INVENEDGEALPHA;

		if (stURI.v4EdgeSize[EScrnResolution].x == m_tORTHOUI_INFO.v4EdgeSize.x &&stURI.v4EdgeSize[EScrnResolution].y== m_tORTHOUI_INFO.v4EdgeSize.y && stURI.v4EdgeSize[EScrnResolution].z == m_tORTHOUI_INFO.v4EdgeSize.z&& stURI.v4EdgeSize[EScrnResolution].w == m_tORTHOUI_INFO.v4EdgeSize.w
			&&INVENEDGEALPHA == m_tORTHOUI_INFO.fEdgeAlpha)
			m_bInvenOpenAni = false;
	}
}

void CInventory::Free()
{

	COrthoUI::Free();
}

_bool CInventory::UI_RenderOn()
{
	InvenOpen();

	return COrthoUI::UI_RenderOn();
}

void CInventory::InvenOpen()
{
	if (!m_tORTHOUI_INFO.bRender)
	{
		m_bInvenOpenAni = true;
		m_tORTHOUI_INFO.v4EdgeSize = { FHALF , FHALF  ,FHALF  ,FHALF };
		m_tORTHOUI_INFO.fEdgeAlpha = FNULL;
	}
}
