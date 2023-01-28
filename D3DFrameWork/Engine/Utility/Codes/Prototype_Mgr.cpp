#include "..\Headers\Prototype_Mgr.h"


USING(Engine)
IMPLEMENT_SINGLETON(CPrototype_Mgr)

CPrototype_Mgr::CPrototype_Mgr()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}


CPrototype_Mgr::~CPrototype_Mgr()
{
	Free();
}

HRESULT CPrototype_Mgr::Add_GameObject_Prototype(const wstring& pProtoTag, CGameObject* pInstance)
{
	CGameObject*		pGameObject = Find_GameObjectPrototype(pProtoTag);

	if (nullptr != pGameObject)
		return E_FAIL;

	m_mapGameObjectPrototype.emplace(pProtoTag, pInstance);

	return S_OK;
}

HRESULT CPrototype_Mgr::Delete_GameObject_Prototype(const wstring & pProtoTag)
{
	auto iter = m_mapGameObjectPrototype.find(pProtoTag);

	if (iter == m_mapGameObjectPrototype.end())
		return E_FAIL;

	Safe_Release(iter->second);
	m_mapGameObjectPrototype.erase(iter);

	return S_OK;
}

HRESULT CPrototype_Mgr::Add_Component_Prototype(const wstring& pProtoTag, CComponent* pInstance)
{
	CComponent*		pComponent = Find_ComponentPrototype(pProtoTag);

	if (nullptr != pComponent)
		return E_FAIL;

	m_mapComponentPrototype.emplace(pProtoTag, pInstance);

	return S_OK;
}

HRESULT CPrototype_Mgr::Add_Item_Prototype(_int _iItemID, const ITEM_INFO& _tItem_Info)
{
	ITEM_INFO tItmeInfo = Find_ItemPrototype(_iItemID);
	if(nullptr != tItmeInfo.tchName)
		return E_FAIL;

	tItmeInfo = _tItem_Info;
	m_mapItemPrototype.emplace(_iItemID, tItmeInfo);

	return S_OK;
}

HRESULT CPrototype_Mgr::Add_MakingItem_Prototype(_int _iItemID, const MAKING_INFO & _tMakingItem_Info)
{
	MAKING_INFO tMakingInfo = Find_MakingItemPrototype(_iItemID);
	if (nullptr != tMakingInfo.tchName)
		return E_FAIL;

	tMakingInfo = _tMakingItem_Info;
	m_mapMakingItemPrototype.emplace(_iItemID, tMakingInfo);

	return S_OK;
}

HRESULT CPrototype_Mgr::Create_Texture(LPDIRECT3DDEVICE9 m_pDevice, const wstring& szFileName, const wstring& szFullPath
	, LPDIRECT3DTEXTURE9 * ppOutTex, const _ulong & dwUsage, const _ulong & dwPool, D3DXIMAGE_INFO * pOutInfo)
{
	LPDIRECT3DTEXTURE9 pTexture = (LPDIRECT3DTEXTURE9)Find_TexturePrototype(szFileName, pOutInfo);

	//생성하고자 하는 파일과 같은 이름의 텍스처가 이미 있을 경우 AddRef() 하고 포인터 반환
	if (pTexture)
	{
		m_iScore++;
		*(LPDIRECT3DTEXTURE9*)ppOutTex = pTexture;
		(*ppOutTex)->AddRef();
		return S_OK;
	}

	//새로운 텍스처인 경우 
	if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice,
		szFullPath.c_str(),
		0,
		0,
		0,
		dwUsage,
		D3DFMT_UNKNOWN,
		(D3DPOOL)dwPool,
		D3DX_FILTER_LINEAR | D3DX_FILTER_DITHER,
		D3DX_DEFAULT,
		0,
		pOutInfo,
		NULL,
		(LPDIRECT3DTEXTURE9*)ppOutTex)))
	{
		//MSG_BOX("Prototype_Mgr::Create_Texture : Failed to Run D3DXCreateTextureFromFileEX()");
		*ppOutTex = nullptr;
		pOutInfo = 0;
		return E_FAIL;
	}
	(*ppOutTex)->AddRef(); //텍스처 생성되자마자 레퍼런스카운트 2 가짐. (프로토 + 반환한 사본)

	//맵에 구조체 형태로 프로토타입 추가
	TEX_PROTO tTex;
	tTex.pTex = *ppOutTex;
	//tTex.tInfo = *pOutInfo;
	tTex.dwUsage = dwUsage;
	tTex.dwFlag = 0;
	m_mapTexturePrototype.emplace(szFileName, tTex);

	return S_OK;
}

