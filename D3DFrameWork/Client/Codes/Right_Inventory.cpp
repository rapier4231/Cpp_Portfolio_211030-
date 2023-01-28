#include "stdafx.h"
#include "..\Headers\Right_Inventory.h"

#include "Export_Function.h"
#include "InvenItem.h"
#include "Inven.h"
#include "Unit.h"
#include "Border.h"
#include "Left_Inventory.h"
#include "Player.h"
#define ScrollBarFixPosX_16To9 14.8f
#define ScrollBarFixPosY_16To9 5.02f
USING(Engine)

CRight_Inventory::CRight_Inventory(LPDIRECT3DDEVICE9 _pDevice)
	: CLR_Inventory(_pDevice)
{
}

CRight_Inventory::CRight_Inventory(const CRight_Inventory & rhs)
	: CLR_Inventory(rhs)
{
}

HRESULT CRight_Inventory::Ready_GameObject_Prototype()
{
	CLR_Inventory::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CRight_Inventory::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CLR_Inventory::Ready_GameObject(_pArg);

	//////////////////
	m_vecInvenMakingInfo.resize(EUSINGMAKINGINFO_END);
	MAKING_INFO tTestMakingItem;
	//tTestMakingItem = Engine::Clone_MakingItem_Prototype(ITEMID_MetalIngot);
	//m_vecInvenMakingInfo[EUSINGMAKINGINFO_MetalIngot] = tTestMakingItem;
	tTestMakingItem = Engine::Clone_MakingItem_Prototype(ITEMID_Club);
	m_vecInvenMakingInfo[EUSINGMAKINGINFO_Club] = tTestMakingItem;
	tTestMakingItem = Engine::Clone_MakingItem_Prototype(ITEMID_StoneAx);
	m_vecInvenMakingInfo[EUSINGMAKINGINFO_StoneAx] = tTestMakingItem;
	tTestMakingItem = Engine::Clone_MakingItem_Prototype(ITEMID_StonePick);
	m_vecInvenMakingInfo[EUSINGMAKINGINFO_StonePick] = tTestMakingItem;
	tTestMakingItem = Engine::Clone_MakingItem_Prototype(ITEMID_Campfire);
	m_vecInvenMakingInfo[EUSINGMAKINGINFO_ITEMID_Campfire] = tTestMakingItem;
	tTestMakingItem = Engine::Clone_MakingItem_Prototype(ITEMID_Bolas);
	m_vecInvenMakingInfo[EUSINGMAKINGINFO_Bolas] = tTestMakingItem;
	tTestMakingItem = Engine::Clone_MakingItem_Prototype(ITEMID_Spear);
	m_vecInvenMakingInfo[EUSINGMAKINGINFO_Spear] = tTestMakingItem;
	tTestMakingItem = Engine::Clone_MakingItem_Prototype(ITEMID_ForgedCrossbow);
	m_vecInvenMakingInfo[EUSINGMAKINGINFO_ForgedCrossbow] = tTestMakingItem;
	tTestMakingItem = Engine::Clone_MakingItem_Prototype(ITEMID_SleepingBag);
	m_vecInvenMakingInfo[EUSINGMAKINGINFO_SleepingBag] = tTestMakingItem;

	tTestMakingItem = Engine::Clone_MakingItem_Prototype(ITEMID_Bow);
	m_vecInvenMakingInfo[EUSINGMAKINGINFO_Bow] = tTestMakingItem;
	tTestMakingItem = Engine::Clone_MakingItem_Prototype(ITEMID_Arrow);
	m_vecInvenMakingInfo[EUSINGMAKINGINFO_Arrow] = tTestMakingItem;

	m_bInvenTap = false;

	return S_OK;
}

