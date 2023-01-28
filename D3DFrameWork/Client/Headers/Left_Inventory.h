#pragma once
#ifndef __Left_Inventory_H__
#define __Left_Inventory_H__

#define B_Order_Making (1 << 0)

#include "Value.h"
#include "LR_Inventory.h"
class COrthoUI;

enum EUSINGMAKINGINFO{
	/*EUSINGMAKINGINFO_MetalIngot ,*/ EUSINGMAKINGINFO_StoneAx, EUSINGMAKINGINFO_StonePick, EUSINGMAKINGINFO_ITEMID_Campfire, EUSINGMAKINGINFO_Club, EUSINGMAKINGINFO_Bolas,  EUSINGMAKINGINFO_Spear,
	EUSINGMAKINGINFO_SleepingBag, EUSINGMAKINGINFO_ForgedCrossbow, EUSINGMAKINGINFO_Bow, EUSINGMAKINGINFO_Arrow, EUSINGMAKINGINFO_END };

enum EScroll_Cut { EScroll_Cut_None, EScroll_Cut_Up, EScroll_Cut_Down};

class CLeft_Inventory : public CLR_Inventory
{
private:
	explicit CLeft_Inventory(LPDIRECT3DDEVICE9 _pDevice);
	explicit CLeft_Inventory(const CLeft_Inventory& rhs);
	virtual ~CLeft_Inventory() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	ITEM_INFO		Get_MousePick_On_Item(_int _UsingAmount = 0);//마우스 아래에 있는 아이템을 넘겨준다.

public:
	static CLeft_Inventory*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*			Clone(void * _pArg = nullptr) override;
	virtual void					Free() override;

public:
	virtual  CInvenItem*			CreateDragItemSlot(POINT _pPos, _int _iIdentityNum) override;
	virtual _int					DeleteDropItemSlot(POINT _pPos, _int _iIdentityNum, _bool _bLeft) override;

public:
	void					Left_InventoryOpen();
	void					Left_InventoryClose();
	
public:
	virtual void			Match_ItemTex_Info() override;

private:
	void					Inventory_Render_On();
	void					Inventory_Render_Off();

	//////////////////////////////////////제작
public:
	_bool					Create_MakingItem(ITEMID _MakingItemID,_bool _bQuickZeroMaking = false);
	//퀵슬롯에서 제작을 할 때, 얘를 불러주고, 퀵슬롯에선 0짜리 아이템의 수를 1 증가시키면 됨.
	//제작 실패하면 false반환이라 증가 ㄴ 하면 됨.
	//레프트 인벤 업뎃에서는 제작 탭 일때 어디를 클릭했나 확인 후
	//그 아이템 슬롯이 가지고있는 메이킹 인포를 가져와서 여기다 넣는 함수를 만든다.
	//물론 가져오기 전에 제작이 가능한 상태인지 확인 후 가져와야 하기 때문에
	//아이템 인포의 아이디 값이 end면 바로 리턴하는 예외처리를 해줘야 한다.
	//_bQuickZeroMaking false면 인벤에 넣어주고 끝, true는 퀵슬에서 0짜리 아이템 제작할때
	//true면 인벤에서 재료만 지우고 끝, 퀵슬롯에서 1 증가시킬 것이기 때문에.
	void					Create_MakingItem_Start_Check(const _double & _dTimeDelta);
	//제작탭이라면 E눌렀을때와 더블클릭 했을 때, 슬롯을 전부 돌아서 마우스 누구 위에 있나 판단하고
	//그 슬롯이 제작 가능한 슬롯이라면 Create_MakingItem 실행시켜 준다.
private:
	_bool					Check_Double_LButton_Click(const _double & _dTimeDelta);
	_float					m_fCheck_Double_LButton_Click = 0.0;

public:
	//void					Set_EmplaceBack_MakingSlot(CMakingItem* _InvenItemSlot){m_vecInvenMakingSlot.emplace_back(_InvenItemSlot);}
	void					ChangeInvenTap();
private:
	_bool						m_bInvenTap = false;
	map<wstring, CTexture*>		m_mapMakingTexture;
	vector<MAKING_INFO>			m_vecInvenMakingInfo;
public:
	CTexture*				Get_pMakingTexture(_tchar* _wstrItemName);
	CTexture*				Create_MakingTexture(_tchar* _wstrItemName);

private:
	virtual void			Scroll_Set() override;
public:
	virtual	void			Scroll_Func() override;
	virtual	void			Scroll_Move_Func(LONG _dwMoveY) override;
	virtual	void			Scroll_Move_Func(_float _fMoveFixRatioY) override;
};

#endif // !__Left_Inventory_H__