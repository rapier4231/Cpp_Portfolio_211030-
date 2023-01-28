#include "stdafx.h"
#include "..\Headers\InfoView_UI.h"

#include "Export_Function.h"
#include "Unit.h"
#include "Player.h"
#include "Dino.h"
#include "WorldFont_UI.h"

//#define InfoViewUISizeX (240.f)
//#define InfoViewUISizeY (150.f)
#define INFOVIEWEDGEALPHA 0.6f
#define GlowLineRestTime 2.f
#define InfoFontColor   D3DXCOLOR(1.f, 1.f, 1.f, 0.8f)
#define TamingFontColor  D3DXCOLOR(1.f, 1.f, 0.8f, 0.8f)
#define InfoViewLineColor  D3DXCOLOR(0.3f, 0.8f, 1.0f, 0.3f)
#define InfoViewLineW 1.f
#define InfoViewLineSpeed 500.f
USING(Engine)

HRESULT CInfoView_UI::Set_Screen_UI()
{
	STInfoView_UIURI stURI;

	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	return S_OK;
}

CInfoView_UI::CInfoView_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CInfoView_UI::CInfoView_UI(const CInfoView_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CInfoView_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CInfoView_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.v4EdgeSize = stURI.v4EdgeSize[EScrnResolution];
	m_tORTHOUI_INFO.fEdgeAlpha = INFOVIEWEDGEALPHA;
	m_tORTHOUI_INFO.bUpdate = false;
	m_tORTHOUI_INFO.bRender = false;
	return S_OK;
}

_int CInfoView_UI::Update_GameObject(const _double & _dTimeDelta)
{
	if (!m_pPlayer)
		m_pPlayer = static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0));
	COrthoUI::Update_GameObject(_dTimeDelta);



	return NO_EVENT;
}

_int CInfoView_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);
	m_pMonster = static_cast<CUnit*>(static_cast<CPlayer*>(m_pPlayer)->Get_pPlayerLookUnit());

	if (m_iDinoNameFontID)//만약 이름 띄운 적 공룡이 있다면
	{
		static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Remove_WorldFont_UI(m_iDinoNameFontID);
		m_iDinoNameFontID = 0;
	}
	if (!m_pMonster) //플레이어 앞에 뭐가 읎으면 안 띄운다
	{
		Reset_Line();
		return NO_EVENT;
	}

	CDino* pDino = static_cast<CDino*>(m_pMonster);

	//유아기 이름 안지었으면 안띄움
	if(!pDino->Get_IsNaming() && pDino->Get_IsBaby())
		return NO_EVENT;

	if(pDino->Get_NowRidingState() || pDino->Get_Hanging())
		return NO_EVENT; //현재 타고 있는 친구면 안띄운다, 매달려있는 친구 안띄움

	if (!m_pMonster->Get_Component(L"Com_Show_Inven", ID_DYNAMIC))
	{
		Reset_Line();
		return NO_EVENT;
	}

	if( 0 > m_pMonster->Get_UnitState())
		return NO_EVENT;

	if (!(m_iDinoNameFontID || pDino->Get_NowTamingState()))//만약 이름 띄운 적 공룡이 없다면
	{
		//이름은 한번만 띄워야 함
		wstring wstrName = pDino->Get_DinoSpeciesKor();
		_vec3 AddPos = DINONAMEADDWORLDPOS;
		m_iDinoNameFontID = static_cast<CWorldFont_UI*>(Engine::Get_GameObject(L"WorldFont_UI", 0))->Print_WorldFont_UI(static_cast<CTransform*>((pDino)->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_Pos(), wstrName, CWorldFont_UI::World_Font_Translucent_Red, CWorldFont_UI::World_FontType_Follow, AddPos, -10.f);
		//m_iNameFontID 죽을때 없앰}
	}

	if (!(pDino->Get_NowTorpidState() || pDino->Get_NowTamingState() || pDino->Get_PeaceTaming()))
	{
		Reset_Line();
		return NO_EVENT; // 기안하면 안띄운다 테이밍 당한애가 아니면 안 띄운다
	}

	if (0.5f > m_fOpenDelay)
	{
		m_fOpenDelay += (_float)_dTimeDelta;
		//m_bLeftView_Check = false;
		return NO_EVENT;
	}

	if( 0 == m_iLineReady)
		m_fLineLenth1 += (_float)_dTimeDelta;
	if (1 == m_iLineReady)
		m_fLineLenth2 += (_float)_dTimeDelta;

	m_fGlowLineFallTime += (_float)_dTimeDelta;
	if (GlowLineRestTime <= m_fGlowLineFallTime)
		m_fGlowLineFallTime = FNULL;

	Engine::Add_RenderGroup(RENDER_WolrdPosUI, this);
	return NO_EVENT;
}

