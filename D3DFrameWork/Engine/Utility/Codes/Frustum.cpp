#include "..\Headers\Frustum.h"
#include "Component.h"

#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum()
{
}

CFrustum::~CFrustum()
{
	Free();
}

HRESULT CFrustum::Ready_Frustum(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (FAILED(Set_Device(pGraphicDev)))
		return E_FAIL;

	m_vPointProj[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPointProj[1] = _vec3(1.f, 1.f, 0.f);
	m_vPointProj[2] = _vec3(1.f, -1.f, 0.f);
	m_vPointProj[3] = _vec3(-1.f, -1.f, 0.f);
	m_vPointProj[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPointProj[5] = _vec3(1.f, 1.f, 1.f);
	m_vPointProj[6] = _vec3(1.f, -1.f, 1.f);
	m_vPointProj[7] = _vec3(-1.f, -1.f, 1.f);

	return S_OK;
}

HRESULT CFrustum::Update_Frustum()
{
	//���/�� ī���� �ʱ�ȭ
	m_iDrawCount = 0;
	m_iCullCount = 0;
	
	//����ü ����
	_mat matProj, matView;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXMatrixInverse(&matView, NULL, &matView);
	matProj *= matView;

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPointProj[i], &matProj);
		//D3DXVec3TransformCoord(&m_vPoint[i], &m_vPointProj[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// z+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	return S_OK;
}

HRESULT CFrustum::Update_LightFrustum_Ortho()
{
	//���/�� ī��Ʈ �ʱ�ȭ
	m_iDrawShadowCount = 0;
	m_iCullShadowCount = 0;

	//����ü ����
	_mat matProj, matView;


	//��������� ������������ε�, ���μ��� 1000.f�� ������(ť��?) ����
	D3DXMatrixOrthoLH(&matProj, 100.f, 100.f, 0.f, 2000.f); //ť����
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	//������� ���� ������ ���� �÷��̾� ����+Adj ������ �ٶ󺸵��� ȸ����Ű�� ����̹Ƿ�
	//1. ���� ���� ���
	_vec3 vLightDir = _vec3(1.f, -10.f, -15.f);		//���� �޾ƿͼ� ��ġ�� <�Ұ�>
	D3DXVec3Normalize(&vLightDir, &vLightDir);

	//2. �÷��̾� ��ġ�� ����(�躤��) ���
	CTransform* pPlayerTransform = (CTransform*)Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC);
	if (!pPlayerTransform)
		return E_FAIL;
	_vec3 vPlayerPos, vPlayerDir;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	pPlayerTransform->Get_Info(INFO_LOOK, &vPlayerDir);
	vPlayerDir.y = 0.f;	//�̶� �躤�ʹ� y���� ���� <-���� �پ��Ű��� �������δ���??�̾
	D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);
	vPlayerPos -= vPlayerDir * m_fLightFrustumCenterAdj;	//At�� �÷��̾� ����+adj ����

	//Eye�� At���� ���� ���������� ���緮 ������ ����. Eye.y == vLightPos.y �� �����ϴ� �������� ���� (������ �������� 100����)
	//Up�� Up
	D3DXMatrixLookAtLH(&matView, &(vPlayerPos - vLightDir * 100.f), &(vPlayerPos), &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixInverse(&matView, NULL, &matView);

	//������ �� ������� �̿��� ����ü ������ ����� ��ȯ
	matProj *= matView;

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPointLight[i], &m_vPointProj[i], &matProj);
		//D3DXVec3TransformCoord(&m_vPointLight[i], &m_vPointProj[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_PlaneLight[0], &m_vPointLight[1], &m_vPointLight[5], &m_vPointLight[6]);

	// x-
	D3DXPlaneFromPoints(&m_PlaneLight[1], &m_vPointLight[4], &m_vPointLight[0], &m_vPointLight[3]);

	// z+
	D3DXPlaneFromPoints(&m_PlaneLight[2], &m_vPointLight[7], &m_vPointLight[6], &m_vPointLight[5]);

	// z-
	D3DXPlaneFromPoints(&m_PlaneLight[3], &m_vPointLight[0], &m_vPointLight[1], &m_vPointLight[2]);

	// y+
	D3DXPlaneFromPoints(&m_PlaneLight[4], &m_vPointLight[4], &m_vPointLight[5], &m_vPointLight[1]);

	// y-
	D3DXPlaneFromPoints(&m_PlaneLight[5], &m_vPointLight[3], &m_vPointLight[2], &m_vPointLight[6]);

	return S_OK;




	////���/�� ī��Ʈ �ʱ�ȭ
	//m_iDrawShadowCount = 0;
	//m_iCullShadowCount = 0;

	////����ü ����
	//_mat matProj, matView;


	////��������� ������������ε�, ���μ��� 1000.f�� ������(ť��?) ����
	//D3DXMatrixOrthoLH(&matProj, 1000.f, 1000.f, 0.f, 200.f);	//��������
	//															//D3DXMatrixOrthoLH(&matProj, 1000.f, 1000.f, 0.f, 2000.f); //ť����
	//D3DXMatrixInverse(&matProj, NULL, &matProj);

	////������� �÷��̾� ���� +250.f ������ �߽����� �÷��̾� ���濡 ������ �ǵ��� zȸ����Ű�� ����̹Ƿ�
	////�÷��̾� ��ġ�� �躤�� ���
	//CTransform* pPlayerTransform = (CTransform*)Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC);
	//if (!pPlayerTransform)
	//	return E_FAIL;
	//_vec3 vPlayerPos, vPlayerDir;
	//pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	//pPlayerTransform->Get_Info(INFO_LOOK, &vPlayerDir);
	//D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);
	////���� Eye, At, Up ���ϱ�
	////���� Eye (��ġ) : �÷��̾��� ��ġ���� ~ �躤�� ���� +250.f �������� ~ ���� �ݴ�������� 100.f(������� far����� ����)��ŭ ������ ��ġ 
	//_vec3 vLightPos = _vec3(-400.f + vPlayerPos.x, 100.f + vPlayerPos.y, 600.f + vPlayerPos.z);
	//vLightPos += m_fLightFrustumCenterAdj * vPlayerDir;
	////���� At : �÷��̾��� ��ġ���� ~ �躤�� ���� +250.f ����
	//_vec3 vLightAt = vLightPos + _vec3(1.f, -10.f, -15.f);
	////���� Up : �÷��̾��� �躤�� <- �̰Ÿ�?�µ�?

	////���� ������ �������� ����� ���
	////D3DXMatrixLookAtLH(&m_matLightView, &m_vecLightPos, &m_vecLightAt, &m_vecLightUp);
	//D3DXMatrixLookAtLH(&matView, &vLightPos, &vLightAt, &vPlayerDir);
	//D3DXMatrixInverse(&matView, NULL, &matView);
	//matProj *= matView;

	//for (_uint i = 0; i < 8; ++i)
	//{
	//	D3DXVec3TransformCoord(&m_vPointLight[i], &m_vPointProj[i], &matProj);
	//	//D3DXVec3TransformCoord(&m_vPointLight[i], &m_vPointProj[i], &matView);
	//}

	//// x+
	//D3DXPlaneFromPoints(&m_PlaneLight[0], &m_vPointLight[1], &m_vPointLight[5], &m_vPointLight[6]);

	//// x-
	//D3DXPlaneFromPoints(&m_PlaneLight[1], &m_vPointLight[4], &m_vPointLight[0], &m_vPointLight[3]);

	//// z+
	//D3DXPlaneFromPoints(&m_PlaneLight[2], &m_vPointLight[7], &m_vPointLight[6], &m_vPointLight[5]);

	//// z-
	//D3DXPlaneFromPoints(&m_PlaneLight[3], &m_vPointLight[0], &m_vPointLight[1], &m_vPointLight[2]);

	//// y+
	//D3DXPlaneFromPoints(&m_PlaneLight[4], &m_vPointLight[4], &m_vPointLight[5], &m_vPointLight[1]);

	//// y-
	//D3DXPlaneFromPoints(&m_PlaneLight[5], &m_vPointLight[3], &m_vPointLight[2], &m_vPointLight[6]);

	//return S_OK;
}

