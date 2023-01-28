#include "stdafx.h"
#include "..\Headers\GuideFont_UI.h"

#include "Export_Function.h"
#include "Unit.h"
//퀵슬롯과 Left Inven이 템 습득하면 이 친구 함수 호출해서 폰트 띄우는 형식으로 가자

////////////////////////////////////////////////Left
#define LEFTTEXTTIME 4.0
#define LEFTNAMETEXTCOLOR {0.8f,0.8f,1.f,1.f}
#define LEFTAMOUNTTEXTCOLOR {0.8f,0.8f,1.f,1.f}
#define LEFTADDTRANSTEXTCOLOR {0.5f,0.5f,1.f,1.f}
#define LEFTREMOVETRANSTEXTCOLOR {1.f,0.5f,0.5f,1.f}
//#define LEFTTEXTTANSPOSX 14.f
//#define LEFTTEXTAMOUNTINTERVALPOSX 30.f
//#define LEFTTEXTNAMETAPPOSX 26.f
#define LEFTTEXTNAMEINTERVALPOSX 0.f //왜 있는지 의문
//#define LEFTTEXTPOSY 290.f
//#define LEFTTEXTTAPY 18.f
//////////////////////////////////////////////Top
#define TOPTEXTTIME 2.0
#define TOPTEXTGREENCOLOR {0.2f,0.9f,0.2f, 1.f}
#define TOPTEXTREDCOLOR {0.8f,0.2f,0.2f,1.f}
//#define TOPTEXTPOSY 4.f
//#define TOPTEXTGREENTAPY 22.f
//#define TOPTEXTREDTAPY 18.f
///////////////////////////////////////////////////
#define NegativeStatusStartTexNum 1
#define NomalStatusBackGroundTexNum 7
#define NegativeStatusBackGroundTexNum 8
#define StatusBackGroundTexAlpha 0.5f
#define StatusTexAlpha 0.7f
USING(Engine)

CGuideFont_UI::CGuideFont_UI(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice)
{
}

CGuideFont_UI::CGuideFont_UI(const CGuideFont_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CGuideFont_UI::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CGuideFont_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(_pArg);

	Add_Component();
	Add_Message();
	//LEFTGUIDEFONT_INFO tTest;
	//tTest.tchRenderText = L"테스트 텍스트 출력 입니다";
	//tTest.dTime = 20.0;
	//tTest.v2Pos = { 20.f,300.f };
	//tTest.tColor = {1.f,1.f,1.f,1.f};
	//m_listLeftGuideFont.emplace_back(tTest);

	///////////텟트
	//Add_StatusIcon(Green_GoTaming);
	//Add_StatusIcon(Red_LessHunger);
	//Add_StatusIcon(Red_LessStamina);
	//Add_StatusIcon(Red_OverWeight);
	//Add_StatusIcon(Red_LessHealth);
	//Add_StatusIcon(Red_Sick_AttRock);
	return S_OK;
}

_int CGuideFont_UI::Update_GameObject(const _double & _dTimeDelta)
{
	CGameObject::Update_GameObject(_dTimeDelta);
	LeftGuideFunc(_dTimeDelta);
	TopGuideFunc(_dTimeDelta);
	return S_OK;
}

_int CGuideFont_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	CGameObject::LateUpdate_GameObject(_dTimeDelta);

	Sort_Status_IconPos(_dTimeDelta);
	Engine::Add_RenderGroup(RENDER_ORTHOUI, this);

	return _int();
}

void CGuideFont_UI::Render_GameObject(_mat _matOrtho)
{
	if (NULL == m_listLeftGuideFont.size()&& NULL == m_listTopGuideFont.size())
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();


	_uint iPassMax = 3;
	pEffect->Begin(&iPassMax, NULL);

	///////////////////////////////////////////////////////////////
	for (auto iter : m_listStatusIconInfo)
	{
		pEffect->CommitChanges();
		_vec2 v2AddBackGroundSize = { stURI.fStatus_BackGround_Icon_Add_SizeXY[EScrnResolution],stURI.fStatus_BackGround_Icon_Add_SizeXY[EScrnResolution] };
		if(iter.iTexNum < NegativeStatusStartTexNum)
			SetUp_ConstantTable(pEffect, _matOrtho, iter.v2NowPos, NomalStatusBackGroundTexNum, StatusBackGroundTexAlpha, v2AddBackGroundSize);
		else
			SetUp_ConstantTable(pEffect, _matOrtho, iter.v2NowPos, NegativeStatusBackGroundTexNum, StatusBackGroundTexAlpha, v2AddBackGroundSize);
		pEffect->BeginPass(2);    ////
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
		/////////////////////////////////////////////////////////////////
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, iter.v2NowPos, iter.iTexNum, StatusTexAlpha);
		pEffect->BeginPass(2);    ////
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
	}
	//////////////////////////////////////////////////////////

	pEffect->End();
	Safe_Release(pEffect);

	Render_LeftFont(_matOrtho);
	////////////////////////////////Top Font Pos CommitChange
	//_mat matWorld;
	//D3DXMatrixIdentity(&matWorld);
	//matWorld._11 = 1.f;
	//matWorld._22 = 1.f;
	//matWorld._33 = FTRUE;

	//matWorld._41 = 400.f - WINCX * FHALF;
	//matWorld._42 = -20.f + WINCY * FHALF;
	//matWorld._43 = FTRUE;

	//pEffect->SetMatrix("g_matWorld", &matWorld);
	//pEffect->CommitChanges();
	//////////////////////////////////////////////////////////

	Render_TopFont(_matOrtho);
}

