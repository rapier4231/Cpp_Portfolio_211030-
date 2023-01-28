#pragma once
#ifndef __EXPORT_UTILITY_H__
#define __EXPORT_UTILITY_H__

#include "Management.h"
#include "Prototype_Mgr.h"
#include "Light_Mgr.h"
#include "Renderer_Mgr.h"
#include "RenderTarget_Mgr.h"
#include "Frustum.h"

#include "VIBuffer_RectColor.h"
#include "VIBuffer_RectTex.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_ParticleTex.h"
#include "VIBuffer_FireParticleTex.h"
#include "VIBuffer_FireSmokeParticleTex.h"
#include "VIBuffer_StructDustParticleTex.h"
#include "VIBuffer_BloodParticleTex.h"
#include "VIBuffer_GoalParticleTex.h"
#include "VIBuffer_WaterTerrain.h"
#include "VIBuffer_CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "Calculator.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "SphereCollider.h"
#include "Optimization.h"
#include "Camera.h"
#include "Shader.h"

#include "SkinnedMesh.h"

BEGIN(Engine)

/// Management (사실상 Scene_Mgr, 나중에 바꾸기)
inline HRESULT				Create_Management(LPDIRECT3DDEVICE9& pDevice, CManagement** ppMaganagement);
inline HRESULT				SetUp_Scene(CScene* pScene);
inline _int					Update_Scene(const _float& fTimeDelta);
inline void					Render_Scene();
inline CLayer*				Get_Layer(const wstring& _wstrGameLayerTag);
inline CGameObject*			Get_GameObject(const wstring & _wstrGameLayerTag, const _int & iObjIdx = 0);
inline CComponent*			Get_Component(const wstring & _wstrLayerTag, const _int & iObjIdx, const wstring & _wstrComponentTag, COMPONENTID eID);
inline HRESULT				Add_GameObjectInLayer(const wstring& _wstrGameLayerTag, CGameObject* _pGameObject);
inline HRESULT				Swap_GameObjectInLayer(const wstring & _wstrPreGameLayerTag, const wstring & _wstrToGameLayerTag, _uint _iIndex);
inline HRESULT				Swap_GameObjectInLayer(const wstring & _wstrPreGameLayerTag, const wstring & _wstrToGameLayerTag, CGameObject * _pGameObject);
inline list<CGameObject*>*	Get_LayerList(const wstring& _wstrGameLayerTag);
inline void					Set_WorldTime(const _double _dWorldTime);
inline HRESULT				RayCast_OnMesh(_vec3* pOut, const _vec3& vRayPos, const _vec3& vRayDir, const wstring& _wstrGameLayerTag);
inline HRESULT				Delete_GameObject(const wstring & _wstrLayerTag, CGameObject* _pGameObject);
inline void					Set_ShadowLight(_vec3 _vLightPos, _vec3 _vLightAt);
inline const SHADOWLIGHT&	Get_ShadowLight();
inline void					Set_Fade(const _float& fFade, const _bool& bFadeIn);
inline HRESULT				Fade_In(const _float& fSec);
inline HRESULT				Fade_Out(const _float& fSec);
inline void					Set_ShadowOnOff(_bool _bShadow);
inline const _bool&			Get_ShadowOnOff();

/// Prototype_Mgr
inline HRESULT				Add_GameObject_Prototype(const wstring& pProtoTag, CGameObject* pInstance);
inline HRESULT				Delete_GameObject_Prototype(const wstring& pProtoTag);
inline HRESULT				Add_Component_Prototype(const wstring& pProtoTag, CComponent* pInstance);
inline HRESULT				Create_Texture(LPDIRECT3DDEVICE9 pDevice, const wstring& szFileName, const wstring& szFullPath, LPDIRECT3DTEXTURE9* pOutTex, const _ulong& dwUsage = 0, const _ulong& dwPool = D3DPOOL_MANAGED, D3DXIMAGE_INFO* pOutInfo = nullptr);
inline HRESULT				Add_Item_Prototype(_int _iItemID, const ITEM_INFO& _tItem_Info);
inline HRESULT				Add_MakingItem_Prototype(_int _iItemID, const MAKING_INFO& _tItem_Info);
inline CGameObject*			Clone_GameObjectPrototype(const wstring& _strProtoTag, void* _pArg = nullptr);
inline CComponent*			Clone_ComponentPrototype(const wstring& _strProtoTag, void* _pArg = nullptr);
inline ITEM_INFO			Clone_Item_Prototype(_int _iItemID, _int _iAmount = 0);
inline MAKING_INFO			Clone_MakingItem_Prototype(_int _iItemID);
inline HRESULT				Get_ProtoTag(TCHAR* _szTagOut, _int _iObjID);
inline HRESULT				Get_ProtoTag_Dino(TCHAR* _szTagOut, _int _iObjOption);
inline HRESULT				Get_LayerTag(TCHAR* _szTagOut, _int _iObjID);
inline HRESULT				Get_MeshTag_Pebble(TCHAR* _szTagOut, _int _iObsID);
inline HRESULT				Get_MeshTag_Bush(TCHAR* _szTagOut, _int _iObsID);
inline HRESULT				Get_MeshTag_Rock(TCHAR* _szTagOut, _int _iObsID);
inline HRESULT				Get_MeshTag_Tree(TCHAR* _szTagOut, _int _iObsID);
inline HRESULT				Get_MeshTag_Obstacle(TCHAR* _szTagOut, _int _iObsID);

