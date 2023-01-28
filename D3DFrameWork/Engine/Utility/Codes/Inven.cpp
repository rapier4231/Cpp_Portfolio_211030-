#include "..\Headers\Inven.h"
#include "Prototype_Mgr.h"

#define ConsumableAllAmount 100
#define CraftAllAmount 100
#define WeaponAllAmount 1
#define EquipAllAmount 1
#define EtcAllAmount 100
USING(Engine)

CInven::CInven(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
{
}

CInven::CInven(const CInven & _rhs)
	:CComponent(_rhs)
{
}

HRESULT CInven::Ready_Component_Prototype()
{
	CComponent::Ready_Component_Prototype();
	return S_OK;
}

HRESULT CInven::Ready_Component(void * _pArg)
{
	CComponent::Ready_Component(_pArg);
	m_eInvenUserUnitTypeID = *(static_cast<InvenUserUnitTypeID*>(_pArg));
	return S_OK;
}

_int CInven::Update_Component(const _double & _dTimeDelta)
{
	CComponent::Update_Component(_dTimeDelta);
	return _int();
}

//void CInven::Ready_Inven(InvenUserUnitTypeID _eInvenUserUnitTypeID)
//{
//	m_eInvenUserUnitTypeID = _eInvenUserUnitTypeID;
//}

CInven * CInven::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CInven*	pInstance = new CInven(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CInven::Clone(void * pArg)
{
	CInven*	pClone = new CInven(*this);

	if (FAILED(pClone->Ready_Component(pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CInven::Free()
{
	m_vecInvenItem.clear();

	CComponent::Free();
}

void CInven::Add_Item(const ITEM_INFO& _tItemInfo)
{
	_int Amount = _tItemInfo.iAmount;
	_int TypeToAllAmount = _tItemInfo.iMaxAmount;

	for (auto& iter : m_vecInvenItem)
	{
		if (0 == Amount)
			return;

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

	if (Amount)
	{
		if (m_iItemAllSlotNum == (m_vecInvenItem.size()))
		{
			/*vector<ITEM_INFO>::iterator iter = m_vecInvenItem.end();
			--iter;
			m_vecInvenItem.erase(iter);
			Item_Set_Identity_Number();*/
			return; //�ȳ�â ����ְų� �ؾ� ��
		}
		ITEM_INFO tInfo = _tItemInfo;

		while (TypeToAllAmount < Amount)
		{
			Amount -= TypeToAllAmount;
			tInfo = _tItemInfo;
			tInfo.iAmount = TypeToAllAmount;
			m_vecInvenItem.emplace_back(tInfo);
		}
		tInfo = _tItemInfo;
		tInfo.iAmount = Amount;
		m_vecInvenItem.emplace_back(tInfo);
	}
	Item_Set_Identity_Number();
}

void CInven::Add_Item(ITEMID _eItemID, _int _iAmount)
{
	ITEM_INFO _tItemInfo = CPrototype_Mgr::GetInstance()->Clone_Item_Prototype(_eItemID, _iAmount);
	_int Amount = _iAmount;
	_int TypeToAllAmount = _tItemInfo.iMaxAmount;

	for (auto& iter : m_vecInvenItem)
	{
		if (0 == Amount)
			return;

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

	if (Amount)
	{
		if (m_iItemAllSlotNum == (m_vecInvenItem.size()))
		{
			/*vector<ITEM_INFO>::iterator iter = m_vecInvenItem.end();
			--iter;
			m_vecInvenItem.erase(iter);
			Item_Set_Identity_Number();*/
			return; //�ȳ�â ����ְų� �ؾ� ��
		}
		ITEM_INFO tInfo = _tItemInfo;
		
		while (TypeToAllAmount < Amount)
		{
			Amount -= TypeToAllAmount;
			tInfo.iAmount = TypeToAllAmount;
			tInfo.iItemID = _tItemInfo.iItemID;
			m_vecInvenItem.emplace_back(tInfo);
		}
		tInfo.iAmount = Amount;
		tInfo.iItemID = _tItemInfo.iItemID;
		m_vecInvenItem.emplace_back(tInfo);
	}
	Item_Set_Identity_Number();
}

_bool CInven::Remove_Item(_int ItemNum)
{
	vector<ITEM_INFO>::iterator iter = m_vecInvenItem.begin();
	
	for (_int i = 0; i < ItemNum; ++i)
		++iter;

	if (iter->iAmount)
	{
		m_vecInvenItem.erase(iter);
		m_vecInvenItem.shrink_to_fit();
		Item_Set_Identity_Number();
		return true;
	}
	else
		return false;
}

ITEM_INFO CInven::Remove_Item(ITEMID _eItemID, _int _iAmount)
{
	ITEM_INFO _RemoveItemName_Amount = CPrototype_Mgr::GetInstance()->Clone_Item_Prototype(_eItemID, _iAmount);
	_int i = (_int)(m_vecInvenItem.size() - 1);
	_int iCalculate = 0;
	for (; -1 < i; --i)
	{
		if (_RemoveItemName_Amount.iItemID == m_vecInvenItem[i].iItemID)
		{
			if ((_RemoveItemName_Amount.iAmount - iCalculate)< m_vecInvenItem[i].iAmount)
			{
				m_vecInvenItem[i].iAmount -= (_RemoveItemName_Amount.iAmount - iCalculate);
				return _RemoveItemName_Amount; //����͵� ����, �׳� �� ���� ���ְ� ��
			}
			else if ((_RemoveItemName_Amount.iAmount - iCalculate) == m_vecInvenItem[i].iAmount)
			{
				Remove_Item(i);
				return _RemoveItemName_Amount;//���� �� �־����� �� �¾Ƽ� �ϳ� ����� ��
			}
			else
			{
				iCalculate += m_vecInvenItem[i].iAmount; // �켱 �� ����.
				Remove_Item(i); // �� �������� ����.
			}

			//������ ����� ���� ����
		}
	}

	Item_Set_Identity_Number();

	//������� �Դٴ°� ���ų� ���� �ȵǰų� �Ѵ� false ��ȯ.
	_RemoveItemName_Amount.iAmount = iCalculate;
	return _RemoveItemName_Amount; //0 �������� ���°�!
}

_bool CInven::UisngRemove_Item(_int _iInvenNum, _int _iAmount)
{
	vector<ITEM_INFO>::iterator iter = m_vecInvenItem.begin();

	for (_int i = 0; i < _iInvenNum; ++i)
		++iter;

	if (iter->iAmount)
	{
		iter->iAmount -= _iAmount;
		if (!(iter->iAmount))
		{
			Remove_Item(iter->iInvenNum);
			return true;
		}
		else
			return true;
	}
	else
	{
		return false;
	}
	return false;
}

void CInven::ChangeItem(const ITEM_INFO& _tChangeSlot, _int _iSlotNum)
{
	m_vecInvenItem[_iSlotNum] = _tChangeSlot;
}

_int CInven::Now_Have_ItemAmount(ITEMID _eItemID)
{
	_int Amount = 0;
	for (auto& iter : m_vecInvenItem)
	{
		if (_eItemID == iter.iItemID)
		{
			Amount +=iter.iAmount;
		}
	}
	return Amount;
}

void CInven::Making_Item_Remove_ingredient(const MAKING_INFO & _tCheckDeficientItem)
{
	MAKING_INFO tMakingInfo = _tCheckDeficientItem;
	for (auto& iter : m_vecInvenItem)
	{
		//�ʿ��� ����� ID���� ������ ����
		if (tMakingInfo.iIngredientItemID_1 == iter.iItemID)
		{
			//�ʿ��� ��� �� ���ش�
			tMakingInfo.iNumber_of_Required_ItemID_1 -= iter.iAmount;
			iter.iAmount = 0;
			//���� ���� ������ �ִ��� �ʿ��� �纸�� �� ���Ҵٸ�
			if (0 > tMakingInfo.iNumber_of_Required_ItemID_1)
			{
				//���� ������ �����ް�
				iter.iAmount = tMakingInfo.iNumber_of_Required_ItemID_1 * -1;
				//���� �ʿ� ��ᷮ�� 0���� �����.
				tMakingInfo.iNumber_of_Required_ItemID_1 = 0;
			}
			else //���� ������ �ִ� ���� ���ڶ��ٸ� 
			{
				//�������� ������� ���� ���̵� ���� 0���� �ٲ�
				iter.iItemID = NULL;
			}
		}
		if (tMakingInfo.iIngredientItemID_2 == iter.iItemID && tMakingInfo.iIngredientItemID_2 != ITEMID_END)
		{
			//�ʿ��� ��� �� ���ش�
			tMakingInfo.iNumber_of_Required_ItemID_2 -= iter.iAmount;
			iter.iAmount = 0;
			//���� ���� ������ �ִ��� �ʿ��� �纸�� �� ���Ҵٸ�
			if (0 > tMakingInfo.iNumber_of_Required_ItemID_2)
			{
				//���� ������ �����ް�
				iter.iAmount = tMakingInfo.iNumber_of_Required_ItemID_2 * -1;
				//���� �ʿ� ��ᷮ�� 0���� �����.
				tMakingInfo.iNumber_of_Required_ItemID_2 = 0;
			}
			else //���� ������ �ִ� ���� ���ڶ��ٸ� 
			{
				//�������� ������� ���� ���̵� ���� 0���� �ٲ�
				iter.iItemID = NULL;
			}
		}
		if (tMakingInfo.iIngredientItemID_3 == iter.iItemID && tMakingInfo.iIngredientItemID_3 != ITEMID_END)
		{
			//�ʿ��� ��� �� ���ش�
			tMakingInfo.iNumber_of_Required_ItemID_3 -= iter.iAmount;
			iter.iAmount = 0;
			//���� ���� ������ �ִ��� �ʿ��� �纸�� �� ���Ҵٸ�
			if (0 > tMakingInfo.iNumber_of_Required_ItemID_3)
			{
				//���� ������ �����ް�
				iter.iAmount = tMakingInfo.iNumber_of_Required_ItemID_3 * -1;
				//���� �ʿ� ��ᷮ�� 0���� �����.
				tMakingInfo.iNumber_of_Required_ItemID_3 = 0;
			}
			else //���� ������ �ִ� ���� ���ڶ��ٸ� 
			{
				//�������� ������� ���� ���̵� ���� 0���� �ٲ�
				iter.iItemID = NULL;
			}
		}
		if (tMakingInfo.iIngredientItemID_4 == iter.iItemID && tMakingInfo.iIngredientItemID_4 != ITEMID_END)
		{
			//�ʿ��� ��� �� ���ش�
			tMakingInfo.iNumber_of_Required_ItemID_4 -= iter.iAmount;
			iter.iAmount = 0;
			//���� ���� ������ �ִ��� �ʿ��� �纸�� �� ���Ҵٸ�
			if (0 > tMakingInfo.iNumber_of_Required_ItemID_4)
			{
				//���� ������ �����ް�
				iter.iAmount = tMakingInfo.iNumber_of_Required_ItemID_4 * -1;
				//���� �ʿ� ��ᷮ�� 0���� �����.
				tMakingInfo.iNumber_of_Required_ItemID_4 = 0;
			}
			else //���� ������ �ִ� ���� ���ڶ��ٸ� 
			{
				//�������� ������� ���� ���̵� ���� 0���� �ٲ�
				iter.iItemID = NULL;
			}
		}
	}
	//���� �����۵� �������ֱ� ���� �θ���.
	vector<ITEM_INFO>::iterator iter = m_vecInvenItem.begin();
	for (; iter != m_vecInvenItem.end();)
	{
		if (iter->iItemID == NULL)
			iter = m_vecInvenItem.erase(iter);
		else
			++iter;
	}
	m_vecInvenItem.shrink_to_fit();
	Item_Set_Identity_Number();
	return;
}

void CInven::Open_Inventory()
{
	
}

vector<ITEM_INFO>* CInven::Get_pInvenItem()
{
	return &m_vecInvenItem;
}

ITEM_INFO CInven::Get_pInvenItem(_int _iItemNumber)
{
	ITEM_INFO tNull;
	tNull.iAmount = 0;
	tNull.iInvenNum = 0;
	tNull.tchName = L"No";

	if (m_vecInvenItem.size() <= _iItemNumber)
		return tNull;

	if (m_vecInvenItem[_iItemNumber].iAmount)
		return (m_vecInvenItem[_iItemNumber]);
	return tNull;
}

void CInven::Item_Set_Identity_Number()
{
	_int ItemIdentityNumber = 0;
	m_NowItemAmount = (_int)m_vecInvenItem.size();
	for(; ItemIdentityNumber<m_NowItemAmount; ++ItemIdentityNumber)
		m_vecInvenItem[ItemIdentityNumber].iInvenNum = ItemIdentityNumber;
}