void CInfoView_UI::Render_GameObject(_mat _matOrtho, _mat matOldProj)
{
	//if (!m_tORTHOUI_INFO.bRender)
	//	return;

	_vec2 ScreenPos = ChangePos_World_To_Screen(matOldProj);

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos);

	_uint iPassMax = 6;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	pEffect->BeginPass(NULL); /////

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();

	//////////////////////////	가로선 그려쥬기
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 4); //가로선!
	pEffect->BeginPass(4);    ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////
	//////////////////////////	비 떨어지는 세로선 그려쥬기
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 5); //세로선!
	pEffect->BeginPass(5);    ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////
	//////////////////////////// 몬스터 스탯들
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, EStatType_HP);//체력
	pEffect->BeginPass(1);      ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, EStatType_Stamina);//기력
	pEffect->BeginPass(1);      ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, EStatType_Food);//배고픔
	pEffect->BeginPass(1);      ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, EStatType_Weight);//무게
	pEffect->BeginPass(1);      ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	////////////////////////////// 조련중 나오는 바
	CDino* pDino = static_cast<CDino*>(m_pMonster);

	if ((!pDino->Get_NowTamingState() && !pDino->Get_Dying() && pDino->Get_NowTorpidState()) || pDino->Get_PeaceTaming())
	{
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 2); //조련중~
		pEffect->BeginPass(2);    ////
		m_pBufferCom->Render_VIBuffer();
	}

	// 성장중
	if (pDino->Get_IsBaby())
	{
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 2);
		pEffect->BeginPass(2);    ////
		m_pBufferCom->Render_VIBuffer();
	}
	// 교배중
	else if (pDino->Get_MatingTime() > 0.f)
	{
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 2);
		pEffect->BeginPass(2);    ////
		m_pBufferCom->Render_VIBuffer();
	}
	// 임신중
	else if (pDino->Get_Pregnant())
	{
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 2);
		pEffect->BeginPass(2);    ////
		m_pBufferCom->Render_VIBuffer();
	}
	// 산후 조리중
	else if (pDino->Get_PostpartumCare())
	{
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 2);
		pEffect->BeginPass(2);    ////
		m_pBufferCom->Render_VIBuffer();
	}

	//////////////////////////	공룡 얼굴
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 3); //꽁룡 얽ㄹ
	pEffect->BeginPass(3);    ////
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////

	pEffect->End();

	Safe_Release(pEffect);

	m_vLine[0].x = ScreenPos.x;
	m_vLine[0].y = ScreenPos.y;

	//if (!m_bLeftView_Check)
	//{
		//m_vLine[0].z = 0.f;
		//m_vLine[1].z = 0.f;
		//m_vLine[2].z = 0.f;
	//	m_bLeftView_Check = true;
	//	if (WINCX*FHALF > ScreenPos.x)
	//		m_bLeftView = true;
	//	else
	//		m_bLeftView = false;
	//}

	_vec2 vDir;
	if (0 == m_iLineReady)
	{
		//if (m_bLeftView) //왼쪽이면
		//{
		vDir = stURI.vLine1_Left[EScrnResolution] - m_vLine[0];
		D3DXVec2Normalize(&vDir, &vDir);
		vDir *= m_fLineLenth1*InfoViewLineSpeed;
		m_vLine[1] = m_vLine[0] + vDir;
		if (stURI.vLine1_Left[EScrnResolution].x > m_vLine[1].x)
			m_vLine[1].x = stURI.vLine1_Left[EScrnResolution].x;
		if (stURI.vLine1_Left[EScrnResolution].y < m_vLine[1].y)
			m_vLine[1].y = stURI.vLine1_Left[EScrnResolution].y;
		if ((m_vLine[1].x == stURI.vLine1_Left[EScrnResolution].x) &&
			m_vLine[1].y == stURI.vLine1_Left[EScrnResolution].y)
			m_iLineReady = 1;
		//}
		//else//오른쪽 이면
		//{
		//	vDir = stURI.vLine1_Right[EScrnResolution] - m_vLine[0];
		//	D3DXVec3Normalize(&vDir, &vDir);
		//	vDir *= m_fLineLenth1*200.f;
		//	m_vLine[1] = m_vLine[0] + vDir;
		//	if (stURI.vLine1_Right[EScrnResolution].x < m_vLine[1].x)
		//		m_vLine[1].x = stURI.vLine1_Right[EScrnResolution].x;
		//	if (stURI.vLine1_Right[EScrnResolution].y < m_vLine[1].y)
		//		m_vLine[1].y = stURI.vLine1_Right[EScrnResolution].y;
		//	if ((m_vLine[1].x == stURI.vLine1_Right[EScrnResolution].x) &&
		//		m_vLine[1].y == stURI.vLine1_Right[EScrnResolution].y)
		//		m_iLineReady = 1;
		//}
	}
	else if (1 == m_iLineReady)
	{
		//if (m_bLeftView) //왼쪽이면
		//{
		vDir = stURI.vLine2_Left[EScrnResolution] - m_vLine[1];
		D3DXVec2Normalize(&vDir, &vDir);
		vDir *= m_fLineLenth2*InfoViewLineSpeed;
		m_vLine[2] = m_vLine[1] + vDir;
		if (stURI.vLine2_Left[EScrnResolution].x > m_vLine[2].x)
			m_vLine[2].x = stURI.vLine2_Left[EScrnResolution].x;
		if ((m_vLine[2].x == stURI.vLine2_Left[EScrnResolution].x))
			m_iLineReady = 2;
		//}
		//else
		//{
		//	vDir = stURI.vLine2_Right[EScrnResolution] - m_vLine[1];
		//	D3DXVec3Normalize(&vDir, &vDir);
		//	vDir *= m_fLineLenth2*200.f;
		//	m_vLine[2] = m_vLine[1] + vDir;
		//	if (stURI.vLine2_Right[EScrnResolution].x < m_vLine[2].x)
		//		m_vLine[2].x = stURI.vLine2_Right[EScrnResolution].x;
		//	if ((m_vLine[2].x == stURI.vLine2_Right[EScrnResolution].x))
		//		m_iLineReady = 2;
		//}
	}

	m_pDevice->BeginScene();
	ID3DXLine *Line;
	D3DXCreateLine(m_pDevice, &Line);
	Line->SetWidth(InfoViewLineW);//굵기 인가 봄
	Line->SetAntialias(true);
	Line->Begin();
	if (0 == m_iLineReady)
		Line->Draw/*Transform*/(m_vLine, 2/*, &(_matOrtho)*/, InfoViewLineColor);
	else if (1 == m_iLineReady)
		Line->Draw/*Transform*/(m_vLine, 3/*, &(_matOrtho)*/, InfoViewLineColor);
	else
		Line->Draw/*Transform*/(m_vLine, 2/*, &(_matOrtho)*/, InfoViewLineColor);
	Line->End();
	Line->Release();
	m_pDevice->EndScene();


	if (2 == m_iLineReady) // 선이 다 그려졌으면
	{
		ScreenPos.x = m_vLine[2].x;
		ScreenPos.y = m_vLine[2].y;
		LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);
		pEffect->AddRef();

		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos);

		_uint iPassMax = 6;

		pEffect->Begin(&iPassMax, NULL);

		pEffect->CommitChanges();

		pEffect->BeginPass(NULL); /////

		m_pBufferCom->Render_VIBuffer();

		pEffect->EndPass();

		//////////////////////////	가로선 그려쥬기
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 4); //가로선!
		pEffect->BeginPass(4);    ////
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
		//////////////////////////
		//////////////////////////	비 떨어지는 세로선 그려쥬기
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 5); //세로선!
		pEffect->BeginPass(5);    ////
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
		//////////////////////////
		//////////////////////////// 몬스터 스탯들
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, EStatType_HP);//체력
		pEffect->BeginPass(1);      ////
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, EStatType_Stamina);//기력
		pEffect->BeginPass(1);      ////
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, EStatType_Food);//배고픔
		pEffect->BeginPass(1);      ////
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, EStatType_Weight);//무게
		pEffect->BeginPass(1);      ////
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
		//////////////////////////	공룡 얼굴
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 3); //꽁룡 얽ㄹ
		pEffect->BeginPass(3);    ////
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
		//////////////////////////
		////////////////////////////// 조련중 나오는 바
		if (!static_cast<CDino*>(m_pMonster)->Get_NowTamingState() &&
			!(static_cast<CDino*>(m_pMonster)->Get_NowTamingState() && static_cast<CDino*>(m_pMonster)->Get_NowTorpidState()) &&
			!static_cast<CDino*>(m_pMonster)->Get_Dying())
		{
			pEffect->CommitChanges();
			SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 2); //조련중~
			pEffect->BeginPass(2);    ////
			m_pBufferCom->Render_VIBuffer();
		}

		// 성장중
		if (static_cast<CDino*>(m_pMonster)->Get_IsBaby())
		{
			pEffect->CommitChanges();
			SetUp_ConstantTable(pEffect, _matOrtho, matOldProj, ScreenPos, 2);
			pEffect->BeginPass(2);    ////
			m_pBufferCom->Render_VIBuffer();
		}

		pEffect->End();

		Safe_Release(pEffect);

		Render_Font(_matOrtho, ScreenPos);
	}
}

