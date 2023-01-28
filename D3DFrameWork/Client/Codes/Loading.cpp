#include "stdafx.h"
#include "..\Headers\Loading.h"

#include "Export_Function.h"

#include "BackGround.h"
#include "Player.h"

#include "Weapon.h"
#include "Bullet.h"

// 구조물
#include "Campfire.h"
#include "SleepingBag.h"
#include "Forge.h"
#include "MortarPestal.h"
#include "Smithy.h"
#include "ThatchBed.h"
#include "WallTorch.h"
#include "WaterWell.h"
#include "Barricade.h"
#include "CompostBin.h"
#include "CropPlot.h"
#include "FeedingTrough.h"
#include "Fireplace.h"
#include "Storage.h"
#include "CookingCampFire.h"

#include "Thatch_Floor.h"
#include "Thatch_Wall.h"
#include "Thatch_Roof.h"
#include "Thatch_DoorFrame.h"
#include "Thatch_Door.h"
#include "GateFrame.h"

// 공룡
#include "Dino_Rex.h"
#include "Dino_Iguanodon.h"
#include "Dino_Para.h"
#include "Dino_Brachio.h"
#include "Dino_Therizino.h"
#include "Dino_Quetzal.h"
#include "Dino_Dragon.h"
#include "Dino_Ptero.h"

#include "PlatformSaddle_Brachio.h"

//지형
#include "Terrain.h"
#include "Tile.h"
#include "SkyDome.h"
#include "Area.h"
#include "Obstacle.h"

//채집물
#include "Harvestable_Rock.h"
#include "Harvestable_Pebble.h"
#include "Harvestable_Bush.h"
#include "Harvestable_Tree.h"

#include "LoadingBackGround.h"


//UI
#include "Left_Inventory.h"
#include "Right_Inventory.h"
#include "Inven.h"
#include "Inventory.h"
#include "InvenTap.h"
#include "InvenTransfer.h"
#include "InvenDropAll.h"
#include "InvenItemSort.h"
#include "InvenFolder.h"
#include "InvenCategory.h"
#include "InvenToolTip.h"
#include "InvenSearchbar.h"
#include "InvenItem.h"
#include "Border.h"
#include "Scroll_UI.h"
// 인벤
//센터 UI
#include "Back_Stat_UI.h"
#include "Back_MonsterStat_UI.h"
#include "Back_StructureStat_UI.h"
#include "Stat_UI.h"
#include "MonsterStat_UI.h"
#include "StructureStat_UI.h"
#include "CenterTap.h"
#include "Stat_UI_Bar.h"
#include "MonsterStat_UI_Bar.h"
#include "StructureStat_UI_Bar.h"
//SummaryStat UI
#include "SummaryStat_UI.h"
#include "SummaryStat_UI_Monster.h"
//QuickSlot UI
#include "QuickSlot_UI.h"
//GuideFont UI
#include "GuideFont_UI.h"
#include "Dino_Allo.h"
#include "Dino_Raptor.h"
#include "Dino_Dodo.h"
//InfoView UI
#include "InfoView_UI.h"
#include "Item_Info_View_UI.h"
//WorldFont_UI
#include "WorldFont_UI.h"
#include "PlayerHit_UI.h"
//MiniMap
#include "MiniMap_UI.h"
//테이밍 네임
#include "TamingName_UI.h"
//링 커맨드
#include "RingCommand_UI.h"
//크로스 헤어
#include "CrossHair_UI.h"

// Effect
#include "Blood.h"
#include "BloodTest.h"
#include "Dust.h"
#include "Fire.h"
#include "Dino_Stego.h"
#include "Dino_Yutyrannus.h"
#include "Dino_Ankylo.h"
#include "FireTest.h"
#include "FireSmoke.h"
#include "StructDust.h"
#include "BloodParticle.h"
#include "Buff.h"
#include "Roar.h"
#include "Goal.h"

// Effect Mesh
#include "EffectObject.h"
#include "EffectObjectRev.h"
#include "EffectObjectArrow.h"

// Water
#include "WaterFall.h"
#include "Water.h"
#include "Dino_Egg.h"

// ShadowPos Test
#include "ShadowPosTest.h"
#include "Basketballhoop.h"
#include "Dodo_Ball.h"

USING(Engine)

CLoading::CLoading(LPDIRECT3DDEVICE9 _pDevice)
	: m_pDevice(_pDevice)
{
	m_pDevice->AddRef();
}

HRESULT CLoading::Ready_Loading(LOADINGID _eLoadingID, CStage** _eStage)
{
	m_eLoadingID = _eLoadingID;
	
	// 크리티컬 세션 생성
	InitializeCriticalSection(&m_Crt);

	pExportScene.pStage = _eStage;
	pExportScene.pLoading = this;

	// 스레드 시작
	m_hThread = (HANDLE*)_beginthreadex(NULL, 0, Thread_Main, &pExportScene, 0, NULL);

	return S_OK;
}