HRESULT CFrustum::Update_LightFrustum_Ortho(_vec3 vLightDir, _vec3 vPlayerPos, _vec3 vPlayerDir)
{
	//���/�� ī��Ʈ �ʱ�ȭ
	m_iDrawShadowCount = 0;
	m_iCullShadowCount = 0;

	//����ü ����
	_mat matProj, matView;


	//��������� ������������ε�, ���μ��� 1000.f�� ������(ť��?) ����
	D3DXMatrixOrthoLH(&matProj, 1000.f, 1000.f, 0.f, 200.f);	//��������
	//D3DXMatrixOrthoLH(&matProj, 1000.f, 1000.f, 0.f, 2000.f); //ť����
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	
	//������� �÷��̾� ���� +250.f ������ �߽����� �÷��̾� ���濡 ������ �ǵ��� zȸ����Ű�� ����̹Ƿ�
	//�÷��̾� ��ġ�� �躤�� ���
	/*CTransform* pPlayerTransform = (CTransform*)Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC);
	_vec3 vPlayerPos, vPlayerDir;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	pPlayerTransform->Get_Info(INFO_LOOK, &vPlayerDir);
	D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);*/
	//���� Eye, At, Up ���ϱ�
	//���� Eye (��ġ) : �÷��̾��� ��ġ���� ~ �躤�� ���� +250.f �������� ~ ���� �ݴ�������� 100.f(������� far����� ����)��ŭ ������ ��ġ 
	/*_vec3 vLightPos = _vec3(-400.f + vPlayerPos.x, 100.f + vPlayerPos.y, 600.f + vPlayerPos.z);
	vLightPos += m_fLightFrustumCenterAdj * vPlayerDir;*/
	//���� At : �÷��̾��� ��ġ���� ~ �躤�� ���� +250.f ����
	//_vec3 vLightAt = vLightPos + _vec3(1.f, -10.f, -15.f);
	//���� Up : �÷��̾��� �躤�� <- �̰Ÿ�?�µ�?


	//���� Eye, At, Up ���ϱ�
	//���� Eye (��ġ) : �÷��̾��� ��ġ���� ~ �躤�� ���� +250.f �������� ~ ���� �ݴ�������� 100.f(������� far����� ����)��ŭ ������ ��ġ 
	vPlayerPos += vPlayerDir * m_fLightFrustumCenterAdj;
	_vec3 vEye = vPlayerPos - vLightDir * 100.f;
	//���� At : �÷��̾��� ��ġ���� ~ �躤�� ���� +250.f ����
	_vec3 vAt = vPlayerPos;
	//���� Up : �÷��̾��� �躤�� <- �̰Ÿ�?�µ�?
	_vec3 vUp = vPlayerDir;
	
	//���� ������ �������� ����� ���
	//D3DXMatrixLookAtLH(&m_matLightView, &m_vecLightPos, &m_vecLightAt, &m_vecLightUp);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	D3DXMatrixInverse(&matView, NULL, &matView);		
	matProj *= matView;

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPointLight[i], &m_vPointProj[i], &matProj);
		//D3DXVec3TransformCoord(&m_vPointLight[i], &m_vPointProj[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_PlaneLight[0], &m_vPointLight[1], &m_vPointLight[5], &m_vPointLight[6]);

	// x-
	D3DXPlaneFromPoints(&m_PlaneLight[1], &m_vPointLight[4], &m_vPointLight[0], &m_vPointLight[3]);

	// z+
	D3DXPlaneFromPoints(&m_PlaneLight[2], &m_vPointLight[7], &m_vPointLight[6], &m_vPointLight[5]);

	// z-
	D3DXPlaneFromPoints(&m_PlaneLight[3], &m_vPointLight[0], &m_vPointLight[1], &m_vPointLight[2]);

	// y+
	D3DXPlaneFromPoints(&m_PlaneLight[4], &m_vPointLight[4], &m_vPointLight[5], &m_vPointLight[1]);

	// y-
	D3DXPlaneFromPoints(&m_PlaneLight[5], &m_vPointLight[3], &m_vPointLight[2], &m_vPointLight[6]);

	return S_OK;
}