HRESULT CInfoView_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pInfoView_BgCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_InfoView_Bg"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pHpTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_SummaryStat_Health"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture2", pComponent);

	pComponent = m_pHpFillTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_SummaryStat_Health_Fill"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture3", pComponent);

	pComponent = m_pStaminaTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_SummaryStat_Stamina"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture4", pComponent);

	pComponent = m_pStaminaFillTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_SummaryStat_Stamina_Fill"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture5", pComponent);

	pComponent = m_pFoodTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_SummaryStat_Food"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture6", pComponent);

	pComponent = m_pFoodFillTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_SummaryStat_Food_Fill"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture7", pComponent);

	pComponent = m_pWeightTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_SummaryStat_Weight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture8", pComponent);

	pComponent = m_pWeightFillTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_SummaryStat_Weight_Fill"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture9", pComponent);
	
	pComponent = m_pLineTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_SplittingLine"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture10", pComponent);	

	pComponent = m_pInventoryBgTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Inventory"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture11", pComponent);

	pComponent = m_pInventoryBgFillterTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Inventory_Filter"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture12", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_InfoView_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CInfoView_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _mat matOldProj, _vec2 _v2ScreenPos, _int _bTwo)
{
	STInfoView_UIURI stURI;
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	if (10 <= _bTwo) // 스텟
	{
		matWorld._11 = stURI.fStatIcon_SizeXY[EScrnResolution];
		matWorld._22 = stURI.fStatIcon_SizeXY[EScrnResolution];
		matWorld._33 = FTRUE;
		//										10 빼주는 건, 아이콘이 10번부터여서 (그냥 몇번짼지 알려고 하는거
		matWorld._41 = (_v2ScreenPos.x + ((_bTwo - 10) * (matWorld._11 + stURI.fStatIcon_IntervalPosX[EScrnResolution]))- stURI.fStatIcon_FixPosX[EScrnResolution]) - WINCX * FHALF;
		matWorld._42 = -(_v2ScreenPos.y + stURI.fStatIcon_FixPosY[EScrnResolution])+ WINCY * FHALF;
		matWorld._43 = FTRUE;
		switch (_bTwo)
		{
		case EStatType_HP:
			m_pHpTexCom->Set_Texture(_pEffect, "g_Stat_Tex");
			m_pHpFillTexCom->Set_Texture(_pEffect, "g_StatFill_Tex");
			m_fMaxMonsterStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_MAXHP);
			m_fNowMonsterStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_HP);
			break;
		case EStatType_Stamina:			
			m_pStaminaTexCom->Set_Texture(_pEffect, "g_Stat_Tex");
			m_pStaminaFillTexCom->Set_Texture(_pEffect, "g_StatFill_Tex");
			m_fMaxMonsterStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_MAXSTAMINA);
			m_fNowMonsterStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_STAMINA);
			break;
		case EStatType_Food:			
			m_pFoodTexCom->Set_Texture(_pEffect, "g_Stat_Tex");
			m_pFoodFillTexCom->Set_Texture(_pEffect, "g_StatFill_Tex");
			m_fMaxMonsterStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_MAXHUNGER);
			m_fNowMonsterStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_HUNGER);
			break;
		case EStatType_Weight:			
			m_pWeightTexCom->Set_Texture(_pEffect, "g_Stat_Tex");
			m_pWeightFillTexCom->Set_Texture(_pEffect, "g_StatFill_Tex");
			m_fMaxMonsterStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_MAXWEIGHT);
			m_fNowMonsterStat = (_float)m_pMonster->Get_Stat(CUnit::STAT_WEIGHT);
			break;
		default:
			break;
		}
		m_fPerMonsterStat = m_fNowMonsterStat / m_fMaxMonsterStat;
		_pEffect->SetFloat("g_PercentStat", FTRUE - m_fPerMonsterStat);
	}
	else if (2 == _bTwo) //아래 조련중 뜨는 바
	{
		matWorld._11 = stURI.fSizeX[EScrnResolution];
		matWorld._22 = stURI.fTamingBar_SizeY[EScrnResolution];
		matWorld._33 = FTRUE;

		matWorld._41 = _v2ScreenPos.x - WINCX * FHALF;
		matWorld._42 = -(_v2ScreenPos.y + (stURI.fSizeY[EScrnResolution] *FHALF) + stURI.fTamingBar_FixPosY[EScrnResolution]) + WINCY * FHALF;
		matWorld._43 = FTRUE;

		_pEffect->SetFloat("g_PercentStat", m_fPerMonsterTamingRate/*0.5f*/);
	}
	else if (3 == _bTwo) //공룡 얼굴
	{
		matWorld._11 = stURI.fDinoFace_SizeXY[EScrnResolution];
		matWorld._22 = stURI.fDinoFace_SizeXY[EScrnResolution];
		matWorld._33 = FTRUE;

		matWorld._41 = (_v2ScreenPos.x - stURI.fDinoFace_FixPosX[EScrnResolution]) - WINCX * FHALF;
		matWorld._42 = -(_v2ScreenPos.y + stURI.fDinoFace_FixPosY[EScrnResolution]) + WINCY * FHALF;
		matWorld._43 = FTRUE;
		if (Get_DinoEmptyTex())
		{
			m_pEmptyDinoTexCom->Set_Texture(_pEffect, "g_Stat_Tex");
			Safe_Release(m_pEmptyDinoTexCom);
		}
	}
	else if (4 == _bTwo) //가로선 그려주기
	{
		matWorld._11 = stURI.fHorizontalLine_SizeX[EScrnResolution];
		matWorld._22 = stURI.fHorizontalLine_SizeY[EScrnResolution];
		matWorld._33 = FTRUE;

		matWorld._41 = (_v2ScreenPos.x + stURI.fHorizontalLine_FixPosX[EScrnResolution]) - WINCX * FHALF;
		matWorld._42 = -(_v2ScreenPos.y - stURI.fHorizontalLine_FixPosY[EScrnResolution]) + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pLineTexCom->Set_Texture(_pEffect, "g_Stat_Tex");
	}
	else if (5 == _bTwo)  //비 내리는 듯한 글로우 만들어 주기
	{
		matWorld._11 = stURI.fSizeX[EScrnResolution];
		matWorld._22 = stURI.fSizeY[EScrnResolution];
		matWorld._33 = FTRUE;

		matWorld._41 = _v2ScreenPos.x - WINCX * FHALF;
		matWorld._42 = -_v2ScreenPos.y + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pInventoryBgTexCom->Set_Texture(_pEffect, "g_Stat_Tex");
		m_pInventoryBgFillterTexCom->Set_Texture(_pEffect, "g_StatFill_Tex");
		_pEffect->SetFloat("g_PercentStat", -m_fGlowLineFallTime);
	}
	else               //백그라운드
	{
		matWorld._11 = stURI.fSizeX[EScrnResolution];
		matWorld._22 = stURI.fSizeY[EScrnResolution];
		matWorld._33 = FTRUE;

		matWorld._41 = _v2ScreenPos.x - WINCX * FHALF;
		matWorld._42 = -_v2ScreenPos.y + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pInfoView_BgCom->Set_Texture(_pEffect, "g_Bg_Tex");
	}

	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);
	return S_OK;
}

