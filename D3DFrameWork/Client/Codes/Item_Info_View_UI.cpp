#include "stdafx.h"
#include "..\Headers\Item_Info_View_UI.h"

#include "Export_Function.h"
#include "Left_Inventory.h"
#include "InvenItem.h"

//#define ItemInfoViewSizeX 192.f 
//#define ItemInfoViewSizeY 96.f 
#define ItemInfoViewAlpha 0.7f;
//#define ItemTexAddPosX -66.f
//#define ItemTexAddPosY -5.f
#define ItemNameFontColor D3DXCOLOR(0.7f, 0.9f, 1.f, 1.f)
#define ItemCraftableColor D3DXCOLOR(0.5f, 1.f, 0.6f, 1.f)
#define ItemUnCraftableColor D3DXCOLOR(1.f, 0.5f, 0.6f, 1.f)

USING(Engine)

HRESULT CItem_Info_View_UI::Set_Screen_UI()
{
	STItem_Info_View_UIURI stURI;
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	return S_OK;
}

CItem_Info_View_UI::CItem_Info_View_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CItem_Info_View_UI::CItem_Info_View_UI(const CItem_Info_View_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CItem_Info_View_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CItem_Info_View_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = ItemInfoViewAlpha;
	return S_OK;
}

_int CItem_Info_View_UI::Update_GameObject(const _double & _dTimeDelta)
{
	if (!m_tORTHOUI_INFO.bRender)
		return NO_EVENT;

	COrthoUI::Update_GameObject(_dTimeDelta);


	return NO_EVENT;
}

_int CItem_Info_View_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_tORTHOUI_INFO.bRender)
		return NO_EVENT;

	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	Engine::Add_RenderGroup(RENDER_LASTUI, this);
	return NO_EVENT;
}

void CItem_Info_View_UI::Render_GameObject(_mat _matOrtho)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 6;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(NULL); /////

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();

	//////////////////////////	
	if (m_pMakingTex_Com[0])
	{
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, 2); //필요 재료 띄우기 - 백 그라운드
		pEffect->BeginPass(0);    ////
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();

		for (_int i = 0; i < 4; ++i)
		{
			if (m_pMakingTex_Com[i])
			{
				pEffect->CommitChanges();
				SetUp_ConstantTable(pEffect, _matOrtho, 3, i); //필요 재료 띄우기
				pEffect->BeginPass(1);    ////
				m_pBufferCom->Render_VIBuffer();
				pEffect->EndPass();
			}
		}
	}
	//////////////////////////	
	//////////////////////////	
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho,1); //아이템 텍스쳐 띄우기
	pEffect->BeginPass(1);    ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////
	

	pEffect->End();

	Safe_Release(pEffect);
	Render_Font();
	m_tORTHOUI_INFO.bRender = false;
}

HRESULT CItem_Info_View_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pItemInfoView_BgCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_InfoView_Bg"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_ItemInfo_View"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CItem_Info_View_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo, _int _IngredientItemNum)
{
	STItem_Info_View_UIURI stURI;

	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	if (1 == _bTwo) // 아이템 텍스쳐
	{
		matWorld._11 = stURI.fItemSizeXY[EScrnResolution];
		matWorld._22 = stURI.fItemSizeXY[EScrnResolution];
		matWorld._41 = (m_tORTHOUI_INFO.fPosX - stURI.fTextureFixPosX[EScrnResolution]) - WINCX * FHALF;
		matWorld._42 = -(m_tORTHOUI_INFO.fPosY - stURI.fTextureFixPosY[EScrnResolution]) + WINCY * FHALF;
		if (m_pItemTex_Com)
			m_pItemTex_Com->Set_Texture(_pEffect, "g_Bg_Tex");
	}
	else if (2 == _bTwo)//필요 재료 띄우기 - 백 그라운드
	{
		matWorld._11 = m_tORTHOUI_INFO.fSizeX - stURI.fMaking_Ingredient_Bg_FixSizeX[EScrnResolution];
		matWorld._22 = m_tORTHOUI_INFO.fSizeY;
		matWorld._41 = (m_tORTHOUI_INFO.fPosX + (m_tORTHOUI_INFO.fSizeX*FHALF)+((m_tORTHOUI_INFO.fSizeX - stURI.fMaking_Ingredient_Bg_FixSizeX[EScrnResolution])*FHALF) + stURI.fMaking_Ingredient_Bg_IntervalX[EScrnResolution]) - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		m_pItemInfoView_BgCom->Set_Texture(_pEffect, "g_Bg_Tex");
	}
	else if (3 == _bTwo)//필요 재료 띄우기
	{
		matWorld._11 = stURI.fMaking_Ingredient_Item_SizeXY[EScrnResolution];
		matWorld._22 = stURI.fMaking_Ingredient_Item_SizeXY[EScrnResolution];
		matWorld._41 = (m_tORTHOUI_INFO.fPosX + (m_tORTHOUI_INFO.fSizeX*FHALF) + ((m_tORTHOUI_INFO.fSizeX - stURI.fMaking_Ingredient_Bg_FixSizeX[EScrnResolution])*FHALF) - stURI.fMaking_Ingredient_Item_IntervalX[EScrnResolution]) - WINCX * FHALF;
		matWorld._42 = -(m_tORTHOUI_INFO.fPosY - stURI.fMaking_Ingredient_Item_FixY[EScrnResolution] + (_IngredientItemNum * stURI.fMaking_Ingredient_Item_IntervalY[EScrnResolution]) )+ WINCY * FHALF;
		m_pMakingTex_Com[_IngredientItemNum]->Set_Texture(_pEffect, "g_Bg_Tex");
	}
	else
	{
		matWorld._11 = m_tORTHOUI_INFO.fSizeX;
		matWorld._22 = m_tORTHOUI_INFO.fSizeY;
		matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		m_pItemInfoView_BgCom->Set_Texture(_pEffect, "g_Bg_Tex");
	}
		matWorld._33 = FTRUE;
		matWorld._43 = FTRUE;

	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);
	_pEffect->SetFloat("g_fAlpha", m_tORTHOUI_INFO.fAlpha);

	return S_OK;
}

