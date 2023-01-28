#pragma once
#ifndef __Stage_H__
#define __Stage_H__

#include "Value.h"
#include "Scene.h"
class CLR_Inventory;
class COrthoUI;
class CCenter_UI;
class CQuickSlot_UI;
//class CLoading;

class CStage : public CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CStage(void) = default;

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual HRESULT Late_Ready_Scene(void) override;
	virtual _int Update_Scene(const _double & dTimeDelta) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT		Ready_Empty_Layer(const wstring& _wstrLayerTag);			// �� ���̾�

	HRESULT		Ready_Terrain_Layer(const wstring& _wstrLayerTag);			// ���� �� ������Obstacle
	HRESULT		Ready_Player_Layer(const wstring& _wstrLayerTag);			// ���� ���̾�
	HRESULT		Ready_Dino_Layer(const wstring& _wstrLayerTag);				// ����

	HRESULT		Ready_Ball_Layer(const wstring& _wstrLayerTag);				// ��
	HRESULT		Ready_BallHoop_Layer(const wstring& _wstrLayerTag);				// ��


	//HRESULT		Ready_Harvestable_Layer(const wstring& _wstrLayerTag);		// ä����
	HRESULT		Ready_Harvest_Tree_Layer(const wstring& _wstrLayerTag);
	HRESULT		Ready_Harvest_Bush_Layer(const wstring& _wstrLayerTag);
	HRESULT		Ready_Harvest_Rock_Layer(const wstring& _wstrLayerTag);
	HRESULT		Ready_Harvest_Pebble_Layer(const wstring& _wstrLayerTag);
	HRESULT		Ready_Environment_Layer(const wstring& _wstrLayerTag);		// ȯ�湰
	HRESULT		Ready_Camera_Layer(const wstring& _wstrLayerTag);			// UI ���̾�
	HRESULT		Ready_UI();
	HRESULT		Ready_Inventory_UI(const wstring& _wstrLayerTag);
	HRESULT		Ready_Center_UI(const wstring& _wstrLayerTag);
	HRESULT     Ready_SummaryStat_UI(const wstring& _wstrLayerTag);
	HRESULT     Ready_QuickSlot_UI(const wstring& _wstrLayerTag);
	HRESULT     Ready_GuideFont_UI(const wstring& _wstrLayerTag);
	HRESULT		Ready_Effect_Layer(const wstring& _wstrLayerTag);
	HRESULT		Ready_Test(const wstring& _wstrLayerTag);
	HRESULT     Ready_InfoView_UI(const wstring& _wstrLayerTag);
	HRESULT     Ready_WorldFont_UI(const wstring& _wstrLayerTag);
	HRESULT     Ready_PlayerHit_UI(const wstring& _wstrLayerTag);
	HRESULT     Ready_MiniMap_UI(const wstring& _wstrLayerTag);
	HRESULT		Ready_Taming_Name_UI(const wstring& _wstrLaerTag);
	HRESULT		Ready_RingCommand_UI(const wstring& _wstrLaerTag);
	HRESULT		Ready_CrossHair_UI(const wstring& _wstrLaerTag);
private:
	HRESULT		Ready_LightInfo(void);										// ���� ����

private:
	HRESULT		Ready_InventoryUI(const wstring& _wstrLayerTag, CGameObject* pLR_Inventory, _bool _LeftInventory);
	HRESULT		Ready_ItemSlot(const wstring& _wstrLayerTag, CLR_Inventory* _pLR_Inventory, COrthoUI* _pInventory, _bool _LeftInventory);
	HRESULT		Ready_Stat_UI(const wstring& _wstrLayerTag, CGameObject* pCenter_UI, CENTER_UIID _eCenter_UI_Type);
	HRESULT		Ready_MonsterStat_UI(const wstring& _wstrLayerTag, CGameObject* pCenter_UI, CENTER_UIID _eCenter_UI_Type);
	HRESULT		Ready_StructureStat_UI(const wstring& _wstrLayerTag, CGameObject* pCenter_UI);
	HRESULT		Ready_EquipSlot(const wstring& _wstrLayerTag, CCenter_UI* _pCenter_UI, COrthoUI* _pCenter_Back_UI, CENTER_UIID _eCenter_UI_Type);
	HRESULT		Ready_QuickSlot(const wstring& _wstrLayerTag, CQuickSlot_UI* _pQuickSlot);

private:
	HRESULT		LoadFile_ObjectList(const wstring & wstrFilePath);	//���Ͽ��� ������Ʈ��ġ �о ������Ʈ Ŭ��

public:
	static CStage* Create(LPDIRECT3DDEVICE9 _pDevice, stSC* pCount);
	virtual void Free() override;

public:
	//HRESULT	ChangeResolutionFunc() // ������ ���� �� ��ġ(�κ� ���� ���â)

public:
	static stSC*	m_pCount;
	_bool			m_bKeyDown = false;
};

#endif // !__Stage_H__