_uint CLoading::Loading_ForStage(CStage** _eStage)
{
	/// 스테이지 리소스 로드 (예외처리 할 것 !!)

	m_stLoadingCount.iLoadingBarCount = 0;
	// 다이나믹 매쉬
	m_wstrLoadNote = L"DynamicMesh Loading...............";
	FAILED_CHECK_MSG(Protot_DynamicMesh_ForStage(), L"Failed Load DynamicMesh");

	// 컴포넌트
	m_wstrLoadNote = L"Component Loading...............";

	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_VIBuffer_RectColor", CVIBuffer_RectColor::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_VIBuffer_RectTex", CVIBuffer_RectTex::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_Calculator", CCalculator::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_Optimization", COptimization::Create(m_pDevice)), E_FAIL);

	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_VIBuffer_Terrain", CVIBuffer_Terrain::Create(m_pDevice, VTXCNTX, VTXCNTZ, 1, false)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_Inven", CInven::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_SphereCollider", CSphereCollider::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_VIBuffer_ParticleTex", CVIBuffer_ParticleTex::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_VIBuffer_FireParticleTex", CVIBuffer_FireParticleTex::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_VIBuffer_FireSmokeParticleTex", CVIBuffer_FireSmokeParticleTex::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_VIBuffer_StructDustParticleTex", CVIBuffer_StructDustParticleTex::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_VIBuffer_BloodParticleTex", CVIBuffer_BloodParticleTex::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_VIBuffer_GoalParticleTex", CVIBuffer_GoalParticleTex::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_VIBuffer_WaterTerrain", CVIBuffer_WaterTerrain::Create(m_pDevice, VTXCNTX, VTXCNTZ, 1)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Com_VIBuffer_CubeTex", CVIBuffer_CubeTex::Create(m_pDevice)), E_FAIL);

	// 텍스처
	m_wstrLoadNote = L"Texture Loading...............";
	FAILED_CHECK_MSG(Protot_Texture_ForStage(), L"Failed Load Textures");

	// 스태틱 매쉬
	m_wstrLoadNote = L"StaticMesh Loading...............";
	FAILED_CHECK_MSG(Protot_StaticMesh_ForStage(), L"Failed Load StaticMesh");

	//지형 및 타일
	m_wstrLoadNote = L"Terrain Loading...............";
	//FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Terrain", CTerrain::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Tile", CTile::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_SkyDome", CSkyDome::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Area", CArea::Create(m_pDevice)), E_FAIL);

	///////////////////////////////////UI 간이 디버깅 확인용 주석
	// 게임오브젝트
	m_wstrLoadNote = L"GameObject Loading...............";
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Player", CPlayer::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Rex", CDino_Rex::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Para", CDino_Para::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Iguanodon", CDino_Iguanodon::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Brachio", CDino_Brachio::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Therizino", CDino_Therizino::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Allo", CDino_Allo::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Raptor", CDino_Raptor::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Dodo", CDino_Dodo::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Stego", CDino_Stego::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Yutyrannus", CDino_Yutyrannus::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Ankylo", CDino_Ankylo::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Quetzal", CDino_Quetzal::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Ptero", CDino_Ptero::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Dragon", CDino_Dragon::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dino_Egg", CDino_Egg::Create(m_pDevice)), E_FAIL);
	///////////////////////////////////////////////////////

	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Dodo_Ball", CDodo_Ball::Create(m_pDevice)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Weapon", CWeapon::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Bullet", CBullet::Create(m_pDevice)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Campfire", CCampfire::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_SleepingBag", CSleepingBag::Create(m_pDevice)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Forge", CForge::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_MortarPestal", CMortarPestal::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Smithy", CSmithy::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_ThatchBed", CThatchBed::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_WallTorch", CWallTorch::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_WaterWell", CWaterWell::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Barricade", CBarricade::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_CompostBin", CCompostBin::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_CropPlot", CCropPlot::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_FeedingTrough", CFeedingTrough::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Fireplace", CFireplace::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Storage", CStorage::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_CookingCampFire", CCookingCampfire::Create(m_pDevice)), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Thatch_Floor", CThatch_Floor::Create(m_pDevice)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Thatch_Wall", CThatch_Wall::Create(m_pDevice)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Thatch_Roof", CThatch_Roof::Create(m_pDevice)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Thatch_DoorFrame", CThatch_DoorFrame::Create(m_pDevice)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Thatch_Door", CThatch_Door::Create(m_pDevice)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_GateFrame", CGateFrame::Create(m_pDevice)), E_FAIL);


	///////////////////////////////////UI 간이 디버깅 확인용 주석
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Thatch_Floor", CThatch_Floor::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Thatch_Wall", CThatch_Wall::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Thatch_Roof", CThatch_Roof::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Thatch_DoorFrame", CThatch_DoorFrame::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Thatch_Door", CThatch_Door::Create(m_pDevice)), E_FAIL);
	
	// 플랫폼 안장
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_PlatformSaddle_Brachio", CPlatformSaddle_Brachio::Create(m_pDevice)), E_FAIL);
	
	// 스테이지 오브젝트 프로토타입
	m_wstrLoadNote = L"Stage GameObject Ready...............";
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Rock_Small", CHarvestable_Rock::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Pebble", CHarvestable_Pebble::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Tree", CHarvestable_Tree::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Bush", CHarvestable_Bush::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Obstacle", CObstacle::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_BasketballHoop", CBasketballHoop::Create(m_pDevice)), E_FAIL);
	////////////////////////////////////////////////////////////


	// UI
	m_wstrLoadNote = L"UI Loading...............";
	FAILED_CHECK_MSG(Protot_UI_ForStage(), L"Failed Load UI");
	
	// Effect 프로토타입
	m_wstrLoadNote = L"Effect Ready...............";
 	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Effect_Blood", CBlood::Create(m_pDevice)), E_FAIL);
 	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Effect_BloodTest", CBloodTest::Create(m_pDevice)), E_FAIL);
 	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Effect_Dust", CDust::Create(m_pDevice)), E_FAIL);
 	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Effect_Fire", CFire::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Effect_FireTest", CFireTest::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Effect_FireSmoke", CFireSmoke::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Effect_StructDust", CStructDust::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_Effect_BloodParticle", CBloodParticle::Create(m_pDevice)), E_FAIL);

	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Effect_Buff", CBuff::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Effect_Roar", CRoar::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Effect_Goal", CGoal::Create(m_pDevice)), E_FAIL);

	// Effect Mesh 프로토타입
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_EffectObject", CEffectObject::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_EffectObjectRev", CEffectObjectRev::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_EffectObjectArrow", CEffectObjectArrow::Create(m_pDevice)), E_FAIL);

	// Water 관련 프로토타입
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Obj_WaterFall", CWaterFall::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_Water", CWater::Create(m_pDevice)), E_FAIL);
	
	// Shadow 조명 위치 확인용
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Obj_ShadowPosTest", CShadowPosTest::Create(m_pDevice)), E_FAIL);
	
	// 스테이지 세팅
	m_wstrLoadNote = L"Stage Loading...............";
	*_eStage = CStage::Create(m_pDevice, &m_stLoadingCount);
	NULL_CHECK_RETURN(_eStage, E_FAIL);

	m_bIsFinish = true;

	m_wstrLoadNote = L"Load Complete!";

	return 0;
}

const wstring & CLoading::Get_String(void)
{
	if (m_stLoadingCount.iLoadingCount)
	{
		TCHAR szCount[64];
		wsprintf(szCount, L"Stage Loading... (%d)", m_stLoadingCount.iLoadingCount);
		//wprintf_s(szCount, m_iLoadingCount);
		m_wstrLoadNote = szCount;
	}

	return m_wstrLoadNote;
}

HRESULT CLoading::Set_String(const wstring & wstrText)
{
	m_wstrLoadNote = wstrText;
	return S_OK;
}

const _int & CLoading::Add_Count(const _int & iAdd)
{
	return (m_stLoadingCount.iLoadingCount += iAdd);
}

