#include "stdafx.h"
#include "..\Headers\QuickSlot_UI.h"

#include "Export_Function.h"
#include "Player.h"
#include "Border.h"
#include "InvenItem.h"
#include "Inven.h"
#include "Left_Inventory.h"
#include "GuideFont_UI.h"

USING(Engine)

CQuickSlot_UI::CQuickSlot_UI(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CQuickSlot_UI::CQuickSlot_UI(const CQuickSlot_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CQuickSlot_UI::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CQuickSlot_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	m_vecQuickItem.resize(10);
	Get_ProtoFrontTexName(static_cast<wstring*>(_pArg)); //아이템 앞 부분 이름 설정

	for (auto& iter : m_vecQuickItem)
	{
		iter.iItemID = ITEMID_END;
	}
	return S_OK;
}

_int CQuickSlot_UI::Update_GameObject(const _double & _dTimeDelta)
{
	CGameObject::Update_GameObject(_dTimeDelta);

	if (!m_pPlayer)
		m_pPlayer=static_cast<CPlayer*>(Get_GameObject(L"Layer_Player", 0));

	if (m_pDrag_DropQuickSlot)
	{
		if (OBJECT_DEAD == m_pDrag_DropQuickSlot->Update_GameObject(_dTimeDelta))
		{
			Match_Tex_Info();
			Safe_Release(m_pDrag_DropQuickSlot);
			Safe_Release(m_pDrag_DropQuickBorder);
			m_pDrag_DropQuickSlot = nullptr;
			m_pDrag_DropQuickBorder = nullptr;
		}
		else
		{
			m_pDrag_DropQuickBorder->Update_GameObject(_dTimeDelta);
		}
	}

	return 0;
}

_int CQuickSlot_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CGameObject::LateUpdate_GameObject(_dTimeDelta);

	if (m_pDrag_DropQuickSlot)
	{
		m_pDrag_DropQuickSlot->LateUpdate_GameObject(_dTimeDelta);
		m_pDrag_DropQuickBorder->LateUpdate_GameObject(_dTimeDelta);
	}

	return 0;
}

void CQuickSlot_UI::Get_ProtoFrontTexName(wstring* _wstrProtoFrontTexName)
{
	m_wstrProtoFrontTexName = *_wstrProtoFrontTexName;
}

CQuickSlot_UI * CQuickSlot_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CQuickSlot_UI*	pInstance = new CQuickSlot_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CQuickSlot_UI::Clone(void * _pArg)
{
	CQuickSlot_UI*	pClone = new CQuickSlot_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CQuickSlot_UI::Free()
{
	map<wstring, CTexture*>::iterator iter = m_mapTexture.begin();
	for (; iter != m_mapTexture.end();)
	{
		if (iter->second)
			Safe_Release(iter->second);
			++iter;
	}

	if (m_pDrag_DropQuickSlot)
		Safe_Release(m_pDrag_DropQuickSlot);

	CGameObject::Free();
}

CTexture * CQuickSlot_UI::Get_pTexture(_tchar* _wstrItemName)
{
	wstring wstrTexName = _wstrItemName;

	auto iter = m_mapTexture.find(wstrTexName);

	if (iter == m_mapTexture.end())
		return Create_Texture(_wstrItemName);

	return iter->second;
}
CTexture * CQuickSlot_UI::Create_Texture(_tchar* _wstrEquipName)
{
	CTexture* pTex = nullptr;
	wstring wstrProtoTexName = m_wstrProtoFrontTexName + _wstrEquipName;

	pTex = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(wstrProtoTexName));
	m_mapTexture.emplace(_wstrEquipName, pTex);
	return pTex;
}

void CQuickSlot_UI::RemoveItem(_int _iItemNum)
{
	ITEM_INFO tNewItem;
	m_vecQuickItem[_iItemNum] = tNewItem;
	m_vecQuickSlot[_iItemNum]->Set_ItemTexture(nullptr);
	m_vecQuickSlot[_iItemNum]->Set_ItemInfo(&m_vecQuickItem[_iItemNum]);
}

_bool CQuickSlot_UI::UisngRemove_Item(_int _iInvenNum, _int _iAmount)
{
	vector<ITEM_INFO>::iterator iter = m_vecQuickItem.begin();

	for (_int i = 0; i < _iInvenNum; ++i)
		++iter;

	if (iter->iAmount)
	{
		iter->iAmount -= _iAmount;
		if (0 == iter->iAmount)
			iter->iItemID = ITEMID_END;
		Match_Tex_Info();
	}
	else
	{
		return false;
	}
	return false;
}