_uint CFrustum::Get_DrawCount()
{
	return m_iDrawCount;
}

_uint CFrustum::Get_CullCount()
{
	return m_iCullCount;
}

_uint CFrustum::Get_DrawShadowCount()
{
	return m_iDrawShadowCount;
}

_uint CFrustum::Get_CullShadowCount()
{
	return m_iCullShadowCount;
}

void CFrustum::Set_LightFrustumCulling(_bool bTorF)
{
	m_bEnable_LightFrustumCulling = bTorF;
}

HRESULT CFrustum::Set_Device(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pDevice = pGraphicDev;
	m_pDevice->AddRef();

	return S_OK;
}

bool CFrustum::CheckPoint(_vec3 vPos, CULL_TYPE eCulltype)
{
	if (eCulltype == CULL_VIEW)
	{
		for (_uint i = 0; i < 6; ++i)
		{
			//�ﰢ���� �ð�������� �������Ƿ� ���� ������ �ٱ����̴�.
			//������� 0���� ũ�� -> ��� �ٱ��̹Ƿ� ����, false ��ȯ
			if (D3DXPlaneDotCoord(&m_Plane[i], &vPos) > 0.f)
			{
				m_iCullCount++;
				return false;
			}
		}

		//��� ���� ��� �����̸� true ��ȯ
		m_iDrawCount++;
		return true;
	}
	else if (eCulltype == CULL_LIGHT)
	{
		if (!m_bEnable_LightFrustumCulling)
		{
			m_iDrawShadowCount = -1;
			m_iCullShadowCount = -1;
			return true;
		}

		for (_uint i = 0; i < 6; ++i)
		{
			//�ﰢ���� �ð�������� �������Ƿ� ���� ������ �ٱ����̴�.
			//������� 0���� ũ�� -> ��� �ٱ��̹Ƿ� ����, false ��ȯ
			if (D3DXPlaneDotCoord(&m_PlaneLight[i], &vPos) > 0.f)
			{
				m_iCullShadowCount++;
				return false;
			}
		}

		//��� ���� ��� �����̸� true ��ȯ
		m_iDrawShadowCount++;
		return true;
	}

	ERR_MSG(L"CFrustrum::CheckPoint() : wrong CULL_TYPE");
	return false;
}

