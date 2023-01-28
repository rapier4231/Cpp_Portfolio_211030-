#include "stdafx.h"
#include "Harvestable_Tree.h"
#include "Export_Function.h"
#include "Inven.h"
#include "LR_Inventory.h"
#include "PhysX_Mgr.h"
#include "QuickSlot_UI.h"
#include "GuideFont_UI.h"
#include "RandomFunc.h"
#include "Dino.h"

USING(Engine)

CHarvestable_Tree::CHarvestable_Tree(LPDIRECT3DDEVICE9 _pDevice)
	: CHarvestable(_pDevice)
{
	ZeroMemory(&m_vDirBreak, sizeof(_vec3));
	ZeroMemory(&m_vColorInstancing, sizeof(_vec4));
}

CHarvestable_Tree::CHarvestable_Tree(const CHarvestable_Tree & rhs)
	: CHarvestable(rhs)
	, m_fHeight(rhs.m_fHeight)
{
}

HRESULT CHarvestable_Tree::Ready_GameObject_Prototype()
{
	CHarvestable::Ready_GameObject_Prototype();

	//m_iID = UNITID_TREE;

	m_tStat.fHp = 100.f;

	return S_OK;
}

HRESULT CHarvestable_Tree::Ready_GameObject(void * _pArg)
{
	CHarvestable::Ready_GameObject(_pArg);
	_ulong dwOption = Check_Arg(_pArg);
	Add_Component(dwOption);

	m_fDeleteTime = Get_RotX();

	//�ɼǰ��� ���� ũ������� �޸� ������.
	//	0		�Ϲ� ����		x1		0.0001	500		1 = 0.05
	//	1		���� ����		x1		0.02	500		0.005
	//	2 ~ 3	���� ��������		x0.5
	float fAdj = 1.f;
	if (1 == dwOption)
	{
		fAdj = 0.04f;
	}
	else if (2 < dwOption)
		fAdj = 0.5f;
	
	_vec3 vPos = *m_pTransformCom->Get_Pos();
	_vec3 vScale = *m_pTransformCom->Get_Scale();
	PxTransform px = PxTransform(vPos.x, vPos.y + 3.f, vPos.z);
	
	PxCapsuleControllerDesc cCapusuledesc;
	cCapusuledesc.height = 3.f;
	cCapusuledesc.radius = vScale.x * 500.f * fAdj;
	cCapusuledesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	cCapusuledesc.material = CPhysX_Mgr::GetInstance()->Get_Physics()->createMaterial(0.f, 0.f, 0.f);
	cCapusuledesc.position = PxExtendedVec3(px.p.x, px.p.y, px.p.z);

	m_pBaseActor = static_cast<PxCapsuleController*>(CPhysX_Mgr::GetInstance()->Get_Manager()->createController(cCapusuledesc));

	return S_OK;
}

_int CHarvestable_Tree::Update_GameObject(const _double & _dTimeDelta)
{
	if (m_tStat.fHp <= 0)
	{
		if(!m_bBreak)
			m_vDirBreak	= CRandomFunc::Rand_Dir_2D();
		m_bBreak = true;

		if (m_pBaseActor)
		{
			m_pBaseActor->release();
			m_pBaseActor = nullptr;
		}

		// ������ ������ ȿ�� �߻�, 
		m_fDissolveAmount += 0.02f;
		m_bDissolve = true;

		if (1.f < m_fDissolveAmount)
			return OBJECT_DEAD;
	}

	if (m_bBreak)
	{
		m_fDeleteTime += 30.f * (_float)_dTimeDelta;
		Set_RotX(D3DXToRadian(m_vDirBreak.x * m_fDeleteTime));
		Set_RotZ(D3DXToRadian(m_vDirBreak.z * m_fDeleteTime));
	}

	return CHarvestable::Update_GameObject(_dTimeDelta);
}

