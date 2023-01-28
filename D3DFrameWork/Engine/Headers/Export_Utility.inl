#include "Export_Utility.h"

/// Management
HRESULT	Create_Management(LPDIRECT3DDEVICE9& pDevice, CManagement** ppMaganagement)
{
	CManagement*		pManagement = CManagement::GetInstance();

	NULL_CHECK_RETURN(pManagement, E_FAIL);
	FAILED_CHECK_RETURN(pManagement->Ready_Shader(pDevice), E_FAIL);

	*ppMaganagement = pManagement;

	return S_OK;
}

HRESULT	SetUp_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->SetUp_Scene(pScene);
}

_int Update_Scene(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}

void Render_Scene()
{
	CManagement::GetInstance()->Render_Scene();
}

CLayer*		Get_Layer(const wstring& _wstrGameLayerTag)
{
	return CManagement::GetInstance()->Get_Layer(_wstrGameLayerTag);
}

CGameObject* Get_GameObject(const wstring & _wstrGameLayerTag, const _int & _iObjIdx)
{
	return CManagement::GetInstance()->Get_GameObject(_wstrGameLayerTag, _iObjIdx);
}

CComponent*	Get_Component(const wstring & _wstrLayerTag, const _int & _iObjIdx, const wstring & _wstrComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(_wstrLayerTag, _iObjIdx, _wstrComponentTag, eID);
}

HRESULT	Add_GameObjectInLayer(const wstring& _wstrGameLayerTag, CGameObject* _pGameObject)
{
	return CManagement::GetInstance()->Add_GameObjectInLayer(_wstrGameLayerTag, _pGameObject);
}
HRESULT Swap_GameObjectInLayer(const wstring & _wstrPreGameLayerTag, const wstring & _wstrToGameLayerTag, _uint _iIndex)
{
	return CManagement::GetInstance()->Swap_GameObjectInLayer(_wstrPreGameLayerTag, _wstrToGameLayerTag, _iIndex);
}
HRESULT Swap_GameObjectInLayer(const wstring & _wstrPreGameLayerTag, const wstring & _wstrToGameLayerTag, CGameObject * _pGameObject)
{
	return CManagement::GetInstance()->Swap_GameObjectInLayer(_wstrPreGameLayerTag, _wstrToGameLayerTag, _pGameObject);
}

void Set_WorldTime(const _double _dWorldTime)
{
	CManagement::GetInstance()->Set_WorldTime(_dWorldTime);
}

inline void Set_ShadowLight(_vec3 _vLightPos, _vec3 _vLightAt)
{
	CManagement::GetInstance()->Set_ShadowLight(_vLightPos, _vLightAt);
}

inline const SHADOWLIGHT & Get_ShadowLight()
{
	return CManagement::GetInstance()->Get_ShadowLight();
}

list<CGameObject*>* Get_LayerList(const wstring& _wstrGameLayerTag)
{
	return CManagement::GetInstance()->Get_LayerList(_wstrGameLayerTag);
}
HRESULT RayCast_OnMesh(_vec3* pOut, const _vec3& vRayPos, const _vec3& vRayDir, const wstring& _wstrGameLayerTag)
{
	return CManagement::GetInstance()->RayCast_OnMesh(pOut, vRayPos, vRayDir, _wstrGameLayerTag);
}

HRESULT	Delete_GameObject(const wstring & _wstrLayerTag, CGameObject* _pGameObject)
{
	return CManagement::GetInstance()->Delete_GameObject(_wstrLayerTag, _pGameObject);
}

void			Set_Fade(const _float& fFade, const _bool& bFadeIn)
{
	CManagement::GetInstance()->Set_Fade(fFade, bFadeIn);
}
HRESULT			Fade_In(const _float& fSec)
{
	return CManagement::GetInstance()->Fade_In(fSec);
}
HRESULT			Fade_Out(const _float& fSec)
{
	return CManagement::GetInstance()->Fade_Out(fSec);
}

void			Set_ShadowOnOff(_bool _bShadow)
{
	CManagement::GetInstance()->Set_ShadowOnOff(_bShadow);
}

const _bool&		Get_ShadowOnOff()
{
	return CManagement::GetInstance()->Get_ShadowOnOff();
}

/// Prototype_Mgr
HRESULT	Add_GameObject_Prototype(const wstring& pProtoTag, CGameObject* pInstance)
{
	return CPrototype_Mgr::GetInstance()->Add_GameObject_Prototype(pProtoTag, pInstance);
}
 HRESULT Delete_GameObject_Prototype(const wstring& pProtoTag)
 {
	 return CPrototype_Mgr::GetInstance()->Delete_GameObject_Prototype(pProtoTag);
 }
