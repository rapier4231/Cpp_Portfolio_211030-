#pragma once
#ifndef __RENDERER_MGR_H__
#define __RENDERER_MGR_H__

#include "Engine_Define.h"
#include "Base.h"
BEGIN(Engine)

class CGameObject;
class ENGINE_DLL CRenderer_Mgr : public CBase
{
	DECLARE_SINGLETON(CRenderer_Mgr)

private:
	explicit CRenderer_Mgr();
	virtual ~CRenderer_Mgr(void);

public:
	HRESULT				Ready_Renderer(LPDIRECT3DDEVICE9& _pDevice, _bool _bIsTool);
public:
	void				Set_ClientSize(_int _iX, _int _iY);

public:
	void				Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
	void				Render_GameObject(const _float& fFade);
	void				Clear_RenderGroup(void);

	//버텍스, 인스턴스데이터 선언하고 각 사이즈 저장
	void				Init_Instancing();
	//렌더그룹 대신 인스턴스데이터에 추가
	void				Add_Instance(const wstring& wstrInstanceTag, RENDERID eID, const _mat& matWorld, const _vec4& vValues, const _vec4& vColor);
	//모든 인스턴스데이터를 순회하며 렌더
	void				Render_Instances(RENDERID eID);
	void				Render_InstancesShadowMap(RENDERID eID);
	//데이터버퍼를 i칸만큼 확장
	void				Expand_DataBuffer(LPDIRECT3DVERTEXBUFFER9& pBuffer, _uint iExpand);
	//자주 변경되는 행렬을 갱신. 매 렌더 직전에 호출됨
	void				Update_MatViewProj();
	//자주 변경되지 않는 행렬을 갱신. 필요할 때 외부에서 호출할 것
	void				Update_MatLightViewProj(const _vec3& matLightPos, const _vec3& matLightAt, const _vec3& matLightUp, const _mat& matLightProj);
	void				Update_MatScaleBias(const _mat& matScaleBias);
	//각 쉐이더에 필요한 상수테이블 한번에 세팅
	void				Set_ConstantTable_Normal(LPD3DXEFFECT _pEffect, const _mat& matWorld);
	void				Set_ConstantTable_ShadowMap(LPD3DXEFFECT _pEffect, const _mat& matWorld);

private:
	//세팅 도우미 인라인함수들
	inline void			Set_ConstantTable_ViewProj(LPD3DXEFFECT _pEffect)
	{
		_pEffect->SetMatrix("g_matView", &m_matView);
		_pEffect->SetMatrix("g_matProj", &m_matProj);
	}

	inline void			Set_ConstantTable_LightViewProj(LPD3DXEFFECT _pEffect)
	{
		_pEffect->SetMatrix("g_matLightViewProj", &m_matLightViewProj);
	}

	inline void			Set_ConstantTable_WorldLightViewProj(LPD3DXEFFECT _pEffect, const _mat& matWorld)
	{
		m_matTemp = matWorld * m_matLightViewProj;
		_pEffect->SetMatrix("g_matWorldLightViewProj", &m_matTemp);
	}

	inline void			Set_ConstantTable_ScaleBias(LPD3DXEFFECT _pEffect)
	{
		_pEffect->SetMatrix("g_matScaleBias", &m_matScaleBias);
	}
	
	inline void			Set_ConstantTable_LightDirLocal(LPD3DXEFFECT _pEffect, const _mat& matWorld)
	{
		D3DXMatrixInverse(&m_matTemp, 0, &matWorld);
		D3DXVec3Transform(&m_v4Temp, &m_vLightPos, &m_matTemp);
		D3DXVec4Normalize(&m_v4Temp, &m_v4Temp);
		m_v4Temp.w = 0;
		_pEffect->SetVector("g_vLightDir", &m_v4Temp);
	}

private:
	void				Render_Priority();
	void				Render_NonAlpha();
	void				Render_Alpha();
	void				Render_UI();
	void				Render_AfterEffect();