CGameObject * CPrototype_Mgr::Clone_GameObjectPrototype(const wstring & _strProtoTag, void* _pArg/* = nullptr*/)
{
	CGameObject*		pGameObject = Find_GameObjectPrototype(_strProtoTag);

	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject->Clone(_pArg);
}

CComponent * CPrototype_Mgr::Clone_ComponentPrototype(const wstring & _strProtoTag, void* _pArg/* = nullptr*/)
{
	CComponent*		pComponent = Find_ComponentPrototype(_strProtoTag);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent->Clone(_pArg);
}

ITEM_INFO CPrototype_Mgr::Clone_Item_Prototype(_int _iItemID, _int _iAmount)
{
	ITEM_INFO tItem = Find_ItemPrototype(_iItemID);

	if (nullptr == tItem.tchName)
		return tItem;

	tItem.iAmount = _iAmount;
	return tItem;
}

MAKING_INFO CPrototype_Mgr::Clone_MakingItem_Prototype(_int _iItemID)
{
	MAKING_INFO tMakingItem = Find_MakingItemPrototype(_iItemID);
	if (nullptr == tMakingItem.tchName)
		return tMakingItem;

	return tMakingItem;
}

CGameObject * CPrototype_Mgr::Pick_GameObjectPrototype(const wstring & _strProtoTag)
{
	return Find_GameObjectPrototype(_strProtoTag);
}

CComponent * CPrototype_Mgr::Pick_ComponentPrototype(const wstring & _strProtoTag)
{
	return Find_ComponentPrototype(_strProtoTag);
}

HRESULT CPrototype_Mgr::Get_ProtoTag(TCHAR * szOut, const _int & eID)
{
	if (m_mapProtoTag.size() < 1)
		CPrototype_Mgr::Load_Map_ProtoTag();

	auto& iter = m_mapProtoTag.find(eID);
	if (iter == m_mapProtoTag.end())
	{
		ERR_MSG(L"No Matching ID");
		return E_FAIL;
	}
	wcscpy_s(szOut, MAX_PATH, iter->second.c_str());
	return S_OK;
}

HRESULT CPrototype_Mgr::Get_ProtoTag_Dino(TCHAR * szOut, const _int & iOption)
{
	if (m_mapProtoTag_Dino.size() < 1)
		CPrototype_Mgr::Load_Map_ProtoTag_Dino();

	auto& iter = m_mapProtoTag_Dino.find(iOption);
	if (iter == m_mapProtoTag_Dino.end())
	{
		ERR_MSG(L"No Matching ID");
		return E_FAIL;
	}
	wcscpy_s(szOut, MAX_PATH, iter->second.c_str());
	return S_OK;
}

HRESULT CPrototype_Mgr::Get_LayerTag(TCHAR * szOut, const _int & eID)
{
	if (m_mapLayerTag.size() < 1)
		CPrototype_Mgr::Load_Map_LayerTag();

	auto& iter = m_mapLayerTag.find(eID);
	if (iter == m_mapLayerTag.end())
	{
		ERR_MSG(L"No Matching ID");
		return E_FAIL;
	}
	wcscpy_s(szOut, MAX_PATH, iter->second.c_str());
	return S_OK;
}

const TCHAR * CPrototype_Mgr::Get_MeshTag_Pebble(_int _iObsID)
{
	if (m_mapMeshTag_Pebble.size() < 1)
		CPrototype_Mgr::Load_Map_MeshTag_Pebble();

	auto& iter = m_mapMeshTag_Pebble.find(_iObsID);
	if (iter == m_mapMeshTag_Pebble.end())
	{
		ERR_MSG(L"No Matching ID");
		return nullptr;;
	}
	return iter->second.c_str();
}