_int CRight_Inventory::Update_GameObject(const _double & _dTimeDelta)
{
	CLR_Inventory::Update_GameObject(_dTimeDelta);
	if (m_bInvenTap)//���� ���̶��
	{
		Create_MakingItem_Start_Check(_dTimeDelta);//���� Ȯ�� �Լ�
	}
	//if (!m_pUnit)
	//{
	//	//�ӽ� �׽�Ʈ ��
	//	m_pUnit =static_cast<CUnit*>((Engine::Get_GameObject(L"Layer_Dino", 0)));
	//}
	//
	//if (m_pUnit->Get_Dead())
	//	m_pUnit = nullptr;

	//if (Engine::KeyDown(KeyCheck_I) && !m_bUpdateItemSlot)
	//{
	//	Inventory_Render_On();
	//	m_bUpdateItemSlot = true;
	//}
	//if (Engine::KeyDown(KeyCheck_I) && m_bUpdateItemSlot)
	//{
	//	Inventory_Render_Off();
	//	m_bUpdateItemSlot = false;
	//}

	if (m_pDrag_DropItemSlot)
	{
		if (OBJECT_DEAD == m_pDrag_DropItemSlot->Update_GameObject(_dTimeDelta))
		{
			Match_ItemTex_Info();
			Safe_Release(m_pDrag_DropItemSlot);
			Safe_Release(m_pDrag_DropItemSlotBorder);
			m_pDrag_DropItemSlot = nullptr;
			m_pDrag_DropItemSlotBorder = nullptr;
		}
		else
		{
			m_pDrag_DropItemSlotBorder->Update_GameObject(_dTimeDelta);
		}
	}
	return 0;
}

_int CRight_Inventory::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CLR_Inventory::LateUpdate_GameObject(_dTimeDelta);
	if (m_pDrag_DropItemSlot)
	{
		m_pDrag_DropItemSlot->LateUpdate_GameObject(_dTimeDelta);
		m_pDrag_DropItemSlotBorder->LateUpdate_GameObject(_dTimeDelta);
	}
	return 0;
}

void CRight_Inventory::Render_GameObject()
{

}