HRESULT	Add_Component_Prototype(const wstring& pProtoTag, CComponent* pInstance)
{
	return CPrototype_Mgr::GetInstance()->Add_Component_Prototype(pProtoTag, pInstance);
}
HRESULT Create_Texture(LPDIRECT3DDEVICE9 pDevice, const wstring& szFileName, const wstring& szFullPath, LPDIRECT3DTEXTURE9* pOutTex, const _ulong& dwUsage, const _ulong& dwPool, D3DXIMAGE_INFO* pOutInfo)
{
	return CPrototype_Mgr::GetInstance()->Create_Texture(pDevice, szFileName, szFullPath, pOutTex, dwUsage, dwPool, pOutInfo);
}
HRESULT	Add_Item_Prototype(_int _iItemID, const ITEM_INFO& _tItem_Info)
{
	return CPrototype_Mgr::GetInstance()->Add_Item_Prototype(_iItemID, _tItem_Info);
}
HRESULT	Add_MakingItem_Prototype(_int _iItemID, const MAKING_INFO& _tItem_Info)
{
	return CPrototype_Mgr::GetInstance()->Add_MakingItem_Prototype(_iItemID, _tItem_Info);
}
CGameObject* Clone_GameObjectPrototype(const wstring& _strProtoTag, void* _pArg/* = nullptr*/)
{
	return CPrototype_Mgr::GetInstance()->Clone_GameObjectPrototype(_strProtoTag, _pArg);
}
CComponent* Clone_ComponentPrototype(const wstring& _strProtoTag, void* _pArg/* = nullptr*/)
{
	return CPrototype_Mgr::GetInstance()->Clone_ComponentPrototype(_strProtoTag, _pArg);
}
ITEM_INFO Clone_Item_Prototype(_int _iItemID, _int _iAmount)
{
	return CPrototype_Mgr::GetInstance()->Clone_Item_Prototype(_iItemID, _iAmount);
}
MAKING_INFO	Clone_MakingItem_Prototype(_int _iItemID)
{
	return CPrototype_Mgr::GetInstance()->Clone_MakingItem_Prototype(_iItemID);
}
HRESULT Get_ProtoTag(TCHAR* _szTagOut, _int _iObjID)
{
	return CPrototype_Mgr::GetInstance()->CPrototype_Mgr::Get_ProtoTag(_szTagOut, _iObjID);
}
HRESULT Get_ProtoTag_Dino(TCHAR* _szTagOut, _int _iObjOption)
{
	return CPrototype_Mgr::GetInstance()->CPrototype_Mgr::Get_ProtoTag_Dino(_szTagOut, _iObjOption);
}
HRESULT Get_LayerTag(TCHAR* _szTagOut, _int _iObjID)
{
	return CPrototype_Mgr::GetInstance()->CPrototype_Mgr::Get_LayerTag(_szTagOut, _iObjID);
}
HRESULT Get_MeshTag_Pebble(TCHAR* _szTagOut, _int _iObsID)
{
	const TCHAR* szSrc = CPrototype_Mgr::GetInstance()->CPrototype_Mgr::Get_MeshTag_Pebble(_iObsID);
	if (!szSrc)
	{
		ERR_MSG(L"Get_MeshTag_Pebble() : No Matching iOption");
		wcscpy_s(_szTagOut, MAX_PATH, L"Proto_StaticMesh_Pebble_A");
	}
	else
		wcscpy_s(_szTagOut, MAX_PATH, szSrc);
	return S_OK;
}
HRESULT Get_MeshTag_Bush(TCHAR* _szTagOut, _int _iObsID)
{
	const TCHAR* szSrc = CPrototype_Mgr::GetInstance()->CPrototype_Mgr::Get_MeshTag_Bush(_iObsID);
	if (!szSrc)
	{
		ERR_MSG(L"Get_MeshTag_Bush() : No Matching iOption");
		wcscpy_s(_szTagOut, MAX_PATH, L"Proto_StaticMesh_Bush_A");
	}
	else
		wcscpy_s(_szTagOut, MAX_PATH, szSrc);
	return S_OK;
}
HRESULT Get_MeshTag_Rock(TCHAR* _szTagOut, _int _iObsID)
{
	const TCHAR* szSrc = CPrototype_Mgr::GetInstance()->CPrototype_Mgr::Get_MeshTag_Rock(_iObsID);
	if (!szSrc)
	{
		ERR_MSG(L"Get_MeshTag_Rock() : No Matching iOption");
		wcscpy_s(_szTagOut, MAX_PATH, L"Proto_StaticMesh_Rock_Small_A");
	}
	else
		wcscpy_s(_szTagOut, MAX_PATH, szSrc);
	return S_OK;
}
HRESULT Get_MeshTag_Tree(TCHAR* _szTagOut, _int _iObsID)
{
	const TCHAR* szSrc = CPrototype_Mgr::GetInstance()->CPrototype_Mgr::Get_MeshTag_Tree(_iObsID);
	if (!szSrc)
	{
		ERR_MSG(L"Get_MeshTag_Tree() : No Matching iOption");
		wcscpy_s(_szTagOut, MAX_PATH, L"Proto_StaticMesh_Tree_A");
	}
	else
		wcscpy_s(_szTagOut, MAX_PATH, szSrc);
	return S_OK;
}
HRESULT Get_MeshTag_Obstacle(TCHAR* _szTagOut, _int _iObsID)
{
	const TCHAR* szSrc = CPrototype_Mgr::GetInstance()->CPrototype_Mgr::Get_MeshTag_Obstacle(_iObsID);
	if (!szSrc)
	{
		ERR_MSG(L"Get_MeshTag_Obstacle() : No Matching iOption");
		wcscpy_s(_szTagOut, MAX_PATH, L"Proto_StaticMesh_Bridge_Half_A");
	}
	else
		wcscpy_s(_szTagOut, MAX_PATH, szSrc);
	return S_OK;
}