_int CQuickSlot_UI::Get_NowMousePickingSlot()
{
	for (_int i = 0; i < 10; ++i)
		if (m_vecQuickSlot[i]->Question_Now_MouseIsUrRange())
			return i;
	return 11;
}

ITEM_INFO CQuickSlot_UI::Set_Item(ITEMID _eItemID, _int _iAmount)
{
	ITEM_INFO _tQuickItem = Engine::Clone_Item_Prototype(_eItemID, _iAmount);
	//얘는 무조건 스왑 되어야 함 ㅇㅇ
	_int iSlotNum = 0;
	for (auto& iter : m_vecQuickSlot)
	{
		if (iter->Question_Now_MouseIsUrRange())
			break;
		++iSlotNum;
	}

	if (10 <= iSlotNum)
		return ITEM_INFO();
	else
	{
		ITEM_INFO tOutNowItem = m_vecQuickItem[iSlotNum];
		m_vecQuickItem[iSlotNum] = _tQuickItem;
		Match_Tex_Info();
		return tOutNowItem;
	}
}

CInvenItem * CQuickSlot_UI::CreateDragItemSlot(POINT _pPos, _int _iIdentityNum)
{
	m_iDrag_DropSlotNumber = _iIdentityNum;
	CInvenItem* pInvenItem = nullptr;
	pInvenItem = static_cast<CInvenItem*>(Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_InvenItem"));
	pInvenItem->Set_IdentifyNumber(_iIdentityNum);
	pInvenItem->Set_Pos_UI((_float)_pPos.x, (_float)_pPos.y);
	pInvenItem->Set_FirstPos(_pPos);
	pInvenItem->Set_DragItem();
	pInvenItem->Set_SlotNum(_iIdentityNum);
	pInvenItem->Set_pSlotOwner(this, QuickSlot);
	pInvenItem->Set_ItemInfo(&m_vecQuickItem[m_iDrag_DropSlotNumber]);
	pInvenItem->Set_ItemTexture(Get_pTexture(m_vecQuickItem[m_iDrag_DropSlotNumber].tchName));
	pInvenItem->Get_ORTHOUI_INFO()->bRender = true;

	POINT tMouse;
	Engine::Get_pMousePos(&tMouse);
	pInvenItem->Set_MouseClickPos(&tMouse);
	m_pDrag_DropQuickSlot = pInvenItem;

	m_pDrag_DropQuickBorder = static_cast<CBorder*>(Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pInvenItem->Get_ORTHOUI_INFO()));
	m_pDrag_DropQuickBorder->Set_BorderRenderID(RENDER_ITEMUI);

	return nullptr;
}

void CQuickSlot_UI::ChangeItem(const ITEM_INFO& _tChangeSlot, _int _iSlotNum)
{
	m_vecQuickItem[_iSlotNum] = _tChangeSlot;
}

_int CQuickSlot_UI::Now_Have_ItemAmount(ITEMID _eItemID)
{
	_int Amount = 0;
	for (auto& iter : m_vecQuickItem)
	{
		if (_eItemID == iter.iItemID)
		{
			Amount += iter.iAmount;
		}
	}
	return Amount;
}