bool CFrustum::CheckPoint(float x, float y, float z, CULL_TYPE eCulltype)
{
	return CheckPoint(_vec3(x, y, z), eCulltype);
}

bool CFrustum::CheckSphere(_vec3 vCenter, float fRadius, CULL_TYPE eCulltype)
{
	if (eCulltype == CULL_VIEW)
	{
		for (_uint i = 0; i < 6; ++i)
		{
			//�ﰢ���� �ð�������� �������Ƿ� ���� ������ �ٱ����̴�.
			//������� ������ ���� �����̹Ƿ� �̰��� (������̸鼭) ���������� ũ�� 
			//	-> ���� ��� ������ ������ �������� ���̹Ƿ� ����, false ��ȯ
			if (D3DXPlaneDotCoord(&m_Plane[i], &vCenter) > fRadius)
			{
				m_iCullCount++;
				return false;
			}
		}

		m_iDrawCount++;
		return true;
	}
	else if (eCulltype == CULL_LIGHT)
	{
		if (!m_bEnable_LightFrustumCulling)
		{
			m_iDrawShadowCount = -1;
			m_iCullShadowCount = -1;
			return true;
		}

		for (_uint i = 0; i < 6; ++i)
		{
			//�ﰢ���� �ð�������� �������Ƿ� ���� ������ �ٱ����̴�.
			//������� ������ ���� �����̹Ƿ� �̰��� (������̸鼭) ���������� ũ�� 
			//	-> ���� ��� ������ ������ �������� ���̹Ƿ� ����, false ��ȯ
			if (D3DXPlaneDotCoord(&m_PlaneLight[i], &vCenter) > fRadius)
			{
				m_iCullShadowCount++;
				return false;
			}
		}

		m_iDrawShadowCount++;
		return true;
	}

	ERR_MSG(L"CFrustrum::CheckSphere() : wrong CULL_TYPE");
	return false;
}

