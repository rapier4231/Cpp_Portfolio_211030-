#include "stdafx.h"
#include "..\Headers\MakingItem.h"

#include "Export_Function.h"
#include "Left_Inventory.h"
#include "Right_Inventory.h"
#include "QuickSlot_UI.h"
#include "Unit.h"
#include "Stat_UI.h"
#include "Inven.h"
#include "Player.h"

#define ItemSlotEDGESIZELEFT 0.1f
#define ItemSlotEDGESIZERIGHT 0.9f
#define ItemSlotEDGESIZETOP 0.1f
#define ItemSlotEDGESIZEBOTTOM 0.9f
#define ItemSlotDragEDGEALPHA	0.2f
#define ItemSlotNomalEDGEALPHA	0.4f
#define ItemSlotMouseEDGEALPHA	0.6f
#define ItemSlotSelecEDGEALPHA	0.8f

USING(Engine)

CMakingItem::CMakingItem(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CMakingItem::CMakingItem(const CMakingItem & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CMakingItem::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CMakingItem::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fSizeX = MakingItemSizeX;
	m_tORTHOUI_INFO.fSizeY = MakingItemSizeY;
	m_tORTHOUI_INFO.fAlpha = FHALF;

	m_tORTHOUI_INFO.v4EdgeSize = { ItemSlotEDGESIZELEFT ,ItemSlotEDGESIZETOP  ,ItemSlotEDGESIZERIGHT, ItemSlotEDGESIZEBOTTOM };
	m_tORTHOUI_INFO.fEdgeAlpha = ItemSlotNomalEDGEALPHA;
	m_tORTHOUI_INFO.bUpdate = true;

	return S_OK;
}

_int CMakingItem::Update_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::Update_GameObject(_dTimeDelta);

	if (m_bDead)
		return OBJECT_DEAD;

	MouseCheck();
	if (m_bDragItem)
	{
		m_tORTHOUI_INFO.fPosX = (_float)(m_tFirstPos.x + ( m_tORTHOUI_INFO.tMousePos.x - m_MouseClickPos.x));
		m_tORTHOUI_INFO.fPosY = (_float)(m_tFirstPos.y + ( m_tORTHOUI_INFO.tMousePos.y - m_MouseClickPos.y));
		if (Engine::KeyUp(KeyCheck_Lbutton))
		{
			DeleteDropItemSlot();
			//어디다 놨는지에 따라 어케든 해줄 함수 부르고 나는 사라진다.
			m_bDead = true;
		}
	}
	else
	{
		MouseCheck();
		ItemSlotSelecFunc();
	}

	return S_OK;
}

_int CMakingItem::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	if (m_tORTHOUI_INFO.bRender)
	{
		if (m_bDragItem)
		{
			Engine::Add_RenderGroup(RENDER_LASTUI, this);

		}
		else
			Engine::Add_RenderGroup(RENDER_ITEMUI, this);
	}

	return _int();
}

void CMakingItem::Render_GameObject(_mat _matOrtho)
{
	if (!m_tORTHOUI_INFO.bRender)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = NULL;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(NULL);

	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
	Render_Font(_matOrtho);
}