void CGuideFont_UI::Render_LeftFont(_mat _matOrtho)
{
	Engine::Render_Font(L"Font_LeftGuide",m_listLeftGuideFont);
}

void CGuideFont_UI::Render_TopFont(_mat _matOrtho)
{
	Engine::Render_Font(L"Font_TopGreenGuide", L"Font_TopRedGuide", m_listTopGuideFont, EGUIDEMESSAGE_Red);
}

void CGuideFont_UI::LeftGuideFunc(const _double & _dTimeDelta)
{
	if (NULL == m_listLeftGuideFont.size())
		return;

	LeftGuide_RenderTime_Controller(_dTimeDelta);
}

void CGuideFont_UI::TopGuideFunc(const _double & _dTimeDelta)
{
	if (NULL == m_listTopGuideFont.size())
		return;

	TopGuide_RenderTime_Controller(_dTimeDelta);
}

void CGuideFont_UI::LeftGuide_RenderTime_Controller(const _double & _dTimeDelta)
{	
	list<LEFTGUIDEFONT_INFO> ::iterator iter = m_listLeftGuideFont.begin();
	for (; iter != m_listLeftGuideFont.end();)
	{
		(*iter).dTime -= _dTimeDelta;
		if (0.0 >= (*iter).dTime)
		{
			Safe_Delete_Array((*iter).tchAmountText);
			//Safe_Delete_Array((*iter).tchNameText);
			Safe_Delete_Array((*iter).tchTransText);
			iter = m_listLeftGuideFont.erase(iter);
			Combine_LeftGuideFont();
			FixPos_LeftGuideFont();
		}
		else
			++iter;
	}
}

void CGuideFont_UI::TopGuide_RenderTime_Controller(const _double & _dTimeDelta)
{
	list<TOPGUIDEFONT_INFO> ::iterator iter = m_listTopGuideFont.begin();
	for (; iter != m_listTopGuideFont.end();)
	{
		(*iter).dTime -= _dTimeDelta;
		if (0.0 >= (*iter).dTime)
		{
			Delete_StatusIcon((EGUIDEMESSAGE)iter->eMessageEnum);
			iter = m_listTopGuideFont.erase(iter);
		}
		else
			++iter;
	}
}

CGuideFont_UI * CGuideFont_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CGuideFont_UI*	pInstance = new CGuideFont_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CGuideFont_UI::Clone(void * _pArg/* = nullptr*/)
{
	CGuideFont_UI*	pClone = new CGuideFont_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

HRESULT CGuideFont_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Status_Icon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Pubilc_UI"));//오타 실화냐 퍼블릭...퍼빌릌...
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Com_VIBuffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

void CGuideFont_UI::Add_Message()
{
	m_vecMessage.resize(EGUIDEMESSAGE_END);

	//////////////////////////////메세지 추가하는 곳, 헤더쪽 이넘도 추가해야함
	m_vecMessage[Green_StatUp] = L"인벤토리에서 능력치를 올려 레벨업을 할 수 있습니다!";
	m_vecMessage[Green_GoTaming] = L"기절시킨 공룡이 있습니다. 음식을 주어 조련시킬 수 있습니다.";
	m_vecMessage[Green_TamingSuccess] = L"조련에 성공하였습니다!";

	m_vecMessage[Red_Cold] = L"추위를 느낍니다. 따뜻한 곳을 찾으세요!";
	m_vecMessage[Red_LessHunger] = L"배고픔을 느낍니다. 음식을 찾으세요!";
	m_vecMessage[Red_LessStamina] = L"지쳤습니다. 휴식을 취하세요!";
	m_vecMessage[Red_OverWeight] = L"너무 무겁습니다. 몸이 둔해집니다!";
	m_vecMessage[Red_LessHealth] = L"체력이 적습니다. 포만감을 채워 체력을 회복하세요!";
	m_vecMessage[Red_Sick_AttRock] = L"도구를 사용하면 피해 없이 채집할 수 있습니다!";
}

HRESULT CGuideFont_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho ,_vec2 _v2Pos , _int _iTexNum , _float _fAlpha, _vec2 _AddSize)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = stURI.fStatus_IconSizeXY[EScrnResolution] + _AddSize.x;
	matWorld._22 = stURI.fStatus_IconSizeXY[EScrnResolution] + _AddSize.y;
	matWorld._33 = FTRUE;

	matWorld._41 = _v2Pos.x - WINCX * FHALF;
	matWorld._42 = -_v2Pos.y + WINCY * FHALF;
	matWorld._43 = FTRUE;

	m_pTextureCom->Set_Texture(_pEffect,"g_Bg_Tex", _iTexNum);
	_pEffect->SetFloat("g_Alpha", _fAlpha);
	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);
	return S_OK;
}

