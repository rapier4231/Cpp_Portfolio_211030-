#include "stdafx.h"
#include "..\Headers\MiniMap_UI.h"

#include "Export_Function.h"
#include "Player.h"
#include "SleepingBag.h"

#define MiniMapAlpha 0.8f
USING(Engine)

HRESULT CMiniMap_UI::Set_Screen_UI()
{
	STMiniMap_UIURI stURI;

	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
	return S_OK;
}

CMiniMap_UI::CMiniMap_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CMiniMap_UI::CMiniMap_UI(const CMiniMap_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CMiniMap_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CMiniMap_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = MiniMapAlpha;

	///////////테스트
	//MAP_BED_POS tBedPos;
	//tBedPos.vRealPos = _vec3{ 300.f, 10.f, 2000.f };
	//tBedPos.v2MapPos = Calculate_Bed_Pos(tBedPos.vRealPos);
	//m_mapBedPos.emplace(1, tBedPos);
	return S_OK;
}

_int CMiniMap_UI::Update_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::Update_GameObject(_dTimeDelta);
	//Calculate_Compass_World();
	//Calculate_Player_Pos();
	Select_Bed_Erase();
	MiniMap_Center_Move(_dTimeDelta);

	//if (Engine::KeyDown(KeyCheck_E))
	//	MiniMap_Move(true);
	//if(Engine::KeyDown(KeyCheck_Lbutton))
	//	MiniMap_Move(false);
	return NO_EVENT;
}

_int CMiniMap_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	Engine::Add_RenderGroup(RENDER_ORTHOBACKUI, this);
	return NO_EVENT;
}

void CMiniMap_UI::Render_GameObject(_mat _matOrtho)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 6;

	pEffect->Begin(&iPassMax, NULL);

	//pEffect->CommitChanges();

	//pEffect->BeginPass(3); ///// 백 그라운드

	//m_pBufferCom->Render_VIBuffer();

	//pEffect->EndPass();

	//////////////////////////	
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho,1);
	pEffect->BeginPass(2);    //// 지도
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////
	//////////////////////////	
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, 3);
	pEffect->BeginPass(2);    //// 나침반
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////
	//////////////////////////	
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, 2);
	pEffect->BeginPass(2);    //// 플레이어 위치
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////

	//침대 띄우기
	if (0 != m_mapBedPos.size())
	{
		for (auto& iter : m_mapBedPos)
		{
			pEffect->CommitChanges();
			SetUp_ConstantTable(pEffect, _matOrtho, 4, iter.second.v2MapPos, iter.first);
			pEffect->BeginPass(2);    //// 침대 위치
			m_pBufferCom->Render_VIBuffer();
			pEffect->EndPass();
		}
	}
	//////////////////////////////////////////

	pEffect->End();

	Safe_Release(pEffect);

	//디버깅용
