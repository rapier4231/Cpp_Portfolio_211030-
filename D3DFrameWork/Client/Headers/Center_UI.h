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
							// ��� �ؽ��� ����� ���� �� ���� �޴� �Լ�

public:
	virtual CGameObject*	Clone(void * _pArg = nullptr) PURE;
	virtual void			Free() PURE;

public:
	void					Set_EmplaceBack_EquipSlot(CInvenItem* _EquipSlot);//��� ���� �ִ� �� m_vecEquipSlot (�� ó���� �ѹ� �ϰ� ��ġ �ϰ� �Ⱦ� ��)
	virtual void			Match_EquipTex_Info() { return; }; //��� �ٲ� ������ ��� ���� ��ġ
	void					Set_EquipOwner(CUnit * _pUnit);

public:
	EQUIP_Position			Get_NowMousePickingSlot();
	CUnit*					Get_pOwner() { return m_pUnit; };

public:
	virtual void			Stat_UI_Close();
	void					All_Off();
protected:
	CTexture*				Get_pEquipTexture(_tchar* _wstrEquipName); //���Ե��� ��� ���� ���� �� �� �Լ�
	CTexture*				Create_EquipTexture(_tchar* _wstrEquipName); //���� ���� �� �� ������ ���� �Լ�

protected:
	wstring						m_wstrProtoFrontEquipTexName = L"";
	map<wstring, CTexture*>		m_mapEquipTexture;
	vector<CInvenItem*>			m_vecEquipSlot; //���� ������ ���� ģ��
	_bool						m_bUpdateEquipSlot = false;
	CUnit*						m_pUnit = nullptr;
};

#endif // !__Center_UI_H__