const ITEM_INFO & CQuickSlot_UI::Get_Item_To_Number(_int _iQuickSlotNumber , _int UsingAmount)
{
	//ITEM_INFO tItemInfo = {};
	//tItemInfo = m_vecQuickItem[_iQuickSlotNumber];
	//switch (tItemInfo.iItemType)
	_int ItemNum = m_vecQuickItem[_iQuickSlotNumber].iItemType;

	if (ITEMID_Consumable == (Engine::ITEMID)m_vecQuickItem[_iQuickSlotNumber].iItemType)
	{
		if (UsingAmount)
			UisngRemove_Item(_iQuickSlotNumber, UsingAmount);
		return m_vecQuickItem[_iQuickSlotNumber];
	}
	else if (ITEMID_Consumable_Meat == (Engine::ITEMID)m_vecQuickItem[_iQuickSlotNumber].iItemType)
	{
		if (UsingAmount)
			UisngRemove_Item(_iQuickSlotNumber, UsingAmount);
		return m_vecQuickItem[_iQuickSlotNumber];
	}
	else if (ITEMID_Craft == (Engine::ITEMID)m_vecQuickItem[_iQuickSlotNumber].iItemType)
	{
		//사용 후 없애야 함. 고민용!
		// //UisngRemove_Item(_iQuickSlotNumber, UsingAmount);
		return m_vecQuickItem[_iQuickSlotNumber];
	}
	else if (ITEMID_Weapon == (Engine::ITEMID)m_vecQuickItem[_iQuickSlotNumber].iItemType)
	{
		return m_vecQuickItem[_iQuickSlotNumber];
	}
	else if (ITEMID_CsWeapon == (Engine::ITEMID)m_vecQuickItem[_iQuickSlotNumber].iItemType)
	{
		if(UsingAmount)
		UisngRemove_Item(_iQuickSlotNumber, UsingAmount);
		return m_vecQuickItem[_iQuickSlotNumber];
	}
	else if (ITEMID_Tool == (Engine::ITEMID)m_vecQuickItem[_iQuickSlotNumber].iItemType)
	{
		return m_vecQuickItem[_iQuickSlotNumber];
	}
	else if (ITEMID_Equip == (Engine::ITEMID)m_vecQuickItem[_iQuickSlotNumber].iItemType)
	{
		return m_vecQuickItem[_iQuickSlotNumber];
	}
	else if (ITEMID_Construct == (Engine::ITEMID)m_vecQuickItem[_iQuickSlotNumber].iItemType)
	{
		return m_vecQuickItem[_iQuickSlotNumber];
	}
	else
		return  m_vecQuickItem[_iQuickSlotNumber];
	//	switch (ItemNum)
	//{
	//case (ITEMID_Consumable):

	//	break;
	//case (ITEMID_Consumable_Meat):

	//	break;
	//case (ITEMID_Craft)://사용 후 없애야 함. 고민용!
	// //UisngRemove_Item(_iQuickSlotNumber, UsingAmount);
	//	break;
	//case (ITEMID_Weapon):
	//	break;
	//case (ITEMID_CsWeapon):
	//	
	//	break;
	//case (ITEMID_Tool):
	//	break;
	//case (ITEMID_Equip):
	//	break;
	//default:
	//	break;
	//}
	//ITEM_INFO tItemInfo2;
	//return tItemInfo2;
}

MAKING_INFO * CQuickSlot_UI::Making_Item_Remove_ingredient(MAKING_INFO * _pCheckDeficientItem)
{
	for (auto& iter : m_vecQuickItem)
	{
		//필요한 재료의 ID값이 같으면 뺀다
		if (_pCheckDeficientItem->iIngredientItemID_1 == iter.iItemID)
		{
			//필요한 재료 다 빼준다
			_pCheckDeficientItem->iNumber_of_Required_ItemID_1 -= iter.iAmount;
			iter.iAmount = 0;
			//만약 내가 가지고 있던게 필요한 양보다 더 많았다면
			if (0 > _pCheckDeficientItem->iNumber_of_Required_ItemID_1)
			{
				//남은 아이템 돌려받고
				iter.iAmount = _pCheckDeficientItem->iNumber_of_Required_ItemID_1 * -1;
				//남은 필요 재료량을 0으로 맞춘다.
				_pCheckDeficientItem->iNumber_of_Required_ItemID_1 = 0;
			}
			else //내가 가지고 있던 양이 모자랐다면 
			{
				//나는 이제 사라진다.
				iter.iItemID = ITEMID_END;
			}
		}
		if (_pCheckDeficientItem->iIngredientItemID_2 == iter.iItemID && _pCheckDeficientItem->iIngredientItemID_2!= ITEMID_END)
		{
			//필요한 재료 다 빼준다
			_pCheckDeficientItem->iNumber_of_Required_ItemID_2 -= iter.iAmount;
			iter.iAmount = 0;
			//만약 내가 가지고 있던게 필요한 양보다 더 많았다면
			if (0 > _pCheckDeficientItem->iNumber_of_Required_ItemID_2)
			{
				//남은 아이템 돌려받고
				iter.iAmount = _pCheckDeficientItem->iNumber_of_Required_ItemID_2 * -1;
				//남은 필요 재료량을 0으로 맞춘다.
				_pCheckDeficientItem->iNumber_of_Required_ItemID_2 = 0;
			}
			else //내가 가지고 있던 양이 모자랐다면 
			{
				//나는 이제 사라진다.
				iter.iItemID = ITEMID_END;
			}
		}
		if (_pCheckDeficientItem->iIngredientItemID_3 == iter.iItemID && _pCheckDeficientItem->iIngredientItemID_3 != ITEMID_END)
		{
			//필요한 재료 다 빼준다
			_pCheckDeficientItem->iNumber_of_Required_ItemID_3 -= iter.iAmount;
			iter.iAmount = 0;
			//만약 내가 가지고 있던게 필요한 양보다 더 많았다면
			if (0 > _pCheckDeficientItem->iNumber_of_Required_ItemID_3)
			{
				//남은 아이템 돌려받고
				iter.iAmount = _pCheckDeficientItem->iNumber_of_Required_ItemID_3 * -1;
				//남은 필요 재료량을 0으로 맞춘다.
				_pCheckDeficientItem->iNumber_of_Required_ItemID_3 = 0;
			}
			else //내가 가지고 있던 양이 모자랐다면 
			{
				//나는 이제 사라진다.
				iter.iItemID = ITEMID_END;
			}
		}
		if (_pCheckDeficientItem->iIngredientItemID_4 == iter.iItemID && _pCheckDeficientItem->iIngredientItemID_4 != ITEMID_END)
		{
			//필요한 재료 다 빼준다
			_pCheckDeficientItem->iNumber_of_Required_ItemID_4 -= iter.iAmount;
			iter.iAmount = 0;
			//만약 내가 가지고 있던게 필요한 양보다 더 많았다면
			if (0 > _pCheckDeficientItem->iNumber_of_Required_ItemID_4)
			{
				//남은 아이템 돌려받고
				iter.iAmount = _pCheckDeficientItem->iNumber_of_Required_ItemID_4 * -1;
				//남은 필요 재료량을 0으로 맞춘다.
				_pCheckDeficientItem->iNumber_of_Required_ItemID_4 = 0;
			}
			else //내가 가지고 있던 양이 모자랐다면 
			{
				//나는 이제 사라진다.
				iter.iItemID = ITEMID_END;
			}
		}
	}
	//빠진 아이템들 정리해주기 위해 부른다.
	Match_Tex_Info();
	return _pCheckDeficientItem;
}

