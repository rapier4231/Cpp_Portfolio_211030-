#include "Frame_Mgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrame_Mgr)

CFrame_Mgr::CFrame_Mgr(void)
{
}

CFrame_Mgr::~CFrame_Mgr(void)
{
	Free();
}

HRESULT CFrame_Mgr::Ready_Frame(const wstring & _strFrameTag, const _double & _dCallLimit)
{
	CFrame*	pFrame = Find_Frame(_strFrameTag);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(_dCallLimit);
	NULL_CHECK_RETURN(pFrame, E_FAIL);

	m_mapFrame.emplace(_strFrameTag, pFrame);

	return S_OK;
}

_bool CFrame_Mgr::IsPermit_Call(const wstring & _strFrameTag, const _double & _dTimeDelta)
{
	CFrame*		pInstance = Find_Frame(_strFrameTag);
	NULL_CHECK_RETURN(pInstance, false);

	return pInstance->IsPermit_Call(_dTimeDelta);
}

CFrame * CFrame_Mgr::Find_Frame(const wstring & _strFrameTag)
{
	auto iter = m_mapFrame.find(_strFrameTag);

	if (iter == m_mapFrame.end())
		return nullptr;

	return iter->second;
}

void CFrame_Mgr::Free(void)
{
	for (auto pPair : m_mapFrame)
		Safe_Release(pPair.second);

	m_mapFrame.clear();
}
