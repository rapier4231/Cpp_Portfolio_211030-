#include "stdafx.h"
#include "..\Headers\InvenItem.h"

#include "Export_Function.h"
#include "Left_Inventory.h"
#include "Right_Inventory.h"
#include "QuickSlot_UI.h"
#include "Unit.h"
#include "Stat_UI.h"
#include "Inven.h"
#include "Player.h"
#include "Item_Info_View_UI.h"
#include "Inventory.h"
#include "Back_Stat_UI.h"

#define ItemSlotEDGESIZELEFT 0.1f
#define ItemSlotEDGESIZERIGHT 0.9f
#define ItemSlotEDGESIZETOP 0.1f
#define ItemSlotEDGESIZEBOTTOM 0.9f
#define ItemSlotDragEDGEALPHA	0.2f
#define ItemSlotNomalEDGEALPHA	0.3f
#define ItemSlotMouseEDGEALPHA	0.4f
#define ItemSlotSelecEDGEALPHA	0.6f

#define ItemFontColor D3DXCOLOR(0.8f, 0.8f, 1.f, 1.f)
#define ItemInfoViewOpenTime 0.3f

#define Scroll_FontCut_UpRatio 0.8f
#define Scroll_FontCut_DownRatio 0.1f
USING(Engine)

HRESULT CInvenItem::Set_Screen_UI()
{
	CInventory* pInvenBackGround = nullptr;
	CBack_Stat_UI* pBackGround = nullptr;
	_float fInvenItemPosX, fInvenItemPosY, fFirstPosX, fFirstPosY;
	_int X, Y;

	m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];

	switch (m_eSlotOwner)
	{
	case Left_Inven:
		pInvenBackGround = static_cast<CInventory*>(Engine::Get_GameObject(L"Left_Inventory", 0));
		fInvenItemPosX = pInvenBackGround->Get_ORTHOUI_INFO()->fPosX;
		fInvenItemPosY = pInvenBackGround->Get_ORTHOUI_INFO()->fPosY;
		fInvenItemPosX -= stURI.fInventory_FirstFixPosX[EScrnResolution];
		fInvenItemPosY -= stURI.fInventory_FirstFixPosY[EScrnResolution];

		fFirstPosX = fInvenItemPosX += stURI.fSizeXY[EScrnResolution] * FHALF;
		fFirstPosY = fInvenItemPosY += stURI.fSizeXY[EScrnResolution] * FHALF;

		Y = (m_iFirstSlotNum - 1) / InvenItemSlotHorizonAmount + 1;
		X = (m_iFirstSlotNum - 1) % InvenItemSlotHorizonAmount;

		fInvenItemPosY += (stURI.fSizeXY[EScrnResolution] + stURI.fInven_Interval_FixPosXY[EScrnResolution])*Y;
		fInvenItemPosX += (stURI.fSizeXY[EScrnResolution] + stURI.fInven_Interval_FixPosXY[EScrnResolution])*X;
		m_tORTHOUI_INFO.fPosX = fInvenItemPosX;
		m_tORTHOUI_INFO.fPosY = fInvenItemPosY;
		Set_FirstPos({ (LONG)fInvenItemPosX, (LONG)fInvenItemPosY });
		break;
	case Right_Inven:
		pInvenBackGround = static_cast<CInventory*>(Engine::Get_GameObject(L"Right_Inventory", 0));
		fInvenItemPosX = pInvenBackGround->Get_ORTHOUI_INFO()->fPosX;
		fInvenItemPosY = pInvenBackGround->Get_ORTHOUI_INFO()->fPosY;
		fInvenItemPosX -= stURI.fInventory_FirstFixPosX[EScrnResolution];
		fInvenItemPosY -= stURI.fInventory_FirstFixPosY[EScrnResolution];

		fFirstPosX = fInvenItemPosX += stURI.fSizeXY[EScrnResolution] * FHALF;
		fFirstPosY = fInvenItemPosY += stURI.fSizeXY[EScrnResolution] * FHALF;

		Y = (m_iFirstSlotNum - 1) / InvenItemSlotHorizonAmount + 1;
		X = (m_iFirstSlotNum - 1) % InvenItemSlotHorizonAmount;

		fInvenItemPosY += (stURI.fSizeXY[EScrnResolution] + stURI.fInven_Interval_FixPosXY[EScrnResolution])*Y;
		fInvenItemPosX += (stURI.fSizeXY[EScrnResolution] + stURI.fInven_Interval_FixPosXY[EScrnResolution])*X;
		m_tORTHOUI_INFO.fPosX = fInvenItemPosX;
		m_tORTHOUI_INFO.fPosY = fInvenItemPosY;
		Set_FirstPos({ (LONG)fInvenItemPosX, (LONG)fInvenItemPosY });
		break;
	case Stat_Player:
		 pBackGround = static_cast<CBack_Stat_UI*>(Engine::Get_GameObject(L"Stat_UI", 0));
		fInvenItemPosX = pBackGround->Get_ORTHOUI_INFO()->fPosX;
		fInvenItemPosY = pBackGround->Get_ORTHOUI_INFO()->fPosY;
		fInvenItemPosX -= stURI.fStat_Player_FirstFixPosX[EScrnResolution]; //조정해야 함
		fInvenItemPosY -= stURI.fStat_Player_FirstFixPosY[EScrnResolution]; //조정해야 함
		fFirstPosX = fInvenItemPosX += stURI.fSizeXY[EScrnResolution] * FHALF;
		fFirstPosY = fInvenItemPosY += stURI.fSizeXY[EScrnResolution] * FHALF;
	
		if((PlayerEquipYAmount-1) < m_iFirstSlotNum) //오른쪽이면 - 0부터니까 1 빼야 함
			fInvenItemPosX = (pBackGround->Get_ORTHOUI_INFO()->fPosX) + stURI.fStat_Interval_FixPosX[EScrnResolution] - (stURI.fSizeXY[EScrnResolution] * FHALF);//조정필
		
		Y = m_iFirstSlotNum % PlayerEquipYAmount;
		fInvenItemPosY += (stURI.fSizeXY[EScrnResolution] + stURI.fStat_Interval_FixPosY[EScrnResolution])*Y;
		m_tORTHOUI_INFO.fPosX = fInvenItemPosX;
		m_tORTHOUI_INFO.fPosY = fInvenItemPosY;
		Set_FirstPos({ (LONG)fInvenItemPosX, (LONG)fInvenItemPosY });
		break;
	case Stat_Monster:
		break;
	case QuickSlot:
		fInvenItemPosX = stURI.fQuickSlot_FirstPosX[EScrnResolution];
		fInvenItemPosY = stURI.fQuickSlot_FirstPosY[EScrnResolution];
		fInvenItemPosX += (stURI.fQuickSizeXY[EScrnResolution] + stURI.fQuickSlot_Interval_FixPosX[EScrnResolution])*m_iFirstSlotNum;
		
		m_tORTHOUI_INFO.fSizeX = stURI.fQuickSizeXY[EScrnResolution];
		m_tORTHOUI_INFO.fSizeY = stURI.fQuickSizeXY[EScrnResolution];
		m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
		break;
	case SlotOwner_END:
		break;
	default:
		break;
	}

	//위치는 아이템 슬롯 각각의 소유 객체들이 한번 쭉 돌려줘야 할 듯.

	return S_OK;
}