_int CHarvestable_Tree::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	_vec3 vSpherePos = m_pSphereColliderCom->Get_InteractPos();
	_vec3 vLightDir = _vec3(1.f, -10.f, -15.f);		//���� �޾ƿͼ� ��ġ�� <�Ұ�>
	D3DXVec3Normalize(&vLightDir, &vLightDir);
	_vec3 vTopPos = _vec3(vSpherePos.x, vSpherePos.y + m_fHeight, vSpherePos.z) + vLightDir * m_fHeight;

	//�׸��� �����׷� �߰� : Interaction �浹ü�� �� ����ü �ȿ� �� ��쿡�� �����׷쿡 �߰��Ѵ�
	//�����ߴٸ� �浹ü�� ������ �����(����)�� �Ű� �ѹ� �� �˻��Ѵ�. (�׸��ڸ� �帮���� �ϴµ��� Interaction �ö��̴��� ���� �ص��� �־� �ø��Ǵ� ���� ��ȭ)
	//if (Engine::Check_Tree(vSpherePos, m_pSphereColliderCom->Get_InteractRadius(), CULL_LIGHT) || 
	//	Engine::Check_Tree(vTopPos, m_pSphereColliderCom->Get_InteractRadius(), CULL_LIGHT))
	//{
	//	g_bInstancing ?
	//		Engine::Add_Instance(m_wstrMeshName, RENDER_SHADOWMAP, m_pTransformCom->Get_WorldMatrix()) :
	//		Engine::Add_RenderGroup(RENDER_SHADOWMAP, this);
	//}

	// �����׷� �߰� : �켱 Interaction �ö��̴��� ���� �˻��Ѵ�.
	_bool bDraw = false;
	if (Engine::Check_Tree(vSpherePos, m_pSphereColliderCom->Get_InteractRadius() /** Get_Scale().x*/))
		bDraw = true;
	else
	{
		//�����ߴٸ� �ٿ���ڽ��� �ѹ� �� �˻��Ѵ�. (������ �÷��ٺ� �� Interaction �ö��̴��� ���� �ص��� �־� �ø��Ǵ� ���� ��ȭ)
		for (size_t i = 0; i < 8; i++)	//������ 4�� ������ �˻�?
		{
			//8�� ���� �� �ϳ��� ����ü �ȿ� �ִٸ� ���� = ��� ������ ����ü ���̶�� ����
			if (true == Engine::Check_Point(m_vBoundingBox[i]))
			{
				bDraw = true;
				break;
			}
		}
	}
	if (bDraw)
	{
		g_bInstancing && !m_bBreak ?
			Engine::Add_Instance(m_wstrMeshName, RENDER_NONALPHA, m_pTransformCom->Get_WorldMatrix(), _vec4(0.f, 0.f, 0.f, 10.f), m_vColorInstancing) :
			Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	}

	return NO_EVENT;
}

void CHarvestable_Tree::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);

	m_pMeshCom->Render_Meshes(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);

#ifdef _DEBUG
	if (m_pSphereColliderCom)
		m_pSphereColliderCom->Render_SphereCollider();

#endif // !_DEBUG
	//////////////////////////////////////////////////////////////////////////
}

void CHarvestable_Tree::Render_GameObjectShadowMap()
{
	LPD3DXEFFECT pEffect = m_pShaderShadowMapCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);

	pEffect->Begin(NULL, 0);

	m_pMeshCom->Render_MeshesShadowMap(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);
}