const TCHAR * CPrototype_Mgr::Get_MeshTag_Bush(_int _iObsID)
{
	if (m_mapMeshTag_Bush.size() < 1)
		CPrototype_Mgr::Load_Map_MeshTag_Bush();

	auto& iter = m_mapMeshTag_Bush.find(_iObsID);
	if (iter == m_mapMeshTag_Bush.end())
	{
		ERR_MSG(L"No Matching ID");
		return nullptr;;
	}
	return iter->second.c_str();
}

const TCHAR * CPrototype_Mgr::Get_MeshTag_Rock(_int _iObsID)
{
	if (m_mapMeshTag_Rock.size() < 1)
		CPrototype_Mgr::Load_Map_MeshTag_Rock();

	auto& iter = m_mapMeshTag_Rock.find(_iObsID);
	if (iter == m_mapMeshTag_Rock.end())
	{
		ERR_MSG(L"No Matching ID");
		return nullptr;;
	}
	return iter->second.c_str();
}

const TCHAR * CPrototype_Mgr::Get_MeshTag_Tree(_int _iObsID)
{
	if (m_mapMeshTag_Tree.size() < 1)
		CPrototype_Mgr::Load_Map_MeshTag_Tree();

	auto& iter = m_mapMeshTag_Tree.find(_iObsID);
	if (iter == m_mapMeshTag_Tree.end())
	{
		ERR_MSG(L"No Matching ID");
		return nullptr;;
	}
	return iter->second.c_str();
}

const TCHAR * CPrototype_Mgr::Get_MeshTag_Obstacle(_int _iObsID)
{
	if (m_mapMeshTag_Obstacle.size() < 1)
		CPrototype_Mgr::Load_Map_MeshTag_Obstacle();

	auto& iter = m_mapMeshTag_Obstacle.find(_iObsID);
	if (iter == m_mapMeshTag_Obstacle.end())
	{
		ERR_MSG(L"No Matching ID");
		return nullptr;;
	}
	return iter->second.c_str();
}

HRESULT CPrototype_Mgr::Load_Map_ProtoTag()
{
	m_mapProtoTag.emplace(0, L"Proto_Obj_Rock_Small");
	m_mapProtoTag.emplace(1, L"Proto_Obj_Pebble");
	m_mapProtoTag.emplace(2, L"Proto_Obj_Tree");
	m_mapProtoTag.emplace(3, L"Proto_Obj_Bush");
	m_mapProtoTag.emplace(4, L"Proto_Obj_Obstacle");
	m_mapProtoTag.emplace(5, L"Dino");
	m_mapProtoTag.emplace(6, L"Proto_Obj_Environment");
	return S_OK;
}

HRESULT CPrototype_Mgr::Load_Map_ProtoTag_Dino()
{
	_int i = 0;
	/* 0*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Dodo");
	/* 1*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Para");
	/* 2*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Raptor");
	/* 3*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Therizino");
	/* 4*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Rex");
	
	/* 5*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Iguanodon");
	/* 6*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Brachio");
	/* 7*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Allo");
	//////////////////////////////////////////////////////////////////////////	New!	//
	/* 8*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Stego");
	/* 9*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Yutyrannus");
	
	/*10*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Ankylo");
	/*11*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Quetzal");
	/*12*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Ptero");
	/*13*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Dragon");
	/*14*/	m_mapProtoTag_Dino.emplace(i++, L"Proto_Obj_Dino_Egg");

	return S_OK;
}

HRESULT CPrototype_Mgr::Load_Map_LayerTag()
{
	m_mapLayerTag.emplace(0, L"Layer_Harvest_Rock");
	m_mapLayerTag.emplace(1, L"Layer_Harvest_Pebble");
	m_mapLayerTag.emplace(2, L"Layer_Harvest_Tree");
	m_mapLayerTag.emplace(3, L"Layer_Harvest_Bush");
	m_mapLayerTag.emplace(4, L"Layer_Obstacle");
	m_mapLayerTag.emplace(5, L"Layer_Dino");
	m_mapLayerTag.emplace(6, L"Layer_Environment");
	return S_OK;
}