unsigned int CLoading::Thread_Main(void * _pArg)
{
	CLoading* pLoading = ((EXPORTSCENE*)_pArg)->pLoading;

	_uint iResult = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iResult = pLoading->Loading_ForStage(((EXPORTSCENE*)_pArg)->pStage);
		break;
	default:
		iResult = 1;
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iResult;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 _pDevice, LOADINGID _eLoadingID, CStage** _eStage)
{
	CLoading*	pInstance = new CLoading(_pDevice);

	if (FAILED(pInstance->Ready_Loading(_eLoadingID, _eStage)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoading::Free()
{
	// 스레드가 끝나기 전까지 대기
	WaitForSingleObject(m_hThread, INFINITE);

	// 스레드 통로 닫기
	CloseHandle(m_hThread);

	// 크리티컬 세션 삭제
	DeleteCriticalSection(&m_Crt);

	// 디바이스 릴리즈
	Safe_Release(m_pDevice);
}

HRESULT CLoading::Protot_StaticMesh_ForStage()
{
	// 드랍 아이탬
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Backpack", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Backpack/", L"Backpack_Lod2_Bin.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_DropBox", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/DropBox/", L"DropBox_Lod1_Bin.X")), E_FAIL);

	/// 구조물
	// 설치물
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_CampFire", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/CampFire/", L"CampFire.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_Forge", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/Forge/", L"Forge.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_MortarPestal", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/MortarPestal/", L"MortarPestal.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_SleepingBag", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/SleepingBag/", L"SleepingBag_Lod1.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_Smithy", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/WorkBench/", L"WorkBench.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchBed", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchBed/", L"ThatchBed.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_WallTorch", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/WallTorch/", L"WallTorch.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_WaterWell", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/WaterWell/", L"WaterWell.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_WoodBarricade", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/WoodBarricade/", L"WoodBarricade.X")), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_CompostBin", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/CompostBin/", L"CompostBin.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_CompostBin_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/CompostBin/", L"CompostBin_Lod4.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_CropPlot", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/CropPlotLarge/", L"CropPlotLarge.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_CropPlot_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/CropPlotLarge/", L"CropPlotLarge_Lod3.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_FeedingTrough", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/FeedingTrough/", L"FeedingTrough.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_FeedingTrough_Empty", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/FeedingTrough/", L"FeedingTrough_Empty.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_FeedingTrough_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/FeedingTrough/", L"FeedingTrough_Lod3.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_Fireplace", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/Fireplace/", L"Fireplace.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_Fireplace_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/Fireplace/", L"Fireplace_Lod2.X")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_S_Storage", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/S_Storage/", L"S_Storage.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_S_Storage_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/S_Storage/", L"S_Storage_Lod3.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_L_Storage", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/L_Storage/", L"L_Storage.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_L_Storage_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/L_Storage/", L"L_Storage_Lod3.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_CookingCampFire", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/CookingCampFire/", L"CookingCampFire.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_CookingCampFire_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/CookingCampFire/", L"CookingCampFire_Lod3.X")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_WoodSloped", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/WoodSloped/", L"WoodSloped.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_WoodSloped_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/WoodSloped/", L"WoodSloped_Lod3.X")), E_FAIL);

	// 짚 건축물
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchFloor", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchFloor/", L"ThatchFloor.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchRoof", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchRoof/", L"ThatchRoof.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchWall", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchWall/", L"ThatchWall.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchDoorFrame", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchDoorFrame/", L"ThatchDoorFrame.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchDoor", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchDoor/", L"ThatchDoor.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchDoorFrame_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchDoorFrame/", L"Thatch_DoorFrame_Lod4.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchSlopedRoof", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchSlopedRoof/", L"ThatchSlopedRoof.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchSlopedRoof_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchSlopedRoof/", L"ThatchSlopedRoof_Lod3.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchSlopedRoofLeft", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchSlopedRoofLeft/", L"ThatchSlopedRoofLeft.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchSlopedRoofLeft_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchSlopedRoofLeft/", L"ThatchSlopedRoofLeft_Lod3.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchSlopedRoofRight", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchSlopedRoofRight/", L"ThatchSlopedRoofRight.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_ThatchSlopedRoofRight_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/ThatchSlopedRoofRight/", L"ThatchSlopedRoofRight_Lod3.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_WoodStairs", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/WoodStairs/", L"WoodStairs.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_WoodStairs_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/WoodStairs/", L"WoodStairs_Lod2.X")), E_FAIL);

	// 석재 건축물
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneFloor", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneFloor/", L"StoneFloor.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneRoof", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneRoof/", L"StoneRoof.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneWall", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneWall/", L"StoneWall.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneWallWindow", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneWallWindow/", L"StoneWallWindow.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneWallWindow_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneWallWindow/", L"StoneWallWindow_Lod3.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneDoorFrame", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneDoorFrame/", L"StoneDoorFrame.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneDoor", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneDoor/", L"StoneDoor.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneSlopedRoof", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneSlopedRoof/", L"StoneSlopedRoof.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneSlopedRoofLeft", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneSlopedRoofLeft/", L"StoneSlopedRoofLeft.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneSlopedRoofRight", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneSlopedRoofRight/", L"StoneSlopedRoofRight.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneStairs", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneStairs/", L"StoneStairs.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneStairs_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneStairs/", L"StoneStairs_Lod2.X")), E_FAIL);

	// 기타 건축물
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneGateFrame", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneGateFrame/", L"StoneGateFrame_Bin.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneGateFrame_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneGateFrame/", L"StoneGateFrame_Bin_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneGateDoor", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneGateDoor/", L"StoneGateDoor.X")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Structure_StoneGateDoor_Lod", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Structure/StoneGateDoor/", L"StoneGateDoor_Lod3.X")), E_FAIL);


	// 플랫폼 안장
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Brachio_Platform", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Platform_Saddle/Brachio_Platform/", L"Brachio_Platform.X")), E_FAIL);

	// 투척물
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bola_Thrown", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Bola_Thrown/", L"Bola_Thrown.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bola_Wrapped", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Bola_Wrapped/", L"Bola_Wrapped.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bow_Arrow", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Bow_Arrow/", L"Bow_Arrow_Lod1.X")), E_FAIL);

	// 무기
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Hatchet_TPV", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Hatchet_TPV/", L"Hatchet_TPV_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Hatchet_FPV", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/hatchet_FPV/", L"hatchet_FPV.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_StonePick_TPV", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/StonePick_TPV/", L"StonePick_TPV_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_StonePick_FPV", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/StonePick_FPV/", L"StonePick_FPV.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_WoodClub_TPV", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/WoodClub_TPV/", L"WoodClub_TPV_LoD2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_WoodClub_FPV", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/WoodClub_FPV/", L"WoodClub_FPV.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Spear_TPV", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Spear_TPV/", L"Spear_TPV_Lod3.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Spear_FPV", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Spear_FPV/", L"Spear_FPV.X")), E_FAIL);

	// 방어구
	//FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Burlap_Hat", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Hat/Burlap_Hat/", L"Burlap_Hat.X")), E_FAIL);

	//알
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Dino_Egg", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Dino_Egg/", L"Dino_Egg_Lod0.X")), E_FAIL);

	//지형 타일
	for (_ulong i = 0; i < 3; i++)
	{
	
		TCHAR szTagName[MAX_PATH] = L"";
		TCHAR szFileName[MAX_PATH] = L"";
		wsprintf(szTagName, L"Proto_StaticMesh_Bio_Tile_%d", i);
		wsprintf(szFileName, L"Bio_Tile_%d.X", i);
	
		FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(szTagName, CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Tile/Bio_Origin/", szFileName)), E_FAIL);
	}

	///////////////////////////////////UI 간이 디버깅 확인용 주석
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Arena_1", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Tile/Arena/", L"Arena_1.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Arena_2", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Tile/Arena/", L"Arena_2.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_ArenaSky", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Tile/Arena/", L"ArenaSky.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_ArenaSmoke", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Tile/Arena/", L"ArenaSmoke_1.X")), E_FAIL);
	
	//채집물 Layer_Harvest_Pebble
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Pebble_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Pebble_ABC/", L"Pebble_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Pebble_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Pebble_ABC/", L"Pebble_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Pebble_C", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Pebble_ABC/", L"Pebble_C_Lod2.X")), E_FAIL);
	
	//채집물 Layer_Harvest_Bush
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bush_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Bush_A/", L"Bush_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bush_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Bush_B/", L"Bush_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bush_C", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Bush_C/", L"Bush_C_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bush_D", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Bush_Fern/", L"Bush_F.X")), E_FAIL);
	
	//채집물 Layer_Harvest_Rock
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Large_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Large_A/", L"Rock_Large_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Large_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Large_B/", L"Rock_Large_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Large_C", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Large_C/", L"Rock_Large_C_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Medium_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Medium_A/", L"Rock_Medium_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Medium_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Medium_B/", L"Rock_Medium_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Small_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Small_A/", L"Rock_Small_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Small_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Small_B/", L"Rock_Small_B_Lod2.X")), E_FAIL);
	
	//채집물 Layer_Harvest_Tree
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tree_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Tree_A/", L"Tree_A.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tree_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Tree_B/", L"Tree_B.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tree_Mushroom_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Tree_Mushroom_A/", L"Tree_Mushroom_A.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tree_Mushroom_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Tree_Mushroom_B/", L"Tree_Mushroom_B.X")), E_FAIL);

	//지형, 장애물 Layer_Obstacle
	//0 : Null
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bridge_Half_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Bridge_Half_A/", L"Bridge_Half_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bridge_Half_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Bridge_Half_B/", L"Bridge_Half_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bridge_Half_C", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Bridge_Half_C/", L"Bridge_Half_C_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bridge_Half_D", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Bridge_Half_D/", L"Bridge_Half_D_Lod2.X")), E_FAIL);
	
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bridge_Half_E", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Bridge_Half_E/", L"Bridge_Half_E_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Cliff_Huge_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Cliff_Huge_A/", L"Cliff_Huge_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Cliff_Huge_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Cliff_Huge_B/", L"Cliff_Huge_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Cliff_Huge_C", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Cliff_Huge_C/", L"Cliff_Huge_C_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Cliff_Huge_D", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Cliff_Huge_D/", L"Cliff_Huge_D_Lod2.X")), E_FAIL);
	//10
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_ObeliskTree", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/ObeliskTree/", L"ObeliskTree.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Huge_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Rock_Huge_A/", L"Rock_Huge_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Huge_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Rock_Huge_B/", L"Rock_Huge_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Huge_C", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Rock_Huge_C/", L"Rock_Huge_C_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tek_EntranceArch", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Tek_EntranceArch/", L"Tek_EntranceArch_Lod2.X")), E_FAIL);

	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tek_EntranceFloor", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Tek_EntranceFloor/", L"Tek_EntranceFloor.X")), E_FAIL);	//LOD 없음
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tek_EntranceWall", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Tek_EntranceWall/", L"Tek_EntranceWall_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tek_Floor", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Tek_Floor/", L"Tek_Floor.X")), E_FAIL);	//LOD 없음
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tek_FloorRamp", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Tek_FloorRamp/", L"Tek_FloorRamp_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tek_Iris_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Tek_Iris_A/", L"Tek_Iris_A_Lod2.X")), E_FAIL);
	//20
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tek_Iris_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Tek_Iris_B/", L"Tek_Iris_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tek_Lib", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Tek_Lib/", L"Tek_Lib_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tek_Portal", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Tek_Portal/", L"Tek_Portal_Lod3.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tek_RailSupport", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Tek_RailSupport/", L"Tek_RailSupport_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tek_Ramp", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Tek_Ramp/", L"Tek_Ramp_Lod2.X")), E_FAIL);

	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Tek_Tunnel", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Tek_Tunnel/", L"Tek_Tunnel_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Obelisk", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Obelisk/", L"Obelisk_Lod1.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Mountain_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Mountain_A/", L"Mountain_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Mountain_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Mountain_B/", L"Mountain_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Mountain_C", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Mountain_C/", L"Mountain_C_Lod2.X")), E_FAIL);
	//30
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_RockDetail", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/RockDetail/", L"RockDetail_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Cliff_Square_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Cliff_Square_A/", L"Cliff_Square_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Cliff_Square_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Cliff_Square_B/", L"Cliff_Square_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Cliff_Square_C", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Cliff_Square_C/", L"Cliff_Square_C_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Platform_Huge_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Platform_Huge_A/", L"Platform_Huge_A_Lod2.X")), E_FAIL);

	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Platform_Huge_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Platform_Huge_B/", L"Platform_Huge_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Platform_Huge_C", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Platform_Huge_C/", L"Platform_Huge_C_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Platform_Vertical_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Platform_Vertical_A/", L"Platform_Vertical_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Platform_Vertical_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Platform_Vertical_B/", L"Platform_Vertical_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Platform_Vertical_C", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Platform_Vertical_C/", L"Platform_Vertical_C_Lod2.X")), E_FAIL);
	//40
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Platform_Medium_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Platform_Medium_A/", L"Platform_Medium_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Platform_Medium_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Platform_Medium_B/", L"Platform_Medium_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Platform_Medium_C", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Platform_Medium_C/", L"Platform_Medium_C_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Platform_Small_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Platform_Small_A/", L"Platform_Small_A_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Platform_Small_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Platform_Small_B/", L"Platform_Small_B_Lod2.X")), E_FAIL);

	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Platform_Small_C", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/Platform_Small_C/", L"Platform_Small_C_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_RedWood_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/RedWood_AB/", L"RedWood_A_Lod3.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_RedWood_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/RedWood_AB/", L"RedWood_B_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_BurnedTree", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/BurnedTree/", L"BurnedTree.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_TekRuins_Vine_A", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/TekRuins_VineAB/", L"TekRuins_VineA.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_StaticMesh_TekRuins_Vine_B", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Obstacle/TekRuins_VineAB/", L"TekRuins_VineB.X")), E_FAIL);
	////////////////////////////////////////////////////////////

 	//스카이돔, 기타
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_SkyDome", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/SkyDome/", L"SkyDome.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_BasketballHoop", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Bogsketball/", L"Bogsketball.X")), E_FAIL);

 	// Effect Mesh
 	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_EffectObject", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Effects/", L"AngledCorn.X")), E_FAIL);
 	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_EffectObjectFunnel", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Effects/", L"Funnel.X")), E_FAIL);

	// Water Fall
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_WaterFall", CStaticMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/WaterFall/", L"WaterFall.X")), E_FAIL);

	return S_OK;
}