void CHarvestable_Tree::Render_GameObjectShadow()
{
	LPD3DXEFFECT pEffect = m_pShaderShadowCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	m_pDevice->GetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &m_matProj);

	m_matWVP = m_pTransformCom->Get_WorldMatrix() * m_matView * m_matProj;
	m_matWLV = m_pTransformCom->Get_WorldMatrix() * m_matLightView * m_matLightProj;
	m_matWLVB = m_matWLV * m_matScaleBias;

	pEffect->SetMatrix("g_matWorldViewProj", &m_matWVP);
	pEffect->SetMatrix("g_matWorldLightViewProj", &m_matWLV);
	pEffect->SetMatrix("g_matWorldLightViewProjBias", &m_matWLVB);

	D3DXMatrixInverse(&m_matWLVB, NULL, &m_pTransformCom->Get_WorldMatrix());
	D3DXVec3Transform(&m_vDir, &m_vecLightPos, &m_matWLVB);
	D3DXVec4Normalize(&m_vDir, &m_vDir);
	m_vDir.w = 0;

	pEffect->SetVector("g_vLightDir", &m_vDir);

	Engine::Get_RenderTargetTexture(pEffect, L"ShadowMap", "g_ShadowMapTexture");

	pEffect->Begin(NULL, 0);

	m_pMeshCom->Render_MeshesShadow(pEffect, m_bDissolve);

	pEffect->End();

	Safe_Release(pEffect);
}

void CHarvestable_Tree::Check_Dying(const _double & _dTimeDelta)
{
	// �׾��� ��
}