_vec2 CInfoView_UI::ChangePos_World_To_Screen(_mat matOldProj)
{
	STInfoView_UIURI stURI;
	CTransform* pMonsterTrans = static_cast<CTransform*>(m_pMonster->Get_Component(L"Com_Transform", ID_DYNAMIC));//몬스터의 트랜스폼
	_mat matWorld, matView, matVP;
	matWorld = pMonsterTrans->Get_WorldMatrix();							 //(몬스터의) 월드 행렬
	_vec3 v3MonsterPos = { matWorld._41, matWorld._42 ,  matWorld._43 };	 //몬스터의 월드 좌표 
	//v3MonsterPos += _vec3{ DinoInfoViewFixWorldPos };							 //기준인 몬스터 네임 포스로 맞춘다
	_vec3 v3MonsterUp = { matWorld._21,matWorld._22 ,matWorld._23 };
	D3DXVec3Normalize(&v3MonsterUp, &v3MonsterUp);
	v3MonsterUp *= 0.35f;
	v3MonsterPos += v3MonsterUp;

  	m_pDevice->GetTransform(D3DTS_VIEW, &matView);							 //뷰 행렬
																			 //m_pDevice->GetTransform(D3DTS_PROJECTION, &m_matProj);				//투영 행렬 - 현재 직교라 이거 X
	matVP = matView * matOldProj;											 //뷰 * 투영
	D3DXVec3TransformCoord(&v3MonsterPos, &v3MonsterPos, &matVP);			 //로컬 좌표 * 월드 * 뷰 * 투영 -> 투영좌표 -1~1 사이
    //D3DVIEWPORT9 ViewPort;
    //ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
    //m_pDevice->GetViewport(&ViewPort);									 //뷰포트 행렬
    //_mat matMvs; //클라로 변형해 줄 뷰폿 매트릭스 생성
    //D3DXMatrixIdentity(&matMvs);
    //matMvs._11 = ViewPort.Width * 0.5f;
    //matMvs._22 = -(ViewPort.Height * 0.5f);
    //matMvs._33 = ViewPort.MaxZ - ViewPort.MinZ;
    //matMvs._41 = ViewPort.X + matMvs._11;
    //matMvs._42 = (-matMvs._22) + ViewPort.Y;
    //matMvs._43 = ViewPort.MinZ;
    //D3DXVec4Transform(&v4MonsterPos, &v4MonsterPos, &matMvs); //로컬 좌표 * 월드 * 뷰 * 투영 -> 투영좌표 -1~1 사이
    //v4MonsterPos.x /= v4MonsterPos.w;
    //v4MonsterPos.y /= v4MonsterPos.w;
	v3MonsterPos.x += 1.f; //위와 비슷함
	v3MonsterPos.x *= 0.5f;
	v3MonsterPos.y += 1.f;
	v3MonsterPos.y *= 0.5f;
	v3MonsterPos.y = 1.f - v3MonsterPos.y;
	v3MonsterPos.x *= WINCX;
	v3MonsterPos.y *= WINCY;//투영 좌표를 스크린 좌표로 바꿔주는 식
	/////////////////////////////////////////////////투영 끝난 후 스크린 포스 v3MonsterPos.x, v3MonsterPos.y

	//현재 이름이 기준이라 이름 포스를 잡았으니 스크린 좌표만큼 빼줘야 한다
	//v3MonsterPos.y += stURI.fFixPosY[EScrnResolution];

	return _vec2{ v3MonsterPos.x  ,v3MonsterPos.y };
}

