#pragma once
#ifndef __TIMER_MGR_H__
#define __TIMER_MGR_H__

#include "Timer.h"

BEGIN(Engine)

class ENGINE_DLL CTimer_Mgr : public CBase
{
	DECLARE_SINGLETON(CTimer_Mgr)

private:
	explicit CTimer_Mgr(void);
	virtual ~CTimer_Mgr(void);

public:
	HRESULT		Ready_Timer(const wstring& _strTimerTag);

public:
	_double		Get_TimeDelta(const wstring& _strTimerTag);
	void		Set_TimeDelta(const wstring& _strTimerTag);

private:
	CTimer*		Find_Timer(const wstring& _strTimerTag);

public:
	virtual void Free() override;

private:
	map<wstring, CTimer*>	m_mapTimers;
};
END
#endif // !__TIMER_MGR_H__