/// Light_Mgr
HRESULT Ready_Light(LPDIRECT3DDEVICE9 _pDevice, const D3DLIGHT9* _pLineInfo, const _uint& _iIndex)
{
	return CLight_Mgr::GetInstance()->Ready_Light(_pDevice, _pLineInfo, _iIndex);
}
CLight* Get_Light(const _uint& _iIndex)
{
	return CLight_Mgr::GetInstance()->Get_Light(_iIndex);
}
void Render_Light(LPD3DXEFFECT& pEffect, _bool _bBlur)
{
	return CLight_Mgr::GetInstance()->Render_Light(pEffect, _bBlur);
}
_int	Get_LightListSize()
{
	return CLight_Mgr::GetInstance()->Get_LightListSize();
}

/// Renderer_Mgr
void Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	CRenderer_Mgr::GetInstance()->Add_RenderGroup(eID, pGameObject);
}
void Render_GameObject(_float fFade)
{
	CRenderer_Mgr::GetInstance()->Render_GameObject(fFade);
}
void Clear_RenderGroup(void)
{
	CRenderer_Mgr::GetInstance()->Clear_RenderGroup();
}
void Add_Instance(const wstring& wstrInstanceTag, RENDERID eID, const _mat& matWorld, const _vec4& vValues, const _vec4& vColor)
{
	CRenderer_Mgr::GetInstance()->Add_Instance(wstrInstanceTag, eID, matWorld, vValues, vColor);
}
//void Set_ConstantTable_ViewProj(LPD3DXEFFECT _pEffect)
//{
//	CRenderer_Mgr::GetInstance()->Set_ConstantTable_ViewProj(_pEffect);
//}
//void Set_ConstantTable_LightViewProj(LPD3DXEFFECT _pEffect)
//{
//	CRenderer_Mgr::GetInstance()->Set_ConstantTable_LightViewProj(_pEffect);
//}
//void Set_ConstantTable_WorldLightViewProj(LPD3DXEFFECT _pEffect, const _mat& matWorld)
//{
//	CRenderer_Mgr::GetInstance()->Set_ConstantTable_WorldLightViewProj(_pEffect, matWorld);
//}
//void Set_ConstantTable_ScaleBias(LPD3DXEFFECT _pEffect)
//{
//	CRenderer_Mgr::GetInstance()->Set_ConstantTable_ScaleBias(_pEffect);
//}
//void Set_ConstantTable_LightDirLocal(LPD3DXEFFECT _pEffect, const _mat& matWorld)
//{
//	CRenderer_Mgr::GetInstance()->Set_ConstantTable_LightDirLocal(_pEffect, matWorld);
//}
void Set_ConstantTable_Normal(LPD3DXEFFECT _pEffect, const _mat& matWorld)
{
	CRenderer_Mgr::GetInstance()->Set_ConstantTable_Normal(_pEffect, matWorld);
}
void Set_ConstantTable_ShadowMap(LPD3DXEFFECT _pEffect, const _mat& matWorld)
{
	CRenderer_Mgr::GetInstance()->Set_ConstantTable_ShadowMap(_pEffect, matWorld);
}
/// RenderTarget_Mgr
HRESULT Ready_Renderer(LPDIRECT3DDEVICE9& _pDevice, _bool _bIsTool)
{
	return CRenderer_Mgr::GetInstance()->Ready_Renderer(_pDevice, _bIsTool);
}
void	Set_ClientSize(_int _iX, _int _iY)
{
	return CRenderer_Mgr::GetInstance()->Set_ClientSize(_iX, _iY);
}
HRESULT Ready_RenderTarget(LPDIRECT3DDEVICE9 _pDevice, D3DFORMAT _Format, D3DXCOLOR _Color, const wstring& _wstrTargetTag, const _uint& _iWidth/* = 800*/, const _uint& _iHeight/* = 600*/, _bool _bShadow)
{
	return CRenderTarget_Mgr::GetInstance()->Ready_RenderTarget(_pDevice, _Format, _Color, _wstrTargetTag, _iWidth, _iHeight, _bShadow);
}
HRESULT Ready_MRT(const wstring& _wstrMRTTag, const wstring& _wstrTargetTag)
{
	return CRenderTarget_Mgr::GetInstance()->Ready_MRT(_wstrMRTTag, _wstrTargetTag);
}
HRESULT Begin_MRT(const wstring& _wstrMRTTag, _bool _bMotionBlur)
{
	return CRenderTarget_Mgr::GetInstance()->Begin_MRT(_wstrMRTTag, _bMotionBlur);
}
HRESULT End_MRT(const wstring& _wstrMRTTag)
{
	return CRenderTarget_Mgr::GetInstance()->End_MRT(_wstrMRTTag);
}
HRESULT Ready_DebugBuffer(const wstring& _wstrTargetTag, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	return CRenderTarget_Mgr::GetInstance()->Ready_DebugBuffer(_wstrTargetTag, _fX, _fY, _fSizeX, _fSizeY);
}
void Render_DebugBuffer(const wstring& _wstrMRTTag)
{
	CRenderTarget_Mgr::GetInstance()->Render_DebugBuffer(_wstrMRTTag);
}
void Get_RenderTargetTexture(LPD3DXEFFECT& pEffect, const wstring& _strTargetTag, const char* _pConstantName)
{
	CRenderTarget_Mgr::GetInstance()->Get_RenderTargetTexture(pEffect, _strTargetTag, _pConstantName);
}

