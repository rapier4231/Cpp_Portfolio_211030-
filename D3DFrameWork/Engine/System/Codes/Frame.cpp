#include "..\Headers\Frame.h"

USING(Engine)

CFrame::CFrame(void)
	: m_dAccTimeDelta(0)
	, m_dCallLimit(0)
{
}

HRESULT CFrame::Ready_Frame(const _double & _dCallLimit)
{
	m_dCallLimit = 1.f / _dCallLimit;

	return S_OK;
}

_bool CFrame::IsPermit_Call(const _double & _dTimeDelta)
{
	m_dAccTimeDelta += _dTimeDelta;

	if (m_dAccTimeDelta >= m_dCallLimit)
	{
		m_dAccTimeDelta = 0;
		return true;
	}

	return false;
}

CFrame * CFrame::Create(const _double & _dCallLimit)
{
	CFrame*	pInstance = new CFrame;

	if (FAILED(pInstance->Ready_Frame(_dCallLimit)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFrame::Free(void)
{
}
