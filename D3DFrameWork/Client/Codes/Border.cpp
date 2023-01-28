#include "stdafx.h"
#include "..\Headers\Border.h"

#include "Export_Function.h"
#include "Left_Inventory.h"
#define EDGESIZE 2.f
USING(Engine)

CBorder::CBorder(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CBorder::CBorder(const CBorder & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CBorder::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CBorder::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	if (_pArg)
		m_pMasterUIInfo = static_cast<ORTHOUI_INFO*>(_pArg);

	m_tORTHOUI_INFO.bUpdate = m_pMasterUIInfo->bUpdate;
	return S_OK;
}

_int CBorder::Update_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::Update_GameObject(_dTimeDelta);
	//OrderCheck();
	return S_OK;
}

_int CBorder::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	if (m_pMasterUIInfo->bRender)
	{
		if (m_pMasterUIInfo->iOrderEdge & B_Order_Making)
		{
			if (!m_pOrderUseTex)
				m_pOrderUseTex = static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Get_pMakingTexture(L"CraftingBroken");
			Engine::Add_RenderGroup(RENDER_ITEMUI, this);
		}
		else
		{
			Engine::Add_RenderGroup(m_eMasterRenderID, this);
		}

	}
	return _int();
}

void CBorder::Render_GameObject(_mat _matOrtho)
{
	if (!m_pMasterUIInfo->bRender)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 2;

	pEffect->Begin(&iPassMax, 0);

	pEffect->CommitChanges();
	if (m_pMasterUIInfo->iOrderEdge & B_Order_Making)
		pEffect->BeginPass(1);
	else
		pEffect->BeginPass(0);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}


CBorder * CBorder::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CBorder*	pInstance = new CBorder(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CBorder::Clone(void * _pArg/* = nullptr*/)
{
	CBorder*	pClone = new CBorder(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CBorder::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Border"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);
	
	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CBorder::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect , _mat _matOrtho)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = m_pMasterUIInfo->fSizeX + EDGESIZE;
	matWorld._22 = m_pMasterUIInfo->fSizeY + EDGESIZE;
	matWorld._33 = FTRUE;

	matWorld._41 = m_pMasterUIInfo->fPosX  - WINCX * FHALF;
	matWorld._42 = -m_pMasterUIInfo->fPosY + WINCY * FHALF;
	matWorld._43 = FTRUE;

	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);

	_pEffect->SetFloat("g_Alpha", m_pMasterUIInfo->fEdgeAlpha);
	_pEffect->SetVector("g_EdgeSize", &m_pMasterUIInfo->v4EdgeSize);

	_pEffect->SetFloat("g_fDrawCutRatio", m_fDrawCutRatio);
	_pEffect->SetInt("g_iDrawCutUp", m_iDrawCutUp);

	if (m_pMasterUIInfo->iOrderEdge & B_Order_Making)
	{
		_pEffect->SetFloat("g_fOrder", m_pMasterUIInfo->fOrderContents);
		m_pOrderUseTex->Set_Texture(_pEffect, "g_OrderTex");
	}
	return S_OK;
}

void CBorder::Set_Scroll_Show(_float _fScroll_MoveY, _float _fShow_TopY, _float _fShow_BottomY)
{
	//_fScroll_MoveY -> 얼마나 아래로 내려왔는지 값, 렌더할 땐 빼주면 될 듯?
	//m_fScroll_FixY = _fScroll_MoveY;

	//_fShow_TopY -> 스크린 짤리는 젤 위에 포스
	//_fShow_BottomY -> 스크린 짤리는 젤 밑에 포스
	//_float				m_fDrawCutRatio = 0.f;
	//_bool					m_bDrawCutUp = false; gㅐ결
	if (WINCY*FHALF > m_pMasterUIInfo->fPosY)//원랜 이러면 안되지만... 지금은 무조건 괜찮을 테니 ㅇㅋ
		m_iDrawCutUp = (_int)EScroll_Cut::EScroll_Cut_Up;
	else
		m_iDrawCutUp = (_int)EScroll_Cut::EScroll_Cut_Down;

	//m_fDrawCutRatio

	if ((_int)EScroll_Cut::EScroll_Cut_Up == m_iDrawCutUp) //위로 올라간 친구면
		m_fDrawCutRatio = (_fShow_TopY - (m_pMasterUIInfo->fPosY - (m_pMasterUIInfo->fSizeY*FHALF))) / m_pMasterUIInfo->fSizeY;
	else if ((_int)EScroll_Cut::EScroll_Cut_Down == m_iDrawCutUp)
		m_fDrawCutRatio = ((m_pMasterUIInfo->fPosY + (m_pMasterUIInfo->fSizeY*FHALF)) - _fShow_BottomY) / m_pMasterUIInfo->fSizeY;


	if (0.f > m_fDrawCutRatio)
		m_fDrawCutRatio = 0.f;
	else if (1.f < m_fDrawCutRatio)
		m_fDrawCutRatio = 1.f;

	return;
}

//void CBorder::OrderCheck()
//{
//	//if (m_pMasterUIInfo->iOrderEdge & B_Order_Making)
//	//{
//
//	//}
//}

void CBorder::Free()
{


	COrthoUI::Free();
}