HRESULT CPrototype_Mgr::Load_Map_MeshTag_Pebble()
{
	_int i = 0;
	m_mapMeshTag_Pebble.emplace(i++, L"Proto_StaticMesh_Pebble_A");
	m_mapMeshTag_Pebble.emplace(i++, L"Proto_StaticMesh_Pebble_B");
	m_mapMeshTag_Pebble.emplace(i++, L"Proto_StaticMesh_Pebble_C");

	return S_OK;
}

HRESULT CPrototype_Mgr::Load_Map_MeshTag_Bush()
{
	_int i = 0;
	m_mapMeshTag_Bush.emplace(i++, L"Proto_StaticMesh_Bush_A");
	m_mapMeshTag_Bush.emplace(i++, L"Proto_StaticMesh_Bush_B");
	m_mapMeshTag_Bush.emplace(i++, L"Proto_StaticMesh_Bush_C");
	m_mapMeshTag_Bush.emplace(i++, L"Proto_StaticMesh_Bush_D");

	return S_OK;
}

HRESULT CPrototype_Mgr::Load_Map_MeshTag_Rock()
{
	_int i = 0;
	m_mapMeshTag_Rock.emplace(i++, L"Proto_StaticMesh_Rock_Small_A");
	m_mapMeshTag_Rock.emplace(i++, L"Proto_StaticMesh_Rock_Small_B");
	m_mapMeshTag_Rock.emplace(i++, L"Proto_StaticMesh_Rock_Medium_A");
	m_mapMeshTag_Rock.emplace(i++, L"Proto_StaticMesh_Rock_Medium_B");
	m_mapMeshTag_Rock.emplace(i++, L"Proto_StaticMesh_Rock_Large_A");
	m_mapMeshTag_Rock.emplace(i++, L"Proto_StaticMesh_Rock_Large_B");
	m_mapMeshTag_Rock.emplace(i++, L"Proto_StaticMesh_Rock_Large_C");

	return S_OK;
}

HRESULT CPrototype_Mgr::Load_Map_MeshTag_Tree()
{
	_int i = 0;
	m_mapMeshTag_Tree.emplace(i++, L"Proto_StaticMesh_Tree_A");
	m_mapMeshTag_Tree.emplace(i++, L"Proto_StaticMesh_Tree_B");
	m_mapMeshTag_Tree.emplace(i++, L"Proto_StaticMesh_Tree_Mushroom_A");
	m_mapMeshTag_Tree.emplace(i++, L"Proto_StaticMesh_Tree_Mushroom_B");

	return S_OK;
}

HRESULT CPrototype_Mgr::Load_Map_MeshTag_Obstacle()
{
	_int i = 0;
	m_mapMeshTag_Obstacle.emplace(i++, L"Null");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Bridge_Half_A");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Bridge_Half_B");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Bridge_Half_C");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Bridge_Half_D");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Bridge_Half_E");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Cliff_Huge_A");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Cliff_Huge_B");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Cliff_Huge_C");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Cliff_Huge_D");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Rock_Huge_A");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Rock_Huge_B");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Rock_Huge_C");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_ObeliskTree");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Tek_EntranceArch");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Tek_EntranceFloor");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Tek_EntranceWall");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Tek_Floor");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Tek_FloorRamp");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Tek_Iris_A");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Tek_Iris_B");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Tek_Lib");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Tek_Portal");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Tek_RailSupport");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Tek_Ramp");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Tek_Tunnel");
	m_mapMeshTag_Obstacle.emplace(i++, L"Null");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Mountain_A");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Mountain_B");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Mountain_C");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_RockDetail");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Cliff_Square_A");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Cliff_Square_B");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Cliff_Square_C");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Platform_Huge_A");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Platform_Huge_B");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Platform_Huge_C");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Platform_Vertical_A");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Platform_Vertical_B");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Platform_Vertical_C");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Platform_Medium_A");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Platform_Medium_B");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Platform_Medium_C");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Platform_Small_A");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Platform_Small_B");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_Platform_Small_C");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_RedWood_A");
	m_mapMeshTag_Obstacle.emplace(i++, L"Proto_StaticMesh_RedWood_B");
	m_mapMeshTag_Obstacle.emplace(i++, L"");
	m_mapMeshTag_Obstacle.emplace(i++, L"");

	return S_OK;
}