CHarvestable_Tree * CHarvestable_Tree::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CHarvestable_Tree*	pInstance = new CHarvestable_Tree(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CHarvestable_Tree::Clone(void * _pArg)
{
	CHarvestable_Tree*	pClone = new CHarvestable_Tree(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CHarvestable_Tree::Free()
{
	CHarvestable::Free();
}

_int CHarvestable_Tree::Check_Arg(void * _pArg)
{
	if (_pArg)
	{
		if (dynamic_cast<OBJINFO*>((OBJINFO*)_pArg))
		{
			OBJINFO tInfo;
			memcpy(&tInfo, _pArg, sizeof(OBJINFO));

			//dwOption���� ���� MeshID�������� �����޽� �Ǵ�
			TCHAR szText[MAX_PATH];
			Engine::Get_MeshTag_Tree(szText, tInfo.dwOption);

			if (!szText || E_FAIL == Init_Mesh(szText))
			{
				ERR_MSG(L"CHarvestable_Tree : Failed to Find MeshTag");
				return -1;
			}
			_float fScaleAdj = 0.0008f;
			if (0.0008f < tInfo.vScale.x)		//�ּ�ũ�� 0.0008f
				fScaleAdj = tInfo.vScale.x;
			if (1 == tInfo.dwOption)			//���̳��� 500�躸��
				fScaleAdj *= 50.f;

			_float fShaker = 0.1f;
			Set_Scale(CRandomFunc::Shake_Float(fScaleAdj, fShaker));
			Set_Rot(CRandomFunc::Shake_Float(tInfo.vRot.x, fShaker)
				, CRandomFunc::Shake_Float(tInfo.vRot.y, fShaker)
				, CRandomFunc::Shake_Float(tInfo.vRot.z, fShaker));
			Set_Pos(tInfo.vPos.x, tInfo.vPos.y, tInfo.vPos.z);

			m_vColor = tInfo.vColor;
			m_vColorInstancing = _vec4(m_vColor.x, m_vColor.y, m_vColor.z, 0.f);
			if (0 == tInfo.dwOption)
				m_vColorInstancing.w = 10.f;

			/*wstring strTagDM = szText + wstring(L"_DM");
			if (E_FAIL == Init_DynamicMesh(strTagDM, tInfo.vScale, tInfo.vRot, tInfo.vPos))
			{
				ERR_MSG(L"CHarvestable_Tree : Failed to Find MeshTag_DM");
				return -1;
			}*/

			//����ġ ���������
			Add_ExpToLevel(tInfo.dwLevel);

			return tInfo.dwOption;
		}
		else
		{
			ERR_MSG(L"Wrong Format Argument");
			return 0;
		}
	}
	else
		//Argument ���� �� : �⺻�޽� �ε�
	{
		Init_Mesh(L"Proto_StaticMesh_Tree_A");
	}
	return 0;
}

_bool CHarvestable_Tree::HitBy(CUnit * pAttacker, _ulong dwAttackType, const _vec3& vCollisionPos)
{
	if (WEAPON_HAND != dwAttackType && WEAPON_DINO != dwAttackType &&
		WEAPON_HATCHET != dwAttackType && WEAPON_STONEPICK != dwAttackType)
		return false;

	_int iAmount = (_int)CRandomFunc::Shake_Float(pAttacker->Get_Stat(STAT_ATTACK), 0.2f);		// �Ǽ� 4~6			// ����,��� 16~24

	if (0 > iAmount)
		iAmount = 1;

	_int iWoodAmount, iThatchAmount;
	if (WEAPON_HAND == dwAttackType)
	{
		// �Ǽ�Ÿ�ݽ�, ���� 0~1, ¤ 2~3
		iWoodAmount = _int(iAmount * 0.2f);
		iThatchAmount = _int(iAmount * 0.5f);
		static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_TopGuideFont(Red_Sick_AttRock);
	}
	else if (WEAPON_HATCHET == dwAttackType)
	{
		// ������ Ÿ�ݽ�
		iWoodAmount = _int(iAmount * 0.5f);
		iThatchAmount = _int(iAmount * 0.2);
	}
	else if (WEAPON_STONEPICK == dwAttackType)
	{
		// ��̷� Ÿ�ݽ�
		iWoodAmount = _int(iAmount * 0.2f);
		iThatchAmount = _int(iAmount * 0.5f);
	}
	else if (WEAPON_DINO == dwAttackType)
	{
		// �������� Ÿ�� �� ���� ȿ�� ���� X
		iWoodAmount = _int(iAmount * 1.f);
		iThatchAmount = _int(iAmount * 1.f);
	}


	_bool bHit = CHarvestable::HitBy(pAttacker, dwAttackType, vCollisionPos);

	if (bHit)
	{
		vCollisionPos; //<<�ǰ�����Ʈ���>> ���� : ���� �ڱ� ��Į, ���������̳� ������
		
		if (WEAPON_DINO == dwAttackType)
		{
			if (0 < iWoodAmount)
			{
				static_cast<CDino*>(pAttacker)->Get_Inven()->Add_Item(ITEMID_Wood, iWoodAmount);
				static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_LeftGuideFont(ITEMID_Wood, (_int)iWoodAmount, 0);
				static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(_float(iWoodAmount));
			}
			if (0 < iThatchAmount)
			{
				static_cast<CDino*>(pAttacker)->Get_Inven()->Add_Item(ITEMID_Thatch, iThatchAmount);
				static_cast<CGuideFont_UI*>(Engine::Get_GameObject(L"GuideFont_UI", 0))->Print_LeftGuideFont(ITEMID_Thatch, (_int)iThatchAmount, 0);
				static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(_float(iThatchAmount));
			}
		}
		else
		{
			if (0 < iWoodAmount)
			{
				static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Add_Item(ITEMID_Wood, iWoodAmount);
				static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(_float(iWoodAmount));
			}
			if (0 < iThatchAmount)
			{
				static_cast<CQuickSlot_UI*>(Engine::Get_GameObject(L"QuickSlot_UI", 0))->Add_Item(ITEMID_Thatch, iThatchAmount);
				static_cast<CUnit*>(Engine::Get_GameObject(L"Layer_Player", 0))->Add_Exp(_float(iThatchAmount));
			}
		}
	}

	CGameObject* pGameObject = nullptr;

	m_fDustScale = 0.001f;

	pGameObject = Engine::Clone_GameObjectPrototype(L"Proto_Obj_Effect_Dust", &m_fDustScale);
	NULL_CHECK_RETURN(pGameObject, false);
	FAILED_CHECK_RETURN(Engine::Add_GameObjectInLayer(L"Layer_Effect", pGameObject), false);
	pGameObject->Set_Color(0.6f, 0.15f, 0.15f);

	_vec3 vPlayerPos = *static_cast<CTransform*>(Engine::Get_Component(L"Layer_Player", 0, L"Com_Transform", ID_DYNAMIC))->Get_Pos();
	_vec3 vDir = vPlayerPos - *m_pTransformCom->Get_Pos();
	D3DXVec3Normalize(&vDir, &vDir);
	vDir.y = 0.f;
	vDir *= 0.8f;
	_vec3 vResult = { (*m_pTransformCom->Get_Pos()).x, vPlayerPos.y + 1.5f, (*m_pTransformCom->Get_Pos()).z};

	static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(vResult + vDir);

	return bHit;
}

HRESULT CHarvestable_Tree::Add_Component(_ulong dwOption)
{
	CComponent*		pComponent = nullptr;

	//�ɼǿ� ���� �ٸ� �޽� �ο� ��
	//pComponent = m_pMeshCom = static_cast<CStaticMesh*>(Engine::Clone_ComponentPrototype(L"Proto_StaticMesh_RedWood_A"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	/*pComponent = m_pDynamicMeshCom = static_cast<CDynamicMesh*>(Engine::Clone_ComponentPrototype(L"Proto_DynamicMesh_Rock_Small_A"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_TestMesh", pComponent);*/

	///�浹ü
	//�ɼǰ��� ���� ũ������� �޸� ������.
	//	0 ~ 1	�Ϲ� ����		x1
	//	2 ~ 3	���� ��������		x0.5
	float fAdj = 1.f;
	if (1 == dwOption)
		fAdj = 0.04f;
	else if (2 < dwOption)
		fAdj = 0.5f;

	_float fSize = Get_Scale().x * fAdj;
	CSphereCollider::SPHEREINFO tInfo;
	tInfo.eColType = CSphereCollider::COL_HIT;
	tInfo.fRadius = 1333.33f * fSize;
	pComponent = m_pSphereColliderCom = static_cast<CSphereCollider*>(Engine::Clone_ComponentPrototype(L"Proto_Com_SphereCollider", &tInfo));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_SphereCollider", pComponent);

	tInfo.fRadius = 1666.f * fSize;
	tInfo.eColType = CSphereCollider::COL_INTERACTION;
	m_pSphereColliderCom->Add_SphereCollider(&tInfo, this);

	//������ġ ����
	_vec3 vPos = Get_Pos();
	vPos.y += 1.5f;
	m_pSphereColliderCom->Set_Pos(vPos);
	
	//������ ������ �˻縦 ���� �ٿ���ڽ��� �߰��� ȹ��
	_vec3 vMin, vMax;
	D3DXComputeBoundingBox(m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxNum(), sizeof(_vec3), &vMin, &vMax);
	m_vBoundingBox[0] = _vec3(vMin.x, vMin.y, vMin.z);	//	- - -
	m_vBoundingBox[1] = _vec3(vMax.x, vMin.y, vMin.z);	//	+ - -
	m_vBoundingBox[2] = _vec3(vMin.x, vMax.y, vMin.z);	//	- + -
	m_vBoundingBox[3] = _vec3(vMax.x, vMax.y, vMin.z);	//	+ + -
	m_vBoundingBox[4] = _vec3(vMin.x, vMin.y, vMax.z);	//	- - +
	m_vBoundingBox[5] = _vec3(vMax.x, vMin.y, vMax.z);	//	+ - +
	m_vBoundingBox[6] = _vec3(vMin.x, vMax.y, vMax.z);	//	- + +
	m_vBoundingBox[7] = _vec3(vMax.x, vMax.y, vMax.z);	//	+ + +

	//������(����)�� ����� ��ȯ�� ����
	m_pTransformCom->Update_Component(0);
	D3DXVec3TransformCoordArray(m_vBoundingBox, sizeof(_vec3), m_vBoundingBox, sizeof(_vec3), &m_pTransformCom->Get_WorldMatrix(), 8);

	m_fHeight = m_vBoundingBox[2].y - m_vBoundingBox[0].y;	//����� ��ȯ�� ������ vMax.y - vMin.y�� Ű���̸� ����

	return S_OK;
}