	void				Render_Deferred();
	void				Render_Effect();
	void				Render_LightAcc();
	void				Render_LightBlur();
	void				Render_ShadowMap();
	void				Render_ShadowBlur();
	//void				Render_Shadow();	//Render_Deferred() 에 합쳐짐. Shader_Normal_Shadow로 함께 출력됨

	void				Render_Blend(const _float& fFade);
	void				Render_LightBloom();
	void				Render_Result();		// 최종 화면

	//void				Render_HDR();

	void				Render_Buffer(LPD3DXEFFECT _pEffect, _int _iPass = 0);

private:
	virtual void		Free(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	list<CGameObject*>		m_RenderGroup[RENDER_END];
	LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9	m_pIB = nullptr;

	// 인스턴싱에서 사용 //////////////////////////////////////////////////////////////////////////
	typedef struct {
		_vec4				vMatWorld1;	// TEXCOORD1~4	월드행렬
		_vec4				vMatWorld2;
		_vec4				vMatWorld3;
		_vec4				vMatWorld4;
		_vec4				vValues;	// TEXCOORD5	A 디졸브 / R / G 바람 / B
		_vec4				vColor;		// COLOR		컬러
	}INSTANCE_DATA;	//각 객체가 인스턴싱에 사용할 값들 = 1번스트림에 들어갈 선언(Decl)

	typedef struct {
		_uint							dwCount = 0;
		vector<INSTANCE_DATA>			vecMatrix;
		LPDIRECT3DVERTEXBUFFER9			pDB = nullptr;
	}INSTANCE_OBJ;			//매 프레임마다 인스턴스데이터를 수집하는 구조체
	_ulong					m_dwDataBufferLimit = 0;			//데이터버퍼가 담을 수 있는 최다 인스턴스데이터 수

	//LPDIRECT3DVERTEXBUFFER9				m_pDB = nullptr;	//인스턴스데이터를 복사해넣을 데이터버퍼
	//_ulong					m_dwDataBufferRemain = 0;		//현재의 데이터버퍼가 더 담을 수 있는 인스턴스데이터 수
	//_ulong					m_dwDataBufferOffset = 0;		//현재의 데이터버퍼에 Write할 때 시작점

	map<const wstring, INSTANCE_OBJ*>	m_mapInstanceData[RENDER_END];	//각 객체별로 하나씩 구조체 보관. 키값은 MeshTag
	LPDIRECT3DVERTEXDECLARATION9		m_pDecl;		//일반적인 정점 선언
	LPDIRECT3DVERTEXDECLARATION9		m_pDeclInst;	//인스턴싱용 정점 선언 (일반정점+인스턴스데이터)
	_ulong					m_dwVertexSize = 0;			//일반적인 정점의 크기 : 로컬Pos, 노멀, 탄젠트, UV
	_ulong					m_dwInstanceDataSize = 0;	//인스턴스데이터의 크기 : 월드행렬, 컬러

	// UI에서 사용 ////////////////////////////////////////////////////////////////////////
	_mat					m_matOrtho;
	_int					m_iClientSizeX = 0;
	_int					m_iClientSizeY = 0;


	// 쉐이더에서 사용 ////////////////////////////////////////////////////////////////////////
	_bool					m_bPass = false;
	_bool					m_bKeyDown = false;

	_int					m_iCount = 1;
	_float					m_fRange = 0.01f;

	// 상수테이블 보관하는 캐시 //////////////////////////////////////////////////////////////////////////
	_mat	m_matView;
	_mat	m_matProj;
	_mat	m_matViewProj;
	_mat	m_matLightViewProj;
	_mat	m_matScaleBias;
	_vec3	m_vLightPos;
	_vec3	m_vLightAt;
	_vec3	m_vLightUp;
	
	_vec4	m_v4Temp;
	_mat	m_matTemp;
};
END
#endif // !__RENDERER_MGR_H__