void CItem_Info_View_UI::On_Item_Info_View(ITEM_INFO _tItem_Info, _vec2 ItemSlotPos)
{
	UI_RenderOn(); //킨다
	m_tView_Item_Info = _tItem_Info;
	MAKING_INFO InitMaking;
	m_tView_Making_Info = InitMaking;
	MII_INFO  tMII;
	m_tMII = tMII;
	Set_MyPos(ItemSlotPos);
	m_pItemTex_Com = static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Get_pItemTexture(_tItem_Info.tchName); //사진 받는다
	
	m_pMakingTex_Com[0] = nullptr;
	m_pMakingTex_Com[1] = nullptr;
	m_pMakingTex_Com[2] = nullptr;
	m_pMakingTex_Com[3] = nullptr;
}

void CItem_Info_View_UI::On_Making_Info_View(MAKING_INFO _tMaking_Info, _vec2 ItemSlotPos, MII_INFO _tMII)
{
	UI_RenderOn();
	m_tView_Making_Info = _tMaking_Info;
	ITEM_INFO InitItem;
	m_tView_Item_Info = InitItem;
	m_tMII = _tMII;
	Set_MyPos(ItemSlotPos);
	m_pItemTex_Com = static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Get_pItemTexture(_tMaking_Info.tchName);
	
	m_pMakingTex_Com[0] = static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Get_pItemTexture(Engine::Clone_Item_Prototype(_tMaking_Info.iIngredientItemID_1).tchName);
	m_pMakingTex_Com[1] = static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Get_pItemTexture(Engine::Clone_Item_Prototype(_tMaking_Info.iIngredientItemID_2).tchName);
	m_pMakingTex_Com[2] = static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Get_pItemTexture(Engine::Clone_Item_Prototype(_tMaking_Info.iIngredientItemID_3).tchName);
	m_pMakingTex_Com[3] = static_cast<CLeft_Inventory*>(Engine::Get_GameObject(L"LR_Inventory", 0))->Get_pItemTexture(Engine::Clone_Item_Prototype(_tMaking_Info.iIngredientItemID_4).tchName);
}

void CItem_Info_View_UI::Set_MyPos(_vec2 ItemSlotPos)
{
	m_tORTHOUI_INFO.fPosX = ItemSlotPos.x;
	m_tORTHOUI_INFO.fPosY = ItemSlotPos.y;
}