HRESULT CLoading::Protot_DynamicMesh_ForStage()
{

	/// 플레이어
	m_wstrLoadNote = L"DM_Player_Male_TPV Loading...............";
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Player_Male_TPV", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Player_Male_TPV_Outfit/", L"Player_Male_TPV_Outfit_Lod2_Bin.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Player_Male_FPV", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Player_Male_FPV_DMWP/", L"Player_Male_FPV_DMWP.X")), E_FAIL);

	/// 공룡
	// 렉스
	m_wstrLoadNote = L"DM_Dino Loading...............";

	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Dodo", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Dodo/", L"Dino_Dodo_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Raptor", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Raptor/", L"Dino_Raptor_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Rex", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Rex/", L"Rex_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Para", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Para/", L"Para_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Iguanodon", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Iguanodon/", L"Dino_Iguanodon_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Sauropod", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Sauropod/", L"Dino_Sauropod.X")), E_FAIL);
	//FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Brachio", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Brachio/", L"Dino_Brachio_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Therizino", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Therizino/", L"Dino_Therizino_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Allo", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Allo/", L"Dino_Allo_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Stego", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Stego/", L"Dino_Stego_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Yutyrannus", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Yutyrannus/", L"Dino_Yutyrannus_Lod1.X")), E_FAIL)
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Ankylo", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Ankylo/", L"Dino_Ankylo_Lod2.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Quetzal", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Quetzal/", L"Dino_Quetzal_Lod1.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Ptero", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Ptero/", L"Dino_Ptero_Lod2.X")), E_FAIL);


	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Dragon", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Dragon/", L"Dino_Dragon_Lod0.X")), E_FAIL);
	////////////////////////////////////////////////////////////

	//FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dino_Rex", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dino_Rex_New/", L"Rex_New_Lod2.X")), E_FAIL);
	//FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Dodo", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Dodo/", L"Dodo.X")), E_FAIL);

	// 무기
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Weapon_Bola_TPV", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Weapon_Bola_TPV/", L"Weapon_Bola_TPV.X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Weapon_Bow_TPV", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Weapon_Bow_TPV/", L"Weapon_Bow_TPV_Lod2.X")), E_FAIL);

	// 방어구
	//m_wstrLoadNote = L"DM_Burlap_Shirt Loading...............";
	//FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_DynamicMesh_Burlap_Shirt", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/DynamicMesh/Shirt/Burlap_Shirt/", L"Burlap_Shirt.X")), E_FAIL);

	// 채집물 (다이나믹메시)
	///////////////////////////////////UI 간이 디버깅 확인용 주석
	m_wstrLoadNote = L"DM_Harvestable Loading...............";
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bush_A_DM", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Bush_A/", L"Bush_A(DM).X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bush_B_DM", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Bush_B/", L"Bush_B(DM).X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bush_C_DM", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Bush_C/", L"Bush_C(DM).X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Bush_D_DM", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Bush_Fern/", L"Bush_F(DM).X")), E_FAIL);

	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Small_A_DM", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Small_A/", L"Rock_Small_A(DM).X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Small_B_DM", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Small_B/", L"Rock_Small_B(DM).X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Medium_A_DM", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Medium_A/", L"Rock_Medium_A(DM).X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Medium_B_DM", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Medium_B/", L"Rock_Medium_B(DM).X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Large_A_DM", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Large_A/", L"Rock_Large_A(DM).X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Large_B_DM", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Large_B/", L"Rock_Large_B(DM).X")), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_StaticMesh_Rock_Large_C_DM", CDynamicMesh::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/Harvestable/Rock_Large_C/", L"Rock_Large_C(DM).X")), E_FAIL);
	////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CLoading::Protot_UI_ForStage()
{
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_UI_Border", CBorder::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_UI_Inventory", CInventory::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_UI_InvenTap", CInvenTap::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_UI_InvenIcon_Transfer", CInvenTransfer::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_UI_InvenIcon_DropAll", CInvenDropAll::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_UI_InvenIcon_ItemSort", CInvenItemSort::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_UI_InvenIcon_Folder", CInvenFolder::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_UI_InvenIcon_Category", CInvenCategory::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_UI_InvenIcon_ToolTip", CInvenToolTip::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_UI_InvenIcon_Searchbar", CInvenSearchbar::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_UI_InvenIcon_InvenItem", CInvenItem::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Left_Inventory", CLeft_Inventory::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Right_Inventory", CRight_Inventory::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_Scroll_UI", CScroll_UI::Create(m_pDevice)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Stat_UI", CStat_UI::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_MonsterStat_UI", CMonsterStat_UI::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_StructureStat_UI", CStructureStat_UI::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Back_Stat_UI", CBack_Stat_UI::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Back_MonsterStat_UI", CBack_MonsterStat_UI::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Back_StructureStat_UI", CBack_StructureStat_UI::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Stat_UI_Bar", CStat_UI_Bar::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_MonsterStat_UI_Bar", CMonsterStat_UI_Bar::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_StructureStat_UI_Bar", CStructureStat_UI_Bar::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_CenterTap", CCenterTap::Create(m_pDevice)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_SummaryStat_UI", CSummaryStat_UI::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_SummaryStat_UI_Monster", CSummaryStat_UI_Monster::Create(m_pDevice)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_QuickSlot_UI", CQuickSlot_UI::Create(m_pDevice)), E_FAIL);
	
	//가이드 폰트
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_GuideFont_UI", CGuideFont_UI::Create(m_pDevice)), E_FAIL);

	//InfoView_UI
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_InfoView_UI", CInfoView_UI::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_Item_Info_View_UI", CItem_Info_View_UI::Create(m_pDevice)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_WorldFont_UI", CWorldFont_UI::Create(m_pDevice)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_PlayerHit_UI", CPlayerHit_UI::Create(m_pDevice)), E_FAIL);
	
	//MiniMap
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_MiniMap_UI", CMiniMap_UI::Create(m_pDevice)), E_FAIL);
		
	///////////////////////////테이밍 네임 
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_TamingName_UI", CTamingName_UI::Create(m_pDevice)), E_FAIL);

	/////////////////////////////링커멘드
	FAILED_CHECK_RETURN(Engine::Add_GameObject_Prototype(L"Proto_RingCommand_UI", CRingCommand_UI::Create(m_pDevice)), E_FAIL);

	/////////////////////////////크로스 헤어
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_GameObject_Prototype(L"Proto_CrossHair_UI", CCrossHair_UI::Create(m_pDevice)), E_FAIL);

	return S_OK;
}