bool CFrustum::CheckSphere(float xCenter, float yCenter, float zCenter, float fRadius, CULL_TYPE eCulltype)
{
	return CheckSphere(_vec3(xCenter, yCenter, zCenter), fRadius, eCulltype);
}

_bool CFrustum::CheckTree(_vec3 vCenter, float fRadius, CULL_TYPE eCulltype)
{
	if (eCulltype == CULL_VIEW)
	{
		for (_uint i = 0; i < 4; ++i)	//y��(4��, 5�� ���)�� ������ �ǽ����� ����
		{
			//�ﰢ���� �ð�������� �������Ƿ� ���� ������ �ٱ����̴�.
			//������� ������ ���� �����̹Ƿ� �̰��� (������̸鼭) ���������� ũ�� 
			//	-> ���� ��� ������ ������ �������� ���̹Ƿ� ����, false ��ȯ
			if (D3DXPlaneDotCoord(&m_Plane[i], &vCenter) > fRadius)
			{
				m_iCullCount++;
				return false;
			}
		}

		m_iDrawCount++;
		return true;
	}
	else if (eCulltype == CULL_LIGHT)
	{
		if (!m_bEnable_LightFrustumCulling)
		{
			m_iDrawShadowCount = -1;
			m_iCullShadowCount = -1;
			return true;
		}

		for (_uint i = 0; i < 4; ++i)	//y��(4��, 5�� ���)�� ������ �ǽ����� ����
		{
			//�ﰢ���� �ð�������� �������Ƿ� ���� ������ �ٱ����̴�.
			//������� ������ ���� �����̹Ƿ� �̰��� (������̸鼭) ���������� ũ�� 
			//	-> ���� ��� ������ ������ �������� ���̹Ƿ� ����, false ��ȯ
			if (D3DXPlaneDotCoord(&m_PlaneLight[i], &vCenter) > fRadius)
			{
				m_iCullShadowCount++;
				return false;
			}
		}

		m_iDrawShadowCount++;
		return true;
	}

	ERR_MSG(L"CFrustrum::CheckSphere() : wrong CULL_TYPE");
	return false;
}

_bool CFrustum::CheckTree(float xCenter, float yCenter, float zCenter, float fRadius, CULL_TYPE eCulltype)
{
	return CheckTree(_vec3(xCenter, yCenter, zCenter), fRadius, eCulltype);
}

//
//bool CFrustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
//{
//	// �� ������ �ҿ� ť���� �� ���� �ִ��� Ȯ���մϴ�.
//	for (int i = 0; i<6; i++)
//	{
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter - radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter - radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter - radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter - radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter + radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter + radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter + radius), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter + radius), 1.0f))) >= 0.0f)
//			continue;
//
//		return false;
//	}
//
//	return true;
//}
//
//
//bool CFrustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
//{
//	// �簢���� 6 ���� ��� �� �ϳ��� �� frustum �ȿ� �ִ��� Ȯ���մϴ�.
//	for (int i = 0; i<6; i++)
//	{
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
//			continue;
//		
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))) >= 0.0f)
//			continue;
//
//		return false;
//	}
//
//	return true;
//}

void CFrustum::Free()
{
	Safe_Release(m_pDevice);
}