void CItem_Info_View_UI::Render_Font()
{
	STItem_Info_View_UIURI stURI;

	wstring wstrTemp;
	if (m_pMakingTex_Com[0]) //메이킹 인포
	{
		ITEM_INFO InitItem;
		InitItem = Engine::Clone_Item_Prototype(m_tView_Making_Info.iMakingItemID);

		//아이템 이름
		wstrTemp = m_tView_Making_Info.tchKoreaName;
		Engine::Render_Font(L"Font_ItemGuide", wstrTemp, &_vec2(m_tORTHOUI_INFO.fPosX - stURI.fFont_ItemName_FixPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_ItemName_FixPosY[EScrnResolution]), ItemNameFontColor, 3);

		//아이템 설명
		wstrTemp = InitItem.tchToolTip;
		Engine::Render_Font(L"Font_ItemGuide", wstrTemp, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_ItemToolTip_FixPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_ItemToolTip_FixPosY[EScrnResolution]), ItemNameFontColor, 3);

		//내구도
		wstrTemp = L"내구도 : ";
		wstrTemp += to_wstring((_int)InitItem.dLife);
		wstrTemp += L"/";
		wstrTemp += to_wstring((_int)InitItem.dMaxLife);
		Engine::Render_Font(L"Font_ItemGuide", wstrTemp, &_vec2(m_tORTHOUI_INFO.fPosX - stURI.fFont_ItemDurability_FixPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_ItemDurability_FixPosY[EScrnResolution]), ItemNameFontColor, 3);

		//요구 재료
		wstrTemp = L"요구 재료";
		Engine::Render_Font(L"Font_ItemGuide", wstrTemp, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_Required_FixPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_Required_FixPosY[EScrnResolution]), ItemNameFontColor, 3);

		for (_int i = 0; i < 4; ++i)
		{
			if (m_pMakingTex_Com[i])
			{
				_int			iR_ItemAmount = 0;
				switch (i)
				{
					//아이템 슬롯에서 계산한걸 가지고있던가 해서 넘겨줘야 댐 ㅇㅇ
				case 0: 
					wstrTemp = Engine::Clone_Item_Prototype(m_tView_Making_Info.iIngredientItemID_1).tchKoreaName;
					wstrTemp += L" ";
					wstrTemp += to_wstring(m_tMII.iIngredientItemID[i]);
					wstrTemp += L"/";
					wstrTemp += to_wstring(m_tView_Making_Info.iNumber_of_Required_ItemID_1);
					iR_ItemAmount = m_tView_Making_Info.iNumber_of_Required_ItemID_1;
					break;
				case 1:
					wstrTemp = Engine::Clone_Item_Prototype(m_tView_Making_Info.iIngredientItemID_2).tchKoreaName;
					wstrTemp += L" ";
					wstrTemp += to_wstring(m_tMII.iIngredientItemID[i]);
					wstrTemp += L"/";
					wstrTemp += to_wstring(m_tView_Making_Info.iNumber_of_Required_ItemID_2); 
					iR_ItemAmount = m_tView_Making_Info.iNumber_of_Required_ItemID_2;
					break;
				case 2:
					wstrTemp = Engine::Clone_Item_Prototype(m_tView_Making_Info.iIngredientItemID_3).tchKoreaName;
					wstrTemp += L" ";
					wstrTemp += to_wstring(m_tMII.iIngredientItemID[i]);
					wstrTemp += L"/";
					wstrTemp += to_wstring(m_tView_Making_Info.iNumber_of_Required_ItemID_3);
					iR_ItemAmount = m_tView_Making_Info.iNumber_of_Required_ItemID_3;
					break;
				case 3:
					wstrTemp = Engine::Clone_Item_Prototype(m_tView_Making_Info.iIngredientItemID_4).tchKoreaName;
					wstrTemp += L" ";
					wstrTemp += to_wstring(m_tMII.iIngredientItemID[i]);
					wstrTemp += L"/";
					wstrTemp += to_wstring(m_tView_Making_Info.iNumber_of_Required_ItemID_4);
					iR_ItemAmount = m_tView_Making_Info.iNumber_of_Required_ItemID_4;
					break;
				}

				if(m_tMII.iIngredientItemID[i] < iR_ItemAmount)
					Engine::Render_Font(L"Font_ItemGuide", wstrTemp, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_IngredientRequired_FixPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_IngredientRequired_FixPosY[EScrnResolution] + (i*stURI.fFont_IngredientRequired_IntervalY[EScrnResolution])), ItemUnCraftableColor, 3);
				else																																																																															   
					Engine::Render_Font(L"Font_ItemGuide", wstrTemp, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_IngredientRequired_FixPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_IngredientRequired_FixPosY[EScrnResolution] + (i*stURI.fFont_IngredientRequired_IntervalY[EScrnResolution])), ItemCraftableColor, 3);
			}
		}
	}
	else //아이템 인포
	{
		//아이템 이름
		wstrTemp = m_tView_Item_Info.tchKoreaName;
		Engine::Render_Font(L"Font_ItemGuide", wstrTemp, &_vec2(m_tORTHOUI_INFO.fPosX - stURI.fFont_ItemName_FixPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_ItemName_FixPosY[EScrnResolution]), ItemNameFontColor, 3);

		//아이템 설명
		wstrTemp = m_tView_Item_Info.tchToolTip;
		Engine::Render_Font(L"Font_ItemGuide", wstrTemp, &_vec2(m_tORTHOUI_INFO.fPosX + stURI.fFont_ItemToolTip_FixPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_ItemToolTip_FixPosY[EScrnResolution]), ItemNameFontColor, 3);

		//내구도
		wstrTemp = L"내구도 : ";
		wstrTemp += to_wstring((_int)m_tView_Item_Info.dLife);
		wstrTemp += L"/";
		wstrTemp += to_wstring((_int)m_tView_Item_Info.dMaxLife);
		Engine::Render_Font(L"Font_ItemGuide", wstrTemp, &_vec2(m_tORTHOUI_INFO.fPosX - stURI.fFont_ItemDurability_FixPosX[EScrnResolution], m_tORTHOUI_INFO.fPosY + stURI.fFont_ItemDurability_FixPosY[EScrnResolution]), ItemNameFontColor, 3);
	}
}

CItem_Info_View_UI * CItem_Info_View_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CItem_Info_View_UI*	pInstance = new CItem_Info_View_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CItem_Info_View_UI::Clone(void * _pArg)
{
	CItem_Info_View_UI*	pClone = new CItem_Info_View_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CItem_Info_View_UI::Free()
{
	COrthoUI::Free();
}