#ifdef _DEBUG
	wstring wstrPos = to_wstring(m_fa);
	Engine::Render_Font(L"Font_Debug_Bold", wstrPos, &_vec2(400.f, 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
#endif
}

HRESULT CMiniMap_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pMiniMapTex_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_MiniMap"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture2", pComponent);

	pComponent = m_pPingTex_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Ping"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture3", pComponent);

	pComponent = m_pCompassTex_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Compass"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture4", pComponent);

	pComponent = m_pBedSelecTex_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_BedSelect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture5", pComponent);

	pComponent = m_pBedUnSelecTex_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_BedUnSelect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture6", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Loading"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CMiniMap_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo, _vec2 _BedMapPos, _int _iBedKey)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	
	if (1 == _bTwo) // 지도
	{
		Calculate_Player_Pos();
		//matWorld = m_matMiniMapWorld;
		matWorld._11 = m_tORTHOUI_INFO.fSizeX;
		matWorld._22 = m_tORTHOUI_INFO.fSizeY;
		matWorld._33 = FTRUE;

		matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;

		m_pMiniMapTex_Com->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_fAlpha", m_tORTHOUI_INFO.fAlpha-0.1f);
	}
	else if (2 == _bTwo) // 플레이어 위치
	{
		matWorld._11 = m_tORTHOUI_INFO.fSizeX*0.1f;
		matWorld._22 = m_tORTHOUI_INFO.fSizeY*0.1f;
		matWorld._33 = FTRUE;

		matWorld._41 = m_v2PlayerPos.x - WINCX * FHALF;
		matWorld._42 = -m_v2PlayerPos.y + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pPingTex_Com->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_fAlpha", m_tORTHOUI_INFO.fAlpha);
	}
	else if (3 == _bTwo) // 나침반
	{
		Calculate_Compass_World();
		matWorld = m_matCompassWorld;
		m_pCompassTex_Com->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_fAlpha", 1.f); 
	}
	else if (4 == _bTwo) //침대
	{
		matWorld._11 = m_tORTHOUI_INFO.fSizeX*0.05f;
		matWorld._22 = m_tORTHOUI_INFO.fSizeY*0.05f;
		matWorld._33 = FTRUE;

		matWorld._41 = _BedMapPos.x - WINCX * FHALF;
		matWorld._42 = -_BedMapPos.y + WINCY * FHALF;
		matWorld._43 = FTRUE;
		if (Now_Mouse_On_BedIcon(_BedMapPos, _vec2{ matWorld._11 , matWorld._22 })&& m_bCenter)
		{
			m_pBedSelecTex_Com->Set_Texture(_pEffect, "g_Bg_Tex");
			if (Engine::KeyDown(KeyCheck_Lbutton))
				/*if(m_bCenter)*/
					Bed_Select(_iBedKey);
		}
		else
			m_pBedUnSelecTex_Com->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_fAlpha", m_tORTHOUI_INFO.fAlpha);
	}
	else
	{
		matWorld._11 = m_tORTHOUI_INFO.fSizeX;
		matWorld._22 = m_tORTHOUI_INFO.fSizeY;
		matWorld._33 = FTRUE;

		matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pMiniMapTex_Com->Set_Texture(_pEffect, "g_Bg2_Tex");
		_pEffect->SetFloat("g_fAlpha", m_tORTHOUI_INFO.fAlpha);
	}


	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);
	return S_OK;
}

void CMiniMap_UI::MiniMap_Center_Move(const _double& _dTimeDelta)
{
	if (2 == m_iCenter)
		return;

	if (1 == m_iCenter)
	{
		_vec3 vMapPos = { m_tORTHOUI_INFO.fPosX , m_tORTHOUI_INFO.fPosY  , 0.f};
		_vec3 vGoalPos = { WINCX*0.5f, WINCY*0.5f,0.f };
		_vec3 vDir = vGoalPos - vMapPos;
		D3DXVec3Normalize(&vDir, &vDir);
		vMapPos += vDir*(_float)_dTimeDelta * WINCX;

		m_tORTHOUI_INFO.fSizeX += (_float)_dTimeDelta * WINCX;
		m_tORTHOUI_INFO.fSizeY += (_float)_dTimeDelta * WINCX;
		m_tORTHOUI_INFO.fPosX = vMapPos.x;
		m_tORTHOUI_INFO.fPosY = vMapPos.y;
		m_tORTHOUI_INFO.fAlpha += (_float)_dTimeDelta;

		if (WINCY*0.8f <= m_tORTHOUI_INFO.fSizeX)
			m_tORTHOUI_INFO.fSizeX = WINCY*0.8f;
		if (WINCY*0.8f <= m_tORTHOUI_INFO.fSizeY)
			m_tORTHOUI_INFO.fSizeY = WINCY*0.8f;
		if (WINCX*0.5f <= m_tORTHOUI_INFO.fPosX)
			m_tORTHOUI_INFO.fPosX = WINCX*0.5f;
		if (WINCY*0.5f >= m_tORTHOUI_INFO.fPosY)
			m_tORTHOUI_INFO.fPosY = WINCY*0.5f;

		if (0.9f <= m_tORTHOUI_INFO.fAlpha)
			m_tORTHOUI_INFO.fAlpha = 0.9f;

		if ((WINCX*0.5f <= m_tORTHOUI_INFO.fSizeX) && (WINCX*0.5f <= m_tORTHOUI_INFO.fSizeY) && (WINCX*0.5f <= m_tORTHOUI_INFO.fPosX)
			&& (WINCY*0.5f <= m_tORTHOUI_INFO.fPosY) && (0.9f <= m_tORTHOUI_INFO.fAlpha))
		{
			m_iCenter = 2;
			m_bCenter = true;
		}
	}
	else if(0 == m_iCenter)
	{
		_vec3 vMapPos = { m_tORTHOUI_INFO.fPosX , m_tORTHOUI_INFO.fPosY  , 0.f };
		_vec3 vGoalPos = { stURI.fPosX[EScrnResolution], stURI.fPosY[EScrnResolution],0.f };
		_vec3 vDir = vGoalPos - vMapPos;
		D3DXVec3Normalize(&vDir, &vDir);
		vMapPos += vDir*(_float)_dTimeDelta * WINCX;

		m_tORTHOUI_INFO.fSizeX -= (_float)_dTimeDelta * WINCX;
		m_tORTHOUI_INFO.fSizeY -= (_float)_dTimeDelta * WINCX;
		m_tORTHOUI_INFO.fPosX = vMapPos.x;
		m_tORTHOUI_INFO.fPosY = vMapPos.y;
		m_tORTHOUI_INFO.fAlpha -= (_float)_dTimeDelta;

		if (stURI.fSizeXY[EScrnResolution] >= m_tORTHOUI_INFO.fSizeX)
			m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
		if (stURI.fSizeXY[EScrnResolution] >= m_tORTHOUI_INFO.fSizeY)
			m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
		if (stURI.fPosX[EScrnResolution] >= m_tORTHOUI_INFO.fPosX)
			m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
		if (stURI.fPosY[EScrnResolution] <= m_tORTHOUI_INFO.fPosY)
			m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];

		if (0.7f >= m_tORTHOUI_INFO.fAlpha)
			m_tORTHOUI_INFO.fAlpha = 0.7f;

		if((stURI.fSizeXY[EScrnResolution] >= m_tORTHOUI_INFO.fSizeX) && (stURI.fSizeXY[EScrnResolution] >= m_tORTHOUI_INFO.fSizeY)&& (stURI.fPosX[EScrnResolution] >= m_tORTHOUI_INFO.fPosX)&&
			(stURI.fPosY[EScrnResolution] >= m_tORTHOUI_INFO.fPosY)&& (0.6f >= m_tORTHOUI_INFO.fAlpha))
		{
			m_iCenter = 2;
			m_bCenter = false;
		}
	}

	//2가 아니라 리턴하지 않으면 침대 위치 계속 갱신
	for (auto& iter : m_mapBedPos)
	{
		iter.second.v2MapPos = Calculate_Bed_Pos(iter.second.vRealPos);
	}
}

