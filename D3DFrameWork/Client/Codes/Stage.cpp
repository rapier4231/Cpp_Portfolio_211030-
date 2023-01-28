#include "stdafx.h"
#include "..\Headers\Stage.h"

#include "Export_Function.h"

//임시//////////////////////////////////////////////////////////////////////////
#include "Tile.h"
//////////////////////////////////////////////////////////////////////////

#include "CameraMrg.h"

#include "BackGround.h"
#include "Player.h"
#include "InvenItem.h"
#include "Left_Inventory.h"
#include "Sound_Mgr.h"
#include "OrthoUI.h"
#include "InvenIcon.h"
#include "Border.h"
#include "InvenTap.h"
#include "Center_UI.h"
#include "QuickSlot_UI.h"
#include "TamingName_UI.h"
#include "PhysX_Mgr.h"
#include "Back_StructureStat_UI.h"

stSC*	CStage::m_pCount = nullptr;

CStage::CStage(LPDIRECT3DDEVICE9 _pDevice)
	: CScene(_pDevice)
{
}

HRESULT CStage::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);

	// 타이틀 변경
	::SetWindowText(g_hWnd, L"Stage");
	
	// 조명 세팅
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	// 지형
	FAILED_CHECK_RETURN(Ready_Terrain_Layer(L"Layer_Terrain"), E_FAIL);
	
	// 카메라
	FAILED_CHECK_RETURN(Ready_Camera_Layer(L"Layer_Camera"), E_FAIL);
	
	// 플레이어
	FAILED_CHECK_RETURN_AND_COUNT_Stage(Ready_Player_Layer(L"Layer_Player"), E_FAIL);
	
	// 무기	
	FAILED_CHECK_RETURN(Ready_Empty_Layer(L"Layer_Weapon_Hatchet"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Empty_Layer(L"Layer_Weapon_StonePick"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Empty_Layer(L"Layer_Weapon_WoodClub"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Empty_Layer(L"Layer_Weapon_Spear"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Empty_Layer(L"Layer_Weapon_Bola"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Empty_Layer(L"Layer_Weapon_Bow"), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT_Stage(Ready_Empty_Layer(L"Layer_Bullet"), E_FAIL);
	
	// 공룡
	FAILED_CHECK_RETURN(Ready_Dino_Layer(L"Layer_Dino"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Empty_Layer(L"Layer_Dino_Tamed"), E_FAIL);

	// 플랫폼 안장
	FAILED_CHECK_RETURN(Ready_Empty_Layer(L"Layer_PlatformSaddle_Brachio"), E_FAIL);

	// 알
	FAILED_CHECK_RETURN(Ready_Empty_Layer(L"Layer_Egg"), E_FAIL);

	// 구조물
	FAILED_CHECK_RETURN(Ready_Empty_Layer(L"Layer_Structure"), E_FAIL);					
	FAILED_CHECK_RETURN_AND_COUNT_Stage(Ready_Empty_Layer(L"Layer_PreViewStructure"), E_FAIL);
	
	// 기타 오브젝트
	FAILED_CHECK_RETURN(Ready_Harvest_Tree_Layer(L"Layer_Harvest_Tree"), E_FAIL);		
	FAILED_CHECK_RETURN(Ready_Harvest_Bush_Layer(L"Layer_Harvest_Bush"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Harvest_Rock_Layer(L"Layer_Harvest_Rock"), E_FAIL);		
	FAILED_CHECK_RETURN(Ready_Harvest_Pebble_Layer(L"Layer_Harvest_Pebble"), E_FAIL);	
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Empty_Layer(L"Layer_Obstacle"), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT_Stage(Ready_Empty_Layer(L"Layer_Building"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_BallHoop_Layer(L"Layer_BasketballHoop"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Ball_Layer(L"Layer_Ball"), E_FAIL);

	// UI
	FAILED_CHECK_RETURN_AND_COUNT_Stage(Ready_UI(), E_FAIL);

	// Effect
	FAILED_CHECK_RETURN(Ready_Effect_Layer(L"Layer_Effect"), E_FAIL);

	///////////////////////////////////UI 간이 디버깅 확인용 주석
	// 스타트구역 오브젝트 불러오기
	//LoadFile_ObjectList(L"../Resources/Data/Object/Harvestable_test.Spawn");	
	LoadFile_ObjectList(L"../Resources/Data/Object/Harvestable.Spawn");	
	////////////////////////////////////////////////////////////

	// Late Ready
	FAILED_CHECK_RETURN(Late_Ready_Scene(), E_FAIL);

	return S_OK;
}

HRESULT CStage::Late_Ready_Scene(void)
{
	for (auto& iter : m_mapLayer)
	{
		list<CGameObject*>* pListObj = iter.second->Get_ListGameObject();

		for (auto& pObj : *pListObj)
		{
			pObj->Late_Ready_GameObject();
		}
	}

	return S_OK;
}

_int CStage::Update_Scene(const _double & _dTimeDelta)
{
	_uint iResult = CScene::Update_Scene(_dTimeDelta);

	CPhysX_Mgr::GetInstance()->Update_PhysX_Mgr_Simulate(_dTimeDelta);

	if (Engine::Get_DIKeyDown(DIK_NUMPAD9))
		FAILED_CHECK_RETURN(Ready_Test(L"Layer_Effect"), E_FAIL);

	//auto& pLayerPlayer = m_mapLayer.find(L"Layer_PlayerColl");
	//auto& pLayerDino = m_mapLayer.find(L"Layer_DinoColl");

	//CColl_Mgr::GetInstance()->Coll_Check(pLayerPlayer->second, pLayerDino->second);

	// Shadow On/Off
	if (GetAsyncKeyState(VK_CAPITAL) & 0x8000 && !m_bKeyDown)
	{
		m_bShadow = !m_bShadow;
		m_bKeyDown = true;
	}

	else if (!(GetAsyncKeyState(VK_CAPITAL) & 0x8000) && m_bKeyDown)
		m_bKeyDown = false;

	return iResult;
}

void CStage::Render_Scene()
{
	//CScene::Render_Scene();
}

HRESULT CStage::Ready_Environment_Layer(const wstring & _strLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;
	
	/*/// 테스트 오브젝트 추가
	OBJINFO tInfo;
	ZeroMemory(&tInfo, sizeof(OBJINFO));
	tInfo.vScale = _vec3(20.f, 2.f, 20.f);
	tInfo.vPos = _vec3(10.f, 2.f, 10.f);
	tInfo.dwOption = DINOTAG_PARA;
	tInfo.dwLevel = 3;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Area", &tInfo);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject);*/

	// 맵 컨테이너에 레이어 추가
	m_mapLayer.emplace(_strLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Empty_Layer(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.emplace(_wstrLayerTag, pLayer);
	return S_OK;
}

HRESULT CStage::Ready_Terrain_Layer(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	/// 오브젝트 추가
	// 터레인
	/*pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Terrain");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);*/

	//타일 파일 로드
	WCHAR szCurPath[MAX_PATH] = __T("");
	lstrcat(szCurPath, L"..\\Resources\\Data\\Cave.scene");	// 뒤에 기본경로로 할 경로 추가

	DWORD dwByte = 0;
	_int iTileCount = 0;
	OBJECT_DESC* pObjectDesc = nullptr;
	_uint iNumObject = 0;
	_ulong dwNameLength = 0;
	_tchar* pName = nullptr;

	HANDLE hFile = CreateFile(szCurPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	_bool bBoss = false;	//(임시)보스방 스위치 ㅎㅎ
	//if (bBoss)
	{
		// 보스방 //////////////////////////////////////////////////////////////////////////

		_vec3 vScale = _vec3(0.01f, 0.01f, 0.01f);
		_vec3 vRot = _vec3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
		_vec3 vPos = _vec3(0.f, 0.f, 0.f) * vScale.x;
		vPos.y += -2000.f;

		pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Tile");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
		static_cast<CTile*>(pGameObject)->Init_Mesh(L"Proto_StaticMesh_Arena_1", vScale, vRot, vPos);

		pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Tile");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
		static_cast<CTile*>(pGameObject)->Init_Mesh(L"Proto_StaticMesh_Arena_2", vScale, vRot, vPos);

		//////////////////////////////////////////////////////////////////////////
	}
	//else
	{
		// 일반맵 //////////////////////////////////////////////////////////////////////////

		for (_uint i = 0; i < 2; ++i)
		{
			TCHAR szTagName[MAX_PATH] = L"";
			wsprintf(szTagName, L"Cave_Tile%d", i);

			pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Tile");
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
			_vec3 vScale = _vec3(0.1f, 0.1f, 0.1f);
			_vec3 vRot = _vec3(D3DXToRadian(0.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
			_vec3 vPos = _vec3(-141000.f, 0.f, 51500.5f) * vScale.x;
			static_cast<CTile*>(pGameObject)->Init_Mesh(i, vScale, vRot, vPos);
		}

		// 테스트용 폭포
		pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_WaterFall");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
		static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(-480.f, -260.f, 850.f);

		// 테스트용 물
		pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Water");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

		//////////////////////////////////////////////////////////////////////////
	}

	// 타일 파일입출력으로 불러오기 //////////////////////////////////////////////////////////////////////////
	//while (true)
	//{
	//	ReadFile(hFile, &iNumObject, sizeof(_uint), &dwByte, nullptr);
	//	if (0 == dwByte)
	//		break;

	//	for (_uint i = 0; i < iNumObject; ++i)
	//	{
	//		pObjectDesc = new OBJECT_DESC;

	//		// 문자열 가져오기
	//		ReadFile(hFile, &dwNameLength, sizeof(_ulong), &dwByte, nullptr);
	//		pName = new _tchar[dwNameLength];
	//		ReadFile(hFile, pName, dwNameLength, &dwByte, nullptr);
	//		pObjectDesc->strName = pName;
	//		Safe_Delete_Array(pName);

	//		// 나머지
	//		ReadFile(hFile, &pObjectDesc->eMeshType, sizeof(MESHTYPE), &dwByte, nullptr);	// 타입
	//		ReadFile(hFile, &pObjectDesc->eMeshID, sizeof(MESHID), &dwByte, nullptr);		// 아이디
	//		ReadFile(hFile, &pObjectDesc->vScale, sizeof(_vec3), &dwByte, nullptr);			// 크기
	//		ReadFile(hFile, &pObjectDesc->vRotate, sizeof(_vec3), &dwByte, nullptr);		// 회전
	//		ReadFile(hFile, &pObjectDesc->vPos, sizeof(_vec3), &dwByte, nullptr);			// 위치

	//		TCHAR szTagName[MAX_PATH] = L"";
	//		wsprintf(szTagName, L"Cave_Tile%d", iTileCount);	//Tile1 ~ Tile9

	//		pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Tile");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(szTagName, pGameObject), E_FAIL);
	//		static_cast<CTile*>(pGameObject)->Init_Tile(iTileCount, pObjectDesc->vScale, pObjectDesc->vRotate, pObjectDesc->vPos); //툴로 세팅한 좌표 불러와 초기화
	//		//static_cast<CTile*>(pGameObject)->Init_Tile(iTileCount, _vec3(1.f, 1.f, 1.f), _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f));	//기본지정된 메시 좌표만 불러와 사용

	//		iTileCount++;
	//		Safe_Delete(pObjectDesc);
	//	}
	//}
	//////////////////////////////////////////////////////////////////////////

	// 스카이돔 
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_SkyDome");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	if (bBoss)
	{
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
		pTransform->Set_Scale(_vec3(0.009f, 0.009f, 0.009f));
	}
	
	// 맵 컨테이너에 레이어 추가
	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Player_Layer(const wstring & _strLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	/// 오브젝트 추가
	// 플레이어
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Player");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	// 맵 컨테이너에 레이어 추가
	m_mapLayer.emplace(_strLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Dino_Layer(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CUnit* pGameObject = nullptr;

	//_uint iIndex1 = 0;
	//// 공룡_렉스
	//OBJINFO tInfo;
	//tInfo.vPos = _vec3(-50.f, 2.f, -25.f);
	//tInfo.vRot = { 0.f,0.f,0.f };
	//tInfo.dwOption = 0;
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &tInfo));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->Set_Pos(-50.f, 2.f, -25.f);


	//// 공룡_렉스
	//iIndex1 = 1;
	//tInfo.vPos = _vec3(-65.f, 2.f, -25.f);
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &tInfo));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->Set_Pos(-65.f, 2.f, -25.f;)
	//
	
	//// 공룡_렉스
	//iIndex1 = 2;
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->Set_Pos(-80.f, 2.f, -25.f);

	//// 공룡_파라
	//_vec3 vPos = _vec3(-36.f, 3.f, -50.f);
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Para", &vPos));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	//vPos = _vec3(-447.f, -75.f, 345.f);
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Para", &vPos));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	//vPos = _vec3(-453.f, -75.f, 360.f);
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Para", &vPos));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	//vPos = _vec3(-435.f, -75.f, 375.f);
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Para", &vPos));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	////////////////////////////////////////////////////////////////////////////

	//// 공룡_이구아노돈
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Iguanodon"));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	//// 공룡_브라키오
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Brachio"));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	//// 공룡_테리지노
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Therizino"));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	//// 공룡_알로사우르스
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Allo"));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	// 공룡_스테고
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Stego"));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	// 공룡_케찰
	for (int i = 0; i < 3; ++i)
	{
		pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Quetzal"));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	}

	// 공룡_드래곤
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Dragon"));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	// 공룡_프테로
	for (int i = 0; i < 3; ++i)
	{
		pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Ptero"));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	}

	//// 공룡_안킬로
	pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Ankylo"));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	
	////// 공룡_유티
	pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Yutyrannus"));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	//// 공룡_랩터
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Raptor"));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	//// 공룡_도도
	//vPos = _vec3(-30.f, 10.f, -50.f);
	//OBJINFO tInfo;
	//tInfo.vPos = _vec3(_vec3(20.f, 15.f, -25.f));
	//tInfo.vRot = { 0.f,0.f,0.f };
	//tInfo.dwOption = 0;
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dodo_Ball", &tInfo));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);


	//// 공룡_렉스
	//iIndex1 = 3;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex4", pGameObject), E_FAIL);
	//
	//// 공룡_렉스
	//iIndex1 = 4;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex5", pGameObject), E_FAIL);

	//// 공룡_렉스
	//iIndex1 = 5;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex6", pGameObject), E_FAIL);
	//// 공룡_렉스
	//iIndex1 = 6;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex7", pGameObject), E_FAIL);
	//// 공룡_렉스
	//iIndex1 = 7;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex8", pGameObject), E_FAIL);
	//// 공룡_렉스
	//iIndex1 = 8;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex9", pGameObject), E_FAIL);
	//// 공룡_렉스
	//iIndex1 = 9;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex10", pGameObject), E_FAIL);
	//// 공룡_렉스
	//iIndex1 = 10;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex11", pGameObject), E_FAIL);
	//// 공룡_렉스
	//iIndex1 = 11;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex12", pGameObject), E_FAIL);
	//// 공룡_렉스
	//iIndex1 = 12;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex13", pGameObject), E_FAIL);
	//// 공룡_렉스
	//iIndex1 = 13;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex14", pGameObject), E_FAIL);
	//// 공룡_렉스
	//iIndex1 = 14;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex15", pGameObject), E_FAIL);
	//// 공룡_렉스
	//iIndex1 = 15;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dino_Rex", &iIndex1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rex16", pGameObject), E_FAIL);
	///

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Ball_Layer(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CUnit* pGameObject = nullptr;

	OBJINFO tInfo;
	tInfo.vPos = _vec3(_vec3(20.f, 15.f, -25.f));
	tInfo.vRot = { 0.f,0.f,0.f };
	tInfo.dwOption = 0;
	pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Dodo_Ball", &tInfo));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_BallHoop_Layer(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	// 테스트용 농구대
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_BasketballHoop");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Harvest_Tree_Layer(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CUnit* pGameObject = nullptr;

	/*/// 오브젝트 추가
	pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Tree"));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	pGameObject->Set_Pos(-35.f, 0.f, -20.f);

	pGameObject->Set_Scale(0.001f);
	pGameObject->Set_Scale(0.02f);*/

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Harvest_Bush_Layer(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CUnit* pGameObject = nullptr;

	/*/// 오브젝트 추가
	pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Bush"));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	pGameObject->Set_Pos(-25.f, 0.f, -10.f);
	pGameObject->Set_Scale(0.02f);*/

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Harvest_Rock_Layer(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CUnit* pGameObject = nullptr;

	/*/// 오브젝트 추가
	pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Rock_Small"));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject->Set_Pos(-35.f, 0.f, -25.f);
	pGameObject->Set_Scale(0.02f);
	pGameObject->Set_Pos(-35.f, 0.f, -10.f);
	pGameObject->Set_Scale(0.02f);*/


	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Harvest_Pebble_Layer(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CUnit* pGameObject = nullptr;

	/*/// 오브젝트 추가
	pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Pebble"));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	pGameObject->Set_Pos(-25.f, 0.f, -20.f);
	pGameObject->Set_Scale(0.02f);*/

	//// 조명 위치 확인용
	//pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(L"Proto_Obj_Pebble"));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->Set_Pos(_vec3(-10.f, 50.f, 150.f));
	//pGameObject->Set_Scale(1.f);

	// 맵 컨테이너에 레이어 추가
	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_UI()
{
	FAILED_CHECK_RETURN(Ready_Inventory_UI(L"LR_Inventory"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Center_UI(L"Center_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_SummaryStat_UI(L"SummaryStat_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_QuickSlot_UI(L"QuickSlot_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GuideFont_UI(L"GuideFont_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_InfoView_UI(L"InfoView_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_WorldFont_UI(L"WorldFont_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_PlayerHit_UI(L"PlayerHit_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MiniMap_UI(L"MiniMap_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Taming_Name_UI(L"Taming_Name_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RingCommand_UI(L"RingCommand_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_CrossHair_UI(L"CrossHair_UI"), E_FAIL);
	return S_OK;
}

HRESULT CStage::Ready_Inventory_UI(const wstring& _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	wstring wstrItemPhotoFront = L"Proto_Texture_";

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Left_Inventory", &wstrItemPhotoFront);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	Ready_InventoryUI(L"Left_Inventory", pGameObject, true); //왼쪽 인벤토리 구성품 생성

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Right_Inventory" , &wstrItemPhotoFront);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	Ready_InventoryUI(L"Right_Inventory", pGameObject, false); //오른쪽 인벤토리 구성품 생성

	//왼쪽 인벤토리 스크롤 바 - 2 
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Scroll_UI");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	//오른쪽 인벤토리 스크롤 바 - 3
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Scroll_UI");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Center_UI(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	wstring wstrItemPhotoFront = L"Proto_Texture_";

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Stat_UI", &wstrItemPhotoFront);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	Ready_Stat_UI(L"Stat_UI", pGameObject, CENTER_STAT_UI); 

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_MonsterStat_UI", &wstrItemPhotoFront);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	Ready_MonsterStat_UI(L"MonsterStat_UI", pGameObject, CENTER_MONSTER_UI);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_StructureStat_UI", &wstrItemPhotoFront);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	Ready_StructureStat_UI(L"StructureStat_UI", pGameObject);

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Camera_Layer(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	/// 카메라 추가
	// 3인칭 카메라
	pGameObject = CCameraMrg::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);


	// 맵 컨테이너에 레이어 추가
	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_LightInfo(void)
{
	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction = _vec3(0.f, -10.f, -15.f);

	//_vec3 vDir = Engine::Get_ShadowLight().vLightDir;
	//D3DXVec3Normalize(&vDir, &vDir);

	//tLightInfo.Direction = vDir;

	//// 점 조명
	//tLightInfo.Type = D3DLIGHT_POINT;
	//tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	//tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Position = _vec3(0.f, 3.f, 5.f);		// 점조명은 Position 필수!
	//tLightInfo.Range = 100.f;							// Position을 기준으로 어느정도 까지 비출지

	//tLightInfo.Type = D3DLIGHT_POINT;
	//tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	//tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Position = _vec3(60.f, -15.f, 15.f);		// 점조명은 Position 필수!
	//tLightInfo.Range = 1000.f;							// Position을 기준으로 어느정도 까지 비출지

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pDevice, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CStage::Ready_InventoryUI(const wstring & _wstrLayerTag, CGameObject* pLR_Inventory, _bool _LeftInventory)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;
	CGameObject* pArg = nullptr; //보더 아그먼트 용
	CGameObject* pRight_Inven = nullptr; //포스 조절용
	CGameObject* pBorder = nullptr; //렌더 순서 결정용
	
	pArg = pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Inventory", &_LeftInventory);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	pBorder = pGameObject;
	static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenTap", pArg);
	if(_LeftInventory)
		static_cast<CInvenTap*>(pGameObject)->Set_TapOwner(1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	//pBorder = pGameObject;
	//static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"InvenTap_Border", pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_Transfer", pArg);
	static_cast<CInvenIcon*>(pGameObject)->Set_LeftInvenIcon(_LeftInventory);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	pBorder = pGameObject;
	static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject( pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_DropAll", pArg);
	static_cast<CInvenIcon*>(pGameObject)->Set_LeftInvenIcon(_LeftInventory);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	pBorder = pGameObject;
	static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_ItemSort", pArg);
	static_cast<CInvenIcon*>(pGameObject)->Set_LeftInvenIcon(_LeftInventory);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	pBorder = pGameObject;
	static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject( pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_Folder", pArg);
	static_cast<CInvenIcon*>(pGameObject)->Set_LeftInvenIcon(_LeftInventory);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	pBorder = pGameObject;
	static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_Category", pArg);
	static_cast<CInvenIcon*>(pGameObject)->Set_LeftInvenIcon(_LeftInventory);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	pBorder = pGameObject;
	static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_ToolTip", pArg);
	static_cast<CInvenIcon*>(pGameObject)->Set_LeftInvenIcon(_LeftInventory);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject( pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	pBorder = pGameObject;
	static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_Searchbar", pArg);
	static_cast<CInvenIcon*>(pGameObject)->Set_LeftInvenIcon(_LeftInventory);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	pBorder = pGameObject;
	static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	CGameObject* pcastLR_Inventory = pLR_Inventory;

	Ready_ItemSlot(_wstrLayerTag, static_cast<CLR_Inventory*>(pcastLR_Inventory), static_cast<COrthoUI*>(pArg), _LeftInventory);

	return S_OK;
}

HRESULT CStage::Ready_Stat_UI(const wstring & _wstrLayerTag, CGameObject* pCenter_UI, CENTER_UIID _eCenter_UI_Type)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;
	CGameObject* pArg = nullptr; //포스 받는 용이자나 ㅡㅡ
	CGameObject* pBorder = nullptr; //렌더 순서 결정용
	
	//스텟창 UI를 구성하는 그림들을 만들기 위한 함수!
	//백그라운드
	///////////////////////////////////////////////////////////////////////
	pArg = pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Back_Stat_UI");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	////////////////////////////////////////////////////////////////////
	//백그라운드 보더
	pBorder = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	NULL_CHECK_RETURN(pBorder, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pBorder), E_FAIL);
	//탭
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_CenterTap", pArg);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//스탯들! (경험치 제외 - 니들은 그냥 각자 알아서 포스 잡자!
	ITEM_INFO tStatbar;

	for (_int i = 0; i < (_int)ESTATbar::STATbar_End; ++i)
	{
		tStatbar.iPosition = i;
		pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Stat_UI_Bar", &tStatbar);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

		pBorder = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
		static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ITEMBORDER);
		NULL_CHECK_RETURN(pBorder, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pBorder), E_FAIL);
	}
	

	CGameObject* pcastCenter_UI = pCenter_UI;

	Ready_EquipSlot(_wstrLayerTag, static_cast<CCenter_UI*>(pcastCenter_UI), static_cast<COrthoUI*>(pArg), _eCenter_UI_Type);

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_MonsterStat_UI(const wstring & _wstrLayerTag, CGameObject * pCenter_UI, CENTER_UIID _eCenter_UI_Type)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;
	CGameObject* pArg = nullptr; //포스 받는 용이자나 ㅡㅡ
	CGameObject* pBorder = nullptr; //렌더 순서 결정용

									//스텟창 UI를 구성하는 그림들을 만들기 위한 함수!
									//백그라운드
									///////////////////////////////////////////////////////////////////////
	pArg = pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Back_MonsterStat_UI");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	////////////////////////////////////////////////////////////////////
	//백그라운드 보더
	pBorder = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	NULL_CHECK_RETURN(pBorder, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pBorder), E_FAIL);
	//스탯들! (경험치 제외 - 니들은 그냥 각자 알아서 포스 잡자!
	ITEM_INFO tStatbar;

	for (_int i = 0; i < (_int)EMonsterSTATbar::EMonsterSTATbar_End; ++i)
	{
		tStatbar.iPosition = i;
		pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_MonsterStat_UI_Bar", &tStatbar);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

		pBorder = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
		static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ITEMBORDER);
		NULL_CHECK_RETURN(pBorder, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pBorder), E_FAIL);
	}


	CGameObject* pcastCenter_UI = pCenter_UI;

	Ready_EquipSlot(_wstrLayerTag, static_cast<CCenter_UI*>(pcastCenter_UI), static_cast<COrthoUI*>(pArg), _eCenter_UI_Type);

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_StructureStat_UI(const wstring & _wstrLayerTag, CGameObject * pCenter_UI)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;
	CGameObject* pArg = nullptr; //포스 받는 용이자나 ㅡㅡ
	CGameObject* pBorder = nullptr; //렌더 순서 결정용

									//스텟창 UI를 구성하는 그림들을 만들기 위한 함수!
									//백그라운드
									///////////////////////////////////////////////////////////////////////
	pArg = pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Back_StructureStat_UI");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	////////////////////////////////////////////////////////////////////
	//백그라운드 보더
	pBorder = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	NULL_CHECK_RETURN(pBorder, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pBorder), E_FAIL);

	pBorder = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<CBack_StructureStat_UI*>(pGameObject)->Get_Button_Rect_Info());
	static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ORTHOUI);
	NULL_CHECK_RETURN(pBorder, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pBorder), E_FAIL);

	//스탯들! (경험치 제외 - 니들은 그냥 각자 알아서 포스 잡자!
	ITEM_INFO tStatbar;

	for (_int i = 0; i < (_int)EStructureSTATbar::EStructureSTATbar_End; ++i)
	{
		tStatbar.iPosition = i;
		pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_StructureStat_UI_Bar", &tStatbar);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

		pBorder = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
		static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ITEMBORDER);
		NULL_CHECK_RETURN(pBorder, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pBorder), E_FAIL);
	}


	CGameObject* pcastCenter_UI = pCenter_UI;

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_EquipSlot(const wstring & _wstrLayerTag, CCenter_UI * _pCenter_UI, COrthoUI* _pCenter_Back_UI, CENTER_UIID _eCenter_UI_Type)
{
	//장비 슬롯 만들기
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;
	CInvenItem* pInvenItem = nullptr;
	CGameObject* pBorder = nullptr;
	CCenter_UI* pCenter_UI = _pCenter_UI;
	COrthoUI* pBackGround = _pCenter_Back_UI;
	wstring GameObjectTag = L"";

	CInvenItem::STInvenItemURI stInvenItemURI;

	_float fInvenItemPosX, fInvenItemPosY, fFirstPosX, fFirstPosY;
	fInvenItemPosX = pBackGround->Get_ORTHOUI_INFO()->fPosX;
	fInvenItemPosY = pBackGround->Get_ORTHOUI_INFO()->fPosY;
	fInvenItemPosX -= stInvenItemURI.fStat_Player_FirstFixPosX[EScrnResolution]; //조정해야 함
	fInvenItemPosY -= stInvenItemURI.fStat_Player_FirstFixPosY[EScrnResolution]; //조정해야 함
	fFirstPosX = fInvenItemPosX += stInvenItemURI.fSizeXY[EScrnResolution] * FHALF;
	fFirstPosY = fInvenItemPosY += stInvenItemURI.fSizeXY[EScrnResolution] * FHALF;

	if (CENTER_STAT_UI == _eCenter_UI_Type)
	{
		for (_int i = 0; i < PlayerEquipAmount; ++i)
		{
			if (PlayerEquipYAmount == i)
			{
				fInvenItemPosX = (pBackGround->Get_ORTHOUI_INFO()->fPosX) + stInvenItemURI.fStat_Interval_FixPosX[EScrnResolution] - (stInvenItemURI.fSizeXY[EScrnResolution] * FHALF);//조정필
				fInvenItemPosY = fFirstPosY;
			}
			GameObjectTag = _wstrLayerTag;
			GameObjectTag = GameObjectTag + to_wstring(i);

			pInvenItem = static_cast<CInvenItem*>(Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_InvenItem"));
			pInvenItem->Set_IdentifyNumber(i);
			pInvenItem->Set_Pos_UI(fInvenItemPosX, fInvenItemPosY);
			pInvenItem->Set_pSlotOwner(_pCenter_UI, ESlotOwner::Stat_Player);
			pInvenItem->Set_SlotNum(i); //아덴 넘버랑 중복 같은데? 아덴 넘버는 인벤 슬롯 몇번째냐? 이거 넣었나? 어쩄든 템에 관련된 수? 슬롯 넘은 UI 슬롯 수?
			POINT tFirstPos;
			tFirstPos.x = (LONG)fInvenItemPosX;
			tFirstPos.y = (LONG)fInvenItemPosY;
			pInvenItem->Set_FirstPos(tFirstPos); //드래그 앤 드롭 할 슬롯 만들 때 쓸 것
			fInvenItemPosY += stInvenItemURI.fSizeXY[EScrnResolution] + stInvenItemURI.fStat_Interval_FixPosY[EScrnResolution]; // Y값 바꿔주는 부분
			NULL_CHECK_RETURN(pInvenItem, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pInvenItem), E_FAIL);
			pCenter_UI->Set_EmplaceBack_EquipSlot(pInvenItem); // 유아이 겜 옵젝에 넣어주기 (관리는 레이어로, 템 정보 전달만 얘로-클라에서 갈아끼니까)

			pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pInvenItem->Get_ORTHOUI_INFO());
			pBorder = pGameObject;
			static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ITEMBORDER);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			GameObjectTag = GameObjectTag + L"_Border";
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pBorder), E_FAIL);
		}
	}
	else if (CENTER_MONSTER_UI == _eCenter_UI_Type)
	{
		GameObjectTag = _wstrLayerTag;
		GameObjectTag = GameObjectTag + to_wstring(0);
		fInvenItemPosY += stInvenItemURI.fStat_Monster_FirstFixPosY[EScrnResolution];
		pInvenItem = static_cast<CInvenItem*>(Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_InvenItem"));
		pInvenItem->Set_IdentifyNumber(0);
		pInvenItem->Set_Pos_UI(fInvenItemPosX, fInvenItemPosY);
		pInvenItem->Set_pSlotOwner(_pCenter_UI, ESlotOwner::Stat_Monster);
		pInvenItem->Set_SlotNum(0); //아덴 넘버랑 중복 같은데? 아덴 넘버는 인벤 슬롯 몇번째냐? 이거 넣었나? 어쩄든 템에 관련된 수? 슬롯 넘은 UI 슬롯 수?
		POINT tFirstPos;
		tFirstPos.x = (LONG)fInvenItemPosX;
		tFirstPos.y = (LONG)fInvenItemPosY;
		pInvenItem->Set_FirstPos(tFirstPos); //드래그 앤 드롭 할 슬롯 만들 때 쓸 것
		fInvenItemPosY += stInvenItemURI.fSizeXY[EScrnResolution] + stInvenItemURI.fStat_Interval_FixPosY[EScrnResolution]; // Y값 바꿔주는 부분
		NULL_CHECK_RETURN(pInvenItem, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pInvenItem), E_FAIL);
		pCenter_UI->Set_EmplaceBack_EquipSlot(pInvenItem); // 유아이 겜 옵젝에 넣어주기 (관리는 레이어로, 템 정보 전달만 얘로-클라에서 갈아끼니까)

		pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pInvenItem->Get_ORTHOUI_INFO());
		pBorder = pGameObject;
		static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ITEMBORDER);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		GameObjectTag = GameObjectTag + L"_Border";
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pBorder), E_FAIL);
	}

	GameObjectTag = _wstrLayerTag + L"_ItemSlot";
	m_mapLayer.emplace(GameObjectTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_QuickSlot(const wstring & _wstrLayerTag, CQuickSlot_UI * _pQuickSlot)
{
	//장비 슬롯 만들기
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;
	CInvenItem* pInvenItem = nullptr;
	CGameObject* pBorder = nullptr;
	CQuickSlot_UI* pQuickSlot_UI = _pQuickSlot;

	wstring GameObjectTag = L"";
	CInvenItem::STInvenItemURI stInvenItemURI;

	_float fInvenItemPosX, fFirstPosX, fFirstPosY;
	fInvenItemPosX = stInvenItemURI.fQuickSlot_FirstPosX[EScrnResolution];
	fFirstPosY = stInvenItemURI.fQuickSlot_FirstPosY[EScrnResolution];
	fFirstPosX = fInvenItemPosX /*+= InvenItemSizeX * FHALF;*/;



	for (_int i = 0; i < QuickSlotAmount; ++i)
	{
		GameObjectTag = _wstrLayerTag;
		GameObjectTag = GameObjectTag + to_wstring(i);

		pInvenItem = static_cast<CInvenItem*>(Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_InvenItem"));
		pInvenItem->Set_IdentifyNumber(i);
		pInvenItem->Set_Pos_UI(fInvenItemPosX, fFirstPosY);
		pInvenItem->Set_pSlotOwner(pQuickSlot_UI, ESlotOwner::QuickSlot);
		pInvenItem->Set_SlotNum(i); //아덴 넘버랑 중복 같은데? 아덴 넘버는 인벤 슬롯 몇번째냐? 이거 넣었나? 어쩄든 템에 관련된 수? 슬롯 넘은 UI 슬롯 수?
		pInvenItem->Get_ORTHOUI_INFO()->bRender = true;
		POINT tFirstPos;
		tFirstPos.x = (LONG)fInvenItemPosX;
		tFirstPos.y = (LONG)fFirstPosY;
		pInvenItem->Set_FirstPos(tFirstPos); //드래그 앤 드롭 할 슬롯 만들 때 쓸 것
		NULL_CHECK_RETURN(pInvenItem, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pInvenItem), E_FAIL);
		pQuickSlot_UI->Set_EmplaceBack_Slot(pInvenItem); // 유아이 겜 옵젝에 넣어주기 (관리는 레이어로, 템 정보 전달만 얘로-클라에서 갈아끼니까)

		pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pInvenItem->Get_ORTHOUI_INFO());
		pBorder = pGameObject;
		static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ITEMBORDER);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		GameObjectTag = GameObjectTag + L"_Border";
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pBorder), E_FAIL);

		fInvenItemPosX += stInvenItemURI.fQuickSizeXY[EScrnResolution] + stInvenItemURI.fQuickSlot_Interval_FixPosX[EScrnResolution];
	}

	GameObjectTag = _wstrLayerTag + L"_ItemSlot";
	m_mapLayer.emplace(GameObjectTag, pLayer);

	return S_OK;
}

HRESULT CStage::LoadFile_ObjectList(const wstring & wstrFilePath)
{
	///////////////////////////////////UI 간이 디버깅 확인용 주석
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"불러오기 실패", L"실패", MB_OK);
		return E_FAIL;
	}
	////////////////////////////////////////

	////대상 레이어 클린업
	//Engine::Get_Layer(L"Layer_Harvest_Tree")->Free();
	//Engine::Get_Layer(L"Layer_Harvest_Bush")->Free();
	//Engine::Get_Layer(L"Layer_Harvest_Rock")->Free();
	//Engine::Get_Layer(L"Layer_Harvest_Pebble")->Free();

	DWORD	dwByte = 0;
	OBJINFO	tInfo;
	_uint iChecker_LoadCount = 0;
	_uint iChecker_CloneCount = 0;

	while (true)
	{
		//Info구조체 읽기
		ReadFile(hFile, &tInfo, sizeof(OBJINFO), &dwByte, NULL);
		if (0 == dwByte)
			break;

		iChecker_LoadCount++;

		//Info구조체의 ID를 ProtoTag로 바꿈
		WCHAR szTag[MAX_PATH];
		wstring strTag;

		// 돌 ->부쉬로 바꾸는 테스트코드
		//if (tInfo.dwID == 0 && t <= 2)
		//{
		//	t++;
		//	tInfo.dwID = 3;
		//}

		///////////////////////////////////UI 간이 디버깅 확인용 주석
		//Engine::Get_ProtoTag(szTag, tInfo.dwID);
		//strTag = szTag;
		//if (strTag == L"Dino")
		//{	
		//	Engine::Get_ProtoTag_Dino(szTag, tInfo.dwOption);
		//	strTag = szTag;

		//	if (!(strTag == L"Proto_Obj_Dino_Dodo" || strTag == L"Proto_Obj_Dino_Raptor"))
		//		continue;
		//}
		Engine::Get_ProtoTag(szTag, tInfo.dwID);
		strTag = szTag;
		if (strTag == L"Dino")
		{
			Engine::Get_ProtoTag_Dino(szTag, tInfo.dwOption);
			strTag = szTag;
		}
		////////////////////////////////////////////////////////////

		//ProtoTag를 참조해 객체 생성
		CGameObject* pGameObject = static_cast<CUnit*>(Engine::Clone_GameObjectPrototype(strTag, &tInfo));
		if (pGameObject)
		{
			//ID를 LayerTag로 바꿈
			Engine::Get_LayerTag(szTag, tInfo.dwID);
			strTag = szTag;

			// 레이어 탐색, 찾는 레이어가 없을 경우 리턴
			auto pair = m_mapLayer.find(strTag);

			if (pair == m_mapLayer.end())
			{
				///////////////////////////////////UI 간이 디버깅 확인용 주석
				ERR_MSG(L"Stage::LoadFile_ObjectList() : Failed to Find Matching Layer");
			}

			// 게임 오브젝트 추가
			if (FAILED(pair->second->Add_GameObject(pGameObject)))
			{
				ERR_MSG(L"Stage::LoadFile_ObjectList() : Failed to Add CloneObj in Layer");
			}
			else
				iChecker_CloneCount++;
		}
		else
		{
			ERR_MSG(L"Stage::LoadFile_ObjectList() : Failed to Clone Obj");
		}

		m_pCount->iLoadingCount += 1;
	}

	if (iChecker_CloneCount != iChecker_LoadCount)
	{
		ERR_MSG(L"Stage::LoadFile_ObjectList() : LoadFile Incomplete");
	}
	////////////////////////////////////////////////////////////

	m_pCount->iLoadingCount = 0;
	++m_pCount->iLoadingBarCount;
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CStage::Ready_SummaryStat_UI(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;
	COrthoUI*    pArg = nullptr;
	CBorder*     pBorder = nullptr;

	_int i = 0;
	ITEM_INFO tSummartStat;
	tSummartStat.tchName = L"Health";
	tSummartStat.iPosition = i++;
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_SummaryStat_UI", &tSummartStat);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	pArg = static_cast<COrthoUI*>(pGameObject);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pArg->Get_ORTHOUI_INFO());
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pBorder = static_cast<CBorder*>(pGameObject);
	pBorder->Set_BorderRenderID(RENDER_BACKUI);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	
	tSummartStat.tchName = L"Stamina";
	tSummartStat.iPosition = i++;
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_SummaryStat_UI", &tSummartStat);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pArg->Get_ORTHOUI_INFO());
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pBorder = static_cast<CBorder*>(pGameObject);
	pBorder->Set_BorderRenderID(RENDER_BACKUI);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	tSummartStat.tchName = L"Food";
	tSummartStat.iPosition = i++;
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_SummaryStat_UI", &tSummartStat);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pArg->Get_ORTHOUI_INFO());
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pBorder = static_cast<CBorder*>(pGameObject);
	pBorder->Set_BorderRenderID(RENDER_BACKUI);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	tSummartStat.tchName = L"Water";
	tSummartStat.iPosition = i++;
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_SummaryStat_UI", &tSummartStat);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pArg->Get_ORTHOUI_INFO());
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pBorder = static_cast<CBorder*>(pGameObject);
	pBorder->Set_BorderRenderID(RENDER_BACKUI);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject( pGameObject), E_FAIL);

	tSummartStat.tchName = L"Weight";
	tSummartStat.iPosition = i++;
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_SummaryStat_UI", &tSummartStat);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject( pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pArg->Get_ORTHOUI_INFO());
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pBorder = static_cast<CBorder*>(pGameObject);
	pBorder->Set_BorderRenderID(RENDER_BACKUI);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	tSummartStat.tchName = L"XP";
	tSummartStat.iPosition = i++;
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_SummaryStat_UI", &tSummartStat);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject( pGameObject), E_FAIL);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pArg->Get_ORTHOUI_INFO());
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pBorder = static_cast<CBorder*>(pGameObject);
	pBorder->Set_BorderRenderID(RENDER_BACKUI);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	//////////////////////////////////////////몬스터 꺼
	i = 0;
	tSummartStat.tchName = L"Health";
	tSummartStat.iPosition = i++;
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_SummaryStat_UI_Monster", &tSummartStat);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	pArg = static_cast<COrthoUI*>(pGameObject);

	tSummartStat.tchName = L"Stamina";
	tSummartStat.iPosition = i++;
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_SummaryStat_UI_Monster", &tSummartStat);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	tSummartStat.tchName = L"Food";
	tSummartStat.iPosition = i++;
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_SummaryStat_UI_Monster", &tSummartStat);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	tSummartStat.tchName = L"Weight";
	tSummartStat.iPosition = i++;
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_SummaryStat_UI_Monster", &tSummartStat);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	tSummartStat.tchName = L"XP";
	tSummartStat.iPosition = i++;
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_SummaryStat_UI_Monster", &tSummartStat);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_mapLayer.emplace(_wstrLayerTag, pLayer);
	return S_OK;
}

HRESULT CStage::Ready_QuickSlot_UI(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	wstring wstrItemPhotoFront = L"Proto_Texture_";

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_QuickSlot_UI", &wstrItemPhotoFront);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);


	Ready_QuickSlot(_wstrLayerTag, static_cast<CQuickSlot_UI*>(pGameObject));
	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_GuideFont_UI(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_GuideFont_UI",0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Effect_Layer(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//CGameObject* pGameObject = nullptr;

	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_MotionBlur");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Test(const wstring & _wstrLayerTag)
{
	CGameObject* pGameObject = nullptr;

	_vec3	vPos = {};
	_float	fScale = 0.f;

	// ShadowMap 조명 위치 확인
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_ShadowPosTest");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);

	// 폭포
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_WaterFall");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Terrain", pGameObject), E_FAIL);
	//_vec3 vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos());
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	//// 피 1
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Blood");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(2.f, 1.f, 0.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	//// 피 2
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_BloodTest");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(2.f, 1.f, 0.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	// 모닥불
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_FireTest");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(0.f, 1.f, 1.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	// 모닥불 연기
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_FireSmoke");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(0.f, 1.3f, 1.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	//// 구조물 설치 연기
	//fScale = 0.004f;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_StructDust", &fScale);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(0.f, 1.f, 0.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	//_int	iTextureNum = 0;

	//// 피 파티클
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_BloodParticle", &iTextureNum);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(1.f, 1.f, 0.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	//iTextureNum = 1;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_BloodParticle", &iTextureNum);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(2.f, 1.f, 0.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	//// 버프 이펙트
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Buff");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(1.f, 1.f, 0.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	//// 포효 이펙트
	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Roar");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(1.f, 1.f, 5.f);
	static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	// 골 이펙트
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Goal");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(1.f, 1.f, 0.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	// 핑 원기둥
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_EffectObject");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(0.f, 0.f, 10.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	// 핑 하단 원형 모양
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_EffectObjectRev");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(0.f, 0.5f, 10.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	// 핑 화살표
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_EffectObjectArrow");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(0.f, 0.f, 10.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	// 이동 흙먼지
	//fScale = 0.005f;
	//pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Dust", &fScale);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(_wstrLayerTag, pGameObject), E_FAIL);
	//vPos = *(static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos()) + _vec3(0.f, 1.f, 0.f);
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vPos);

	return S_OK;
}

HRESULT CStage::Ready_InfoView_UI(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	CGameObject* pGameObject = nullptr;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_InfoView_UI",0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Item_Info_View_UI");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	m_mapLayer.emplace(_wstrLayerTag, pLayer);
	
	return S_OK;
}

HRESULT CStage::Ready_WorldFont_UI(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_WorldFont_UI", 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_mapLayer.emplace(_wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_PlayerHit_UI(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_PlayerHit_UI", 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_mapLayer.emplace(_wstrLayerTag, pLayer);
	
	return S_OK;
}

HRESULT CStage::Ready_MiniMap_UI(const wstring & _wstrLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_MiniMap_UI", 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_mapLayer.emplace(_wstrLayerTag, pLayer);
	return S_OK;
}

HRESULT CStage::Ready_Taming_Name_UI(const wstring & _wstrLaerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_TamingName_UI");
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	CTamingName_UI* pTaming_UI = static_cast<CTamingName_UI*>(pGameObject);

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", static_cast<COrthoUI*>(pGameObject)->Get_ORTHOUI_INFO());
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	static_cast<CBorder*>(pGameObject)->Set_BorderRenderID(RENDER_LASTUI);//테이밍 유아이 백 그라운드 보더

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pTaming_UI->Get_Name_Rect_Info());
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	static_cast<CBorder*>(pGameObject)->Set_BorderRenderID(RENDER_LASTUI);;//테이밍 유아이 네임 렉트 보더

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pTaming_UI->Get_Ok_Rect_Info());
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	static_cast<CBorder*>(pGameObject)->Set_BorderRenderID(RENDER_LASTUI);//테이밍 오케이 렉트 보더

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pTaming_UI->Get_Cancel_Rect_Info());
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	static_cast<CBorder*>(pGameObject)->Set_BorderRenderID(RENDER_LASTUI);//테이밍 캔슬 렉트 보더

	m_mapLayer.emplace(_wstrLaerTag, pLayer);
	return S_OK;
}

HRESULT CStage::Ready_RingCommand_UI(const wstring & _wstrLaerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_RingCommand_UI");
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_mapLayer.emplace(_wstrLaerTag, pLayer);
	return S_OK;
}

HRESULT CStage::Ready_CrossHair_UI(const wstring & _wstrLaerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_CrossHair_UI");
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);

	m_mapLayer.emplace(_wstrLaerTag, pLayer);
	return S_OK;
}

