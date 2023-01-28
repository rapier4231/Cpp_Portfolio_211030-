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
	ITEM_INFO		Get_MousePick_On_Item(_int _UsingAmount = 0);//���콺 �Ʒ��� �ִ� �������� �Ѱ��ش�.

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

	//////////////////////////////////////����
public:
	_bool					Create_MakingItem(ITEMID _MakingItemID,_bool _bQuickZeroMaking = false);
	//�����Կ��� ������ �� ��, �긦 �ҷ��ְ�, �����Կ��� 0¥�� �������� ���� 1 ������Ű�� ��.
	//���� �����ϸ� false��ȯ�̶� ���� �� �ϸ� ��.
	//����Ʈ �κ� ���������� ���� �� �϶� ��� Ŭ���߳� Ȯ�� ��
	//�� ������ ������ �������ִ� ����ŷ ������ �����ͼ� ����� �ִ� �Լ��� �����.
	//���� �������� ���� ������ ������ �������� Ȯ�� �� �����;� �ϱ� ������
	//������ ������ ���̵� ���� end�� �ٷ� �����ϴ� ����ó���� ����� �Ѵ�.
	//_bQuickZeroMaking false�� �κ��� �־��ְ� ��, true�� �������� 0¥�� ������ �����Ҷ�
	//true�� �κ����� ��Ḹ ����� ��, �����Կ��� 1 ������ų ���̱� ������.
	void					Create_MakingItem_Start_Check(const _double & _dTimeDelta);
	//�������̶�� E���������� ����Ŭ�� ���� ��, ������ ���� ���Ƽ� ���콺 ���� ���� �ֳ� �Ǵ��ϰ�
	//�� ������ ���� ������ �����̶�� Create_MakingItem ������� �ش�.
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