void CMiniMap_UI::MiniMap_Move(_bool _bCenter)
{
	if (_bCenter)
		m_iCenter = 1;
	else
		m_iCenter = 0;
	return;
}

_int CMiniMap_UI::Bed_Create(_vec3 _vBedWorldPos)
{
	MAP_BED_POS tBedPos;
	tBedPos.vRealPos = _vBedWorldPos;
	tBedPos.v2MapPos = Calculate_Bed_Pos(tBedPos.vRealPos);
	m_mapBedPos.emplace(m_iBedNum, tBedPos);

	return m_iBedNum++;
}

_bool CMiniMap_UI::Bed_Delete(_int _iBedKeyValue)
{
	if (m_mapBedPos.end() == m_mapBedPos.find(_iBedKeyValue))
		return false;
	m_mapBedPos.erase(_iBedKeyValue);
	return true;
}

size_t CMiniMap_UI::Get_BedCount()
{
	return m_mapBedPos.size();
}

void CMiniMap_UI::Bed_Select(_int _iBedKey)
{
	//전부 테스트용
	auto& iter = m_mapBedPos.find(_iBedKey);
	if (iter == m_mapBedPos.end())
		return;
	
	m_iEraseBedNum = _iBedKey; //이거 값 넣으면 지워짐 아이콘

	_vec3 vPos = iter->second.vRealPos;
	vPos.y += 1.f;	//땅속에 안 파묻히게 하기 위한 보정값 <- 리스폰시 누워서 시작할 경우 수정. (y회전도 수정해야)
	static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0))->Player_Respawn(vPos);	//해당 위치에 플레이어 스폰
	MiniMap_Move(false);
	//Bed_Delete(_iBedKey);	//해당 키값 삭제
	//m_iEraseBedNum = _iBedKey;	//맵에서 삭제

	//키값에 해당하는 침대 찾아서 소멸처리
	list<CGameObject*>* pListBed = Engine::Get_LayerList(L"Layer_Structure");
	for (auto& pObj : *pListBed)
	{
		CSleepingBag* pBag = static_cast<CSleepingBag*>(pObj);
		if (_iBedKey == pBag->Get_BedID())
		{
			pBag->TurnOff_Structure();
			return;
		}
	}

	//키값에 해당하는 침대 찾지 못했다면 에러
	ERR_MSG(L" CMiniMap_UI::Bed_Select() : No Matching BedKey");
}