void CMakingItem::Render_Font(_mat _matOrtho)
{
	if (ITEMID_END != m_ItemInfo.iItemID)
	{
		wstring wstrNum = to_wstring(m_ItemInfo.iAmount);
		if(0<=m_ItemInfo.iAmount && 10 > m_ItemInfo.iAmount)
			Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + 6.f , m_tORTHOUI_INFO.fPosY + 7.f), D3DXCOLOR(0.8f, 0.8f, 1.f, 1.f));
		else if(10 <= m_ItemInfo.iAmount && 100 > m_ItemInfo.iAmount)
			Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + 3.f, m_tORTHOUI_INFO.fPosY + 7.f), D3DXCOLOR(0.8f, 0.8f, 1.f, 1.f));
		else if (100 <= m_ItemInfo.iAmount)
			Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX , m_tORTHOUI_INFO.fPosY + 7.f), D3DXCOLOR(0.8f, 0.8f, 1.f, 1.f));

	}
}
CMakingItem * CMakingItem::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CMakingItem*	pInstance = new CMakingItem(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CMakingItem::Clone(void * _pArg/* = nullptr*/)
{
	CMakingItem*	pClone = new CMakingItem(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CMakingItem::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_ItemSlot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);
	return S_OK;
}

HRESULT CMakingItem::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho)
{
	if (m_pTextureCom)
	{
		m_pTextureCom->Set_Texture(_pEffect, "g_ItemTexture");
		_pEffect->SetBool("g_bNoTex", false);
	}
	else
		_pEffect->SetBool("g_bNoTex", true);

	if (!m_ItemInfo.iAmount)
		_pEffect->SetBool("g_bDrag", true); //드래그 당했을 때 아이템 알파 조정
	else
		_pEffect->SetBool("g_bDrag", false);

	if(m_bDragItem)
		_pEffect->SetBool("g_bDrop", true);
	else
		_pEffect->SetBool("g_bDrop", false);

	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = m_tORTHOUI_INFO.fSizeX;
	matWorld._22 = m_tORTHOUI_INFO.fSizeY;
	matWorld._33 = FTRUE;

	matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
	matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
	matWorld._43 = FTRUE;

	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);

	_pEffect->SetFloat("g_Alpha", m_tORTHOUI_INFO.fAlpha);

	return S_OK;
}

void CMakingItem::ItemSlotSelecFunc()
{
	if (m_bBoxSelect)
	{
		if (m_ItemInfo.iAmount)
		{
			CreateDragItemSlot();
			m_ItemInfo.iAmount = 0;
			m_tORTHOUI_INFO.fAlpha = ItemSlotDragEDGEALPHA;
			m_tORTHOUI_INFO.fEdgeAlpha = ItemSlotDragEDGEALPHA;
		}
		else
		{
			if(Engine::KeyUp(KeyCheck_Lbutton) || !(MouseOnUI & m_tORTHOUI_INFO.uchSelectUI))
			{
				m_tORTHOUI_INFO.fAlpha = ItemSlotNomalEDGEALPHA;
				m_tORTHOUI_INFO.fEdgeAlpha = ItemSlotNomalEDGEALPHA;
				m_bBoxSelect = false;
			}
		}
	}
	else
	{
		if (m_ItemInfo.iAmount) // 드래그앤드랍으로 뺏겼을 때 안 탈려고 알파값 안 바뀌게
		{
			if (!(MouseOnUI & m_tORTHOUI_INFO.uchSelectUI))
			{
				m_bBoxSelect = false;
				m_tORTHOUI_INFO.fAlpha = ItemSlotNomalEDGEALPHA;
				m_tORTHOUI_INFO.fEdgeAlpha = ItemSlotNomalEDGEALPHA;
			}
			if ((MouseOnUI & m_tORTHOUI_INFO.uchSelectUI))
			{
				m_bBoxSelect = false;
				m_tORTHOUI_INFO.fAlpha = ItemSlotMouseEDGEALPHA;
				m_tORTHOUI_INFO.fEdgeAlpha = ItemSlotMouseEDGEALPHA;
			}
			if ((MouseClickUI & m_tORTHOUI_INFO.uchSelectUI))
			{
				m_bBoxSelect = true;
				m_tORTHOUI_INFO.fAlpha = ItemSlotSelecEDGEALPHA;
				m_tORTHOUI_INFO.fEdgeAlpha = ItemSlotSelecEDGEALPHA;
			}
		}
	}
}

