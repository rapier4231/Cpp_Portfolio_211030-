#pragma once
#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

#include "Engine_Define.h"
#include "Base.h"
BEGIN(Engine)

class ENGINE_DLL CFrustum : public CBase
{
	DECLARE_SINGLETON(CFrustum)
private:
	explicit CFrustum();
	virtual	~CFrustum();

public:
	HRESULT Ready_Frustum(LPDIRECT3DDEVICE9 pGraphicDev);	//Ŭ�󿡼� ����1ȸ ȣ���� ��
	HRESULT Update_Frustum();			//CScene�� ������Ʈ ~ ���վ�����Ʈ ���̿� ȣ������
	HRESULT Update_LightFrustum_Ortho();			//CScene�� ������Ʈ ~ ���վ�����Ʈ ���̿� ȣ������
	HRESULT Update_LightFrustum_Ortho(_vec3 vLightDir, _vec3 vPlayerPos, _vec3 vPlayerDir);			//Ŭ���̾�Ʈ���� �ҷ��� �����̸� �̰ɷ�

private:
	virtual void Free() override;

public:
	//���� ����ü �ȿ� �ִ��� Ȯ���� ��ȯ
	_bool CheckPoint(_vec3 vPos, CULL_TYPE eCulltype);
	_bool CheckPoint(float x, float y, float z, CULL_TYPE eCulltype);

	//���� ����ü �ȿ� �ִ��� Ȯ���� ��ȯ
	_bool CheckSphere(_vec3 vCenter, float fRadius, CULL_TYPE eCulltype);
	_bool CheckSphere(float xCenter, float yCenter, float zCenter, float fRadius, CULL_TYPE eCulltype);

	//������ ���� ����ü �ȿ� �ִ��� Ȯ���� ��ȯ. �̶� y�� ������ �ǽ����� ����
	_bool CheckTree(_vec3 vCenter, float fRadius, CULL_TYPE eCulltype);
	_bool CheckTree(float xCenter, float yCenter, float zCenter, float fRadius, CULL_TYPE eCulltype);

	//bool CheckCube(float, float, float, float);
	//bool CheckRectangle(float, float, float, float, float, float);

	_uint Get_DrawCount();
	_uint Get_CullCount();
	_uint Get_DrawShadowCount();
	_uint Get_CullShadowCount();

	void Set_LightFrustumCulling(_bool bTorF);

private:
	HRESULT Set_Device(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;
	_vec3				m_vPointProj[8];	//������� ���� ���� (-1 ~ 1, -1 ~ 1, 0 ~ 1)

	//�� �������� �ø�
	_vec3				m_vPoint[8];
	D3DXPLANE			m_Plane[6];
	_uint				m_iDrawCount = 0;
	_uint				m_iCullCount = 0;
	
	//����Ʈ �������� �ø�
	_bool				m_bEnable_LightFrustumCulling = true;
	_float				m_fLightFrustumCenterAdj = 25.f;	//�÷��̾� �������� n��ŭ ������ ������ ����ü�� �߽����� �ΰڴ�
	_vec3				m_vPointLight[8];
	D3DXPLANE			m_PlaneLight[6];
	_uint				m_iDrawShadowCount = 0;
	_uint				m_iCullShadowCount = 0;
};
END
#endif // !__FRUSTUM_H__