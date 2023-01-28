//////////////////////////////////////////////////////////////////////////
// 안씁니다
//////////////////////////////////////////////////////////////////////////
#include "Collision.h"

USING(Engine)

CCollision::CCollision(LPDIRECT3DDEVICE9 _pDevice)
	: CComponent(_pDevice)
	, m_eCollisionState(COLLISIONSTATE_END)
{
	D3DXMatrixIdentity(&m_matWorld);
}

CCollision::CCollision(const CCollision & rhs)
	: CComponent(rhs)
	, m_matWorld(rhs.m_matWorld)
	, m_pMesh(rhs.m_pMesh)
	, m_pBuffer(rhs.m_pBuffer)
	, m_fRadius(rhs.m_fRadius)
	, m_eCollisionState(rhs.m_eCollisionState)
{
	if (m_pMesh)
		m_pMesh->AddRef();
	if (m_pBuffer)
		m_pBuffer->AddRef();
}

HRESULT CCollision::Ready_Component_Prototype()
{
	m_fRadius = 1.f;	//스케일값 디폴트 1배

	//디버그용으로 출력할 구체 프로토타입 생성
#ifdef _DEBUG
	D3DXCreateSphere(m_pDevice, 1.f, 9, 9, &m_pMesh, &m_pBuffer);
	//CStaticMesh* pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Mesh_Sphere"));
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
#endif

	return S_OK;
}

HRESULT CCollision::Ready_Component(void * _pArg/* = nullptr*/)
{
	m_fRadius = *(_float*)_pArg;

	return S_OK;
}

HRESULT CCollision::Render_VIBuffer()
{
#ifdef _DEBUG
	if (COL_FALSE == m_eCollisionState)
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pMesh->DrawSubset(0);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_eCollisionState = COL_FALSE;
#endif

	return S_OK;
}

const _float & CCollision::Get_Radius()
{
	return m_fRadius;
}

void CCollision::Set_Radius(const _float & fRadius)
{
	m_matWorld._11 = m_matWorld._22 = m_matWorld._33 = m_fRadius = fRadius;
}

void CCollision::Set_Pos(const _vec3 & _vPos)
{
	memcpy(&m_matWorld._41, &_vPos, sizeof(_vec3));
}

void CCollision::Set_WorldMatrixPos(const _mat& matWorld)
{
	memcpy(&m_matWorld._41, &matWorld._41, sizeof(_vec3));
}

//_bool CCollision::IsMeshPicking(_vec3 * _pOutVec, const POINT& _pt)
//{
//	if (!m_pMesh)
//		return false;
//
//	_vec3		vMousePos;
//
//	D3DVIEWPORT9	ViewPort;
//	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
//
//	m_pDevice->GetViewport(&ViewPort);
//
//	// 뷰포트 영역에 있는 마우스를 투영 영역으로 변환
//	vMousePos.x = _pt.x / (ViewPort.Width * 0.5f) - 1.f;
//	vMousePos.y = _pt.y / -(ViewPort.Height * 0.5f) + 1.f;
//	vMousePos.z = 0.f;
//
//	// 투영 영역에 있는 마우스를 뷰스페이스로 변환
//	_mat		matProj;
//	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
//	D3DXMatrixInverse(&matProj, NULL, &matProj);
//	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);
//
//	_vec3	vRayPos, vRayDir;
//
//	vRayPos = _vec3(0.f, 0.f, 0.f);
//	vRayDir = vMousePos - vRayPos;
//
//	// 뷰스페이스의 vRayPos, vRayDir을 월드 영역으로 변환
//	_mat	matView;
//	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
//	D3DXMatrixInverse(&matView, NULL, &matView);
//
//	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
//	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
//
//	// 월드 영역의 vRayPos, vRayDir을 로컬 영역으로 변환
//	_mat matWorld = m_matWorld;
//	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
//
//	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
//	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
//
//
//	//////////////////////////////////////////////
//	_float fU, fV, fDis;
//	_ulong dwFaceIndex;
//	_ulong AllHit;
//	_int bIsPick;
//	//LPD3DXBUFFER pBuffer = nullptr;
//
//	D3DXIntersect(m_pMesh, &vRayPos, &vRayDir, &bIsPick, &dwFaceIndex, &fU, &fV, &fDis, NULL, &AllHit);
//
//	if (0 != bIsPick)
//	{
//		memcpy(_pOutVec, &m_matWorld.m[3][0], sizeof(_vec3));
//		return true;
//	}
//
//	return false;
//}

CCollision* CCollision::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CCollision*	pInstance = new CCollision(_pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CCollision::Clone(void * pArg/* = nullptr*/)
{
	CComponent* pClone = new CCollision(*this);

	if (FAILED(pClone->Ready_Component(pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CCollision::Free()
{
	if (m_pBuffer)
		Safe_Release(m_pBuffer);
	if(m_pMesh)
		Safe_Release(m_pMesh);

	CComponent::Free();
}
