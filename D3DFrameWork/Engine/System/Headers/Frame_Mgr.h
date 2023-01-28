#pragma once
#ifndef __FRAME_MGR_H__
#define __FRAME_MGR_H__

#include "Engine_Define.h"
#include "Base.h"
#include "Frame.h"

BEGIN(Engine)

class ENGINE_DLL CFrame_Mgr : public CBase
{
	DECLARE_SINGLETON(CFrame_Mgr)

private:
	explicit CFrame_Mgr(void);
	virtual ~CFrame_Mgr(void);

public:
	HRESULT			Ready_Frame(const wstring& _strFrameTag, const _double& _dCallLimit);

public:
	_bool			IsPermit_Call(const wstring& _strFrameTag, const _double& _dTimeDelta);

private:
	CFrame*			Find_Frame(const wstring& _strFrameTag);

public:
	virtual	void	Free(void) override;
private:
	map<wstring, CFrame*>	m_mapFrame;
};
END
#endif // !__FRAME_MGR_H__