CInvenItem::CInvenItem(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CInvenItem::CInvenItem(const CInvenItem & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CInvenItem::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CInvenItem::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];

	m_tORTHOUI_INFO.fAlpha = ItemSlotNomalEDGEALPHA;
	m_tORTHOUI_INFO.fEdgeAlpha = ItemSlotNomalEDGEALPHA;
	m_tORTHOUI_INFO.bUpdate = true;

	return S_OK;
}

_int CInvenItem::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_bDead)
		return OBJECT_DEAD;

	if (m_bScroll_No_Render)
		return S_OK;

	COrthoUI::Update_GameObject(_dTimeDelta);

	if (!Scroll_Hide_OnMouse_Func_Lock())
	{
		MouseCheck();
		Create_Item_Info_View(_dTimeDelta);
	}

	if (m_bNow_Making)//레프트 인벤에서 제작하라는 명령이 떨어지면! 
		Making_Item_Loading_And_Create(_dTimeDelta); //로딩 하고(제작시간) 다 되면 재료 판단 후 제작 ㄱ!

	if (m_pEngram_BackGroundCom) // Making 이면
	{
		Decide_MakingColor();
	}
	else // 그냥 아이템 보여주는 슬롯일 때면
	{
		if (m_bDragItem)
		{
			m_tORTHOUI_INFO.fPosX = (_float)(m_tFirstPos.x + (m_tORTHOUI_INFO.tMousePos.x - m_MouseClickPos.x));
			m_tORTHOUI_INFO.fPosY = (_float)(m_tFirstPos.y + (m_tORTHOUI_INFO.tMousePos.y - m_MouseClickPos.y));
			if (Engine::KeyUp(KeyCheck_Lbutton))
			{
				DeleteDropItemSlot();
				//어디다 놨는지에 따라 어케든 해줄 함수 부르고 나는 사라진다.
				m_bDead = true;
			}
		}
		else
		{
			if (!Scroll_Hide_OnMouse_Func_Lock())
			{
				MouseCheck();
				ItemSlotSelecFunc();
			}

			if (Engine::KeyUp(KeyCheck_Lbutton))
				m_bCreateDragSlot = false;

		}
	}

	return S_OK;
}

