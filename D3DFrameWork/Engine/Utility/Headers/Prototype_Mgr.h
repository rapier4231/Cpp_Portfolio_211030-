#pragma once
#ifndef __PROTOTYPE_MGR_H__
#define __PROTOTYPE_MGR_H__

#include "Engine_Define.h"
#include "Base.h"

#include "Component.h"
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CPrototype_Mgr : public CBase
{
	DECLARE_SINGLETON(CPrototype_Mgr)

private:
	explicit CPrototype_Mgr();
	virtual ~CPrototype_Mgr();

public:
	HRESULT			Add_GameObject_Prototype(const wstring& pProtoTag, CGameObject* pInstance);
	HRESULT			Delete_GameObject_Prototype(const wstring& pProtoTag);

	HRESULT			Add_Component_Prototype(const wstring& pProtoTag, CComponent* pInstance);
	HRESULT			Add_Item_Prototype(_int _iItemID, const ITEM_INFO& _tItem_Info);
	HRESULT			Add_MakingItem_Prototype(_int _iItemID, const MAKING_INFO& _tMakingItem_Info);
	HRESULT			Create_Texture(LPDIRECT3DDEVICE9 pDevice, const wstring& szFileName, const wstring& szFullPath, LPDIRECT3DTEXTURE9* ppOutTex, const _ulong& dwUsage = 0, const _ulong& dwPool = D3DPOOL_MANAGED, D3DXIMAGE_INFO* pOutInfo = nullptr);
	//HRESULT			Create_Texture(const wstring& szFileName, LPDIRECT3DTEXTURE9* pOutTex, const _ulong& dwUsage = D3DUSAGE_DYNAMIC, const _ulong& dwPool = D3DPOOL_DEFAULT, D3DXIMAGE_INFO* pOutInfo = nullptr);

public:
	CGameObject*	Clone_GameObjectPrototype(const wstring& _strProtoTag, void* _pArg = nullptr);
	CComponent*		Clone_ComponentPrototype(const wstring& _strProtoTag, void* _pArg = nullptr);
	ITEM_INFO		Clone_Item_Prototype(_int _iItemID, _int _iAmount = 0);
	MAKING_INFO		Clone_MakingItem_Prototype(_int _iItemID);
	
	CGameObject*	Pick_GameObjectPrototype(const wstring& _strProtoTag);	//인스턴싱 등 엔진내부에서 사용
	CComponent*		Pick_ComponentPrototype(const wstring& _strProtoTag);	//

	HRESULT			Get_ProtoTag(TCHAR* szOut, const _int& eID);	//ID값에 따라 프로토태그 반환
	HRESULT			Get_ProtoTag_Dino(TCHAR* szOut, const _int& iOption);	//Option값에 따라 프로토태그 반환
	HRESULT			Get_LayerTag(TCHAR* szOut, const _int& eID);	//ID값에 따라 레이어태그 반환

	const TCHAR*	Get_MeshTag_Pebble(_int _iObsID);				//Option값에 따라 메시태그 반환
	const TCHAR*	Get_MeshTag_Bush(_int _iObsID);					//Option값에 따라 메시태그 반환
	const TCHAR*	Get_MeshTag_Rock(_int _iObsID);					//Option값에 따라 메시태그 반환
	const TCHAR*	Get_MeshTag_Tree(_int _iObsID);					//Option값에 따라 메시태그 반환
	const TCHAR*	Get_MeshTag_Obstacle(_int _iObsID);				//Option값에 따라 메시태그 반환

	HRESULT			Load_Map_ProtoTag();						  	//초기로딩
	HRESULT			Load_Map_ProtoTag_Dino();					  	//초기로딩
	HRESULT			Load_Map_LayerTag();							//초기로딩

	HRESULT			Load_Map_MeshTag_Pebble();						//초기로딩
	HRESULT			Load_Map_MeshTag_Bush();						//초기로딩
	HRESULT			Load_Map_MeshTag_Rock();						//초기로딩
	HRESULT			Load_Map_MeshTag_Tree();						//초기로딩
	HRESULT			Load_Map_MeshTag_Obstacle();					//초기로딩

private:	
	CGameObject*	Find_GameObjectPrototype(const wstring& _strProtoTag);
	CComponent*		Find_ComponentPrototype(const wstring& _strProtoTag);
	IDirect3DBaseTexture9*		Find_TexturePrototype(const wstring& _strTextrueFileName, D3DXIMAGE_INFO* pOutInfo = nullptr);
	ITEM_INFO		Find_ItemPrototype(_int _iItemID);
	MAKING_INFO		Find_MakingItemPrototype(_int _iItemID);

public:
	virtual void Free() override;

private:
	map<wstring, CGameObject*>	m_mapGameObjectPrototype;
	map<wstring, CComponent*>	m_mapComponentPrototype;

	map<_int, ITEM_INFO>		m_mapItemPrototype;
	map<_int, MAKING_INFO>		m_mapMakingItemPrototype;
	
	map<_int, wstring>			m_mapProtoTag;
	map<_int, wstring>			m_mapProtoTag_Dino;
	map<_int, wstring>			m_mapLayerTag;

	map<_int, wstring>			m_mapMeshTag_Pebble;
	map<_int, wstring>			m_mapMeshTag_Bush;
	map<_int, wstring>			m_mapMeshTag_Rock;
	map<_int, wstring>			m_mapMeshTag_Tree;
	map<_int, wstring>			m_mapMeshTag_Obstacle;

	typedef struct {
		IDirect3DBaseTexture9*	pTex;
		//D3DXIMAGE_INFO			tInfo;
		_ulong					dwUsage;
		_ulong					dwFlag;
	}TEX_PROTO;

	map<wstring, TEX_PROTO>		m_mapTexturePrototype;

	_int m_iScore = 0;
	//_int* m_iChecker = nullptr;
};
END
#endif // !__PROTOTYPE_MGR_H__
