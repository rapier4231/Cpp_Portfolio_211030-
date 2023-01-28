#pragma once
#ifndef __LR_Inventory_H__
#define __LR_Inventory_H__

#include "Value.h"
#include "Scroll.h"
#include "OrthoUI.h"
#include "Scroll_UI.h"

#define ScrollShowSizeY_16To9 6.f
#define ScrollBarSizeY_16To9 6.f//À§¶û °°¾Æ¾ßÁö
#define ScrollBarSizeX_16To9 0.2f
#define FIRSTITEMSOLTAMOUNT 60

class CUnit;
class COrthoUI;
class CInvenItem;
class CBorder;
BEGIN(Engine)
class CInven;
class CTexture;
END

class CLR_Inventory : public CScroll
{
protected:
	explicit CLR_Inventory(LPDIRECT3DDEVICE9 _pDevice);
	explicit CLR_Inventory(const CLR_Inventory& rhs);
	virtual ~CLR_Inventory() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() PURE;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) PURE;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) PURE;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) PURE;
	virtual void			Render_GameObject() PURE;

public:
	void					Get_ProtoFrontItemTexName(const wstring& _wstrProtoFrontItemTexName);

public:
	virtual CGameObject*	Clone(void * _pArg = nullptr) PURE;
	virtual void			Free() PURE;

	////////////////////////////////////////////////////½ºÅ©·Ñ ²¨
public:
	virtual	void			Scroll_Func() PURE;
	virtual	void			Scroll_Move_Func(LONG _dwMoveY) PURE;
	virtual	void			Scroll_Move_Func(_float _fMoveFixRatioY) PURE;
protected:
	virtual void			Scroll_Set() PURE;
	//////////////////////////////////////////////////////////////
public:
	virtual CInvenItem*		CreateDragItemSlot(POINT _pPos, _int _iIdentityNum) PURE;
	virtual _int			DeleteDropItemSlot(POINT _pPos, _int _iIdentityNum, _bool _bLeft) PURE;

public:
	_int					Get_ItemSlotAmount(_bool _bAllNum);
	_int					Plus_ItmeSlotAmount();

public:
	virtual void			Set_InvenOwner(CUnit* _pUnit);
	virtual CUnit*			Get_InvenOwner();

public:
	void					Set_EmplaceBack_ItemSlot(CInvenItem* _InvenItemSlot);
	virtual void			Match_ItemTex_Info() PURE;

protected:
	_int					m_iDrag_DropSlotNumber = 0;
	CInvenItem*				m_pDrag_DropItemSlot = nullptr;
	CBorder*				m_pDrag_DropItemSlotBorder = nullptr;

public:
	CTexture*				Get_pItemTexture(_tchar* _wstrItemName);
	CTexture*				Create_ItemTexture(_tchar* _wstrItemName);

protected:
	wstring						m_wstrProtoFrontItemTexName = L"";
	map<wstring, CTexture*>		m_mapItemTexture;
	vector<CInvenItem*>			m_vecInvenItemSlot;
	_int						m_iLRInvenHaveItemSlotAmount = NULL;
	_int						m_iLRInvenHaveItemSlotAllAmount = FIRSTITEMSOLTAMOUNT;
	_bool						m_bUpdateItemSlot = false;
	CUnit*						m_pUnit = nullptr;
};

#endif // !__LR_Inventory_H__