_int CInvenItem::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (m_bScroll_No_Render)
		return S_OK;

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

void CInvenItem::Render_GameObject(_mat _matOrtho)
{
	if (!m_tORTHOUI_INFO.bRender)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 2;
	pEffect->Begin(&iPassMax, NULL);
		pEffect->CommitChanges();

	if (m_pEngram_BackGroundCom)
		pEffect->BeginPass(1);
	else
		pEffect->BeginPass(NULL);

	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
	Render_Font(_matOrtho);
}

void CInvenItem::Render_Font(_mat _matOrtho)
{
	if (m_pEngram_BackGroundCom)
	{/////////////////////////이건 뭐지
	//	if (!m_pTextureCom)
	//		m_ItemInfo.iItemID = ITEMID_END;
	//	if (ITEMID_END != m_ItemInfo.iItemID)
	//	{
	//		wstring wstrNum = L"테ST";
	//		if (0 <= m_ItemInfo.iAmount && 10 > m_ItemInfo.iAmount)
	//			Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + 6.f, m_tORTHOUI_INFO.fPosY + 7.f), D3DXCOLOR(0.8f, 0.8f, 1.f, 1.f));
	//	}
	//	///////////////////////으에에ㅔㅔ
	}
	else if (Stat_Monster == m_eSlotOwner)
	{
		wstring wstrNum = L"";
		switch (m_iSlotNum)
		{
		case 0:
			wstrNum = L"안장";
			break;
		}
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX - stURI.fFont_Equip_Type_PosXY[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_Equip_Type_PosXY[EScrnResolution]), ItemFontColor);
	}
	else if (Stat_Player == m_eSlotOwner)
	{
		wstring wstrNum = L"";
		switch (m_iSlotNum)
		{
		case 0:
			wstrNum = L"모자";
			break;
		case 1:
			wstrNum = L"상의";
			break;
		case 2:
			wstrNum = L"하의";
			break;
		case 3:
			wstrNum = L"장갑";
			break;
		case 4:
			wstrNum = L"추가 장착";
			break;
		case 5:
			wstrNum = L"신발";
			break;
		default:
			break;
		}
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX - stURI.fFont_Equip_Type_PosXY[EScrnResolution], m_tORTHOUI_INFO.fPosY - stURI.fFont_Equip_Type_PosXY[EScrnResolution]), ItemFontColor);
	}
	else
	{
		if (!m_pTextureCom)
			m_ItemInfo.iItemID = ITEMID_END;
		if (ITEMID_END != m_ItemInfo.iItemID)
		{
			wstring wstrNum = to_wstring(m_ItemInfo.iAmount);
			if (QuickSlot != m_eSlotOwner)
			{		//스크롤 안보일 때 안나오게 하기 위해서
				if (!(((EScroll_Cut::EScroll_Cut_Up == m_iDrawCutUp) && (m_fDrawCutRatio > Scroll_FontCut_UpRatio))||((EScroll_Cut::EScroll_Cut_Down == m_iDrawCutUp)&& (m_fDrawCutRatio > Scroll_FontCut_DownRatio))))
				{
					if (0 <= m_ItemInfo.iAmount && 10 > m_ItemInfo.iAmount)
						Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Item_Amount1_PosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_Item_Amount_PosY[EScrnResolution]), ItemFontColor);
					else if (10 <= m_ItemInfo.iAmount && 100 > m_ItemInfo.iAmount)
						Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Item_Amount10_PosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_Item_Amount_PosY[EScrnResolution]), ItemFontColor);
					else if (100 <= m_ItemInfo.iAmount)
						Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Item_Amount100_PosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_Item_Amount_PosY[EScrnResolution]), ItemFontColor);
				}
			}
			else
			{
				if (0 <= m_ItemInfo.iAmount && 10 > m_ItemInfo.iAmount)
					Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Quick_Item_Amount1_PosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_Quick_Item_Amount_PosY[EScrnResolution]), ItemFontColor);
				else if (10 <= m_ItemInfo.iAmount && 100 > m_ItemInfo.iAmount)
					Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Quick_Item_Amount10_PosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_Quick_Item_Amount_PosY[EScrnResolution]), ItemFontColor);
				else if (100 <= m_ItemInfo.iAmount)
					Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Quick_Item_Amount100_PosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_Quick_Item_Amount_PosY[EScrnResolution]), ItemFontColor);
			}
		}
	}
}