///Frustum
HRESULT Ready_Frustum(LPDIRECT3DDEVICE9& _pDevice)
{
	return CFrustum::GetInstance()->Ready_Frustum(_pDevice);
}
_bool Check_Point(_vec3 vPos, CULL_TYPE eCulltype)
{
	return CFrustum::GetInstance()->CheckPoint(vPos, eCulltype);
}
_bool Check_Point(float x, float y, float z, CULL_TYPE eCulltype)
{
	return CFrustum::GetInstance()->CheckPoint(x, y, z, eCulltype);
}
_bool Check_Sphere(_vec3 vCenter, float fRadius, CULL_TYPE eCulltype)
{
	return CFrustum::GetInstance()->CheckSphere(vCenter, fRadius, eCulltype);
}
_bool Check_Sphere(float xCenter, float yCenter, float zCenter, float fRadius, CULL_TYPE eCulltype)
{
	return CFrustum::GetInstance()->CheckSphere(xCenter, yCenter, zCenter, fRadius, eCulltype);
}
_bool Check_Tree(_vec3 vCenter, float fRadius, CULL_TYPE eCulltype)
{
	return CFrustum::GetInstance()->CheckTree(vCenter, fRadius, eCulltype);
}
_bool Check_Tree(float xCenter, float yCenter, float zCenter, float fRadius, CULL_TYPE eCulltype)
{
	return CFrustum::GetInstance()->CheckTree(xCenter, yCenter, zCenter, fRadius, eCulltype);
}
_uint Get_DrawCount()
{
	return CFrustum::GetInstance()->Get_DrawCount();
}
_uint Get_CullCount()
{
	return CFrustum::GetInstance()->Get_CullCount();
}
_uint Get_DrawShadowCount()
{
	return CFrustum::GetInstance()->Get_DrawShadowCount();
}
_uint Get_CullShadowCount()
{
	return CFrustum::GetInstance()->Get_CullShadowCount();
}

/// System Release
void Release_Utility(void)
{
	CFrustum::DestroyInstance();
	CRenderTarget_Mgr::DestroyInstance();
	CRenderer_Mgr::DestroyInstance();
	CLight_Mgr::DestroyInstance();
	CPrototype_Mgr::DestroyInstance();
	CManagement::DestroyInstance();
}