void CMiniMap_UI::Select_Bed_Erase()
{
	//이것도 테스트 용
	if (-1 != m_iEraseBedNum)
	{
		m_mapBedPos.erase(m_iEraseBedNum);
		m_iEraseBedNum = -1;
	}
}

void CMiniMap_UI::Calculate_Compass_World()
{
	STMiniMap_UIURI stURI;

	_mat matPlayerWorld;
	matPlayerWorld = static_cast<CTransform*>(Engine::Get_GameObject(L"Layer_Player", 0)->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_WorldMatrix();
	
	_vec2 vNorth;
	vNorth = { 0.f, 1.f };
	_vec3 vPlayerLook;
	vPlayerLook = { matPlayerWorld._31,0.f,matPlayerWorld._33 };
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	_float fDot = vNorth.x*vPlayerLook.x + vNorth.y*vPlayerLook.z;
	_float fDet = vNorth.x*vPlayerLook.z - vNorth.y*vPlayerLook.x;
	_float fAngle = atan2(fDet, fDot);
	if (0 > fAngle)
		fAngle += D3DXToRadian(360.f);

#ifdef _DEBUG
	m_fa = fAngle *(180.f / 3.141592f);
#endif
	D3DXMatrixIdentity(&m_matCompassWorld);

	m_matCompassWorld._11 = stURI.fCompassSizeXY[EScrnResolution]; //스케일
	m_matCompassWorld._22 = stURI.fCompassSizeXY[EScrnResolution];
	m_matCompassWorld._33 = FTRUE;
	 
	_mat matRotZ;
	D3DXMatrixRotationZ(&matRotZ, fAngle); //스자이공부 중 자 말곤 없으니 그냥 얘가 월드, 뷰가 항등이라 Z축 회전
	m_matCompassWorld *= matRotZ;
	D3DXMatrixRotationZ(&matRotZ,D3DXToRadian(180.f));
	m_matCompassWorld *= matRotZ;

	m_matCompassWorld._41 = m_v2PlayerPos.x - WINCX * FHALF;
	m_matCompassWorld._42 = -(m_v2PlayerPos.y)+ WINCY * FHALF;
	
}

void CMiniMap_UI::Calculate_Player_Pos()
{
	_vec2 v2MapPos = { m_tORTHOUI_INFO.fPosX ,m_tORTHOUI_INFO.fPosY };
	//_vec2 v2MapSize = { (m_tORTHOUI_INFO.fSizeY*0.75f)*(332.f/ 500.f) ,m_tORTHOUI_INFO.fSizeY*0.75f }; //그림 비율 맞춰주기
	_vec2 v2MapSize = { m_tORTHOUI_INFO.fSizeY ,m_tORTHOUI_INFO.fSizeY}; 

	_vec2 v2RealMapSize = { 1260.f ,4350.f };
	_vec2 v2RealMapStaryPos = { -700.f ,-110.f };
	_mat matPlayerWorld;
	matPlayerWorld = static_cast<CTransform*>(Engine::Get_GameObject(L"Layer_Player", 0)->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_WorldMatrix();
	_vec2 v2PlayerPos = { matPlayerWorld._41,matPlayerWorld._43 }; //현 플레이어 포스

	_vec2 v2FixPlayerPos = v2PlayerPos - v2RealMapStaryPos; //리얼 맵 사이즈에 0,0부터 시작한 걸로 맞춰줌
															//스크린상 맵 사이즈랑 0,0이 어딘지 어케 알지...맵도 딱 안 맞아서 후
	//////////////////사진 총 픽셀이 332 x 500
	//////////////////y 최 상단 21 최 남단 471
	//////////////////x 좌측 끝 23 우측 끝 285

	//////////////////y 상단 빈공간 비율 4.2%
	//////////////////y 하단 빈공간 비율 5.8%
	//////////////////x 좌측 빈공간 비율 6.9%
	//////////////////x 우측 빈공간 비율 14.1%

	////////////////////픽셀 계산 합시다.....
	////////////////////y 상단 빈공간 픽셀
	//////////////////_float fTopEmpty = (v2MapSize.y / 100.f) * 4.2f;
	////////////////////y 하단 빈공간 픽셀
	//////////////////_float fBottomEmpty = (v2MapSize.y / 100.f) * 5.8f;
	////////////////////x 좌측 빈공간 픽셀
	//////////////////_float fLeftEmpty = (v2MapSize.x / 100.f) * 6.9f;
	////////////////////x 우측 빈공간 픽셀
	//////////////////_float fRightEmpty = (v2MapSize.x / 100.f) * 14.1f;

	// 사진 총 픽셀이 768 x 768
	// y 최 상단 69 최 남단 637
	// x 좌측 끝 232 우측 끝 537
	
	// y 상단 빈공간 비율 9.f%
	// y 하단 빈공간 비율 17.05%
	// x 좌측 빈공간 비율 30.2%
	// x 우측 빈공간 비율 30.7%

	//픽셀 계산 합시다.....
	//y 상단 빈공간 픽셀
	_float fTopEmpty = (v2MapSize.y / 100.f) * 9.f;
	//y 하단 빈공간 픽셀
	_float fBottomEmpty = (v2MapSize.y / 100.f) * 17.05f;
	//x 좌측 빈공간 픽셀
	_float fLeftEmpty = (v2MapSize.x / 100.f) * 30.2f;
	//x 우측 빈공간 픽셀
	_float fRightEmpty = (v2MapSize.x / 100.f) * 30.7f;

	//맵 스크린 좌표상 시작 픽셀
	_vec2 v2FixMapOnePos;
	v2FixMapOnePos.x = v2MapPos.x - (v2MapSize.x * 0.5f);
	v2FixMapOnePos.y = v2MapPos.y + (v2MapSize.y * 0.5f); //Y는 더해야댐!

	v2FixMapOnePos.x += fLeftEmpty;
	v2FixMapOnePos.y -= fBottomEmpty; //Y는 빼야댐!

	//실제 지도 사이즈
	v2MapSize.x = v2MapSize.x - fLeftEmpty - fRightEmpty;
	v2MapSize.y = v2MapSize.y - fTopEmpty - fBottomEmpty;
	//실제 맵 크기와 스크린 맵 상의 비율 (x , y 따로)
	_vec2 v2Scale = { (v2MapSize.x/v2RealMapSize.x) ,(v2MapSize.y / v2RealMapSize.y)};

	//플레이어 픽스 포스에서 배율 곱해줘서 맵 스크린 좌표상 시작 픽셀에다 더해주면 거기가 플레이어 위치!
	m_v2PlayerPos.x = v2FixMapOnePos.x + (v2FixPlayerPos.x * v2Scale.x);
	m_v2PlayerPos.y = v2FixMapOnePos.y - (v2FixPlayerPos.y * v2Scale.y); // Y는 빼줘야징

	////하.. 실제 맵이 8.5도 돌아가 있다.... 
	//플레이어 포스를 원점 (m_tORTHOUI_INFO.fPosX , m_tORTHOUI_INFO.fPosY 기준으로) 170도 돌려주자
	m_v2PlayerPos.x -= m_tORTHOUI_INFO.fPosX;
	m_v2PlayerPos.y -= m_tORTHOUI_INFO.fPosY;

	_mat matRotZ;
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-8.5f));			//자전
	_vec3 vRotZPlayer = { m_v2PlayerPos.x , m_v2PlayerPos.y ,0.f  };
	D3DXVec3TransformNormal(&vRotZPlayer, &vRotZPlayer, &matRotZ);
	m_v2PlayerPos.x = vRotZPlayer.x + m_tORTHOUI_INFO.fPosX;
	m_v2PlayerPos.y = vRotZPlayer.y + m_tORTHOUI_INFO.fPosY;

	////이미지도 돌려주자..

	//D3DXMatrixIdentity(&m_matMiniMapWorld);

	//m_matMiniMapWorld._11 = m_tORTHOUI_INFO.fSizeX*0.75f;
	//m_matMiniMapWorld._22 = m_tORTHOUI_INFO.fSizeY*0.75f;
	//m_matMiniMapWorld._33 = FTRUE;                             //스케일

	//m_matMiniMapWorld *= matRotZ;

	//m_matMiniMapWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
	//m_matMiniMapWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF; //이동
}

