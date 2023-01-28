#include "..\Headers\Timer_Mgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimer_Mgr)

CTimer_Mgr::CTimer_Mgr(void)
{
}

CTimer_Mgr::~CTimer_Mgr(void)
{
	Free();
}

HRESULT CTimer_Mgr::Ready_Timer(const wstring & _strTimerTag)
{
	// 키 중복체크
	CTimer* pTimer = Find_Timer(_strTimerTag);

	// 찾은 키가 이미 있는 경우 실패 반환
	if (nullptr != pTimer)
		return E_FAIL;

	// 새로운 타이머 생성
	pTimer = CTimer::Create();
	NULL_CHECK_RETURN(pTimer, E_FAIL);

	// 생성한 타이머를 맵 컨테이너에 추가
	m_mapTimers.emplace(_strTimerTag, pTimer);

	return S_OK;
}

_double CTimer_Mgr::Get_TimeDelta(const wstring & _strTimerTag)
{
	CTimer* pTimer = Find_Timer(_strTimerTag);

	NULL_CHECK_RETURN(pTimer, 0);

	return pTimer->Get_TimeDelta();
}

void CTimer_Mgr::Set_TimeDelta(const wstring & _strTimerTag)
{
	CTimer* pTimer = Find_Timer(_strTimerTag);

	NULL_CHECK_RETURN(pTimer,);

	pTimer->SetUp_TimeDelta();
}

void CTimer_Mgr::Free()
{
	for (auto pPair : m_mapTimers)
		Safe_Release(pPair.second);

	m_mapTimers.clear();
}

CTimer * CTimer_Mgr::Find_Timer(const wstring & _strTimerTag)
{
	//auto iter = find_if(m_mapTimers.begin(), m_mapTimers.end(),)
	
	auto iter = m_mapTimers.find(_strTimerTag);

	if (iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}
