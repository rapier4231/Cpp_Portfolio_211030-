#pragma once
#ifndef __QuickSlot_UI_H__
#define __QuickSlot_UI_H__

#include "Value.h"
#include "OrthoUI.h"

class CPlayer;
class CBorder;
class CInvenItem;
BEGIN(Engine)
class CTexture;
END

class CQuickSlot_UI : public CGameObject
{
private:
	explicit CQuickSlot_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CQuickSlot_UI(const CQuickSlot_UI& rhs);
	virtual ~CQuickSlot_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() { return; };

public:
	void					Get_ProtoFrontTexName(wstring* _wstrProtoFrontTexName);

public:
	static  CQuickSlot_UI*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	void					Add_Item(ITEMID _eItemID, _int _iAmount);
private:
	_int					Get_ItemTypeAllAmount(ITEMID _eItemID);

public:
	void					Set_EmplaceBack_Slot(CInvenItem* _QuickSlot);
	virtual void			Match_Tex_Info(); 

public:
	_int					Get_NowMousePickingSlot();
	ITEM_INFO				Set_Item(ITEMID _eItemID, _int _iAmount = 1);
	void					RemoveItem(_int _iItemNum);
	_bool					UisngRemove_Item(_int _iInvenNum, _int _iAmount);

	CInvenItem*				CreateDragItemSlot(POINT _pPos, _int _iIdentityNum);
	void					ChangeItem(const ITEM_INFO& _tChangeSlot, _int _iSlotNum);
	_int					Now_Have_ItemAmount(ITEMID _eItemID);

public:
	const ITEM_INFO&        Get_Item_To_Number(_int _iQuickSlotNumber , _int UsingAmount = 0);

public:
	MAKING_INFO*			Making_Item_Remove_ingredient(MAKING_INFO* _pCheckDeficientItem);

private:
	CTexture*				Get_pTexture(_tchar* _wstrEquipName); //슬롯들이 장비 사진 받을 때 쓸 함수
	CTexture*				Create_Texture(_tchar* _wstrEquipName); //위에 사진 줄 때 없으면 만들 함수

private:
	vector<ITEM_INFO>		m_vecQuickItem;
	vector<CInvenItem*>		m_vecQuickSlot; //슬롯 가지고 있을 친구

	_int					m_iDrag_DropSlotNumber = 0;
	CInvenItem*				m_pDrag_DropQuickSlot = nullptr;
	CBorder*				m_pDrag_DropQuickBorder = nullptr;
	
private:
	map<wstring, CTexture*>		m_mapTexture;
	wstring						m_wstrProtoFrontTexName = L"";
	CPlayer*					m_pPlayer = nullptr;
};

#endif // !__QuickSlot_UI_H__