void CGuideFont_UI::Add_StatusIcon(EGUIDEMESSAGE _eMessage)
{
	_int iTexNum = Switching_Messege_To_Int(_eMessage);
	SII stTemp;
	stTemp.iTexNum = iTexNum;

	_vec2 v2Pos = {};
	v2Pos.x = stURI.fStatus_IconPosX[EScrnResolution];
	v2Pos.y = stURI.fStatus_IconPosY[EScrnResolution];

	iTexNum = (_int)m_listStatusIconInfo.size(); //재활용 , 갯수임 
	stTemp.v2NowPos.x = v2Pos.x - ((iTexNum % 3)* stURI.fStatus_IconIntervalPosX[EScrnResolution]);
	stTemp.v2NowPos.y = v2Pos.y - ((iTexNum / 3)* stURI.fStatus_IconIntervalPosY[EScrnResolution]);

	m_listStatusIconInfo.emplace_back(stTemp);
}

void CGuideFont_UI::Delete_StatusIcon(EGUIDEMESSAGE _eMessage)
{
	list<SII>::iterator iter = m_listStatusIconInfo.begin();
	_int iDeleteNum = Switching_Messege_To_Int(_eMessage);
	if (-1 == iDeleteNum)
		return;

	for (; iter != m_listStatusIconInfo.end();)
	{
		if (iDeleteNum == iter->iTexNum)
		{
			iter = m_listStatusIconInfo.erase(iter);
			return;
		}
		else
			++iter;
	}
	return;
}

_int CGuideFont_UI::Switching_Messege_To_Int(EGUIDEMESSAGE _eMessage)
{
	switch (_eMessage)
	{
	case Green_GoTaming:
		return 0;
		break;
	case Red_LessHunger:
		return 1;
		break;
	case Red_LessStamina:
		return 2;
		break;
	case Red_OverWeight:
		return 3;
		break;
	case Red_LessHealth:
		return 4;
		break;
	case Red_Sick_AttRock:
		return 6;
		break;
	default:
		return -1;
		break;
	}
	return -1;
}

void CGuideFont_UI::Sort_Status_IconPos(const _double & _dTimeDelta)
{
	_vec2 v2Pos = {};
	_vec3 vDir = {0.f,0.f,0.f};
	_int iNum = 0;
	v2Pos.x = stURI.fStatus_IconPosX[EScrnResolution];
	v2Pos.y = stURI.fStatus_IconPosY[EScrnResolution];
	for (auto& iter : m_listStatusIconInfo)
	{
		iter.v2GoalPos.x = v2Pos.x - ((iNum % 3)* stURI.fStatus_IconIntervalPosX[EScrnResolution]);
		iter.v2GoalPos.y = v2Pos.y - ((iNum++ / 3)* stURI.fStatus_IconIntervalPosY[EScrnResolution]);

		vDir.x = iter.v2GoalPos.x - iter.v2NowPos.x;
		vDir.y = iter.v2GoalPos.y - iter.v2NowPos.y;
		D3DXVec3Normalize(&vDir, &vDir);

		iter.v2NowPos.x += vDir.x * (_float)_dTimeDelta * stURI.fStatus_Icon_Speed[EScrnResolution];
		iter.v2NowPos.y += vDir.y * (_float)_dTimeDelta * stURI.fStatus_Icon_Speed[EScrnResolution];
	}
}

