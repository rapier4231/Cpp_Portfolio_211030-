#pragma once
#ifndef __FUNCTION_MGR_H__
#define __FUNCTION_MGR_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;

class CFunction_Mgr : public CBase
{
	DECLARE_SINGLETON(CFunction_Mgr)

private:
	explicit CFunction_Mgr(void);
	virtual ~CFunction_Mgr(void);

public:

public:
	virtual void Free() override;
};
END
#endif // !__FUNCTION_MGR_H__
