#pragma once
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 _pDevice);
	explicit CComponent(const CComponent& _rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT			Ready_Component_Prototype() PURE;
	virtual HRESULT			Ready_Component(void* _pArg = nullptr) PURE;
	virtual _int			Update_Component(const _double& _dTimeDelta) { return 0; }

public:
	virtual CComponent*		Clone(void * pArg = nullptr) PURE;
	virtual void			Free() PURE;

protected:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	_bool					m_bIsClone = false;
};
END

#endif // !__COMPONENT_H__