CRight_Inventory * CRight_Inventory::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CRight_Inventory*	pInstance = new CRight_Inventory(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CRight_Inventory::Clone(void * _pArg/* = nullptr*/)
{
	CRight_Inventory*	pClone = new CRight_Inventory(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CRight_Inventory::Free()
{
	if (m_pDrag_DropItemSlot)
		Safe_Release(m_pDrag_DropItemSlot);
	
	CLR_Inventory::Free();
}

CInvenItem * CRight_Inventory::CreateDragItemSlot(POINT _pPos, _int _iIdentityNum)
{
	CLR_Inventory::CreateDragItemSlot(_pPos, _iIdentityNum)->Set_pSlotOwner(this, ESlotOwner::Right_Inven);
	return nullptr;
}

_int CRight_Inventory::DeleteDropItemSlot(POINT _pPos, _int _iIdentityNum, _bool _bLeft)
{
	CLR_Inventory::DeleteDropItemSlot(_pPos, _iIdentityNum, _bLeft);
	Match_ItemTex_Info();
	static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Match_ItemTex_Info();
	return 0;
}

void CRight_Inventory::Right_InventoryOpen()
{
	m_bUpdateItemSlot = false;
	Inventory_Render_On();
	//�ӽ�
	static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Left_InventoryOpen();

}

void CRight_Inventory::Right_InventoryClose()
{
	m_bUpdateItemSlot = true;
	Inventory_Render_Off();
	//�ӽ�
	static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Left_InventoryClose();
}

void CRight_Inventory::Match_ItemTex_Info()
{
	if (!m_pUnit)
		return;
	CInven* pInven = nullptr;

	if (nullptr == static_cast<CInven*>(m_pUnit->Get_Component(L"Com_Show_Inven", ID_DYNAMIC)))
		pInven = static_cast<CInven*>(m_pUnit->Get_Component(L"Com_Inven", ID_DYNAMIC));
	else
		pInven = static_cast<CInven*>(m_pUnit->Get_Component(L"Com_Show_Inven", ID_DYNAMIC));

	if(!pInven)
		return;

	if (m_bInvenTap)
	{
		vector<MAKING_INFO>::iterator iter = m_vecInvenMakingInfo.begin();
		_int iCheckItemNum = 0;
		for (; iter != m_vecInvenMakingInfo.end(); ++iter)
		{
			m_vecInvenItemSlot[iCheckItemNum]->Set_ItemInfo(nullptr);
			m_vecInvenItemSlot[iCheckItemNum]->Set_MakingInfo(&m_vecInvenMakingInfo[iCheckItemNum]);
			m_vecInvenItemSlot[iCheckItemNum]->Set_ItemTexture(Get_pItemTexture((*iter).tchName), Get_pItemTexture(L"Engram_BackGround"), Get_pItemTexture(L"CraftingBroken"));
			m_vecInvenItemSlot[iCheckItemNum]->Set_SlotNum(iCheckItemNum);
			++iCheckItemNum;
		}

		for (_int i = iCheckItemNum; i < m_vecInvenItemSlot.size(); ++i)
		{
			m_vecInvenItemSlot[i]->Set_MakingInfo(nullptr);
			m_vecInvenItemSlot[i]->Set_ItemInfo(nullptr);
			m_vecInvenItemSlot[i]->Set_ItemTexture(nullptr);
			m_vecInvenItemSlot[iCheckItemNum]->Set_SlotNum(iCheckItemNum);
		}
	}
	else
	{
		vector<ITEM_INFO> vecInvenItem = *(pInven->Get_pInvenItem());
		vector<ITEM_INFO>::iterator iter = vecInvenItem.begin();
		_int iCheckItemNum = 0;
		for (; iter != vecInvenItem.end(); ++iter)
		{
			m_vecInvenItemSlot[iCheckItemNum]->Set_ItemInfo(&(vecInvenItem[iCheckItemNum]));
			m_vecInvenItemSlot[iCheckItemNum]->Set_ItemTexture(Get_pItemTexture((*iter).tchName));
			m_vecInvenItemSlot[iCheckItemNum]->Set_SlotNum(iCheckItemNum);
			++iCheckItemNum;
		}

		for (_int i = iCheckItemNum; i < m_vecInvenItemSlot.size(); ++i)
		{
			m_vecInvenItemSlot[i]->Set_ItemInfo(nullptr);
			m_vecInvenItemSlot[i]->Set_ItemTexture(nullptr);
			m_vecInvenItemSlot[iCheckItemNum]->Set_SlotNum(iCheckItemNum);
		}
	}
}

void CRight_Inventory::Set_InvenOwner(CUnit * _pUnit)
{
	if (!_pUnit)
		return;
	CLR_Inventory::Set_InvenOwner(_pUnit);
	InvenUserUnitTypeID eEnum;
	//� ������ �׳� �κ� ������ ����
	if (nullptr == static_cast<CInven*>(_pUnit->Get_Component(L"Com_Show_Inven", ID_DYNAMIC)))
		eEnum = static_cast<CInven*>(_pUnit->Get_Component(L"Com_Inven", ID_DYNAMIC))->Get_InvenUserUnitTypeID();
	else
		eEnum = static_cast<CInven*>(_pUnit->Get_Component(L"Com_Show_Inven", ID_DYNAMIC))->Get_InvenUserUnitTypeID();

	if (InvenUserUnitTypeID_Monster == eEnum)
	{
		m_bUpdateItemSlot = false;
		Inventory_Render_On();
	}
	else if (InvenUserUnitTypeID_Campfire <= eEnum && InvenUserUnitTypeID_Harvestable > eEnum)
	{
		m_bUpdateItemSlot = false;
		Inventory_Render_On();
	}
}

void CRight_Inventory::Inventory_Render_On()
{
	if (m_pUnit && !m_bUpdateItemSlot)
	{
		Match_ItemTex_Info();
		for (auto& iter : *(Engine::Get_Layer(L"Right_Inventory")->Get_ListGameObject()))
			static_cast<COrthoUI*>(iter)->UI_RenderOn();
		for (auto& iter : *(Engine::Get_Layer(L"Right_Inventory_ItemSlot")->Get_ListGameObject()))
			static_cast<COrthoUI*>(iter)->UI_RenderOn();
		m_bUpdateItemSlot = true;

		static_cast<COrthoUI*>(Engine::Get_GameObject(L"LR_Inventory", 3))->UI_RenderOn();

	}
}

void CRight_Inventory::Inventory_Render_Off()
{
	for (auto& iter : *(Engine::Get_Layer(L"Right_Inventory")->Get_ListGameObject()))
		static_cast<COrthoUI*>(iter)->UI_RenderOff();
	for (auto& iter : *(Engine::Get_Layer(L"Right_Inventory_ItemSlot")->Get_ListGameObject()))
		static_cast<COrthoUI*>(iter)->UI_RenderOff();
	m_pUnit = nullptr;

	static_cast<COrthoUI*>(Engine::Get_GameObject(L"LR_Inventory", 3))->UI_RenderOff();
}

void CRight_Inventory::Scroll_Set()
{
	ORTHOUI_INFO stTemp;

	if (Engine::CGraphicDevice::ESCREEN_RESOULTION_16To9 == EScrnResolution)
		m_fShow_SizeY = (ScrollShowSizeY_16To9 * ScrnRatio);

	_int iFilter_Out_Border = 0;

	for (auto& iter : *(Engine::Get_Layer(L"Right_Inventory_ItemSlot")->Get_ListGameObject()))
	{
		if (NULL == (iFilter_Out_Border % 2))
		{
			if (NULL == stTemp.fPosX)// �� ù��° ģ���� ������ �־�� ��
			{
				stTemp = *static_cast<COrthoUI*>(iter)->Get_ORTHOUI_INFO();
				m_fMax_Left_ItemPosX = stTemp.fPosX - (stTemp.fSizeX*FHALF);
				m_fMax_Right_ItemPosX = stTemp.fPosX + (stTemp.fSizeX*FHALF);
				m_fMax_Top_ItemPosY = stTemp.fPosY - (stTemp.fSizeY*FHALF);
				m_fMax_Bottom_ItemPosY = stTemp.fPosY + (stTemp.fSizeY*FHALF);
			}
			else
			{
				stTemp = *static_cast<COrthoUI*>(iter)->Get_ORTHOUI_INFO();

				if (m_fMax_Left_ItemPosX > stTemp.fPosX - (stTemp.fSizeX*FHALF))
					m_fMax_Left_ItemPosX = stTemp.fPosX - (stTemp.fSizeX*FHALF);
				if (m_fMax_Right_ItemPosX < stTemp.fPosX + (stTemp.fSizeX*FHALF))
					m_fMax_Right_ItemPosX = stTemp.fPosX + (stTemp.fSizeX*FHALF);
				if (m_fMax_Top_ItemPosY > stTemp.fPosY - (stTemp.fSizeY*FHALF))
					m_fMax_Top_ItemPosY = stTemp.fPosY - (stTemp.fSizeY*FHALF);
				if (m_fMax_Bottom_ItemPosY < stTemp.fPosY + (stTemp.fSizeY*FHALF))
					m_fMax_Bottom_ItemPosY = stTemp.fPosY + (stTemp.fSizeY*FHALF);
			}
		}
		++iFilter_Out_Border;
	}
	m_fStart_Show_PosY = m_fMax_Top_ItemPosY + m_fShow_SizeY*FHALF;
	m_fEnd_Show_PosY = m_fMax_Bottom_ItemPosY - m_fShow_SizeY*FHALF;

	m_fAll_SizeY = m_fMax_Bottom_ItemPosY - m_fMax_Top_ItemPosY;

	m_fShow_Top_ItemPosY = m_fMax_Top_ItemPosY;
	/*�� Ʋ�ȴ� �̰� ������ ���ְ� �Ʒ� �Լ��� ������ �־�� �ϴµ�, ��ũ�� Ŭ�������� ������ ���� ���� �÷��� �ϳ� ���� ����*/
	//���� ���� ���޾Ҵµ� ��ħ�� ���ϱ� �ؾ������ ����;;
	//m_fShow_SizeY -> ������ �� ������� �� ��
	m_fShow_Bottom_ItemPosY = m_fShow_Top_ItemPosY + m_fShow_SizeY;

	static_cast<CScroll_UI*>(Engine::Get_GameObject(L"LR_Inventory", 3))->Set_Scroll_Master_Info(this, (m_fMax_Left_ItemPosX + (m_fMax_Right_ItemPosX - m_fMax_Left_ItemPosX)*FHALF), (m_fShow_Top_ItemPosY + m_fShow_SizeY*FHALF), ((m_fMax_Right_ItemPosX - m_fMax_Left_ItemPosX)), m_fShow_SizeY);
	if (Engine::CGraphicDevice::ESCREEN_RESOULTION_16To9 == EScrnResolution)
		static_cast<CScroll_UI*>(Engine::Get_GameObject(L"LR_Inventory", 3))->Set_Scroll_Info(ScrollBarFixPosX_16To9*ScrnRatio, ScrollBarFixPosY_16To9*ScrnRatio, ScrollBarSizeX_16To9*ScrnRatio, ScrollBarSizeY_16To9*ScrnRatio);
}

void CRight_Inventory::Scroll_Func()
{										//���� ��ü�� ��Ʈ ������ Y ���� ���Ѱű� ������ 
	m_fShow_PosY = m_fMax_Top_ItemPosY + ((m_fAll_SizeY - m_fShow_SizeY) * m_fShow_RatioY) + (m_fShow_SizeY*FHALF);
	m_fShow_Top_ItemPosY = m_fShow_PosY - m_fShow_SizeY*FHALF;
	m_fShow_Bottom_ItemPosY = m_fShow_PosY + m_fShow_SizeY*FHALF;
	static_cast<CScroll_UI*>(Engine::Get_GameObject(L"LR_Inventory", 3))->Set_Scroll_Rect(m_fMax_Top_ItemPosY, m_fMax_Bottom_ItemPosY, m_fShow_Top_ItemPosY, m_fShow_Bottom_ItemPosY);

	_float fFixPosY = m_fShow_PosY - (m_fMax_Top_ItemPosY + (m_fShow_SizeY*FHALF));
	// �����۵� �Ⱥ��̰� �� ��� ��
	_int iFilter_Out_Border = 0;
	for (auto& iter : *(Engine::Get_Layer(L"Right_Inventory_ItemSlot")->Get_ListGameObject()))
	{
		//���� �۽�Ʈ ���� ������ ���� �״ϱ� ����? ���� Y ž ���� �ΰ� �༭ �˾Ƽ� ���ؼ� �׸��� ���� ����
		//												���� �������� �߾� ���� Y, �� ������ Y �̰� �ΰ��� �˰���?
		// _fShowPosY - (_fMax_Top_PosY + (_fShowSizeY*FHALF));
		if (NULL == (iFilter_Out_Border % 2))
			static_cast<CInvenItem*>(iter)->Set_Scroll_Show(fFixPosY, m_fMax_Top_ItemPosY, m_fMax_Top_ItemPosY + m_fShow_SizeY);
		else
			static_cast<CBorder*>(iter)->Set_Scroll_Show(fFixPosY, m_fMax_Top_ItemPosY, m_fMax_Top_ItemPosY + m_fShow_SizeY);
		++iFilter_Out_Border;
	}
}

void CRight_Inventory::Scroll_Move_Func(LONG _dwMoveY)
{
	m_fShow_RatioY += (_float)_dwMoveY / MouseWheelFix;
	if (0.f > m_fShow_RatioY)
		m_fShow_RatioY = 0.f;
	if (1.f < m_fShow_RatioY)
		m_fShow_RatioY = 1.f;
	Scroll_Func();
}

void CRight_Inventory::Scroll_Move_Func(_float _fMoveFixRatioY)
{
	m_fShow_RatioY = _fMoveFixRatioY;
	if (0.f > m_fShow_RatioY)
		m_fShow_RatioY = 0.f;
	if (1.f < m_fShow_RatioY)
		m_fShow_RatioY = 1.f;

	Scroll_Func();
}

_bool CRight_Inventory::Create_MakingItem(ITEMID _MakingItemID, _bool _bQuickZeroMaking)
{
	//MAKING_INFO ����ó��
	if (ITEMID_END == _MakingItemID)
		return false;
	MAKING_INFO tCheckDeficientItem = Engine::Clone_MakingItem_Prototype(_MakingItemID);
	//�κ����� ������ �����Ѵ�
	CInven* pForgeInven = static_cast<CInven*> (static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0))->Get_pPlayerLookUnit()->Get_Component(L"Com_Inven", ID_DYNAMIC));
	//�κ��� ������ ������ ������ �̵��� ������ϱ� �� ����� ���Ѵ�.
	//���� ������ ���� Ȯ���ϰ� ���� �����̹Ƿ� �Ұ��� �Ǵ��� ���� �ʴ´�.
	pForgeInven->Making_Item_Remove_ingredient(tCheckDeficientItem);


	pForgeInven->Add_Item(_MakingItemID, 1);//������ ����!
	Match_ItemTex_Info();

	////���� �Ϸ��, ��� �Ҹ� & ��� ���� �ȳ� ��Ʈ
	//CGuideFont_UI* pGuideFont_UI = static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0));
	//if (ITEMID_END != tCheckDeficientItem.iIngredientItemID_1)
	//	pGuideFont_UI->Print_LeftGuideFont((ITEMID)tCheckDeficientItem.iIngredientItemID_1, tCheckDeficientItem.iNumber_of_Required_ItemID_1, 1);
	//if (ITEMID_END != tCheckDeficientItem.iIngredientItemID_2)
	//	pGuideFont_UI->Print_LeftGuideFont((ITEMID)tCheckDeficientItem.iIngredientItemID_2, tCheckDeficientItem.iNumber_of_Required_ItemID_2, 1);
	//if (ITEMID_END != tCheckDeficientItem.iIngredientItemID_3)
	//	pGuideFont_UI->Print_LeftGuideFont((ITEMID)tCheckDeficientItem.iIngredientItemID_3, tCheckDeficientItem.iNumber_of_Required_ItemID_3, 1);
	//if (ITEMID_END != tCheckDeficientItem.iIngredientItemID_4)
	//	pGuideFont_UI->Print_LeftGuideFont((ITEMID)tCheckDeficientItem.iIngredientItemID_4, tCheckDeficientItem.iNumber_of_Required_ItemID_4, 1);
	//pGuideFont_UI->Print_LeftGuideFont((ITEMID)tCheckDeficientItem.iMakingItemID, 1, 0);

	return true;
}

void CRight_Inventory::Create_MakingItem_Start_Check(const _double & _dTimeDelta)
{
	if (Engine::KeyDown(KeyCheck_E) || Check_Double_LButton_Click(_dTimeDelta))//����Ŭ�� Ȯ�� �Լ�
	{
		for (auto& iter : m_vecInvenItemSlot) //���� ���� ���鼭
		{
			if (iter->Question_Now_MouseIsUrRange())//���콺�� Ư�� ���� ���� �ִٸ�
			{
				if (ITEMID_END == iter->Get_CanCreateItme_Question())//���� �Ұ��� �ϴٸ�
				{
					return;
				}
				else//���� �����ϴٸ�
				{
					iter->Set_Go_Making();
					//Create_MakingItem(iter->Get_CanCreateItme_Question());
					//������ ������ ���� �ð� ���� �� ���� ���� ���� �Ǵ� �� �ҷ��ٰ���
				}
			}
		}
	}
}

_bool CRight_Inventory::Check_Double_LButton_Click(const _double & _dTimeDelta)
{
	m_fCheck_Double_LButton_Click += (_float)_dTimeDelta;
	if (Engine::KeyDown(KeyCheck_Lbutton) && (m_fCheck_Double_LButton_Click < (-9.6)))
	{
		m_fCheck_Double_LButton_Click = 0.0;
		return true;
	}
	if (Engine::KeyDown(KeyCheck_Lbutton))
	{
		m_fCheck_Double_LButton_Click = -10;
		return false;
	}
	return false;
}

void CRight_Inventory::ChangeInvenTap()
{
	if (m_bInvenTap)
		m_bInvenTap = false;
	else
		m_bInvenTap = true;
	Inventory_Render_On();
	Match_ItemTex_Info();
}

CTexture * CRight_Inventory::Get_pMakingTexture(_tchar * _wstrItemName)
{
	if (nullptr == _wstrItemName)
		return nullptr;

	auto iter = m_mapMakingTexture.find(_wstrItemName);

	if (iter == m_mapMakingTexture.end())
		return Create_MakingTexture(_wstrItemName);

	return iter->second;
}

CTexture * CRight_Inventory::Create_MakingTexture(_tchar * _wstrItemName)
{
	CTexture* pTex = nullptr;
	wstring wstrProtoTexName = m_wstrProtoFrontItemTexName + _wstrItemName;

	pTex = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(wstrProtoTexName));
	m_mapMakingTexture.emplace(_wstrItemName, pTex);
	return pTex;
}