void CMakingItem::CreateDragItemSlot()
{
	switch (m_eSlotOwner)
	{
	case ESlotOwner::Left_Inven:
		static_cast<CLR_Inventory*>(m_pSlotOwner)->CreateDragItemSlot(m_tFirstPos, m_iSlotNum);
		break;
	case ESlotOwner::Right_Inven:
		static_cast<CLR_Inventory*>(m_pSlotOwner)->CreateDragItemSlot(m_tFirstPos, m_iSlotNum);
		break;
	case ESlotOwner::Stat_Player:
		static_cast<CStat_UI*>(m_pSlotOwner)->CreateDragEquipSlot(m_tFirstPos, m_iSlotNum);
		break;
	case ESlotOwner::Stat_Monster:
		break;
	case ESlotOwner::QuickSlot:
		static_cast<CQuickSlot_UI*>(m_pSlotOwner)->CreateDragItemSlot(m_tFirstPos, m_iSlotNum);
		break;
	case ESlotOwner::SlotOwner_END:
		break;
	}
}
void CMakingItem::DeleteDropItemSlot()
{
	Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
	//내가 드랍된 위치 알려줘
	CInven* pRemoveInven = nullptr;
	CUnit* pOwnerUnit = nullptr;
	//날 없앨 인벤이 어디야?
	switch (m_eSlotOwner)
	{
	case ESlotOwner::Left_Inven:
		pOwnerUnit = static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Get_InvenOwner();
		pRemoveInven = static_cast<CInven*>(pOwnerUnit->Get_Component(L"Com_Inven", ID_DYNAMIC));
		break;
	case ESlotOwner::Right_Inven:
		pOwnerUnit = static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Get_InvenOwner();
		pRemoveInven = static_cast<CInven*>(pOwnerUnit->Get_Component(L"Com_Inven", ID_DYNAMIC));
		break;
	}

	if (ESlotOwner::Right_Inven != m_eSlotOwner)
	{
		//나는 오른쪽 인벤이 아닌데 오른쪽 인벤에 떨궈졌네?
		if (m_tORTHOUI_INFO.tMousePos.x >= 517
			&& m_tORTHOUI_INFO.tMousePos.x <= 717
			&& m_tORTHOUI_INFO.tMousePos.y >= 137
			&& m_tORTHOUI_INFO.tMousePos.y <= 493)
		{
			//오른쪽 인벤에 추가해
			static_cast<CInven*>(static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Get_InvenOwner()->Get_Component(L"Com_Inven", ID_DYNAMIC))->Add_Item(m_ItemInfo);
			//내가 있던 곳에선 빼
			Remove_DropItem_To_Inven(pRemoveInven);
		}
	}

	if (ESlotOwner::Left_Inven != m_eSlotOwner)
	{
		//나는 왼쪽 인벤이 아닌데 왼쪽 인벤에 떨궈졌네?
		if (m_tORTHOUI_INFO.tMousePos.x >= 77
			&& m_tORTHOUI_INFO.tMousePos.x <= 277
			&& m_tORTHOUI_INFO.tMousePos.y >= 137
			&& m_tORTHOUI_INFO.tMousePos.y <= 493)
		{
			//왼쪽 인벤에 추가해
			static_cast<CInven*>(static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Get_InvenOwner()->Get_Component(L"Com_Inven", ID_DYNAMIC))->Add_Item(m_ItemInfo);
			//내가 있던 곳에선 빼
			Remove_DropItem_To_Inven(pRemoveInven);
		}
	}

	if (ESlotOwner::Stat_Player != m_eSlotOwner)
	{
		if (3 == (m_ItemInfo.iItemID/1000))
			//나는 장비고. Center 플레이어 장비 인벤이 아닌데 플레이어 장비에 장착됬네?
		{
			CStat_UI* pCenter_Stat_UI = static_cast<CStat_UI*>(Engine::Get_GameObject(L"Center_UI", 0));
			_int EquipNum = pCenter_Stat_UI->Get_NowMousePickingSlot();
			if (EQUIP_Position::EQUIP_Position_END != EquipNum) //장비창에 마우스가 떨어졌다면?
			{
				if (EquipNum ==m_ItemInfo.iPosition) //맞는 부위에 장착 했다면?
				{
					pCenter_Stat_UI->Set_Equip((ITEMID)m_ItemInfo.iItemID, pOwnerUnit);
					Remove_DropItem_To_Inven(pRemoveInven);
				}
			}
		}
	}

	/*if (ESlotOwner::QuickSlot != m_eSlotOwner) // 퀵슬롯은 퀵슬롯 끼리도 바뀌어야 하기 때문에 이거 뺌 ㅇㅇ
	{*/
		//나는 퀵슬롯이 아닌데 퀵슬롯에 떨어졌네?
		CQuickSlot_UI* pQuickSlot_UI = static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0));
		_int EquipNum = pQuickSlot_UI->Get_NowMousePickingSlot(); // 퀵슬롯 몇번에 떨어졌는지..?
		if (11 != EquipNum) //퀵슬롯에 마우스가 떨어졌다면?
		{
			ITEM_INFO tChangeItem;
			tChangeItem = pQuickSlot_UI->Set_Item((ITEMID)m_ItemInfo.iItemID, m_ItemInfo.iAmount);
			if (tChangeItem.iAmount)
			{
				if (ESlotOwner::QuickSlot == m_eSlotOwner)
				{
					pQuickSlot_UI->ChangeItem(tChangeItem, m_iSlotNum);
				}
				else
				{
					static_cast<CInven*>(static_cast<CPlayer*>(Get_GameObject(L"Layer_Player", 0))->Get_Component(L"Com_Inven", ID_DYNAMIC))->ChangeItem(tChangeItem, m_iSlotNum);
				}
			}
			else
				Remove_DropItem_To_Inven(pRemoveInven);
		}
	/*}*/

	//-> + 템 합치기 ( 수 + 짜투리 남음)
	//두번째는 버리기
	//장비창
	//같은 인벤 내 스왑은 ㄴㄴ

	static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Match_ItemTex_Info();
	static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Match_ItemTex_Info();
	static_cast<CCenter_UI*>(Engine::Get_GameObject(L"Center_UI", 0))->Match_EquipTex_Info();
	static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Match_Tex_Info();
}
void CMakingItem::Remove_DropItem_To_Inven(CInven * _pInven)
{
	switch (m_eSlotOwner)
	{
	case Left_Inven:
		_pInven->Remove_Item(m_ItemInfo.iInvenNum);
		break;
	case Right_Inven:
		_pInven->Remove_Item(m_ItemInfo.iInvenNum);
		break;
	case Stat_Player:
		static_cast<CStat_UI*>(Engine::Get_GameObject(L"Center_UI", 0))->RemoveEquip(m_ItemInfo.iPosition);
		break;
	case Stat_Monster:
		static_cast<CStat_UI*>(Engine::Get_GameObject(L"Center_UI", 0))->RemoveEquip(m_ItemInfo.iPosition);
		//이건 나중에 바꿔야댐~!
		break;
	case QuickSlot:
		static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->RemoveItem(m_iSlotNum);
		break;
	case SlotOwner_END:
		break;
	default:
		break;
	}
}
void CMakingItem::Set_MouseClickPos(POINT* _tNowMouse)
{
	m_MouseClickPos.x = _tNowMouse->x;
	m_MouseClickPos.y = _tNowMouse->y;
}
void CMakingItem::Set_FirstPos(POINT _tPos)
{
	m_tFirstPos = _tPos;
}
void CMakingItem::Free()
{
	m_pTextureCom = nullptr; //에드 레퍼런스 안해줘서 그냥 초기화만
	COrthoUI::Free();
}

void CMakingItem::Set_ItemTexture(CTexture * _pItemTexture)
{
	m_pTextureCom = _pItemTexture;
}

void CMakingItem::Set_ItemInfo(const ITEM_INFO * _pItemInfo, ITEMID _eItemID)
{
	if (nullptr == _pItemInfo)
	{
		m_ItemInfo.iAmount = 0;
		m_ItemInfo.tchName = nullptr;
		m_ItemInfo.iItemID = _eItemID;
	}
	else
	{
		m_ItemInfo = *_pItemInfo;
	}
}

void CMakingItem::Set_pSlotOwner(CGameObject* _pSlotOwner, ESlotOwner _eSlotOwner)
{
	m_pSlotOwner = _pSlotOwner;
	m_eSlotOwner = _eSlotOwner;
}