CGameObject * CPrototype_Mgr::Find_GameObjectPrototype(const wstring & _strProtoTag)
{
	auto iter = m_mapGameObjectPrototype.find(_strProtoTag);

	if (iter == m_mapGameObjectPrototype.end())
		return nullptr;

	return iter->second;
}

CComponent * CPrototype_Mgr::Find_ComponentPrototype(const wstring & _strProtoTag)
{
	auto iter = m_mapComponentPrototype.find(_strProtoTag);

	if (iter == m_mapComponentPrototype.end())
		return nullptr;

	return iter->second;
}

IDirect3DBaseTexture9* CPrototype_Mgr::Find_TexturePrototype(const wstring & _strTextrueFileName, D3DXIMAGE_INFO* pOutInfo)
{
	auto iter = m_mapTexturePrototype.find(_strTextrueFileName);

	if (iter == m_mapTexturePrototype.end())
		return nullptr;

	//*pOutInfo = iter->second.tInfo;
	return iter->second.pTex;
}

ITEM_INFO CPrototype_Mgr::Find_ItemPrototype(_int _iItemID)
{
	ITEM_INFO tItemInfo;
	auto iter = m_mapItemPrototype.find(_iItemID);
	if (iter == m_mapItemPrototype.end())
		return tItemInfo;

	tItemInfo = iter->second;
	return tItemInfo;
}

MAKING_INFO CPrototype_Mgr::Find_MakingItemPrototype(_int _iItemID)
{
	MAKING_INFO tMakingInfo;
	auto iter = m_mapMakingItemPrototype.find(_iItemID);
	if (iter == m_mapMakingItemPrototype.end())
		return tMakingInfo;

	tMakingInfo = iter->second;
	return tMakingInfo;
}

void CPrototype_Mgr::Free()
{
	//맵들 지우기
	m_mapItemPrototype.clear();
	map<_int, ITEM_INFO>		().swap(m_mapItemPrototype);
	m_mapMakingItemPrototype.clear();
	map<_int, MAKING_INFO>		().swap(m_mapMakingItemPrototype);
	m_mapProtoTag.clear();
	map<_int, wstring>			().swap(m_mapProtoTag);
	m_mapProtoTag_Dino.clear();
	map<_int, wstring>			().swap(m_mapProtoTag_Dino);
	m_mapLayerTag.clear();
	map<_int, wstring>			().swap(m_mapLayerTag);
	m_mapMeshTag_Pebble.clear();
	map<_int, wstring>			().swap(m_mapMeshTag_Pebble);
	m_mapMeshTag_Bush.clear();
	map<_int, wstring>			().swap(m_mapMeshTag_Bush);
	m_mapMeshTag_Rock.clear();
	map<_int, wstring>			().swap(m_mapMeshTag_Rock);
	m_mapMeshTag_Tree.clear();
	map<_int, wstring>			().swap(m_mapMeshTag_Tree);
	m_mapMeshTag_Obstacle.clear();
	map<_int, wstring>			().swap(m_mapMeshTag_Obstacle);
	
	for (auto& pPair : m_mapGameObjectPrototype)
		Safe_Release(pPair.second);
	for (auto& pPair : m_mapComponentPrototype)
		Safe_Release(pPair.second);
	for (auto& pPair : m_mapTexturePrototype)
	{
		_tchar szText[MAX_PATH] = L"";
		wstring strFileName = pPair.first + L" : %d RefCount Left\n";
		_int iChecker = Safe_Release(pPair.second.pTex);
		if (iChecker != 0)
		{
			//wsprintf(szText, strFileName.c_str(), iChecker);
			//OutputDebugString(szText);
		}
	}
	
	_tchar szTextScore[MAX_PATH] = L"";
	wstring strTextScore = L"TextureMgr Score : %d\n";
	wsprintf(szTextScore, strTextScore.c_str(), m_iScore);
	OutputDebugString(szTextScore);

	//m_iChecker = new int;
	m_mapGameObjectPrototype.clear();
	map<wstring, CGameObject*>().swap(m_mapGameObjectPrototype);
	m_mapComponentPrototype.clear();
	map<wstring, CComponent*>().swap(m_mapComponentPrototype);
	m_mapTexturePrototype.clear();
	map<wstring, TEX_PROTO>().swap(m_mapTexturePrototype);
}