CInvenItem * CInvenItem::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CInvenItem*	pInstance = new CInvenItem(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CInvenItem::Clone(void * _pArg/* = nullptr*/)
{
	CInvenItem*	pClone = new CInvenItem(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CInvenItem::Add_Component(void)
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

HRESULT CInvenItem::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho)
{
	if (m_pTextureCom)
	{
		m_pTextureCom->Set_Texture(_pEffect, "g_ItemTexture");
		_pEffect->SetBool("g_bNoTex", false);
	}
	else
		_pEffect->SetBool("g_bNoTex", true);

	if (m_pEngram_BackGroundCom)
	{
		m_pEngram_BackGroundCom->Set_Texture(_pEffect,"g_Engram_BackGround" );
		m_pCraftingBrokenCom->Set_Texture(_pEffect, "g_CraftingBroken");
		_pEffect->SetBool("g_bPossibleMaking", m_bPossibleMaking);
		_pEffect->SetBool("g_bPossibleMakingLv", m_bPossibleMakingLv);
	}

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
	matWorld._42 = -(m_tORTHOUI_INFO.fPosY) + WINCY * FHALF;
	matWorld._43 = FTRUE;

	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);

	_pEffect->SetFloat("g_Alpha", m_tORTHOUI_INFO.fAlpha);

	_pEffect->SetFloat("g_fDrawCutRatio", m_fDrawCutRatio);
	_pEffect->SetInt("g_iDrawCutUp", m_iDrawCutUp);

	return S_OK;
}

void CInvenItem::ItemSlotSelecFunc()
{
	if (Engine::KeyUp(KeyCheck_Lbutton))
	{
		m_tORTHOUI_INFO.fAlpha = ItemSlotNomalEDGEALPHA;
		m_tORTHOUI_INFO.fEdgeAlpha = ItemSlotNomalEDGEALPHA;
	}

	if (m_bBoxSelect)
	{
		if (m_ItemInfo.iAmount)
		{
			if (!m_bCreateDragSlot)
			{
				_bool* pBool;
				pBool = &m_bCreateDragSlot;
				CreateDragItemSlot();
				m_ItemInfo.iAmount = 0;
				m_tORTHOUI_INFO.fAlpha = ItemSlotDragEDGEALPHA;
				m_tORTHOUI_INFO.fEdgeAlpha = ItemSlotDragEDGEALPHA;
				m_bBoxSelect = false;
			}
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

void CInvenItem::CreateDragItemSlot()
{
	POINT stMyNowScreenPos;
	switch (m_eSlotOwner)
	{
	case ESlotOwner::Left_Inven:
		stMyNowScreenPos.x = (LONG)m_tORTHOUI_INFO.fPosX;
		stMyNowScreenPos.y = (LONG)m_tORTHOUI_INFO.fPosY;
		static_cast<CLR_Inventory*>(m_pSlotOwner)->CreateDragItemSlot(stMyNowScreenPos, m_iSlotNum);
		break;
	case ESlotOwner::Right_Inven:
		stMyNowScreenPos.x = (LONG)m_tORTHOUI_INFO.fPosX;
		stMyNowScreenPos.y = (LONG)m_tORTHOUI_INFO.fPosY;
		static_cast<CLR_Inventory*>(m_pSlotOwner)->CreateDragItemSlot(stMyNowScreenPos, m_iSlotNum);
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

	m_bCreateDragSlot = true;
}
void CInvenItem::DeleteDropItemSlot()
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
		if (nullptr == static_cast<CInven*>(static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Get_InvenOwner()->Get_Component(L"Com_Show_Inven", ID_DYNAMIC)))
			pRemoveInven = static_cast<CInven*>(pOwnerUnit->Get_Component(L"Com_Inven", ID_DYNAMIC));
		else
			pRemoveInven = static_cast<CInven*>(pOwnerUnit->Get_Component(L"Com_Show_Inven", ID_DYNAMIC));
		break;
	}

	if (ESlotOwner::Right_Inven != m_eSlotOwner)
	{
		if (static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Get_InvenOwner())
		{	//나는 오른쪽 인벤이 아닌데 오른쪽 인벤에 떨궈졌네?

			ORTHOUI_INFO tRightInventoryInfo = *static_cast<CInventory*>(Engine::Get_GameObject(L"Right_Inventory", 0))->Get_ORTHOUI_INFO();
			if(tRightInventoryInfo.bRender)
			if (m_tORTHOUI_INFO.tMousePos.x >= (tRightInventoryInfo.fPosX - (tRightInventoryInfo.fSizeX*FHALF))
				&& m_tORTHOUI_INFO.tMousePos.x <= (tRightInventoryInfo.fPosX + (tRightInventoryInfo.fSizeX*FHALF))
				&& m_tORTHOUI_INFO.tMousePos.y >= (tRightInventoryInfo.fPosY - (tRightInventoryInfo.fSizeY*FHALF))
				&& m_tORTHOUI_INFO.tMousePos.y <= (tRightInventoryInfo.fPosY + (tRightInventoryInfo.fSizeY*FHALF)))
			{
				//오른쪽 인벤에 추가해
				if (static_cast<CInven*>(static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Get_InvenOwner()->Get_Component(L"Com_Show_Inven", ID_DYNAMIC)))
					static_cast<CInven*>(static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Get_InvenOwner()->Get_Component(L"Com_Show_Inven", ID_DYNAMIC))->Add_Item(m_ItemInfo);
				else
					static_cast<CInven*>(static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 1))->Get_InvenOwner()->Get_Component(L"Com_Inven", ID_DYNAMIC))->Add_Item(m_ItemInfo);
				//내가 있던 곳에선 빼
				Remove_DropItem_To_Inven(pRemoveInven);
			}
		}
	}

	if (ESlotOwner::Left_Inven != m_eSlotOwner)
	{
		//나는 왼쪽 인벤이 아닌데 왼쪽 인벤에 떨궈졌네?
		ORTHOUI_INFO tLeftInventoryInfo = *static_cast<CInventory*>(Engine::Get_GameObject(L"Left_Inventory", 0))->Get_ORTHOUI_INFO();
		if (tLeftInventoryInfo.bRender)
		if (m_tORTHOUI_INFO.tMousePos.x >= (tLeftInventoryInfo.fPosX - (tLeftInventoryInfo.fSizeX*FHALF))
			&& m_tORTHOUI_INFO.tMousePos.x <= (tLeftInventoryInfo.fPosX + (tLeftInventoryInfo.fSizeX*FHALF))
			&& m_tORTHOUI_INFO.tMousePos.y >= (tLeftInventoryInfo.fPosY - (tLeftInventoryInfo.fSizeY*FHALF))
			&& m_tORTHOUI_INFO.tMousePos.y <= (tLeftInventoryInfo.fPosY + (tLeftInventoryInfo.fSizeY*FHALF)))
		{
			//왼쪽 인벤에 추가해
			static_cast<CInven*>(static_cast<CLR_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Get_InvenOwner()->Get_Component(L"Com_Inven", ID_DYNAMIC))->Add_Item(m_ItemInfo);
			//내가 있던 곳에선 빼
			Remove_DropItem_To_Inven(pRemoveInven);
		}
	}

	if (ESlotOwner::Stat_Player != m_eSlotOwner)
	{
		ORTHOUI_INFO tBack_Stat_Info = *static_cast<CBack_Stat_UI*>(Engine::Get_GameObject(L"Stat_UI", 0))->Get_ORTHOUI_INFO();
		if(tBack_Stat_Info.bRender)
		if (ITEMID_Equip == (m_ItemInfo.iItemType))
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
			else if(0 == tChangeItem.iAmount)
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
void CInvenItem::Remove_DropItem_To_Inven(CInven * _pInven)
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
void CInvenItem::Set_MouseClickPos(POINT* _tNowMouse)
{
	m_MouseClickPos.x = _tNowMouse->x;
	m_MouseClickPos.y = _tNowMouse->y;
}
void CInvenItem::Set_FirstPos(POINT _tPos)
{
	m_tFirstPos = _tPos;
}
void CInvenItem::Set_Go_Making()
{
	if (m_pEngram_BackGroundCom)
	{
		m_bNow_Making = true;
		m_MakingStartItmeInfo = m_MakingInfo;
		m_fMakingTime = FNULL;
		m_tORTHOUI_INFO.iOrderEdge |= B_Order_Making;
	}
}
void CInvenItem::Making_Item_Loading_And_Create(const _double & _dTimeDelta)
{
	m_fMakingTime += (_float)_dTimeDelta;
	m_tORTHOUI_INFO.fOrderContents = m_fMakingTime / (_float)m_MakingStartItmeInfo.dMakingTime;
	if (1.f <= m_tORTHOUI_INFO.fOrderContents) //제작 시간이 다 지났으면!
	{
		Decide_MakingColor();
		m_bNow_Making = false;
		m_fMakingTime = FNULL;
		m_tORTHOUI_INFO.fOrderContents = FNULL;
		m_tORTHOUI_INFO.iOrderEdge &= ~B_Order_Making; // 렌더되는 그림은 인벤토리 탭으로 바껴도 없어져야 하기 때문에 셋 텍스쳐에서 엔그램(제작아이템 상태) 텍스쳐가 없으면 오더를 취소한다
		if (m_bPossibleMakingLv)
			if (m_bPossibleMaking)
				static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Create_MakingItem((ITEMID)m_MakingStartItmeInfo.iMakingItemID);
		MAKING_INFO tReset;
		m_MakingStartItmeInfo = tReset;
	}
}
void CInvenItem::Decide_MakingColor()
{
	//if(!m_bPossibleMakingLv)//렙이 안되면 렙이 되나 계속 확인한다, 한번 렙 넘으면 줄을 일 없으니 안바꿈
		m_bPossibleMakingLv = Check_MakingPossibleLv();
	//else //렙이 되면 제작 가능한지 확인한다
	//{
		/*if(m_bPossibleMakingLv)*/
		//제작이 불가능해도 재료는 확인 가능해야 함
		m_bPossibleMaking = Check_MakingPossible();
	//}
}
_bool CInvenItem::Check_MakingPossible()
{
	CInven* pPlayerInven = static_cast<CInven*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Inven", ID_DYNAMIC));
	m_tMII.iIngredientItemID[0] = pPlayerInven->Now_Have_ItemAmount((ITEMID)m_MakingInfo.iIngredientItemID_1);
	m_tMII.iIngredientItemID[1]  = pPlayerInven->Now_Have_ItemAmount((ITEMID)m_MakingInfo.iIngredientItemID_2);
	m_tMII.iIngredientItemID[2]  = pPlayerInven->Now_Have_ItemAmount((ITEMID)m_MakingInfo.iIngredientItemID_3);
	m_tMII.iIngredientItemID[3]  = pPlayerInven->Now_Have_ItemAmount((ITEMID)m_MakingInfo.iIngredientItemID_4);

	CQuickSlot_UI* pQuickSlot = static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0));
	m_tMII.iIngredientItemID[0] += pQuickSlot->Now_Have_ItemAmount((ITEMID)m_MakingInfo.iIngredientItemID_1);
	m_tMII.iIngredientItemID[1] += pQuickSlot->Now_Have_ItemAmount((ITEMID)m_MakingInfo.iIngredientItemID_2);
	m_tMII.iIngredientItemID[2] += pQuickSlot->Now_Have_ItemAmount((ITEMID)m_MakingInfo.iIngredientItemID_3);
	m_tMII.iIngredientItemID[3] += pQuickSlot->Now_Have_ItemAmount((ITEMID)m_MakingInfo.iIngredientItemID_4);

	if (m_MakingInfo.iNumber_of_Required_ItemID_1 <= m_tMII.iIngredientItemID[0]
		&& m_MakingInfo.iNumber_of_Required_ItemID_2 <= m_tMII.iIngredientItemID[1]
		&& m_MakingInfo.iNumber_of_Required_ItemID_3 <= m_tMII.iIngredientItemID[2]
		&& m_MakingInfo.iNumber_of_Required_ItemID_4 <= m_tMII.iIngredientItemID[3])
		return true;
	else
		return false;

}
_bool CInvenItem::Check_MakingPossibleLv()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0));
	
	if(m_MakingInfo.iMakingPossibleLv <= pPlayer->Get_Stat(CUnit::STAT_ENGRAM))
		return true;
	else
		return false;
	return true;
}
void CInvenItem::Create_Item_Info_View(const _double & _dTimeDelta)
{
	if (!m_tORTHOUI_INFO.bRender)
		return;

	if (!m_pTextureCom)
		return;

	if (m_bDragItem)
		return;

	if(m_eSlotOwner == QuickSlot)
		return;

	if((m_MakingInfo.iMakingItemID) == ITEMID_END && (m_ItemInfo.iAmount == 0))
		return;

	if ((MouseOnUI & m_tORTHOUI_INFO.uchSelectUI))
		m_fItem_Info_View_Time += (_float)_dTimeDelta;
	else
		m_fItem_Info_View_Time = 0.f;

	_vec2 v2Pos = { m_tORTHOUI_INFO.fPosX ,m_tORTHOUI_INFO.fPosY };

	if (ItemInfoViewOpenTime < m_fItem_Info_View_Time)
		if(ITEMID_END == m_ItemInfo.iItemID)
			static_cast<CItem_Info_View_UI*>(Engine::Get_GameObject(L"InfoView_UI", 1))->On_Making_Info_View(m_MakingInfo, v2Pos,m_tMII);
		else
			static_cast<CItem_Info_View_UI*>(Engine::Get_GameObject(L"InfoView_UI", 1))->On_Item_Info_View(m_ItemInfo, v2Pos);

}
void CInvenItem::Set_Scroll_Show(_float _fScroll_MoveY , _float _fShow_TopY ,_float _fShow_BottomY)
{
	//_fScroll_MoveY -> 얼마나 아래로 내려왔는지 값, 렌더할 땐 빼주면 될 듯?
	m_fScroll_FixY = _fScroll_MoveY;

	m_fShow_TopY = _fShow_TopY;
	m_fShow_BottomY = _fShow_BottomY;

	if (!m_bDragItem) //드랍 템이 아니면
		m_tORTHOUI_INFO.fPosY = m_tFirstPos.y - m_fScroll_FixY;

	//_fShow_TopY -> 스크린 짤리는 젤 위에 포스
	//_fShow_BottomY -> 스크린 짤리는 젤 밑에 포스
	//_float				m_fDrawCutRatio = 0.f;
	//_bool					m_bDrawCutUp = false; gㅐ결

	if (WINCY*FHALF > m_tORTHOUI_INFO.fPosY)//원랜 이러면 안되지만... 지금은 무조건 괜찮을 테니 ㅇㅋ
		m_iDrawCutUp = (_int)EScroll_Cut::EScroll_Cut_Up;
	else
		m_iDrawCutUp = (_int)EScroll_Cut::EScroll_Cut_Down;

	//m_fDrawCutRatio

	if ((_int)EScroll_Cut::EScroll_Cut_Up == m_iDrawCutUp) //위로 올라간 친구면
		m_fDrawCutRatio = (_fShow_TopY - (m_tORTHOUI_INFO.fPosY - (m_tORTHOUI_INFO.fSizeY*FHALF))) / m_tORTHOUI_INFO.fSizeY;
	else if ((_int)EScroll_Cut::EScroll_Cut_Down == m_iDrawCutUp)
		m_fDrawCutRatio = ((m_tORTHOUI_INFO.fPosY + (m_tORTHOUI_INFO.fSizeY*FHALF))- _fShow_BottomY)  / m_tORTHOUI_INFO.fSizeY;


	if (0.f > m_fDrawCutRatio)
		m_fDrawCutRatio = 0.f;
	else if(1.f < m_fDrawCutRatio)
		m_fDrawCutRatio = 1.f;


	if (1.f == m_fDrawCutRatio)
		m_bScroll_No_Render = true;
	else
		m_bScroll_No_Render = false;
}
_bool CInvenItem::Scroll_Hide_OnMouse_Func_Lock()
{
	if (EScroll_Cut::EScroll_Cut_None == m_iDrawCutUp) //인벤 아닌 슬롯들 걸러내고 인벤도 어느정도 걸러낸 후
		return false;

	Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
	_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y;

	if (MouseY > m_fShow_TopY
		&& MouseY < m_fShow_BottomY)
		return false;
		
	return true;
}
void CInvenItem::Free()
{
	m_pTextureCom = nullptr; //에드 레퍼런스 안해줘서 그냥 초기화만
	COrthoUI::Free();
}