void CQuickSlot_UI::Add_Item(ITEMID _eItemID, _int _iAmount)
{
	ITEM_INFO _tItemInfo = Engine::Clone_Item_Prototype(_eItemID, _iAmount);
	_int Amount = _iAmount;
	_int TypeToAllAmount = Get_ItemTypeAllAmount(_eItemID);

	for (auto& iter : m_vecQuickItem)
	{
		if (0 == Amount)
		{
			Match_Tex_Info();
			//////////////////////////////GuideFont UI에 띄우라고 정보 보내주는 곳
			static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_LeftGuideFont(_eItemID, _iAmount, 0);
			return;
		}

		if (_tItemInfo.iItemID == iter.iItemID)
		{
			iter.iAmount += Amount;
			Amount = 0;
			if (TypeToAllAmount < iter.iAmount)
			{
				Amount = iter.iAmount - TypeToAllAmount;
				iter.iAmount = TypeToAllAmount;
				continue;
			}
		}
	}

	static_cast<CInven*>(m_pPlayer->Get_Component(L"Com_Inven", ID_DYNAMIC))->Add_Item(_eItemID, Amount);
	static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Match_ItemTex_Info();
	Match_Tex_Info();

	//////////////////////////////GuideFont UI에 띄우라고 정보 보내주는 곳
	static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_LeftGuideFont(_eItemID, _iAmount,0);
}

_int CQuickSlot_UI::Get_ItemTypeAllAmount(ITEMID _eItemID)
{
	ITEM_INFO t = Engine::Clone_Item_Prototype(_eItemID);

	return t.iMaxAmount;
}

void CQuickSlot_UI::Set_EmplaceBack_Slot(CInvenItem * _EquipSlot)
{
	m_vecQuickSlot.emplace_back(_EquipSlot);
}

void CQuickSlot_UI::Match_Tex_Info()
{
	for (_int i = 0; i < 10; ++i)//퀵슬롯이니 10개 고정
	{
		if (ITEMID::ITEMID_END != m_vecQuickItem[i].iItemID) // 없으면 아이디 이거 end로 바꿔버리자! 0이여도 퀵슬롯 템은 살아있을 수 있으니
		{
			m_vecQuickSlot[i]->Set_ItemInfo(&(m_vecQuickItem[i]));
			m_vecQuickSlot[i]->Set_ItemTexture(Get_pTexture(m_vecQuickItem[i].tchName));
		}
		else
		{
			m_vecQuickSlot[i]->Set_ItemInfo(nullptr);
			m_vecQuickSlot[i]->Set_ItemTexture(nullptr);
		}
		m_vecQuickSlot[i]->Set_SlotNum(i);
	}
}