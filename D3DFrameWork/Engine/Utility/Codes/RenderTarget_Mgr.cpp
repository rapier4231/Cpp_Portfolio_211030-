#include "RenderTarget_Mgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderTarget_Mgr)

CRenderTarget_Mgr::CRenderTarget_Mgr()
{
}

CRenderTarget_Mgr::~CRenderTarget_Mgr()
{
	Free();
}

HRESULT CRenderTarget_Mgr::Ready_RenderTarget(LPDIRECT3DDEVICE9 _pDevice, D3DFORMAT _Format, D3DXCOLOR _Color, const wstring & _wstrTargetTag, const _uint & _iWidth, const _uint & _iHeight, _bool _bShadow)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(_wstrTargetTag);

	if (nullptr != pRenderTarget)
		return E_FAIL;

	pRenderTarget = CRenderTarget::Create(_pDevice, _iWidth, _iHeight, _Format, _Color, _bShadow);
	NULL_CHECK_RETURN(pRenderTarget, E_FAIL);

	m_mapRenderTareget.emplace(_wstrTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CRenderTarget_Mgr::Ready_MRT(const wstring & _wstrMRTTag, const wstring & _wstrTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(_wstrTargetTag);
	NULL_CHECK_RETURN(pRenderTarget, E_FAIL);

	list<CRenderTarget*>* pMRTList = Find_MRT(_wstrMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*> MRTList;
		MRTList.emplace_back(pRenderTarget);
		m_mapMRT.emplace(_wstrMRTTag, MRTList);
	}
	else
		pMRTList->emplace_back(pRenderTarget);

	return S_OK;
}

HRESULT CRenderTarget_Mgr::Begin_MRT(const wstring & _wstrMRTTag, _bool _bMotionBlur)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(_wstrMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL);

	for (auto& iter : *pMRTList)
		iter->Clear_RenderTarget(_bMotionBlur);

	_uint iIndex = 0;

	for (auto& iter : *pMRTList)
		iter->SetUp_OnGraphicDev	(iIndex++);

	return S_OK;
}

HRESULT CRenderTarget_Mgr::End_MRT(const wstring & _wstrMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(_wstrMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL);

	_uint iIndex = 0;

	for (auto& iter : *pMRTList)
		iter->Release_OnGraphicDev(iIndex++);

	return S_OK;
}

HRESULT CRenderTarget_Mgr::Ready_DebugBuffer(const wstring & _wstrTargetTag, const _float & _fX, const _float & _fY, const _float & _fSizeX, const _float & _fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(_wstrTargetTag);
	NULL_CHECK_RETURN(pRenderTarget, E_FAIL);

	FAILED_CHECK_RETURN(pRenderTarget->Ready_DebugBuffer(_wstrTargetTag, _fX, _fY, _fSizeX, _fSizeY), E_FAIL);

	return S_OK;
}

void CRenderTarget_Mgr::Render_DebugBuffer(const wstring & _wstrMRTTag)
{
	list < CRenderTarget*>*	pMRTList = Find_MRT(_wstrMRTTag);
	NULL_CHECK(pMRTList);

	for (auto& iter : *pMRTList)
		iter->Render_DebugBuffer();
}

void CRenderTarget_Mgr::Get_RenderTargetTexture(LPD3DXEFFECT & pEffect, const wstring & _strTargetTag, const char * _pConstantName)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(_strTargetTag);
	NULL_CHECK(pRenderTarget);

	pRenderTarget->Set_RendetTargetTexture(pEffect, _pConstantName);
}

void CRenderTarget_Mgr::Free()
{
	for (auto& pair : m_mapRenderTareget)
		Safe_Release(pair.second);

	m_mapRenderTareget.clear();
	
	for (auto& iter : m_mapMRT)
		iter.second.clear();

	m_mapMRT.clear();
}

CRenderTarget * CRenderTarget_Mgr::Find_RenderTarget(const wstring & _wstrTargetTag)
{
	auto iter = m_mapRenderTareget.find(_wstrTargetTag);

	if (iter == m_mapRenderTareget.end())
		return nullptr;

	return iter->second;
}

list<CRenderTarget*>* CRenderTarget_Mgr::Find_MRT(const wstring & _wstrMRTTag)
{
	auto iter = m_mapMRT.find(_wstrMRTTag);

	if (iter == m_mapMRT.end())
		return nullptr;

	return &iter->second;
}