void CInvenItem::Set_ItemTexture(CTexture * _pItemTexture, CTexture* _pEngram_BackGroundTexture, CTexture* _pCraftingBrokenTexture)
{
	m_pTextureCom = _pItemTexture;
	m_pEngram_BackGroundCom = _pEngram_BackGroundTexture;
	m_pCraftingBrokenCom = _pCraftingBrokenTexture;
	if (!m_pEngram_BackGroundCom) //제작 중에 바뀌면 제작 이미지 안 띄워야 하기 때문에
	{
		m_tORTHOUI_INFO.fOrderContents = FNULL;
		m_tORTHOUI_INFO.iOrderEdge &= ~B_Order_Making;
	}
}

void CInvenItem::Set_ItemInfo(const ITEM_INFO * _pItemInfo, ITEMID _eItemID)
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

void CInvenItem::Set_MakingInfo(const MAKING_INFO * _pMakingInfo)
{
	if (nullptr == _pMakingInfo)
	{
		m_MakingInfo.iMakingItemID = 0;
		m_MakingInfo.tchName = nullptr;
		m_MakingInfo.iMakingItemID = ITEMID_END;
	}
	else
	{
		m_MakingInfo = *_pMakingInfo;
	}
}

void CInvenItem::Set_pSlotOwner(CGameObject* _pSlotOwner, ESlotOwner _eSlotOwner)
{
	m_pSlotOwner = _pSlotOwner;
	m_eSlotOwner = _eSlotOwner;

	if (ESlotOwner::QuickSlot == _eSlotOwner)
	{
		m_tORTHOUI_INFO.fSizeX = stURI.fQuickSizeXY[EScrnResolution];
		m_tORTHOUI_INFO.fSizeY = stURI.fQuickSizeXY[EScrnResolution];
	}
}

ITEMID CInvenItem::Get_CanCreateItme_Question()
{
	if (m_pEngram_BackGroundCom)
	{
		if (m_bPossibleMaking&&m_bPossibleMakingLv)
			return (ITEMID)m_MakingInfo.iMakingItemID;
		else
			return ITEMID_END;
	}
	else
		return ITEMID_END;
}

void CInvenItem::Set_SlotNum(_int _Num)
{
	 m_iSlotNum = _Num;

	 if (!m_bFirstSlotCheck)
	 {
		 m_iFirstSlotNum = _Num;
		 m_bFirstSlotCheck = true;
	 }
}