void CGuideFont_UI::Free()
{
	list<LEFTGUIDEFONT_INFO> ::iterator iter = m_listLeftGuideFont.begin();
	for (; iter != m_listLeftGuideFont.end();)
	{
		Safe_Delete_Array((*iter).tchAmountText);
		//Safe_Delete_Array((*iter).tchNameText);
		Safe_Delete_Array((*iter).tchTransText);
		iter = m_listLeftGuideFont.erase(iter);
	}
	list<TOPGUIDEFONT_INFO> ::iterator iiter = m_listTopGuideFont.begin();
	for (; iiter != m_listTopGuideFont.end();)
	{
		//delete[](*iiter).tchText; 뉴 할당 안했음.
		iiter = m_listTopGuideFont.erase(iiter);
	}
	vector<TCHAR*>::iterator iiiter = m_vecMessage.begin();
	for (; iiiter != m_vecMessage.end();)
	{
		iiiter = m_vecMessage.erase(iiiter);
	}
	CGameObject::Free();
}

void CGuideFont_UI::Print_LeftGuideFont(ITEMID _eItemID, _int _iAmount, _int _eTransCheck)
{
	STGuideFont_UIURI stURI;

	for (auto& iter : m_listLeftGuideFont)
	{
		if (_eItemID == iter.iItemID && _eTransCheck == iter.iTransType)
		{
			iter.dTime = LEFTTEXTTIME;
			iter.iAmount += _iAmount;
			Combine_LeftGuideFont();
			FixPos_LeftGuideFont();
			return;
		}
	}

	ITEM_INFO _tItemInfo = Engine::Clone_Item_Prototype(_eItemID, _iAmount);
	LEFTGUIDEFONT_INFO tInput_Font;
	tInput_Font.iItemID = _eItemID;
	tInput_Font.iTransType = _eTransCheck;
	tInput_Font.iAmount = _iAmount;
	tInput_Font.tchNameText = _tItemInfo.tchKoreaName;
	tInput_Font.dTime = LEFTTEXTTIME;

	if(ETRANSTYPE::ADD_Green == _eTransCheck)
		tInput_Font.tTransColor = LEFTADDTRANSTEXTCOLOR;
	else if(ETRANSTYPE::REMOVE_Red== _eTransCheck)
		tInput_Font.tTransColor = LEFTREMOVETRANSTEXTCOLOR;

	tInput_Font.tAmountColor = LEFTAMOUNTTEXTCOLOR;
	tInput_Font.tNameColor = LEFTNAMETEXTCOLOR;
	m_listLeftGuideFont.emplace_back(tInput_Font);

	Combine_LeftGuideFont();
	FixPos_LeftGuideFont();
	return;
}

void CGuideFont_UI::Print_TopGuideFont(EGUIDEMESSAGE _eMessage)
{
	for (auto& iter : m_listTopGuideFont)
	{
		if (_eMessage == iter.eMessageEnum || iter.tchText == m_vecMessage[_eMessage]) //문자열 비교 함수 써야하나?
		{
			iter.dTime = LEFTTEXTTIME;
			Combine_TopGuideFont();
			FixPos_TopGuideFont();
			return;
		}
	}

	TOPGUIDEFONT_INFO tTopGudie;
	tTopGudie.dTime = TOPTEXTTIME;
	tTopGudie.eMessageEnum = _eMessage;
	if (EGUIDEMESSAGE::EGUIDEMESSAGE_Red > _eMessage)
		tTopGudie.tColor = TOPTEXTGREENCOLOR;
	else if (EGUIDEMESSAGE::EGUIDEMESSAGE_Red < _eMessage)
		tTopGudie.tColor = TOPTEXTREDCOLOR;
	m_listTopGuideFont.emplace_back(tTopGudie);
	Combine_TopGuideFont();
	FixPos_TopGuideFont();
	Add_StatusIcon(_eMessage);
}

void CGuideFont_UI::Combine_LeftGuideFont()
{
	for (auto& iter : m_listLeftGuideFont)
	{
		ITEM_INFO _tItemInfo = Engine::Clone_Item_Prototype(iter.iItemID, iter.iAmount);
		/////////////////////////////Trans Text
		wstring wstrText = L"";
		if (ETRANSTYPE::ADD_Green == iter.iTransType)
			wstrText = L"추가";
		else if(ETRANSTYPE::REMOVE_Red == iter.iTransType)
			wstrText = L"제거";
		
		Safe_Delete_Array((iter).tchAmountText);
		Safe_Delete_Array((iter).tchTransText);

		_tchar* ptchar = new _tchar[64];
		wcscpy_s(ptchar, 64, wstrText.c_str());
		iter.tchTransText = ptchar;
		///////////////////////////////Amount
		ptchar = new _tchar[64];
		wstrText = to_wstring(iter.iAmount) + L"x ";
		wcscpy_s(ptchar, 64, wstrText.c_str());
		iter.tchAmountText = ptchar;
		/////////////////////////////Name
		//ptchar = new _tchar[64];
		//wstrText = _tItemInfo.tchKoreaName;
		//wcscpy_s(ptchar, 64, wstrText.c_str());
		iter.tchNameText = _tItemInfo.tchKoreaName;
		//////////////////////////////////////
	}
}