CTexture * CInfoView_UI::Get_DinoEmptyTex()
{
	if (!m_pMonster)
		return nullptr;

	wstring TexName = L"Proto_Texture_Empty_";
	TexName += static_cast<CDino*>(m_pMonster)->Get_DinoSpecies();
	
	m_pEmptyDinoTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(TexName));
	NULL_CHECK_RETURN(m_pEmptyDinoTexCom, nullptr);

	return m_pEmptyDinoTexCom;
}

void CInfoView_UI::Render_Font(_mat _matOrtho, _vec2 _v2ScreenPos)
{
	STInfoView_UIURI stURI;
	if (!m_pMonster)
		return;

	wstring wstrNum = L"야생";
	CDino* pDino = static_cast<CDino*>(m_pMonster);

	switch (pDino->Get_Gender())
	{
	case CDino::GENDER::MALE:
		wstrNum += L" 수컷 ";
		break;
	case CDino::GENDER::FEMALE:
		wstrNum += L" 암컷 ";
		break;
	}

	wstrNum += pDino->Get_DinoSpeciesKor();
	wstrNum += L" - 레벨 무한";

	Engine::Render_Font(L"Font_LeftGuide", wstrNum, &_vec2(_v2ScreenPos.x, _v2ScreenPos.y - stURI.fFont_Monster_Name_IntervalPosY[EScrnResolution]), InfoFontColor,2);
	wstrNum = L"조련 가능자:글로리 Jo";
	Engine::Render_Font(L"Font_StatGuide", wstrNum, &_vec2(_v2ScreenPos.x, _v2ScreenPos.y - stURI.fFont_Player_Name_IntervalPosY[EScrnResolution]), InfoFontColor, 2);
	wstrNum = L"성별:자웅동체";
	Engine::Render_Font(L"Font_StatGuide", wstrNum, &_vec2(_v2ScreenPos.x, _v2ScreenPos.y - stURI.fFont_Player_Gender_Name_IntervalPosY[EScrnResolution]), InfoFontColor, 2);

	_float fTamingRate = m_pMonster->Get_Stat(CUnit::STAT_TAMEDRATE);
	_float fMaxTamingRate = m_pMonster->Get_Stat(CUnit::STAT_MAXTAMEDRATE);

	m_fPerMonsterTamingRate = fTamingRate / fMaxTamingRate;
	
	if ((!pDino->Get_NowTamingState() && pDino->Get_NowTorpidState()) || pDino->Get_PeaceTaming())
	{
		_int fSoSuJum = (_int)((m_fPerMonsterTamingRate*10.f) - ((_int)m_fPerMonsterTamingRate * 10));
		wstrNum = L"조련중: ";
		wstrNum += to_wstring((_int)(m_fPerMonsterTamingRate * 100.f)) + L"." + to_wstring(fSoSuJum) + L"%";
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(_v2ScreenPos.x, _v2ScreenPos.y + stURI.fFont_Taming_Progress_IntervalPosY[EScrnResolution]),TamingFontColor, 2);
	}

	_float fMatingTime = pDino->Get_MatingTime();
	_float fPer = 0.f;
	if (fMatingTime > 0.f)
	{
		fPer = fMatingTime / MATINGTIME;

		if ((_int)fPer >= 100)
			fPer = 100.f;

		wstrNum = L"교배중: ";
		wstrNum += to_wstring((_int)fPer) + L"." + to_wstring((_int)((fPer - (_int)fPer) * 10.f)) + L" %";
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(_v2ScreenPos.x, _v2ScreenPos.y + stURI.fFont_Taming_Progress_IntervalPosY[EScrnResolution]), TamingFontColor, 2);
	}
	else if (pDino->Get_PostpartumCare())
	{
		_float fPostpartumCareTime = pDino->Get_PostpartumCareTime();

		fPer = fPostpartumCareTime / PREGTIME;

		if ((_int)fPer >= 100)
			fPer = 100.f;

		wstrNum = L"산후조리중: ";
		wstrNum += to_wstring((_int)fPer) + L"." + to_wstring((_int)((fPer - (_int)fPer) * 10.f)) + L" %";
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(_v2ScreenPos.x, _v2ScreenPos.y + stURI.fFont_Taming_Progress_IntervalPosY[EScrnResolution]), TamingFontColor, 2);

	}
	else if (pDino->Get_Pregnant())
	{
		_float fPregnatTime = pDino->Get_PregnatTime();
		fPer = fPregnatTime / PREGTIME;

		if ((_int)fPer >= 100)
			fPer = 100.f;

		wstrNum = L"임신중: ";
		wstrNum += to_wstring((_int)fPer) + L"." + to_wstring((_int)((fPer - (_int)fPer) * 10.f)) + L" %";
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(_v2ScreenPos.x, _v2ScreenPos.y + stURI.fFont_Taming_Progress_IntervalPosY[EScrnResolution]), TamingFontColor, 2);
	}
	else if (pDino->Get_IsBaby())
	{
		_float fGrowthTime = pDino->Get_BabyGrowthTime();
		
		fPer = fGrowthTime / GROWTHTIME;
			
		if ((_int)fPer >= 100)
			fPer = 100.f;

		wstrNum = L"성장중: ";
		wstrNum += to_wstring((_int)fPer) + L"." + to_wstring((_int)((fPer - (_int)fPer) * 10.f)) + L" %";
		Engine::Render_Font(L"Font_ItemGuide", wstrNum, &_vec2(_v2ScreenPos.x, _v2ScreenPos.y + stURI.fFont_Taming_Progress_IntervalPosY[EScrnResolution]), TamingFontColor, 2);
	}
}

void CInfoView_UI::Reset_Line()
{
	m_fOpenDelay = 0.f;
	m_fLineLenth1 = 0.f;
	m_fLineLenth2 = 0.f;
	m_iLineReady = 0;
	m_vLine[0] = { 0.f,0.f };
	m_vLine[1] = { 0.f,0.f };
	m_vLine[2] = { 0.f,0.f };
}

CInfoView_UI * CInfoView_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CInfoView_UI*	pInstance = new CInfoView_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CInfoView_UI::Clone(void * _pArg)
{
	CInfoView_UI*	pClone = new CInfoView_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CInfoView_UI::Free()
{
	if(m_pEmptyDinoTexCom)
		Safe_Release(m_pEmptyDinoTexCom);
	COrthoUI::Free();
}