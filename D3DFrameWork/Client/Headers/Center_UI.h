#pragma once
#ifndef __Center_UI_H__
#define __Center_UI_H__

#include "Value.h"
#include "OrthoUI.h"

class CUnit;
class CBorder;
class CInvenItem;
BEGIN(Engine)
class CTexture;
END

class CCenter_UI : public CGameObject
{
protected:
	explicit CCenter_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CCenter_UI(const CCenter_UI& rhs);
	virtual ~CCenter_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() PURE;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) PURE;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) PURE;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) PURE;
	virtual void			Render_GameObject() { return; };

public:
	void					Get_ProtoFrontEquipTexName(const wstring& _wstrProtoFrontEquipTexName);
							// 장비 텍스쳐 만들기 위해 앞 글자 받는 함수

public:
	virtual CGameObject*	Clone(void * _pArg = nullptr) PURE;
	virtual void			Free() PURE;

public:
	void					Set_EmplaceBack_EquipSlot(CInvenItem* _EquipSlot);//장비 슬롯 넣는 거 m_vecEquipSlot (맨 처음에 한번 하고 매치 하고 안쓸 듯)
	virtual void			Match_EquipTex_Info() { return; }; //장비 바뀔 때마다 장비 정보 매치
	void					Set_EquipOwner(CUnit * _pUnit);

public:
	EQUIP_Position			Get_NowMousePickingSlot();
	CUnit*					Get_pOwner() { return m_pUnit; };

public:
	virtual void			Stat_UI_Close();
	void					All_Off();
protected:
	CTexture*				Get_pEquipTexture(_tchar* _wstrEquipName); //슬롯들이 장비 사진 받을 때 쓸 함수
	CTexture*				Create_EquipTexture(_tchar* _wstrEquipName); //위에 사진 줄 때 없으면 만들 함수

protected:
	wstring						m_wstrProtoFrontEquipTexName = L"";
	map<wstring, CTexture*>		m_mapEquipTexture;
	vector<CInvenItem*>			m_vecEquipSlot; //슬롯 가지고 있을 친구
	_bool						m_bUpdateEquipSlot = false;
	CUnit*						m_pUnit = nullptr;
};

#endif // !__Center_UI_H__