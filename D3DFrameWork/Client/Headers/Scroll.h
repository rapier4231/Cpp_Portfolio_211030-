#pragma once
#ifndef __Scroll_H__
#define __Scroll_H__

#include "Value.h"
#include "GameObject.h"
#define MouseWheelFix -2400.f //휠 한번에 120씩 가서, 그리고 위 아래 느낌이 반대로임 ㅇㅇ 그래서 마이너스

class CScroll : public CGameObject
{
protected:
	explicit CScroll(LPDIRECT3DDEVICE9 _pDevice);
	explicit CScroll(const CScroll& rhs);
	virtual ~CScroll() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() PURE;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) PURE;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) PURE;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) PURE;
	virtual void			Render_GameObject() PURE;

public:
	virtual CGameObject*	Clone(void * _pArg = nullptr) PURE;
	virtual void			Free() PURE;

public:
	virtual	void			Scroll_Func() PURE;
	virtual	void			Scroll_Move_Func(LONG _dwMoveY) PURE;
	virtual	void			Scroll_Move_Func(_float _fMoveFixRatioY) PURE;
	virtual _float			Get_Now_RatioY() {return m_fShow_RatioY;}
protected:
	virtual void			Scroll_Set() PURE;
	_bool					m_bSet = false;

	_float m_fMax_Top_ItemPosY = 0.f;
	_float m_fMax_Bottom_ItemPosY = 0.f;
	_float m_fStart_Show_PosY = 0.f;
	_float m_fEnd_Show_PosY = 0.f;
	_float m_fAll_SizeY = 0.f;
	_float m_fMax_Left_ItemPosX = 0.f;
	_float m_fMax_Right_ItemPosX = 0.f;
	_float m_fShow_Top_ItemPosY = 0.f;
	_float m_fShow_Bottom_ItemPosY = 0.f;
	_float m_fShow_SizeY = 0.f;
	_float m_fShow_PosY = 0.f;
	_float m_fShow_RatioY = 0.f;

public:
	enum EScroll_SwapRatioY{ EScroll_Swap_SaveY_1Slot,EScroll_SwapRatioY_END };//이케하믄 안될텐데 ..후
	virtual void			Scroll_RatioY_Swap(_int _iSwapNum);

protected:
	_float m_fSave_RatioY[EScroll_SwapRatioY_END] = { 0.f };//인벤 탭 처럼 여러개 일 때 스왑용, 지금은 최대가 인벤이라 1 때려박았지만 딴거 생기면 늘리고 디파인이나 이넘?
};

#endif // !__Scroll_H__