void CGuideFont_UI::Combine_TopGuideFont()
{
	for (auto& iter : m_listTopGuideFont)
		iter.tchText = m_vecMessage[iter.eMessageEnum];
}

void CGuideFont_UI::FixPos_LeftGuideFont()
{
	STGuideFont_UIURI stURI;

	_int iNum = 0;
	for (auto& iter : m_listLeftGuideFont)
	{
		if (ETRANSTYPE::ADD_Green == iter.iTransType)
		{
			_float PosY = stURI.fFont_LeftText_PosY[EScrnResolution] + (iNum*stURI.fFont_LeftText_IntervalPosY[EScrnResolution]);
			iter.v2TransPos = { stURI.fFont_LeftText_PosX[EScrnResolution] ,PosY };
			iter.v2AmountPos = { stURI.fFont_LeftText_PosX[EScrnResolution] + stURI.fFont_LeftText_Amount_IntervalPosX[EScrnResolution] , PosY };
			if (10 > iter.iAmount)
				iter.v2NamePos = { iter.v2AmountPos.x + stURI.fFont_LeftText_Name_IntervalPosX[EScrnResolution] + LEFTTEXTNAMEINTERVALPOSX ,PosY };
			else if (10 <= iter.iAmount && 100 > iter.iAmount)
				iter.v2NamePos = { iter.v2AmountPos.x + stURI.fFont_LeftText_Name_IntervalPosX[EScrnResolution] + (LEFTTEXTNAMEINTERVALPOSX * 2.f) ,PosY };
			else
				iter.v2NamePos = { iter.v2AmountPos.x + stURI.fFont_LeftText_Name_IntervalPosX[EScrnResolution] + (LEFTTEXTNAMEINTERVALPOSX * 3.f) ,PosY };
			++iNum;
		}
	}
	
	for (auto& iter : m_listLeftGuideFont)
	{
		if (ETRANSTYPE::REMOVE_Red == iter.iTransType)
		{
			_float PosY = stURI.fFont_LeftText_PosY[EScrnResolution] + (iNum*stURI.fFont_LeftText_IntervalPosY[EScrnResolution]);
			iter.v2TransPos = { stURI.fFont_LeftText_PosX[EScrnResolution] ,PosY };
			iter.v2AmountPos = { stURI.fFont_LeftText_PosX[EScrnResolution] + stURI.fFont_LeftText_Amount_IntervalPosX[EScrnResolution] , PosY };
			if (10 > iter.iAmount)
				iter.v2NamePos = { iter.v2AmountPos.x + stURI.fFont_LeftText_Name_IntervalPosX[EScrnResolution] + LEFTTEXTNAMEINTERVALPOSX ,PosY };
			else if (10 <= iter.iAmount && 100 > iter.iAmount)
				iter.v2NamePos = { iter.v2AmountPos.x + stURI.fFont_LeftText_Name_IntervalPosX[EScrnResolution] + (LEFTTEXTNAMEINTERVALPOSX * 2.f) ,PosY };
			else
				iter.v2NamePos = { iter.v2AmountPos.x + stURI.fFont_LeftText_Name_IntervalPosX[EScrnResolution] + (LEFTTEXTNAMEINTERVALPOSX * 3.f) ,PosY };
			++iNum;
		}
	}
}

void CGuideFont_UI::FixPos_TopGuideFont()
{
	_float fPosY = stURI.fFont_TopText_PosY[EScrnResolution];
	for (auto& iter : m_listTopGuideFont)
	{
		if (EGUIDEMESSAGE_Red > iter.eMessageEnum) // 그린이다
		{
			iter.v2Pos = { WINCX*FHALF , fPosY };
			fPosY += stURI.fFont_TopText_GreenIntervalPosY[EScrnResolution];
		}
	}
	for (auto& iter : m_listTopGuideFont)
	{
		if (EGUIDEMESSAGE_Red < iter.eMessageEnum) // 레드다
		{
			iter.v2Pos = { WINCX*FHALF , fPosY };
			fPosY += stURI.fFont_TopText_RedPosY[EScrnResolution];
		}
	}
}