HRESULT CLoading::Protot_Texture_ForStage()
{
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Inventory", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Big_Background%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Inventory_Filter", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/T_UI_HexPack%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_InvenTap_Left", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/InventoryTabSeparator%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_InvenTap_Right", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/InventoryTabSeparatorMirrored%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_InvenIcon_Transfer", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/IconTransfer%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_InvenIcon_DropAll", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/IconDropAll%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_InvenIcon_ItemSort", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/IconSort%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_InvenIcon_Folder", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/IconFolder%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_InvenIcon_Category", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/IconCategory%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_InvenIcon_ToolTip", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/IconToolTip%d.tga", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_InvenIcon_Box_Nomal", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/ItemHighlightBox%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Texture_InvenIcon_Box_Highlight", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/ItemNormalBox%d.tga", TEX_NORMAL)), E_FAIL);
	//FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Texture_BaseListWideButtonHighlight", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Stat_Icon/BaseListWideButtonHighlight%d.tga", TEX_NORMAL)), E_FAIL);
	//FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Texture_BaseListWideButtonNormal", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Stat_Icon/BaseListWideButtonNormal%d.tga", TEX_NORMAL)), E_FAIL);

	///////////////////Center UI
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_CenterTap_Left", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/StatTabSeparator%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_CenterTap_Right", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/StatTabSeparatorMirrored%d.tga", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_CraftSpeed", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Stat_Icon/CraftSpeed_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Fortitude", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Stat_Icon/Fortitude_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Melee", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Stat_Icon/Melee_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Speed", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Stat_Icon/Speed_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Torpid", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Stat_Icon/Torpid_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_PanelTabTextGlow", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Stat_Icon/PanelTabTextGlow%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Texture_ArrowUp", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Stat_Icon/ArrowUp%d.tga", TEX_NORMAL)), E_FAIL);

	///////////////////SummaryStat_UI
	
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_Amount", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Storage_Icon_White%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_Food", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Food_Icon_White%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_Food_Fill", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Food_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_Health", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Health_Icon_White%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_Health_Fill", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Health_Icon%d.tga", TEX_NORMAL)), E_FAIL); 
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_Stamina", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Stamina_Icon_White%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_Stamina_Fill", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Stamina_Icon%d.tga", TEX_NORMAL)), E_FAIL); 
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_Water", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Water_Icon_White%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_Water_Fill", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Water_Icon%d.tga", TEX_NORMAL)), E_FAIL); 
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_Weight", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Weight_Icon_White%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_Weight_Fill", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/Weight_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_XP", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/XP_Icon_White%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SummaryStat_XP_Fill", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/SummaryStat/XP_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Texture_PlusStat", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Plus_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	
	////////////////////InfoView_UI
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_InfoView_Bg", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/ToolTipBg%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_SplittingLine", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/InfoView/SplittingLine%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_T_InventoryItemGlowLines", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/InfoView/T_InventoryItemGlowLines%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Empty_Rex", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/InfoView/Empty_Rex%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Empty_Para", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/InfoView/Empty_Para%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Empty_Igu", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/InfoView/Empty_Igu%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Empty_Allo", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/InfoView/Empty_Allo%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Empty_Rap", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/InfoView/Empty_Rap%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Empty_Dodo", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/InfoView/Empty_Dodo%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Texture_Empty_Theri", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/InfoView/Empty_Therizino%d.tga", TEX_NORMAL)), E_FAIL);
	//브라키오 테리지노 없움
	
	////////////////////////MiniMap
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_MiniMap", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Map/ArkMap%d.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Ping", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Map/Ping%d.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Compass", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Map/Compass%d.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_BedUnSelect", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Map/IconBedUnselected%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Texture_BedSelect", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Map/IconBedSelected%d.tga", TEX_NORMAL)), E_FAIL);
	
	////////////////////////RingCommand
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_RingCommand", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Ring_Command/RingCommand%d.png", TEX_NORMAL, 3)), E_FAIL);

	////////////////////////Status_Icon
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Status_Icon", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/StatusIcon/Status_Icon%d.tga", TEX_NORMAL,9)), E_FAIL);

	////////////////////////CrossHair
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_CrossHair", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/CrossHair/CrossHair%d.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Texture_HitMarker", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/CrossHair/HitMarker%d.tga", TEX_NORMAL)), E_FAIL);


	//소비 아이템 사진
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_AmarBerry", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/AmarBerry%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"AmarBerry",L"아마르 베리", L"수분을 함량하여 약간의 배고픔과 목마름을 해결해 줍니다.", ITEMID_AmarBerry, 1, 120, 1, 0.1f, 100);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Meat", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Meat_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Meat", L"생고기", L"배고픔을 충족시키고, 소화되는 동안 체력을 회복시킵니다.", ITEMID_Meat, 3, 140, -5, 0.1f, 40);
	
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_CookedMeat", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/CookedMeat_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"CookedMeat", L"구운 고기", L"체력을 회복시키고 배고픔을 충족시켜주며, 소화되는 동안 체력을 회복시킵니다.", ITEMID_CookedMeat, 15, 600, 8, 0.1f, 40);
	Protot_MakingItem_ForStage(L"CookedMeat", L"구운 고기 제작", ITEMID_CookedMeat, 1, ITEMID_Meat, ITEMID_END, ITEMID_END, ITEMID_END, 1, 0, 0, 0, 5.0);
	

	//제작 아이템 사진
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Fiber", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Fiber_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Fiber", L"섬유", L"섬세하게 식물 가닥을 모았습니다. 실, 천, 로프를 만들기 좋습니다.", ITEMID_Fiber, 0, 0, 0, 0.1f, 300);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Flint", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Flint_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Flint", L"부싯돌", L"일반 돌보다 날카로운 부싯돌", ITEMID_Flint, 0, 0, 0, 0.2f, 100);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Stone", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Stone_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Stone", L"돌", L"무거운 돌 덩어리", ITEMID_Stone, 0, 0, 0, 0.2f, 100);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Thatch", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Thatch_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Thatch", L"짚", L"나무에서 뜯겨진 조각. 원시 매듭에 유용합니다.", ITEMID_Thatch, 0, 0, 0, 0.1f, 200);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Wood", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Wood_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Wood", L"목재", L"나무에서 견고하게 절단된 두꺼운 목재.", ITEMID_Wood, 0, 0, 0, 0.1f, 100);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Hide", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Hide_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Hide", L"가죽", L"피부의 두꺼운 부분, 죽은 동물로부터 잘라 냈습니다.", ITEMID_Hide, 0, 0, 0, 0.1f, 200);

	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_MetalOre", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/MetalOre_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"MetalOre", L"철", L"정제되지 않은 철광석 덩어리. 높은 산이나 동굴에서 발견되는 바위에서 주로 채굴합니다. 화로에서 정제할 수 있습니다.", ITEMID_MetalOre, 0, 0, 0, 1.f, 300);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Texture_MetalIngot", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/MetalIngot_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"MetalIngot", L"철 주괴", L"철광석을 화로에서 정제하여 만들어집니다.", ITEMID_MetalIngot, 0, 0, 0, 1.f, 200);
	Protot_MakingItem_ForStage(L"MetalIngot", L"철 주괴 제작", ITEMID_MetalIngot, 1, ITEMID_MetalOre, ITEMID_END, ITEMID_END, ITEMID_END
		, 2, 0, 0, 0, 5.0);


	//무기 아이템 사진
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Club", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Weapon/Club_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Club", L"목제 둔기", L"대상을 기절시키기에 최적인 쉽게 제작 가능한 근접무기. 단거리에서만 효과적입니다.", ITEMID_Club, 5 ,40,20,2.0f);
	Protot_MakingItem_ForStage(L"Club", L"목제 둔기 제작", ITEMID_Club,1, ITEMID_Wood, ITEMID_Fiber, ITEMID_END, ITEMID_END
		,4,15,0,0,2.0,2);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_StoneAx", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Weapon/StoneHatchet_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"StoneAx", L"돌 도끼", L"나무에서 목재, 바위에서 돌, 시체에서 가죽을 얻기 위한 날카로운 손도끼.", ITEMID_StoneAx, 20, 40,10,2.0f);
	Protot_MakingItem_ForStage(L"StoneAx", L"돌 도끼 제작", ITEMID_StoneAx, 1, ITEMID_Thatch, ITEMID_Flint, ITEMID_Wood, ITEMID_END
		, 10, 1, 1, 0, 3.0,1);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_StonePick", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Weapon/Stonepick_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"StonePick", L"돌 곡괭이", L"주로 돌에서 부싯돌, 나무에서 짚, 시체에서 고기를 얻기 위한 대형 석재 곡괭이.", ITEMID_StonePick, 16, 40, 10, 2.0f);
	Protot_MakingItem_ForStage(L"StonePick", L"돌 곡괭이 제작", ITEMID_StonePick, 1, ITEMID_Thatch, ITEMID_Wood, ITEMID_Stone, ITEMID_END
		, 10, 1, 1, 0, 3.0,1);

	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Bow", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Weapon/Bow_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Bow", L"활", L"활의 대가는 종종 위대한 정복자가 되었습니다. 화살이 필요합니다.", ITEMID_Bow, 0/*화살에 따라 다름*/, 50, 0, 8.0f);
	//아이디	//한번에 만들 수 있는 갯수?//필요 재료 4개
	Protot_MakingItem_ForStage(L"Bow", L"활 제작", ITEMID_Bow, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_END, ITEMID_END
		, 15, 50, 0, 0, 2.0, 3);

													//아이디				//공격력		//내구도//스턴력//무게
	//FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_ForgedCrossbow", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/ForgedCrossbow_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	//Protot_Item_ForStage(L"ForgedCrossbow", L"쇠뇌", L"활보다 훨씬 더 강력하지만, 빠른 속도로 발사할 수 없습니다. 물속에서 발사할 수 있습니다.", ITEMID_ForgedCrossbow, 0/*화살에 따라 다름*/, 100, 0, 8.0f);
	//														//아이디	//한번에 만들 수 있는 갯수?//필요 재료 4개
	//Protot_MakingItem_ForStage(L"ForgedCrossbow", L"쇠뇌 제작", ITEMID_ForgedCrossbow, 1, ITEMID_MetalIngot, ITEMID_Wood, ITEMID_Fiber, ITEMID_END
	//	, 7, 10, 35, 0, 2.0,20);
	//필요 재료마다 필요 수량4개//제작시간/제작가능 렙 = 1

	//무기 - 소모 아이템 사진
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Bolas", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Weapon/Bolas_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Bolas", L"볼라", L"붕붕 돌리다가 던지세요!", ITEMID_Bolas, 0, 40, 20, 2.0f,10);
	Protot_MakingItem_ForStage(L"Bolas", L"볼라 제작", ITEMID_Bolas, 1, ITEMID_Thatch, ITEMID_Stone, ITEMID_Fiber, ITEMID_Hide
		, 10, 3, 15, 3, 2.0,2);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Spear", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Weapon/Spear_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Spear", L"투창", L"쉽게 제작 가능한 투척 가능한 근접 무기. 사용 시 파손 가능.", ITEMID_Spear, 5, 40, 20, 2.0f, 10);
	Protot_MakingItem_ForStage(L"Spear", L"투창 제작", ITEMID_Spear, 1, ITEMID_Wood, ITEMID_Flint, ITEMID_Fiber, ITEMID_END
		, 8, 2, 12, 0, 2.0,3);
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Texture_Arrow", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Weapon/Arrow_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Arrow", L"돌 화살", L"부싯돌 화살촉이 달린 기본 화살. 망가지지 않았다면 재장전이 가능합니다.", ITEMID_Arrow, 5, 40, 20, 2.0f, 100);
	Protot_MakingItem_ForStage(L"Arrow", L"돌 화살 제작", ITEMID_Arrow, 1, ITEMID_Thatch, ITEMID_Fiber, ITEMID_Flint, ITEMID_END
		, 2, 2, 1, 0, 1.0, 3);

	//장비 아이템 사진
	//가죽
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Leather_Hat", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Armor/HideHat_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Leather_Hat", L"가죽 모자", L"약간의 물리적인 보호와 보온을 해줍니다...", ITEMID_Leather_Hat,20, 45,0, 0.5f);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Leather_Top", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Armor/HideShirt_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Leather_Top", L"가죽 옷", L"약간의 물리적인 보호와 보온을 해줍니다...", ITEMID_Leather_TOP, 20, 45,1, 0.5f);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Leather_Legs", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Armor/HideLegs_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Leather_Legs", L"가죽 바지", L"약간의 물리적인 보호와 보온을 해줍니다...", ITEMID_Leather_Legs, 20, 45,2, 0.5f);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Leather_Gloves", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Armor/HideGloves_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Leather_Gloves", L"가죽 장갑", L"약간의 물리적인 보호와 보온을 해줍니다...", ITEMID_Leather_Gloves, 20, 45,3, 0.5f);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Leather_Boots", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Armor/HideBoots_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Leather_Boots", L"가죽 신발", L"약간의 물리적인 보호와 보온을 해줍니다...", ITEMID_Leather_Boots,20, 45,5, 0.5f);
	//방탄
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Metal_Hat", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Armor/MetalHat_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Metal_Hat", L"방탄 모자", L"약간의 물리적인 보호와 보온을 해줍니다...", ITEMID_Metal_Hat, 20, 45, 0, 0.5f);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Metal_Top", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Armor/MetalShirt_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Metal_Top", L"방탄 옷", L"약간의 물리적인 보호와 보온을 해줍니다...", ITEMID_Metal_TOP, 20, 45, 1, 0.5f);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Metal_Legs", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Armor/MetalPants_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Metal_Legs", L"방탄 바지", L"약간의 물리적인 보호와 보온을 해줍니다...", ITEMID_Metal_Legs, 20, 45, 2, 0.5f);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Metal_Gloves", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Armor/MetalGloves_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Metal_Gloves", L"방탄 장갑", L"약간의 물리적인 보호와 보온을 해줍니다...", ITEMID_Metal_Gloves, 20, 45, 3, 0.5f);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Metal_Boots", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Armor/MetalBoots_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Metal_Boots", L"방탄 신발", L"약간의 물리적인 보호와 보온을 해줍니다...", ITEMID_Metal_Boots, 20, 45, 5, 0.5f);

	//방패
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Wood_Shield", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Weapon/Shield_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Wood_Shield", L"나무 방패", L"사용 시 전방으로부터의 피해를 흡수합니다.", ITEMID_Wood_Sheild, 1000, 400, 4, 20.f);

	///구조물
	// 모닥불
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Campfire", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/Campfire_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Campfire", L"모닥불", L"고기를 요리하기 완벽하고, 따뜻함을 유지해주며 빛을 제공합니다.", ITEMID_Campfire, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"Campfire", L"모닥불 제작", ITEMID_Campfire, 1, ITEMID_Wood, ITEMID_Thatch, ITEMID_Stone, ITEMID_Flint, 2, 12, 16, 1, 2.0, 2);
	// 1회용 침낭
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Bed", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/SleepingBag_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Bed", L"침대", L"짚은 가장 편안한 매트리스가 될 수 없지만, 당신과 당신 부족을 위한 부활 포인트가 되어 줍니다.", ITEMID_SleepingBag, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"Bed", L"침대 제작", ITEMID_SleepingBag, 1, ITEMID_Wood, ITEMID_Fiber , ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 화로
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Forge", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/Forge2_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Forge", L"화로", L"화로입니다.", ITEMID_Forge, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"Forge", L"화로 제작", ITEMID_Forge, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 절구와 공이
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_MortarPestle", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/MortarPestle_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"MortarPestle", L"절구와 공이", L"절구와 공이입니다.", ITEMID_MortarPestle, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"MortarPestle", L"절구와 공이 제작", ITEMID_MortarPestle, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 대장간
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Smithy", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/Smithy_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Smithy", L"대장간", L"대장간입니다.", ITEMID_Smithy, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"Smithy", L"대장간 제작", ITEMID_Smithy, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 짚 침대
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_ThatchBed", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/Bed_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"ThatchBed", L"짚 침대", L"짚 침대입니다.", ITEMID_ThatchBed, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"ThatchBed", L"짚 침대 제작", ITEMID_ThatchBed, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 벽 횃불
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_WallTorch", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/PrimitiveWallMountedTorch_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"WallTorch", L"벽 횃불", L"벽 횃불입니다.", ITEMID_WallTorch, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"WallTorch", L"벽 횃불 제작", ITEMID_WallTorch, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 우물
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_WallWell", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/WaterWell_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"WallWell", L"우물", L"우물입니다.", ITEMID_WaterWell, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"WallWell", L"우물 제작", ITEMID_WaterWell, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 바리게이트
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Barricade", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/WoodBarricade_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Barricade", L"바리게이트", L"바리게이트입니다.", ITEMID_Barricade, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"Barricade", L"바리게이트 제작", ITEMID_Barricade, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 퇴비통
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_CompostBin", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/CompostBin%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"CompostBin", L"퇴비통", L"퇴비통입니다.", ITEMID_CompostBin, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"CompostBin", L"퇴비통 제작", ITEMID_CompostBin, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 작물밭
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_CropPlot", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/CropPlot_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"CropPlot", L"작물밭", L"작물밭입니다.", ITEMID_CropPlot, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"CropPlot", L"작물밭 제작", ITEMID_CropPlot, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 여물통
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_FeedingTrough", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/FeedingTrough_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"FeedingTrough", L"여물통", L"여물통입니다.", ITEMID_FeedingTrough, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"FeedingTrough", L"여물통 제작", ITEMID_FeedingTrough, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 벽난로
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Fireplace", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/Fireplace_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"Fireplace", L"벽난로", L"벽난로입니다.", ITEMID_Fireplace, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"Fireplace", L"벽난로 제작", ITEMID_Fireplace, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 작은 보관함
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_S_Storage", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/SmallStorage_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"S_Storage", L"작은 보관함", L"작은 보관함입니다.", ITEMID_S_Storage, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"S_Storage", L"작은 보관함 제작", ITEMID_S_Storage, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 큰 보관함
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_L_Storage", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/Bookshelf_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"L_Storage", L"큰 보관함", L"큰 보관함입니다.", ITEMID_L_Storage, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"L_Storage", L"큰 보관함 제작", ITEMID_L_Storage, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 요리용 냄비
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_CookingCampFire", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/CookingCampfire_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"CookingCampFire", L"요리용 냄비", L"요리용 냄비입니다.", ITEMID_CookingCampFire, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"CookingCampFire", L"요리용 냄비 제작", ITEMID_CookingCampFire, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	//// 나무 경사로
	//FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_L_Storage", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/WoodRamp_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	//Protot_Item_ForStage(L"L_Storage", L"나무 경사로", L"나무 경사로입니다.", ITEMID_L_Storage, 0, 100, 0, 4.f);
	//Protot_MakingItem_ForStage(L"L_Storage", L"나무 경사로 제작", ITEMID_L_Storage, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);


	/// 짚 건축
	// 짚 토대
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_ThatchFloor", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/ThatchFloor_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"ThatchFloor", L"짚 토대", L"짚 토대입니다.", ITEMID_ThatchFloor, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"ThatchFloor", L"짚 토대 제작", ITEMID_ThatchFloor, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 짚 벽
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_ThatchWall", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/ThatchWall_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"ThatchWall", L"짚 벽", L"짚 벽입니다.", ITEMID_ThatchWall, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"ThatchWall", L"짚 벽 제작", ITEMID_ThatchWall, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 짚 지붕
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_ThatchRoof", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/ThatchRoof_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"ThatchRoof", L"짚 지붕", L"짚 지붕입니다.", ITEMID_ThatchRoof, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"ThatchRoof", L"짚 지붕 제작", ITEMID_ThatchRoof, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 짚 문 프레임
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_ThatchDoorFrame", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/ThatchDoorway_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"ThatchDoorFrame", L"짚 문 프레임", L"짚 문 프레임입니다.", ITEMID_ThatchDoorFrame, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"ThatchDoorFrame", L"짚 문 프레임 제작", ITEMID_ThatchDoorFrame, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 짚 문
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Texture_ThatchDoor", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/ThatchDoor_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"ThatchDoor", L"짚 문", L"짚 문입니다.", ITEMID_ThatchDoor, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"ThatchDoor", L"짚 문 제작", ITEMID_ThatchDoor, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	
	/// 석제 건축
	// 석제 토대
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_StoneFloor", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/StoneFloor_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"StoneFloor", L"석제 토대", L"석제 토대입니다.", ITEMID_StoneFloor, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"StoneFloor", L"석제 토대 제작", ITEMID_StoneFloor, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 석제 벽
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_StoneWall", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/StoneWall_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"StoneWall", L"석제 벽", L"석제 벽입니다.", ITEMID_StoneWall, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"StoneWall", L"석제 벽 제작", ITEMID_StoneWall, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 석제 지붕
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_StoneRoof", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/StoneRoof_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"StoneRoof", L"석제 지붕", L"석제 지붕입니다.", ITEMID_StoneRoof, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"StoneRoof", L"석제 지붕 제작", ITEMID_StoneRoof, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 석제 문 프레임
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_StoneDoorFrame", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/StoneDoorway_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"StoneDoorFrame", L"석제 문 프레임", L"석제 문 프레임입니다.", ITEMID_StoneDoorFrame, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"StoneDoorFrame", L"석제 문 프레임 제작", ITEMID_StoneDoorFrame, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);
	// 석제 문
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_StoneDoor", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/StoneDoor_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"StoneDoor", L"석제 문", L"석제 문입니다.", ITEMID_StoneDoor, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"StoneDoor", L"석제 문 제작", ITEMID_StoneDoor, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);

	// 석재 게이트 프레임
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_StoneGateFrame", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Structure/StoneGateFrameIcon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"StoneGateFrame", L"석재 게이트 프레임", L"석재 게이트 프레임입니다.", ITEMID_StoneGateFrame, 0, 100, 0, 4.f);
	Protot_MakingItem_ForStage(L"StoneGateFrame", L"석재 게이트 프레임 제작", ITEMID_StoneGateFrame, 1, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Hide, 1, 1, 1, 1, 2.0, 1);


	// 마비화살
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_TranqArrow", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Weapon/TranqArrow_Icon%d.tga", TEX_NORMAL)), E_FAIL);
	Protot_Item_ForStage(L"TranqArrow", L"마비 화살", L"마비 화살.", ITEMID_TranqArrow, 5, 40, 20, 2.0f, 100);
	Protot_MakingItem_ForStage(L"TranqArrow", L"마비 화살 제작", ITEMID_TranqArrow, 1, ITEMID_Thatch, ITEMID_Fiber, ITEMID_Flint, ITEMID_END, 2, 2, 1, 0, 1.0, 3);


	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Player_Hit_UI", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Player_Hit_UI%d.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Player_Hit_Dir_UI", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Player_Hit_Dir%d.png", TEX_NORMAL)), E_FAIL);

	// Effect
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Blood", CTexture::Create(m_pDevice, L"../Resources/Texture/Effect/Blood/Blood%d.tga", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_BloodTest", CTexture::Create(m_pDevice, L"../Resources/Texture/Effect/Blood/Blood%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Dust", CTexture::Create(m_pDevice, L"../Resources/Texture/Effect/Dust/Dust%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Fire", CTexture::Create(m_pDevice, L"../Resources/Texture/Effect/Fire/Fire%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Dissolve", CTexture::Create(m_pDevice, L"../Resources/Texture/Effect/Dissolve/Dissolve%d.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_FireTest", CTexture::Create(m_pDevice, L"../Resources/Texture/Effect/FireTest/FireTest%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_FireNoise", CTexture::Create(m_pDevice, L"../Resources/Texture/Effect/FireNoise/FireNoise%d.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_FireSub", CTexture::Create(m_pDevice, L"../Resources/Texture/Effect/FireSub/FireSub%d.tga", TEX_NORMAL)), E_FAIL);
	
	FAILED_CHECK_RETURN_AND_COUNT(Engine::Add_Component_Prototype(L"Proto_Texture_FireSmoke", CTexture::Create(m_pDevice, L"../Resources/Texture/Effect/FireSmoke/FireSmoke%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_BuffRing", CTexture::Create(m_pDevice, L"../Resources/Texture/Effect/Buff/BuffRing%d.tga", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_BuffEffect", CTexture::Create(m_pDevice, L"../Resources/Texture/Effect/Buff/BuffEffect%d.tga", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Goal", CTexture::Create(m_pDevice, L"../Resources/Texture/Effect/Goal/Goal%d.tga", TEX_NORMAL)), E_FAIL);

	//제작용 엔그램? 사진
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Engram_BackGround", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Engram/Engram_Background%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_CraftingBroken", CTexture::Create(m_pDevice, L"../Resources/Texture/UI/Engram/CraftingBrokenIcon%d.tga", TEX_NORMAL)), E_FAIL);

	// WaterFall
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_WaterFallNoise", CTexture::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/WaterFall/cloudNoise_Soft_VDM%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_WaterFallGradient", CTexture::Create(m_pDevice, L"../Resources/Mesh/StaticMesh/WaterFall/SoftEdgeGradient%d.tga", TEX_NORMAL)), E_FAIL);

	// Water
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_Water", CTexture::Create(m_pDevice, L"../Resources/Texture/Water/tx_WaterFoamTile_01.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_WaterNormal", CTexture::Create(m_pDevice, L"../Resources/Texture/Water/tx_WaterFoamTile_01_N.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_WaterNoise", CTexture::Create(m_pDevice, L"../Resources/Texture/WaterNoise/WaterNoise%d.tga", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_WaterNormal2", CTexture::Create(m_pDevice, L"../Resources/Texture/Water/WaterNormal2.bmp", TEX_NORMAL)), E_FAIL);

	// Cube Test
	FAILED_CHECK_RETURN(Engine::Add_Component_Prototype(L"Proto_Texture_CubeTest", CTexture::Create(m_pDevice, L"../Resources/Texture/Test/Test0.dds", TEX_CUBE)), E_FAIL);

	return S_OK;
}
//_int				iStat = NULL;			                                	//포만감	||	포만감		||   공격력   ||    방어력
//_int				iPosition = NULL;		                                	//목마름	||  체력 회복	||   스턴력   ||   장착 위치
//_double			dLife = 0.0;			                                	//부패 시간	||  부패 시간	||   내구도   ||    내구도
//_double			dMaxLife = 0.0;			                                	//			           상          동     (Max)
//_float			fWeight = FNULL;		                                	//무게	    ||    무게		||    무게	  ||     무게
HRESULT CLoading::Protot_Item_ForStage(_tchar* _tchName, _tchar* _tchKoreaName, _tchar* _tchToolTip, ITEMID _iItemID, _int _iStat, _double _dMaxLife, _int _iPosition, _float _fWeight, _int _iMaxAmount)
{
	ITEM_INFO tItem;
	tItem.tchName = _tchName;
	tItem.tchKoreaName = _tchKoreaName;
	tItem.tchToolTip = _tchToolTip;
	tItem.dMaxLife = tItem.dLife = _dMaxLife;
	tItem.iMaxAmount = _iMaxAmount;
	tItem.iItemType = ((_iItemID / 100)*100);
	tItem.iItemID = _iItemID;
	tItem.iStat = _iStat;
	tItem.iPosition = _iPosition;
	tItem.fWeight = _fWeight;
	Engine::Add_Item_Prototype(_iItemID, tItem);
	return S_OK;
}

