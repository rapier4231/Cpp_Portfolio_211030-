#pragma once
#ifndef __FRAME_H__
#define __FRAME_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFrame : public CBase
{
private:
	explicit CFrame(void);
	virtual ~CFrame(void) = default;

public:
	HRESULT			Ready_Frame(const _double& _dCallLimit);

public:
	_bool			IsPermit_Call(const _double& _dTimeDelta);

public:
	static CFrame*	Create(const _double& _dCallLimit);
	virtual void	Free(void) override;

private:
	_double			m_dCallLimit;
	_double			m_dAccTimeDelta;
};
END
#endif // !__FRAME_H__
