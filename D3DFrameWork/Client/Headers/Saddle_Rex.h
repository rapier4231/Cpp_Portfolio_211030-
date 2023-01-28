#pragma once
#ifndef __SADDLE_REX_H__
#define __SADDLE_REX_H__

#include "GameObject.h"

class CSaddle_Rex : public CGameObject
{
private:
	explicit CSaddle_Rex(LPDIRECT3DDEVICE9 _pDevice);
	explicit CSaddle_Rex(const CSaddle_Rex& rhs);
	virtual ~CSaddle_Rex() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int Update_GameObject(const _double & _dTimeDelta) override;
	virtual void Render_GameObject() override;
	virtual CGameObject * Clone(void * _pArg = nullptr) override;
	virtual void Free() override;
};

#endif // !__SADDLE_REX_H__