/// Light_Mgr
inline HRESULT				Ready_Light(LPDIRECT3DDEVICE9 _pDevice, const D3DLIGHT9* _pLineInfo, const _uint& _iIndex);
inline CLight*				Get_Light(const _uint& _iIndex);
inline void					Render_Light(LPD3DXEFFECT& pEffect, _bool _bBlur = false);
inline _int					Get_LightListSize();

/// Renderer_Mgr
inline void					Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void					Render_GameObject(_float fFade);
inline void					Clear_RenderGroup(void);
inline void					Add_Instance(const wstring& wstrInstanceTag, RENDERID eID, const _mat& matWorld, const _vec4& vValues = _vec4(0.f, 0.f, 0.f, 0.f), const _vec4& vColor = _vec4(1.f, 1.f, 1.f, 1.f));

/// RenderTarget_Mgr
inline HRESULT				Ready_Renderer(LPDIRECT3DDEVICE9& _pDevice, _bool _bIsTool);
inline void					Set_ClientSize(_int _iX, _int _iY);
inline HRESULT				Ready_RenderTarget(LPDIRECT3DDEVICE9 _pDevice, D3DFORMAT _Format, D3DXCOLOR _Color, const wstring& _wstrTargetTag, const _uint& _iWidth = 800, const _uint& _iHeight = 600, _bool _bShadow = false);
inline HRESULT				Ready_MRT(const wstring& _wstrMRTTag, const wstring& _wstrTargetTag);
inline HRESULT				Begin_MRT(const wstring& _wstrMRTTag, _bool _bMotionBlur = false);
inline HRESULT				End_MRT(const wstring& _wstrMRTTag);
inline HRESULT				Ready_DebugBuffer(const wstring& _wstrTargetTag, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY);
inline void					Render_DebugBuffer(const wstring& _wstrMRTTag);
inline void					Get_RenderTargetTexture(LPD3DXEFFECT& pEffect, const wstring& _strTargetTag, const char* _pConstantName);
inline void					Set_ConstantTable_Normal(LPD3DXEFFECT _pEffect, const _mat& matWorld);
inline void					Set_ConstantTable_ShadowMap(LPD3DXEFFECT _pEffect, const _mat& matWorld);

/// Frustum
inline HRESULT				Ready_Frustum(LPDIRECT3DDEVICE9& _pDevice);
inline _bool				Check_Point(_vec3 vPos, CULL_TYPE eCulltype = CULL_VIEW);
inline _bool				Check_Point(float x, float y, float z, CULL_TYPE eCulltype = CULL_VIEW);
inline _bool				Check_Sphere(_vec3 vCenter, float fRadius, CULL_TYPE eCulltype = CULL_VIEW);
inline _bool				Check_Sphere(float xCenter, float yCenter, float zCenter, float fRadius, CULL_TYPE eCulltype = CULL_VIEW);
inline _bool				Check_Tree(_vec3 vCenter, float fRadius, CULL_TYPE eCulltype = CULL_VIEW);
inline _bool				Check_Tree(float xCenter, float yCenter, float zCenter, float fRadius, CULL_TYPE eCulltype = CULL_VIEW);
inline _uint				Get_DrawCount();
inline _uint				Get_CullCount();
inline _uint				Get_DrawShadowCount();
inline _uint				Get_CullShadowCount();

/// System Release
inline void					Release_Utility(void);

#include "Export_Utility.inl"

END

#endif // !__EXPORT_UTILITY_H__