HRESULT CStage::Ready_ItemSlot(const wstring& _wstrLayerTag, CLR_Inventory* _pLR_Inventory, COrthoUI* _pInventory, _bool _LeftInventory)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;
	CInvenItem* pInvenItem = nullptr;
	CGameObject* pBorder = nullptr;
	CLR_Inventory* pLRInven = _pLR_Inventory;
	COrthoUI* pInvenBackGround = _pInventory;
	wstring GameObjectTag = L"";
	CInvenItem::STInvenItemURI stInvenItemURI; //아이템 꺼

	_float fInvenItemPosX, fInvenItemPosY, fFirstPosX, fFirstPosY;
	fInvenItemPosX = pInvenBackGround->Get_ORTHOUI_INFO()->fPosX;
	fInvenItemPosY = pInvenBackGround->Get_ORTHOUI_INFO()->fPosY;
	fInvenItemPosX -= stInvenItemURI.fInventory_FirstFixPosX[EScrnResolution];
	fInvenItemPosY -= stInvenItemURI.fInventory_FirstFixPosY[EScrnResolution];

	fFirstPosX = fInvenItemPosX += stInvenItemURI.fSizeXY[EScrnResolution] * FHALF;
	fFirstPosY = fInvenItemPosY += stInvenItemURI.fSizeXY[EScrnResolution] * FHALF;

	for (; pLRInven->Get_ItemSlotAmount(false) < pLRInven->Get_ItemSlotAmount(true); )
	{
		if (NULL == (pLRInven->Get_ItemSlotAmount(false) % InvenItemSlotHorizonAmount))
		{
			fInvenItemPosX = fFirstPosX;
			fInvenItemPosY += stInvenItemURI.fSizeXY[EScrnResolution] + stInvenItemURI.fInven_Interval_FixPosXY[EScrnResolution];
		}
		GameObjectTag = _wstrLayerTag;
		GameObjectTag = GameObjectTag + to_wstring(pLRInven->Get_ItemSlotAmount(false));
		pInvenItem = static_cast<CInvenItem*>(Engine::Clone_GameObjectPrototype(L"Proto_UI_InvenIcon_InvenItem"));
		pInvenItem->Set_IdentifyNumber(pLRInven->Plus_ItmeSlotAmount());
		pInvenItem->Set_Pos_UI(fInvenItemPosX, fInvenItemPosY);
		if(_LeftInventory)
			pInvenItem->Set_pSlotOwner(_pLR_Inventory, ESlotOwner::Left_Inven);
		else
			pInvenItem->Set_pSlotOwner(_pLR_Inventory, ESlotOwner::Right_Inven);
		pInvenItem->Set_SlotNum(pLRInven->Get_ItemSlotAmount(false));
		POINT tFirstPos;
		tFirstPos.x = (LONG)fInvenItemPosX;
		tFirstPos.y = (LONG)fInvenItemPosY;
		pInvenItem->Set_FirstPos(tFirstPos);
		fInvenItemPosX += stInvenItemURI.fSizeXY[EScrnResolution] + stInvenItemURI.fInven_Interval_FixPosXY[EScrnResolution];
		NULL_CHECK_RETURN(pInvenItem, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pInvenItem), E_FAIL);
		_pLR_Inventory->Set_EmplaceBack_ItemSlot(pInvenItem);

		pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_UI_Border", pInvenItem->Get_ORTHOUI_INFO());
		pBorder = pGameObject;
		static_cast<CBorder*>(pBorder)->Set_BorderRenderID(RENDER_ITEMBORDER);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		GameObjectTag = GameObjectTag + L"_Border";
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	}

	GameObjectTag = _wstrLayerTag + L"_ItemSlot";
	m_mapLayer.emplace(GameObjectTag, pLayer);
	return S_OK;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 _pDevice, stSC* pCount)
{
	CStage*	pInstance = new CStage(_pDevice);
	
	m_pCount = pCount;	//로딩 중 진행률 카운트할 int포인터 세팅

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStage::Free()
{
	CScene::Free();
}