HRESULT CLoading::Protot_MakingItem_ForStage(_tchar * _tchName, _tchar * _tchKoreaName, ITEMID _iMakingItemID, _int _iMaxMakingAmount, ITEMID iIngredientItemID_1, ITEMID iIngredientItemID_2, ITEMID iIngredientItemID_3, ITEMID iIngredientItemID_4, _int _iNumber_of_Required_ItemID_1, _int _iNumber_of_Required_ItemID_2, _int _iNumber_of_Required_ItemID_3, _int _iNumber_of_Required_ItemID_4, _double _dMakingTime, _int _iMakingPossibleLv)
{
	MAKING_INFO tMakingItem;
	tMakingItem.tchName = _tchName;
	tMakingItem.tchKoreaName = _tchKoreaName;
	tMakingItem.iMakingItemID = _iMakingItemID;
	tMakingItem.iMaxMakingAmount = _iMaxMakingAmount;
	tMakingItem.iIngredientItemID_1 = iIngredientItemID_1;
	tMakingItem.iIngredientItemID_2 = iIngredientItemID_2;
	tMakingItem.iIngredientItemID_3 = iIngredientItemID_3;
	tMakingItem.iIngredientItemID_4 = iIngredientItemID_4;
	tMakingItem.iNumber_of_Required_ItemID_1 = _iNumber_of_Required_ItemID_1;
	tMakingItem.iNumber_of_Required_ItemID_2 = _iNumber_of_Required_ItemID_2;
	tMakingItem.iNumber_of_Required_ItemID_3 = _iNumber_of_Required_ItemID_3;
	tMakingItem.iNumber_of_Required_ItemID_4 = _iNumber_of_Required_ItemID_4;
	tMakingItem.dMakingTime = _dMakingTime;
	tMakingItem.iMakingPossibleLv = _iMakingPossibleLv;
	Engine::Add_MakingItem_Prototype(_iMakingItemID, tMakingItem);
	return S_OK;
}
