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

	//���ؽ�, �ν��Ͻ������� �����ϰ� �� ������ ����
	void				Init_Instancing();
	//�����׷� ��� �ν��Ͻ������Ϳ� �߰�
	void				Add_Instance(const wstring& wstrInstanceTag, RENDERID eID, const _mat& matWorld, const _vec4& vValues, const _vec4& vColor);
	//��� �ν��Ͻ������͸� ��ȸ�ϸ� ����
	void				Render_Instances(RENDERID eID);
	void				Render_InstancesShadowMap(RENDERID eID);
	//�����͹��۸� iĭ��ŭ Ȯ��
	void				Expand_DataBuffer(LPDIRECT3DVERTEXBUFFER9& pBuffer, _uint iExpand);
	//���� ����Ǵ� ����� ����. �� ���� ������ ȣ���
	void				Update_MatViewProj();
	//���� ������� �ʴ� ����� ����. �ʿ��� �� �ܺο��� ȣ���� ��
	void				Update_MatLightViewProj(const _vec3& matLightPos, const _vec3& matLightAt, const _vec3& matLightUp, const _mat& matLightProj);
	void				Update_MatScaleBias(const _mat& matScaleBias);
	//�� ���̴��� �ʿ��� ������̺� �ѹ��� ����
	void				Set_ConstantTable_Normal(LPD3DXEFFECT _pEffect, const _mat& matWorld);
	void				Set_ConstantTable_ShadowMap(LPD3DXEFFECT _pEffect, const _mat& matWorld);

private:
	//���� ����� �ζ����Լ���
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
	//void				Render_Shadow();	//Render_Deferred() �� ������. Shader_Normal_Shadow�� �Բ� ��µ�

	void				Render_Blend(const _float& fFade);
	void				Render_LightBloom();
	void				Render_Result();		// ���� ȭ��

	//void				Render_HDR();

	void				Render_Buffer(LPD3DXEFFECT _pEffect, _int _iPass = 0);

private:
	virtual void		Free(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	list<CGameObject*>		m_RenderGroup[RENDER_END];
	LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9	m_pIB = nullptr;

	// �ν��Ͻ̿��� ��� //////////////////////////////////////////////////////////////////////////
	typedef struct {
		_vec4				vMatWorld1;	// TEXCOORD1~4	�������
		_vec4				vMatWorld2;
		_vec4				vMatWorld3;
		_vec4				vMatWorld4;
		_vec4				vValues;	// TEXCOORD5	A ������ / R / G �ٶ� / B
		_vec4				vColor;		// COLOR		�÷�
	}INSTANCE_DATA;	//�� ��ü�� �ν��Ͻ̿� ����� ���� = 1����Ʈ���� �� ����(Decl)

	typedef struct {
		_uint							dwCount = 0;
		vector<INSTANCE_DATA>			vecMatrix;
		LPDIRECT3DVERTEXBUFFER9			pDB = nullptr;
	}INSTANCE_OBJ;			//�� �����Ӹ��� �ν��Ͻ������͸� �����ϴ� ����ü
	_ulong					m_dwDataBufferLimit = 0;			//�����͹��۰� ���� �� �ִ� �ִ� �ν��Ͻ������� ��

	//LPDIRECT3DVERTEXBUFFER9				m_pDB = nullptr;	//�ν��Ͻ������͸� �����س��� �����͹���
	//_ulong					m_dwDataBufferRemain = 0;		//������ �����͹��۰� �� ���� �� �ִ� �ν��Ͻ������� ��
	//_ulong					m_dwDataBufferOffset = 0;		//������ �����͹��ۿ� Write�� �� ������

	map<const wstring, INSTANCE_OBJ*>	m_mapInstanceData[RENDER_END];	//�� ��ü���� �ϳ��� ����ü ����. Ű���� MeshTag
	LPDIRECT3DVERTEXDECLARATION9		m_pDecl;		//�Ϲ����� ���� ����
	LPDIRECT3DVERTEXDECLARATION9		m_pDeclInst;	//�ν��Ͻ̿� ���� ���� (�Ϲ�����+�ν��Ͻ�������)
	_ulong					m_dwVertexSize = 0;			//�Ϲ����� ������ ũ�� : ����Pos, ���, ź��Ʈ, UV
	_ulong					m_dwInstanceDataSize = 0;	//�ν��Ͻ��������� ũ�� : �������, �÷�

	// UI���� ��� ////////////////////////////////////////////////////////////////////////
	_mat					m_matOrtho;
	_int					m_iClientSizeX = 0;
	_int					m_iClientSizeY = 0;


	// ���̴����� ��� ////////////////////////////////////////////////////////////////////////
	_bool					m_bPass = false;
	_bool					m_bKeyDown = false;

	_int					m_iCount = 1;
	_float					m_fRange = 0.01f;

	// ������̺� �����ϴ� ĳ�� //////////////////////////////////////////////////////////////////////////
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
