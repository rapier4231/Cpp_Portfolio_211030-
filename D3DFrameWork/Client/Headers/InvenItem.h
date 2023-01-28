#pragma once
#ifndef __InvenItem_H__
#define __InvenItem_H__

#include "Value.h"
#include "OrthoUI.h"

//#define InvenItemSizeX 36.f
//#define InvenItemSizeY 36.f
#define InvenItemSlotHorizonAmount 5
#define PlayerEquipAmount 6
#define PlayerEquipYAmount 3
#define QuickSlotAmount 10
enum ESlotOwner { Left_Inven, Right_Inven, Stat_Player, Stat_Monster, QuickSlot, SlotOwner_END };

class CLR_Inventory;
BEGIN(Engine)
class CInven;
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END

class CInvenItem : public COrthoUI
{
public:
	typedef struct stInvenItem_Resolution_Info : STOURI
	{
		_float fSizeXY[ESNRT_END] = { 0.18f*ScrnRatio, 0.75f*ScrnRatio };
		_float fQuickSizeXY[ESNRT_END] = { 0.18f*ScrnRatio, 0.5f*ScrnRatio };
		_vec4  v4EdgeSize[ESNRT_END] = {{ 0.1f ,0.1f  , 0.9f, 0.9f },
										/*{ 0.03f ,0.03f  , 0.96f, 0.96f }*/{ 0.03f ,0.03f  , 0.97f, 0.97f } };
		_float fFont_Equip_Type_PosXY[ESNRT_END] = { 0.085f*ScrnRatio, 0.3f*ScrnRatio };
		_float fFont_Item_Amount1_PosX[ESNRT_END] = { 0.03f*ScrnRatio, 0.27f*ScrnRatio };
		_float fFont_Item_Amount10_PosX[ESNRT_END] = { 0.015f*ScrnRatio, 0.18f*ScrnRatio };
		_float fFont_Item_Amount100_PosX[ESNRT_END] = { 0.f*ScrnRatio, 0.12f*ScrnRatio };
		_float fFont_Item_Amount_PosY[ESNRT_END] = { 0.035f*ScrnRatio, 0.23f*ScrnRatio };

		_float fFont_Quick_Item_Amount1_PosX[ESNRT_END] = { 0.03f*ScrnRatio, 0.15f*ScrnRatio };
		_float fFont_Quick_Item_Amount10_PosX[ESNRT_END] = { 0.015f*ScrnRatio, 0.09f*ScrnRatio };
		_float fFont_Quick_Item_Amount100_PosX[ESNRT_END] = { 0.f*ScrnRatio, 0.04f*ScrnRatio };
		_float fFont_Quick_Item_Amount_PosY[ESNRT_END] = { 0.035f*ScrnRatio, 0.1f*ScrnRatio };

		_float fInventory_FirstFixPosX[ESNRT_END] = { { 0.505f*ScrnRatio },{ 2.2f*ScrnRatio } };
		_float fInventory_FirstFixPosY[ESNRT_END] = { { 0.96f*ScrnRatio },{ 3.3f*ScrnRatio } };
		_float fInven_Interval_FixPosXY[ESNRT_END] = { { 0.02f*ScrnRatio },{ 0.1f*ScrnRatio } };

		_float fStat_Player_FirstFixPosX[ESNRT_END] = { { 0.45f*ScrnRatio },{ 1.8f*ScrnRatio } };
		_float fStat_Player_FirstFixPosY[ESNRT_END] = { { 0.94f*ScrnRatio },{ 3.f*ScrnRatio } };
		_float fStat_Monster_FirstFixPosY[ESNRT_END] = { { 0.4f*ScrnRatio },{ 0.5f*ScrnRatio }  };
		_float fStat_Interval_FixPosX[ESNRT_END] = { { 0.45f*ScrnRatio },{ 1.8f*ScrnRatio } };
		_float fStat_Interval_FixPosY[ESNRT_END] = { { 0.05f*ScrnRatio },{ 0.1f*ScrnRatio } };


		_float fQuickSlot_FirstPosX[ESNRT_END] = { { 1.055f*ScrnRatio },{ 5.37f*ScrnRatio } };
		_float fQuickSlot_FirstPosY[ESNRT_END] = { { 2.86f*ScrnRatio },{ 8.7f*ScrnRatio } };
		_float fQuickSlot_Interval_FixPosX[ESNRT_END] = { { 0.03f*ScrnRatio },{ 0.1f*ScrnRatio } };

	}STInvenItemURI;

private:
	STInvenItemURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

private:
	explicit CInvenItem(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInvenItem(const CInvenItem& rhs);
	virtual ~CInvenItem() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;
private:
	void					Render_Font(_mat _matOrtho);

public:
	static CInvenItem*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	void					Set_ItemTexture(CTexture* _pItemTexture, CTexture* _pEngram_BackGroundTexture = nullptr, CTexture* _pCraftingBrokenTexture = nullptr);
	void					Set_ItemInfo(const ITEM_INFO*	_pItemInfo, ITEMID _eItemID = ITEMID_END);
	void					Set_MakingInfo(const MAKING_INFO*_pMakingInfo);
	void					Set_pSlotOwner(CGameObject* _pSlotOwner, ESlotOwner _eSlotOwner);

public:
	const ITEM_INFO&		Get_ItemInfo(){ return m_ItemInfo; }

public:
	ITEMID					Get_CanCreateItme_Question();

public:
	void					Set_SlotNum(_int _Num);
	_int					Get_SlotNum() { return m_iSlotNum; }

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
	CTexture*				m_pEngram_BackGroundCom = nullptr;
	CTexture*				m_pCraftingBrokenCom = nullptr;
	_bool					m_bPossibleMaking = false;
	_bool					m_bPossibleMakingLv = false;

public:
	void					Set_Go_Making();
private:
	_bool					m_bNow_Making = false;
	_float					m_fMakingTime = FNULL;
	void					Making_Item_Loading_And_Create(const _double & _dTimeDelta);
private:
	void					Decide_MakingColor();
	_bool					Check_MakingPossible();
	_bool					Check_MakingPossibleLv();
	MAKING_INFO				m_MakingStartItmeInfo;

private:
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

private:
	_bool					m_bBoxSelect = false;
	wstring					m_strItemName = L"";	
	ITEM_INFO				m_ItemInfo;			
	MAKING_INFO				m_MakingInfo;
	ESlotOwner				m_eSlotOwner = SlotOwner_END;
	CGameObject*			m_pSlotOwner = nullptr;

private:
	_bool					m_bDragItem = false;
public:
	void					Set_DragItem() { m_bDragItem = true; };

private:
	_int					m_iSlotNum = 0;

	_int					m_iFirstSlotNum = 0;
	_bool					m_bFirstSlotCheck = false;

private:
	_bool					m_bCreateDragSlot =false;

private:
	_float					m_fItem_Info_View_Time = 0.f;
	void					Create_Item_Info_View(const _double & _dTimeDelta);

private:
	MII_INFO				m_tMII;

public:
	void					Set_Scroll_Show(_float _fScroll_MoveY, _float _fShow_TopY, _float _fShow_BottomY);
	_float					m_fScroll_FixY=0.f;
	_bool					m_bScroll_No_Render = false;
	_float					m_fDrawCutRatio = 0.f;
	_int					m_iDrawCutUp = 0; //0은 안하는거, 1은 위 , 2는 아래

private:
	_bool					Scroll_Hide_OnMouse_Func_Lock();
	_float					m_fShow_TopY = 0.f;
	_float					m_fShow_BottomY = 0.f;
};

#endif // !__InvenItem_H__