_vec2 CMiniMap_UI::Calculate_Bed_Pos(_vec3 _vRealBedPos)
{
	_vec3 vBedMapPos;

	_vec2 v2MapPos = { m_tORTHOUI_INFO.fPosX ,m_tORTHOUI_INFO.fPosY };
	_vec2 v2MapSize = { m_tORTHOUI_INFO.fSizeY ,m_tORTHOUI_INFO.fSizeY }; //그림 비율 맞춰주기

	_vec2 v2RealMapSize = { 1260.f ,4350.f };
	_vec2 v2RealMapStaryPos = { -700.f ,-110.f };
	_vec2 v2BedPos = { _vRealBedPos.x,_vRealBedPos.z }; //현 침대 포스

	_vec2 v2FixBedPos = v2BedPos - v2RealMapStaryPos; //리얼 맵 사이즈에 0,0부터 시작한 걸로 맞춰줌
															//픽셀 계산 합시다.....
															//y 상단 빈공간 픽셀
	_float fTopEmpty = (v2MapSize.y / 100.f) * 9.f;
	//y 하단 빈공간 픽셀
	_float fBottomEmpty = (v2MapSize.y / 100.f) * 17.05f;
	//x 좌측 빈공간 픽셀
	_float fLeftEmpty = (v2MapSize.x / 100.f) * 30.2f;
	//x 우측 빈공간 픽셀
	_float fRightEmpty = (v2MapSize.x / 100.f) * 30.7f;

	//맵 스크린 좌표상 시작 픽셀
	_vec2 v2FixMapOnePos;
	v2FixMapOnePos.x = v2MapPos.x - (v2MapSize.x * 0.5f);
	v2FixMapOnePos.y = v2MapPos.y + (v2MapSize.y * 0.5f); //Y는 더해야댐!

	v2FixMapOnePos.x += fLeftEmpty;
	v2FixMapOnePos.y -= fBottomEmpty; //Y는 빼야댐!

									  //실제 지도 사이즈
	v2MapSize.x = v2MapSize.x - fLeftEmpty - fRightEmpty;
	v2MapSize.y = v2MapSize.y - fTopEmpty - fBottomEmpty;
	//실제 맵 크기와 스크린 맵 상의 비율 (x , y 따로)
	_vec2 v2Scale = { (v2MapSize.x / v2RealMapSize.x) ,(v2MapSize.y / v2RealMapSize.y) };

	//플레이어 픽스 포스에서 배율 곱해줘서 맵 스크린 좌표상 시작 픽셀에다 더해주면 거기가 플레이어 위치!
	vBedMapPos.x = v2FixMapOnePos.x + (v2FixBedPos.x * v2Scale.x);
	vBedMapPos.y = v2FixMapOnePos.y - (v2FixBedPos.y * v2Scale.y); // Y는 빼줘야징

																		 //하.. 실제 맵이 -8.5도 돌아가 있다.... 
																		 //플레이어 포스를 원점 (m_tORTHOUI_INFO.fPosX , m_tORTHOUI_INFO.fPosY 기준으로) 170도 돌려주자
	vBedMapPos.x -= m_tORTHOUI_INFO.fPosX;
	vBedMapPos.y -= m_tORTHOUI_INFO.fPosY;

	_mat matRotZ;
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-8.5f));			//자전
	_vec3 vRotZBed = { vBedMapPos.x , vBedMapPos.y ,0.f };
	D3DXVec3TransformNormal(&vRotZBed, &vRotZBed, &matRotZ);
	vBedMapPos.x = vRotZBed.x + m_tORTHOUI_INFO.fPosX;
	vBedMapPos.y = vRotZBed.y + m_tORTHOUI_INFO.fPosY;

	return _vec2{ vBedMapPos.x,vBedMapPos.y};
}

_bool CMiniMap_UI::Now_Mouse_On_BedIcon(_vec2 _vRealBedPos, _vec2 _vRealSize)
{
	Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
	_float MouseX = (_float)m_tORTHOUI_INFO.tMousePos.x;
	_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y;

	_float fHalfX = _vRealSize.x * FHALF;
	_float fHalfY = _vRealSize.y * FHALF;
	if (MouseX >= _vRealBedPos.x - fHalfX
		&& MouseX <= _vRealBedPos.x + fHalfX
		&& MouseY >= _vRealBedPos.y - fHalfY
		&& MouseY <= _vRealBedPos.y + fHalfY)
		return true;
	else
		return false;
}

CMiniMap_UI * CMiniMap_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CMiniMap_UI*	pInstance = new CMiniMap_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CMiniMap_UI::Clone(void * _pArg)
{
	CMiniMap_UI*	pClone = new CMiniMap_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CMiniMap_UI::Free()
{
	COrthoUI::Free();
}