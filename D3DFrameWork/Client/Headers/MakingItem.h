#pragma once
#ifndef __MakingItem_H__
#define __MakingItem_H__

#include "Value.h"
#include "OrthoUI.h"

#define MakingItemSizeX 36.f
#define MakingItemSizeY 36.f

enum ESlotOwner { Left_Inven, Right_Inven, Stat_Player, Stat_Monster, QuickSlot, SlotOwner_END };

class CLR_Inventory;
BEGIN(Engine)
class CInven;
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END

class CMakingItem : public COrthoUI
{
private:
	explicit CMakingItem(LPDIRECT3DDEVICE9 _pDevice);
	explicit CMakingItem(const CMakingItem& rhs);
	virtual ~CMakingItem() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;
private:
	void Render_Font(_mat _matOrtho);

public:
	static CMakingItem*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	void					Set_ItemTexture(CTexture* _pItemTexture);
	void					Set_ItemInfo(const ITEM_INFO*	_pItemInfo,ITEMID _eItemID = ITEMID_END);
	void					Set_pSlotOwner(CGameObject* _pSlotOwner, ESlotOwner _eSlotOwner);

public:
	void					Set_SlotNum(_int _Num) { m_iSlotNum = _Num; }
	_int					Get_SlotNum(_int _Num) { return m_iSlotNum; }

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho);

private:
	void					ItemSlotSelecFunc();
	void					CreateDragItemSlot();
	void					DeleteDropItemSlot();
	POINT					m_MouseClickPos;
	void					Remove_DropItem_To_Inven(CInven* _pInven=nullptr);

public:
	void					Set_MouseClickPos(POINT* _tNowMouse);
	POINT					m_tFirstPos;
public:
	void					Set_FirstPos(POINT _tPos);

private:
	CTexture*				m_pTextureCom = nullptr;

	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

private:
	_bool					m_bBoxSelect = false;
	wstring					m_strItemName = L"";	
	ITEM_INFO				m_ItemInfo ;			
	ESlotOwner				m_eSlotOwner = SlotOwner_END;
	CGameObject*			m_pSlotOwner = nullptr;

private:
	_bool					m_bDragItem = false;
public:
	void					Set_DragItem() { m_bDragItem = true; };

private:
	_int					m_iSlotNum = 0;
};

#endif // !__MakingItem_H__