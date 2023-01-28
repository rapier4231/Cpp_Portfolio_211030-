#pragma once
#ifndef __TIMER_H__
#define __TIMER_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer(void);
	virtual ~CTimer(void) = default;

public:
	HRESULT		Ready_Timer(void);
	void		SetUp_TimeDelta(void);

public:
	_double	Get_TimeDelta(void) { return m_dTimeDelta; }

public:
	static CTimer*	Create(void);
	virtual void Free() override;

private:
	LARGE_INTEGER	m_FrameTime;
	LARGE_INTEGER	m_LastTime;
	LARGE_INTEGER	m_FixTime;
	LARGE_INTEGER	m_CpuTick;

	_double		m_dTimeDelta;
};
END
#endif // !__TIMER_H__
