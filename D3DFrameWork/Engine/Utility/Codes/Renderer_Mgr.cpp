#include "../Headers/Renderer_Mgr.h"
#include "../Headers/Prototype_Mgr.h"

#include "Export_Function.h"

#include "Shader.h"
USING(Engine)
IMPLEMENT_SINGLETON(CRenderer_Mgr)

CRenderer_Mgr::CRenderer_Mgr()
{
}


CRenderer_Mgr::~CRenderer_Mgr()
{
	Free();
}

HRESULT CRenderer_Mgr::Ready_Renderer(LPDIRECT3DDEVICE9 & _pDevice, _bool _bIsTool)
{
	m_pDevice = _pDevice;
	m_pDevice->AddRef();

	FAILED_CHECK_RETURN(_pDevice->CreateVertexBuffer(sizeof(VTX_SCREEN) * 4,
		0,  // ���� 0�� ��� ���� ���۸� ���
		FVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_pVB, NULL), E_FAIL);

	// �ε��� ���� ���� �Լ�
	FAILED_CHECK_RETURN(_pDevice->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,  // ���� 0�� ��� ���� ���۸� ���
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB, NULL), E_FAIL);

	D3DVIEWPORT9		ViewPort;
	_pDevice->GetViewport(&ViewPort);

	VTX_SCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(0.f - 0.5f, 0.f - 0.5f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec4(_float(ViewPort.Width - 0.5f), 0.f - 0.5f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4(_float(ViewPort.Width - 0.5f), _float(ViewPort.Height - 0.5f), 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);
	
	pVertex[3].vPosition = _vec4(0.f - 0.5f, _float(ViewPort.Height - 0.5f), 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*	pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	// �ν��Ͻ� ////////////////////////////////////////////////////////////////////////
	Init_Instancing();

	// ĳ�� ��ĵ� �ʱ�ȭ ////////////////////////////////////////////////////////////////////////
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matViewProj);

	m_vLightPos = _vec3(-400.f, 100.f, 600.f);
	m_vLightAt = m_vLightPos + _vec3(0.f, -10.f, -15.f);
	m_vLightUp = _vec3(0.f, 1.f, 0.f);
	_mat matLightProj;
	D3DXMatrixOrthoLH(&matLightProj, 100.f, 100.f, 0.f, 10000.f);
	Update_MatLightViewProj(m_vLightPos, m_vLightAt, m_vLightUp, matLightProj);

	float fOffsetX = 0.5f + (0.5f / SHADOW_WIDTH);
	float fOffsetY = 0.5f + (0.5f / SHADOW_HEIGHT);
	m_matScaleBias = _mat(0.5f, 0.0f, 0.0f, 0.0f,
	0.0f, -0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	fOffsetX, fOffsetY, 0.0f, 1.0f);

	return S_OK;
}

void CRenderer_Mgr::Set_ClientSize(_int _iX, _int _iY)
{
	m_iClientSizeX = _iX;
	m_iClientSizeY = _iY;
}

void CRenderer_Mgr::Add_RenderGroup(RENDERID eID, CGameObject * pGameObject)
{
	if (RENDER_END <= eID)
		return;

	m_RenderGroup[eID].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer_Mgr::Render_GameObject(const _float& fFade)
{
	Update_MatViewProj();

	_mat	matLightProj;
	D3DXMatrixOrthoLH(&matLightProj, 100.f, 100.f, 0.f, 10000.f);
	Update_MatLightViewProj(Engine::Get_ShadowLight().vLightPos, Engine::Get_ShadowLight().vLightAt, _vec3(0.f, 1.f, 0.f), matLightProj);

	/// ���۵� //////////////////////
	if (!Engine::Get_ShadowOnOff())
	{
		Render_ShadowMap();
		Render_Deferred();
		//Render_Shadow();	//Render_Deferred()�� �� �Բ� ��µ�
		//Render_ShadowBlur();
	}
	else
		Render_Deferred();

	Render_ShadowBlur();
	Render_LightAcc();
	//Render_HDR();
	Render_Blend(fFade);
	Render_LightBloom();
	Render_LightBlur();

	Render_Result();		// ����
	/////////////////////////////////

	Render_Effect();

	Render_UI();

//#ifdef _DEBUG
		/// ����� ���� Ÿ�� ���
		if (Get_DIKeyState(DIK_TAB) & 0x80)
		{
			Engine::Render_DebugBuffer(L"MRT_Deferred");
			Engine::Render_DebugBuffer(L"MRT_LightAcc");
			Engine::Render_DebugBuffer(L"MRT_LightBlur");
			Engine::Render_DebugBuffer(L"MRT_LightBloom");
			Engine::Render_DebugBuffer(L"MRT_Blend");
			Engine::Render_DebugBuffer(L"MRT_ShadowMap");
			Engine::Render_DebugBuffer(L"MRT_Shadow");
			Engine::Render_DebugBuffer(L"MRT_ShadowBlur");
		}

		else if (Get_DIKeyState(DIK_F2) & 0x80)
		{
			Engine::Render_DebugBuffer(L"MRT_HDRSource");
			Engine::Render_DebugBuffer(L"MRT_HDRScaledSource");
			Engine::Render_DebugBuffer(L"MRT_HDRBrightFilter");
		}
//#endif
		Clear_RenderGroup();
}

void CRenderer_Mgr::Clear_RenderGroup(void)
{
	for (_ulong i = 0; i < RENDER_END; ++i)
	{
		for (auto& iter : m_RenderGroup[i])
			Safe_Release(iter);
		m_RenderGroup[i].clear();

		for (auto& iter : m_mapInstanceData[i])	//�ν��Ͻ������ʹ� ī��Ʈ�� 0���� �ʱ�ȭ�ϰ� ���������̳ʴ� �ʱ�ȭ���� ����
			iter.second->dwCount = 0;			//��ü���� ���� ���������̳� 100ĭ�� �� ������ ���Ҵ����� �ʰ� ����鼭 ����
	}
}

void CRenderer_Mgr::Init_Instancing()
{
	//�⺻ �������� �� �ν��Ͻ��� ��������(�޽� ���ؽ�, �ε���). �ν��Ͻ��� ������ �׻� �⺻ �������� �ǵ��� ��
	const D3DVERTEXELEMENT9 tDecl_Geometry[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	////�� �ν��Ͻ��� ����(�������). ���� �����ص���
	//const D3DVERTEXELEMENT9 g_VBDecl_InstanceData[] =
	//{
	//	{ 1,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
	//	{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
	//	{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
	//	{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
	//	{ 1, 64, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
	//	D3DDECL_END()
	//};
	//�� �ΰ� ��ģ��. �ν��Ͻ��� ���� �� ������ �����
	const D3DVERTEXELEMENT9 tDecl_Instancing[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	//��Ʈ���� �Ѿ �� Offset�� 0���� �ٽ� ������
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
		{ 1, 64, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
		{ 1, 80, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};
	m_dwVertexSize = D3DXGetDeclVertexSize(tDecl_Instancing, 0);			//44
	m_dwInstanceDataSize = D3DXGetDeclVertexSize(tDecl_Instancing, 1);	//80 -> 96 ���� Ȯ������

	m_pDevice->CreateVertexDeclaration(tDecl_Geometry, &m_pDecl);	//�⺻ ����
	//m_pDevice->CreateVertexDeclaration(g_VBDecl_InstanceData, &m_pDeclInst);	//�ν��Ͻ������͸�. ���� �����ص���
	m_pDevice->CreateVertexDeclaration(tDecl_Instancing, &m_pDeclInst);	//�ν��Ͻ̿� ����

	//�ν��Ͻ������͸� �ٷ� �������� ���� (Ǯ, ����, ���� ���) <- ��ü���� �Űܾ� �� <�Ұ�>
	m_dwDataBufferLimit = 10;	//�Ѳ����� �ִ� �� ���� �ν��Ͻ��� �����Ұ����� ����
}

void CRenderer_Mgr::Add_Instance(const wstring& wstrInstanceTag, RENDERID eID, const _mat& matWorld, const _vec4& vValues, const _vec4& vColor)
{
	//�ش��ϴ� �����׷쿡�� �ش� ����ƽ�޽��� �ν��Ͻ������͸� Ž��	//ex)����� ����
	auto iter = m_mapInstanceData[eID].find(wstrInstanceTag);
	if (iter == m_mapInstanceData[eID].end())
	{
		//�ش��ϴ� �ν��Ͻ������͸� �� ã�Ҵٸ� ���ο� �ν��Ͻ������� ����ü ����
		INSTANCE_OBJ* pInstanceData = new INSTANCE_OBJ;
		
		//���� ������ �ν��Ͻ������Ͱ� ������ �ش� ����ƽ�޽��� �ν��Ͻ��� ����� �� �ְ� �ʿ� �߰�
		m_mapInstanceData[eID].emplace(wstrInstanceTag, pInstanceData);

		//�� �ν��Ͻ������͸� ����Ű���� iter ��Ž��
		iter = m_mapInstanceData[eID].find(wstrInstanceTag);

		//�� �ν��Ͻ������Ϳ� ���� 10ĭ�� �غ��� 0�ʱ�ȭ�ص� (10�� �̻��̸� ���� Ȯ��ɰ���)
		pInstanceData->vecMatrix.reserve(10);
		while (10 != pInstanceData->vecMatrix.size())	
		{
			INSTANCE_DATA tInst;
			ZeroMemory(&tInst, sizeof(INSTANCE_DATA));
			iter->second->vecMatrix.emplace_back(tInst);
		}
		m_pDevice->CreateVertexBuffer(m_dwInstanceDataSize * m_dwDataBufferLimit, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &pInstanceData->pDB, 0);
	}

	//���� ī��Ʈ�� ������ �� �����͸� ��� ��ġ�� �����ϰ� �ν��Ͻ������� ����, ī��Ʈ +1
	_ulong dwAddress = iter->second->dwCount++;
	memcpy(&(iter->second->vecMatrix[dwAddress].vMatWorld1), matWorld, sizeof(_vec4) * 4);
	memcpy(&(iter->second->vecMatrix[dwAddress].vColor), vColor, sizeof(_vec4));
	//memcpy(&(iter->second->vecMatrix[dwAddress].vValues), vValues, sizeof(_vec4));

	//�׸��� �׽�Ʈ��	//////////////////////////////////////////////////////////
	_mat matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &matWorld);
	_vec4 vLightDir_4;
	D3DXVec3Transform(&vLightDir_4, &_vec3(-400.f, 100.f, 600.f), &matInvWorld);
	D3DXVec4Normalize(&vLightDir_4, &vLightDir_4);
	vLightDir_4.w = 0.f;
	memcpy(&(iter->second->vecMatrix[dwAddress].vValues), vLightDir_4, sizeof(_vec4));
	//////////////////////////////////////////////////////////////////////////

	//���� �ν��Ͻ������Ͱ� 100���� �����ߴٸ� 
	//���� LateUpdate()�̱� ������ ��� ������ �� ����
	//���� ĭ�� �ø��ų� ���ο� �ν��Ͻ������� �����ؾ� ��
	if (iter->second->vecMatrix.size() - 1 <= iter->second->dwCount)
	{
		//ERR_MSG(L"�ν��Ͻ��� �ʹ� ���Ƽ� ���� ����� �����߾��!");
		iter->second->vecMatrix.reserve(iter->second->vecMatrix.size() + 10);
		for (size_t i = 0; i < 10; i++)
		{
			INSTANCE_DATA tInst;
			ZeroMemory(&tInst, sizeof(INSTANCE_DATA));
			iter->second->vecMatrix.emplace_back(tInst);
		}
	}
}

void CRenderer_Mgr::Render_Instances(RENDERID eID)
{
	if (0 == m_mapInstanceData[eID].size())
		return;

	//�ν��Ͻ̿� ��������� ����
	m_pDevice->SetVertexDeclaration(m_pDeclInst);

	//����Ʈ��ü ����
	CShader* pShaderCom = static_cast<CShader*>(CPrototype_Mgr::GetInstance()->Pick_ComponentPrototype(L"Proto_Shader_Instance"));
	if (!pShaderCom)
	{
		ERR_MSG(L"���̴� ������Ʈ �����͸� �� �����Խ��ϴ�.");
		return;
	}
	LPD3DXEFFECT pEffect = pShaderCom->Get_EffectHandle();
	if (!pEffect)
	{
		ERR_MSG(L"���̴� ������Ʈ���� ����Ʈ �ڵ��� �� �����Խ��ϴ�.");
		return;
	}
	pEffect->AddRef();

	//����Ʈ��ü�� ������̺� ���� : ��� �����, ��� �ν��Ͻ� ������
	//matWVP
	pEffect->SetMatrix("g_matView", &m_matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);
	
	//matLVPB
	/*_mat matLightView, matLightProj, matLVP, matScaleBias;
	_vec3 vLightPos = _vec3(-400.f, 100.f, 600.f);
	_vec3 vLightAt = vLightPos + _vec3(1.f, -10.f, -15.f);
	_vec3 vLightUp = _vec3(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&matLightView, &vLightPos, &vLightAt, &vLightUp);
	D3DXMatrixOrthoLH(&matLightProj, 100.f, 100.f, 0.f, 10000.f);
	matLVP = matLightView * matLightProj;*/
	pEffect->SetMatrix("g_matLightViewProj", &m_matLightViewProj);

	/*float fOffsetX = 0.5f + (0.5f / SHADOW_WIDTH);
	float fOffsetY = 0.5f + (0.5f / SHADOW_HEIGHT);
	matScaleBias = _mat(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		fOffsetX, fOffsetY, 0.0f, 1.0f);*/
	pEffect->SetMatrix("g_matScaleBias", &m_matScaleBias);

	//ShadowMap
	Engine::Get_RenderTargetTexture(pEffect, L"ShadowMap", "g_ShadowMapTexture");

	//Dissolve
	CTexture* pTextureCom = static_cast<CTexture*>(CPrototype_Mgr::GetInstance()->Pick_ComponentPrototype(L"Proto_Texture_Dissolve"));
	pTextureCom->Set_Texture(pEffect, "g_DissolveTexture", 0);

	//��� �ν��Ͻ������͸� ��ȸ�ϸ� ������
	for (auto& tPair : m_mapInstanceData[eID])
	{
		if (0 == tPair.second->dwCount)
			continue;
		if (tPair.second->vecMatrix.size() <= tPair.second->dwCount)
		{
			ERR_MSG(L"���������� ������ �ν��Ͻ������͸� �����Ϸ��� �մϴ�.");
			continue;
		}

		//�ν��Ͻ������͸� �����ϱ� ���� �����͹����� ũ�Ⱑ ������� Ȯ��
		if (m_dwDataBufferLimit < tPair.second->dwCount)
		{
			//���ڶ�� �׸�ŭ �����͹��� Ȯ��
			Expand_DataBuffer(tPair.second->pDB, tPair.second->dwCount + 1);
		}

		//���������̳ʿ� ��Ƶ� �ν��Ͻ��������� �ּҰ� ���
		void* pRawData;
		pRawData = tPair.second->vecMatrix.data();

		//��Ƶ� �ν��Ͻ������͸� �����͹��ۿ� ����
		INSTANCE_DATA*		pInstData = nullptr;
		tPair.second->pDB->Lock(0, 0, (void**)&pInstData, D3DLOCK_DISCARD);	//�������ּҸ� �޾ƿԾ�
		memcpy(pInstData, pRawData, m_dwInstanceDataSize * tPair.second->dwCount);	//�޾ƿ� �ּҿ� �ε����͸� �����س���
		tPair.second->pDB->Unlock();


		//�׸� �޽ð�ü�� ������Ŵ������� ������
		CStaticMesh* pMeshCom = static_cast<CStaticMesh*>(CPrototype_Mgr::GetInstance()->Pick_ComponentPrototype(tPair.first));
		if (!pMeshCom)
		{
			ERR_MSG(L"�ν��Ͻ��������� �±׿� �ش��ϴ� ����ƽ�޽ø� ã�� �� �����ϴ�.");
			continue;
		}

		//�޽��� ���ؽ�����, �ε������� ������
		LPDIRECT3DVERTEXBUFFER9 pVB;
		LPDIRECT3DINDEXBUFFER9 pIB;
		pMeshCom->Get_Mesh()->GetVertexBuffer(&pVB);
		pMeshCom->Get_Mesh()->GetIndexBuffer(&pIB);

		//�������� ����
		m_pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | tPair.second->dwCount);
		m_pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);

		//�� ���۸� ��Ʈ���� ���ε� : ��� ����� �� �׸� ������ ������
		m_pDevice->SetStreamSource(0, pVB, 0, m_dwVertexSize);
		m_pDevice->SetStreamSource(1, tPair.second->pDB, 0, m_dwInstanceDataSize);
		m_pDevice->SetIndices(pIB);

		{	//�� ���� ��üũ�� �׽�Ʈ�ڵ�
			//VTXTEST* pVtxCheck = nullptr; //= (VTXTEST*)pVB;
			//pVB->Lock(0, 0, (void**)&pVtxCheck, 0);
			//pVB->Unlock();

			INSTANCE_DATA* pInstDataCheck = nullptr; // = (INSTANCE_MAT_COLOR*)pDB;
			tPair.second->pDB->Lock(0, 0, (void**)&pInstDataCheck, 0);
			tPair.second->pDB->Unlock();
		}

		//���� ����
		_uint iPassMax = 0;
		_int iPass = 0;
		pEffect->Begin(&iPassMax, 0);

		//Ư���� �н� ����
		if (tPair.second->vecMatrix.front().vColor.w < 0.f)
			iPass = 4;	//��ǻ��1 ���� : Portal ��
		else if (tPair.second->vecMatrix.front().vColor.w < -5.f)
			iPass = 7;	//Metalic
		else if (5.f < tPair.second->vecMatrix.front().vColor.w)
			iPass = 5;	//ANR ��ָ� : Tree_A

	   //��� ����� ��ȸ�ϸ� ����
		_ulong iSubsetCount = pMeshCom->Get_SubsetCount();
		for (_ulong i = 0; i < iSubsetCount; i++)
		{
			//�ش� ������� �ؽ�ó ���� + �׸���
			pMeshCom->Render_Subset_ForInstancing(pEffect, i, iPass);
		}
		//�ν��Ͻ� ���� ��,
		pEffect->End();

		//���� ���� �ν��Ͻ��� �� ���� �ʱ�ȭ. �ʿ��ϴٸ� ���⼭ ���������̳ʵ� ZeroMemory() �ǽ�
		tPair.second->dwCount = 0;

		//���� ���� �ν��Ͻ��� �����ּ� ����
		Safe_Release(pVB);
		Safe_Release(pIB);

		//�̻� ��� �ν��Ͻ��� ����� ������ �ݺ�

	//�Ϲ����� �������÷� �ǵ���
		m_pDevice->SetStreamSourceFreq(0, 1);
		m_pDevice->SetStreamSourceFreq(1, 1);
	}

	//�Ϲ����� ������������ �ǵ���
	m_pDevice->SetVertexDeclaration(m_pDecl);

	//����Ʈ��ü ����
	Safe_Release(pEffect);
}

void CRenderer_Mgr::Render_InstancesShadowMap(RENDERID eID)
{
	if (0 == m_mapInstanceData[eID].size())
		return;

	//�ν��Ͻ̿� ��������� ����
	m_pDevice->SetVertexDeclaration(m_pDeclInst);

	//����Ʈ��ü ����
	CShader* pShaderCom = static_cast<CShader*>(CPrototype_Mgr::GetInstance()->Pick_ComponentPrototype(L"Proto_Shader_Instance_ShadowMap"));
	if (!pShaderCom)
	{
		ERR_MSG(L"���̴� ������Ʈ �����͸� �� �����Խ��ϴ�.");
		return;
	}
	LPD3DXEFFECT pEffect = pShaderCom->Get_EffectHandle();
	if (!pEffect)
	{
		ERR_MSG(L"���̴� ������Ʈ���� ����Ʈ �ڵ��� �� �����Խ��ϴ�.");
		return;
	}
	pEffect->AddRef();

	//����Ʈ��ü�� ������̺� ���� : ��� �����, ��� �ν��Ͻ� ������
	/*_mat matLightView, matLightProj, matLVP;

	_vec3 vLightPos = _vec3(-400.f, 100.f, 600.f);
	_vec3 vLightAt = vLightPos + _vec3(1.f, -10.f, -15.f);
	_vec3 vLightUp = _vec3(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&matLightView, &vLightPos, &vLightAt, &vLightUp);
	D3DXMatrixOrthoLH(&matLightProj, 100.f, 100.f, 0.f, 10000.f);
	matLVP = matLightView * matLightProj;*/
	pEffect->SetMatrix("g_matLightViewProj", &m_matLightViewProj);

	//�������ؽ�ó ���� : ��� �����, ��� �ν��Ͻ� ������
	CTexture* pTextureCom = static_cast<CTexture*>(CPrototype_Mgr::GetInstance()->Pick_ComponentPrototype(L"Proto_Texture_Dissolve"));
	pTextureCom->Set_Texture(pEffect, "g_DissolveTexture", 0);

	//��� �ν��Ͻ������͸� ��ȸ�ϸ� ������
	for (auto& tPair : m_mapInstanceData[eID])
	{
		if (0 == tPair.second->dwCount)
			continue;
		if (tPair.second->vecMatrix.size() <= tPair.second->dwCount)
		{
			ERR_MSG(L"���������� ������ �ν��Ͻ������͸� �����Ϸ��� �մϴ�.");
			continue;
		}

		//�ν��Ͻ������͸� �����ϱ� ���� �����͹����� ũ�Ⱑ ������� Ȯ��
		if (m_dwDataBufferLimit < tPair.second->dwCount)
		{
			//���ڶ�� �׸�ŭ �����͹��� Ȯ��
			Expand_DataBuffer(tPair.second->pDB, tPair.second->dwCount + 1);
		}

		//���������̳ʿ� ��Ƶ� �ν��Ͻ��������� �ּҰ� ���
		void* pRawData;
		pRawData = tPair.second->vecMatrix.data();

		//��Ƶ� �ν��Ͻ������͸� �����͹��ۿ� ����
		INSTANCE_DATA*		pInstData = nullptr;
		tPair.second->pDB->Lock(0, 0, (void**)&pInstData, D3DLOCK_DISCARD);	//�������ּҸ� �޾ƿԾ�
		memcpy(pInstData, pRawData, m_dwInstanceDataSize * tPair.second->dwCount);	//�޾ƿ� �ּҿ� �ε����͸� �����س���
		tPair.second->pDB->Unlock();


		//�׸� �޽ð�ü�� ������Ŵ������� ������
		CStaticMesh* pMeshCom = static_cast<CStaticMesh*>(CPrototype_Mgr::GetInstance()->Pick_ComponentPrototype(tPair.first));
		if (!pMeshCom)
		{
			ERR_MSG(L"�ν��Ͻ��������� �±׿� �ش��ϴ� ����ƽ�޽ø� ã�� �� �����ϴ�.");
			continue;
		}

		//�޽��� ���ؽ�����, �ε������� ������
		LPDIRECT3DVERTEXBUFFER9 pVB;
		LPDIRECT3DINDEXBUFFER9 pIB;
		pMeshCom->Get_Mesh()->GetVertexBuffer(&pVB);
		pMeshCom->Get_Mesh()->GetIndexBuffer(&pIB);

		//�������� ����
		m_pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | tPair.second->dwCount);
		m_pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);

		//�� ���۸� ��Ʈ���� ���ε� : ��� ����� �� �׸� ������ ������
		m_pDevice->SetStreamSource(0, pVB, 0, m_dwVertexSize);
		m_pDevice->SetStreamSource(1, tPair.second->pDB, 0, m_dwInstanceDataSize);
		m_pDevice->SetIndices(pIB);

		//{	//�� ���� ��üũ�� �׽�Ʈ�ڵ�
		//	VTXTEST* pVtxCheck = nullptr; //= (VTXTEST*)pVB;
		//	pVB->Lock(0, 0, (void**)&pVtxCheck, 0);
		//	pVB->Unlock();
		//
		//	INSTANCE_MAT_COLOR* pInstDataCheck = nullptr; // = (INSTANCE_MAT_COLOR*)pDB;
		//	pDB->Lock(0, 0, (void**)&pInstDataCheck, 0);
		//	pDB->Unlock();
		//}

		//���� ����
		_uint iPassMax = 0;
		_int iPass = 0;
		pEffect->Begin(&iPassMax, 0);

		//��� ����� ��ȸ�ϸ� ����
		_ulong iSubsetCount = pMeshCom->Get_SubsetCount();
		for (_ulong i = 0; i < iSubsetCount; i++)
		{
			//�ش� ������� �ؽ�ó ���� + �׸���
			pMeshCom->Render_Subset_ForInstancing(pEffect, i, iPass);
		}

		//�ν��Ͻ� ���� ��
		pEffect->End();

		//���� ���� �ν��Ͻ��� �� ���� �ʱ�ȭ. �ʿ��ϴٸ� ���⼭ ���������̳ʵ� ZeroMemory() �ǽ�
		tPair.second->dwCount = 0;

		//���� ���� �ν��Ͻ��� �����ּ� ����
		Safe_Release(pVB);
		Safe_Release(pIB);

		//�̻� ��� �ν��Ͻ��� ����� ������ �ݺ�

		//�Ϲ����� �������÷� �ǵ���
		m_pDevice->SetStreamSourceFreq(0, 1);
		m_pDevice->SetStreamSourceFreq(1, 1);
	}

	//�Ϲ����� ������������ �ǵ���
	m_pDevice->SetVertexDeclaration(m_pDecl);

	//����Ʈ��ü ����
	Safe_Release(pEffect);
}

void CRenderer_Mgr::Expand_DataBuffer(LPDIRECT3DVERTEXBUFFER9& pBuffer, _uint iExpand)
{
	Safe_Release(pBuffer);
	m_pDevice->CreateVertexBuffer(m_dwInstanceDataSize * iExpand, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &pBuffer, 0);
}

void CRenderer_Mgr::Update_MatViewProj()
{
	m_pDevice->GetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &m_matProj);
	m_matViewProj = m_matView * m_matProj;
}

void CRenderer_Mgr::Update_MatLightViewProj(const _vec3 & vLightPos, const _vec3 & vLightAt, const _vec3 & vLightUp, const _mat & matLightProj)
{
	m_vLightPos = vLightPos;
	m_vLightAt = vLightAt;
	m_vLightUp = vLightUp;
	_mat matLightView;
	D3DXMatrixLookAtLH(&matLightView, &m_vLightPos, &m_vLightAt, &m_vLightUp);
	m_matLightViewProj = matLightView * matLightProj;
}

void CRenderer_Mgr::Update_MatScaleBias(const _mat & matScaleBias)
{
	m_matScaleBias = matScaleBias;
}

void CRenderer_Mgr::Set_ConstantTable_Normal(LPD3DXEFFECT _pEffect, const _mat & matWorld)
{
	_pEffect->SetMatrix("g_matWorld", &matWorld);
	Set_ConstantTable_ViewProj(_pEffect);
	Set_ConstantTable_LightViewProj(_pEffect);
	Set_ConstantTable_ScaleBias(_pEffect);
	Set_ConstantTable_LightDirLocal(_pEffect, matWorld);
	Get_RenderTargetTexture(_pEffect, L"ShadowMap", "g_ShadowMapTexture");
}

void CRenderer_Mgr::Set_ConstantTable_ShadowMap(LPD3DXEFFECT _pEffect, const _mat & matWorld)
{
	_pEffect->SetMatrix("g_matWorld", &matWorld);
	Set_ConstantTable_WorldLightViewProj(_pEffect, matWorld);
}

void CRenderer_Mgr::Render_Priority()
{
	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_GameObject();
}

void CRenderer_Mgr::Render_NonAlpha()
{
	Render_Instances(RENDER_NONALPHA);

	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
	{
		iter->Render_GameObject();
		//iter->Render_GameObjectShadow();
	}
}

void CRenderer_Mgr::Render_Alpha()
{
	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_GameObject();
}

void CRenderer_Mgr::Render_UI()
{
	//��Ʈ ��ġ�� ������̶� ��ġ ���� ������Ʈ ����
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	for (auto& iter : m_RenderGroup[RENDER_BACKUI])
		iter->Render_GameObject();

	_mat matOldView, matOldProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matOldProj);

	D3DXMatrixOrthoLH(&m_matOrtho, (_float)m_iClientSizeX, (_float)m_iClientSizeY, 0, 1.f);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matOrtho);
	for (auto& iter : m_RenderGroup[RENDER_WolrdPosUI])
		iter->Render_GameObject(m_matOrtho, matOldProj);
	
	for (auto& iter : m_RenderGroup[RENDER_ORTHOBACKUI])
		iter->Render_GameObject(m_matOrtho);

	for (auto& iter : m_RenderGroup[RENDER_ORTHOUI])
		iter->Render_GameObject(m_matOrtho);

	for (auto& iter : m_RenderGroup[RENDER_ITEMBORDER])
		iter->Render_GameObject(m_matOrtho);

	for (auto& iter : m_RenderGroup[RENDER_ITEMUI])
		iter->Render_GameObject(m_matOrtho);

	for (auto& iter : m_RenderGroup[RENDER_LASTUI])
		iter->Render_GameObject(m_matOrtho);
	m_pDevice->SetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer_Mgr::Render_AfterEffect()
{
	for (auto& iter : m_RenderGroup[RENDER_AFTEREFFECT])
		iter->Render_GameObject();
}

void CRenderer_Mgr::Render_Deferred()
{
	Begin_MRT(L"MRT_Deferred");

	Render_Priority();
	Render_NonAlpha();
	Render_Alpha();
	//Render_Effect();

	End_MRT(L"MRT_Deferred");
}

void CRenderer_Mgr::Render_LightAcc()
{
	Begin_MRT(L"MRT_LightAcc");

	// ���̴� Ŭ��
	CShader*	pShader = static_cast<CShader*>(Clone_ComponentPrototype(L"Proto_Shader_Shade"));
	NULL_CHECK(pShader);

	// ����Ʈ �����
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();

	// �븻 ����Ÿ�� ��������
	Engine::Get_RenderTargetTexture(pEffect, L"NormalResult", "g_NormalTexture");
	Engine::Get_RenderTargetTexture(pEffect, L"Specular", "g_SpecularTexture");
	Engine::Get_RenderTargetTexture(pEffect, L"Depth", "g_DepthTexture");
	Engine::Get_RenderTargetTexture(pEffect, L"ShadowBlurY", "g_ShadowTexture");
	//Engine::Get_RenderTargetTexture(pEffect, L"Shadow", "g_ShadowTexture");

	pEffect->Begin(NULL, 0);

	Engine::Render_Light(pEffect);

	pEffect->End();

	End_MRT(L"MRT_LightAcc");

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

void CRenderer_Mgr::Render_LightBlur()
{
	Begin_MRT(L"MRT_LightBlur");

	CShader*	pShader = static_cast<CShader*>(Clone_ComponentPrototype(L"Proto_Shader_Blur"));
	NULL_CHECK(pShader);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();

	// Bloom Ÿ��(Shade���� ���� �� �κи� ������ Ÿ��) ��������
	Engine::Get_RenderTargetTexture(pEffect, L"Bloom", "g_BloomTexture");

	pEffect->Begin(NULL, 0);

	Render_Buffer(pEffect);

	// BlurX Ÿ��(Bloom�� X �������� Blur ó���� Ÿ��) �����ͼ� Blur Y ó��
	Engine::Get_RenderTargetTexture(pEffect, L"BlurX", "g_BlurXTexture");

	Render_Buffer(pEffect, 1);

	if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
		m_iCount--;

	if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
		m_iCount++;

	for (_int i = 0; i < m_iCount + 7; ++i)
	//for (_int i = 0; i < 8; ++i)
	{
		// BlurY Ÿ��(BlurX�� Y �������� Blur ó���� Ÿ��) ������ ������ Blur ó���Ͽ� �ڿ������� �����
		Engine::Get_RenderTargetTexture(pEffect, L"BlurY", "g_BlurYTexture");

		Render_Buffer(pEffect, 2);
	}

	pEffect->End();

	End_MRT(L"MRT_LightBlur");

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

_bool		Compare_Z(CGameObject* pDest, CGameObject* pSrc)
{
	return pDest->Get_ViewZ() > pSrc->Get_ViewZ();
}

void CRenderer_Mgr::Render_Effect()
{
	m_RenderGroup[RENDER_EFFECT].sort(Compare_Z);

	for (auto& iter : m_RenderGroup[RENDER_EFFECT])
		iter->Render_GameObject();
}

void CRenderer_Mgr::Render_ShadowMap()
{
	Begin_MRT(L"MRT_ShadowMap");

	Render_InstancesShadowMap(RENDER_SHADOWMAP);

	for (auto& iter : m_RenderGroup[RENDER_SHADOWMAP])
		iter->Render_GameObjectShadowMap();

	End_MRT(L"MRT_ShadowMap");
}

void CRenderer_Mgr::Render_ShadowBlur()
{
	Begin_MRT(L"MRT_ShadowBlur");

	CShader*	pShader = static_cast<CShader*>(Clone_ComponentPrototype(L"Proto_Shader_ShadowBlur"));
	NULL_CHECK(pShader);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();


	pEffect->Begin(NULL, 0);

	Engine::Get_RenderTargetTexture(pEffect, L"Shadow", "g_ShadowTexture");
	Render_Buffer(pEffect);

	Engine::Get_RenderTargetTexture(pEffect, L"ShadowBlurX", "g_BlurXTexture");
	Render_Buffer(pEffect, 1);

	for (_int i = 0; i < 4; ++i)
	{
		Engine::Get_RenderTargetTexture(pEffect, L"ShadowBlurY", "g_BlurYTexture");
		Render_Buffer(pEffect, 2);
	}

	pEffect->End();

	End_MRT(L"MRT_ShadowBlur");

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

//void CRenderer_Mgr::Render_Shadow()
//{
//	Begin_MRT(L"MRT_Shadow");
//
//	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
//		iter->Render_GameObjectShadow();
//
//	End_MRT(L"MRT_Shadow");
//}

void CRenderer_Mgr::Render_Blend(const _float& fFade)
{
	Begin_MRT(L"MRT_Blend");
	CShader*	pShader = static_cast<CShader*>(Clone_ComponentPrototype(L"Proto_Shader_Blend"));
	NULL_CHECK(pShader);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();

	Engine::Get_RenderTargetTexture(pEffect, L"Albedo", "g_AlbedoTexture");
	Engine::Get_RenderTargetTexture(pEffect, L"Shade", "g_ShadeTexture");
	Engine::Get_RenderTargetTexture(pEffect, L"Specular", "g_SpecularTexture");
	Engine::Get_RenderTargetTexture(pEffect, L"Depth", "g_DepthTexture");
	Engine::Get_RenderTargetTexture(pEffect, L"Shadow", "g_ShadowTexture");

	pEffect->SetFloat("g_fFade", fFade);

	pEffect->Begin(NULL, 0);

	Render_Buffer(pEffect, 1);

	pEffect->End();

	End_MRT(L"MRT_Blend");

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

void CRenderer_Mgr::Render_LightBloom()
{
	Begin_MRT(L"MRT_LightBloom");

	CShader*	pShader = static_cast<CShader*>(Clone_ComponentPrototype(L"Proto_Shader_Bloom"));
	NULL_CHECK(pShader);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();

	Engine::Get_RenderTargetTexture(pEffect, L"Blend", "g_BlendTexture");

	pEffect->Begin(NULL, 0);

	Render_Buffer(pEffect);

	pEffect->End();

	End_MRT(L"MRT_LightBloom");

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

void CRenderer_Mgr::Render_Result()
{
	CShader*	pShader = static_cast<CShader*>(Clone_ComponentPrototype(L"Proto_Shader_Result"));
	NULL_CHECK(pShader);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();

	Engine::Get_RenderTargetTexture(pEffect, L"BlurY", "g_BlurTexture");
	Engine::Get_RenderTargetTexture(pEffect, L"Blend", "g_BlendTexture");
	pEffect->SetFloat("g_fRange", m_fRange);

	pEffect->Begin(NULL, 0);

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		if (0.75f > m_fRange)
			m_fRange += 0.005f;
		else
			m_fRange = 0.75f;

		Render_Buffer(pEffect, 1);
	}

	else
	{
		if (0.01f < m_fRange)
		{
			m_fRange -= 0.02f;
			Render_Buffer(pEffect, 1);
		}

		else
		{
			m_fRange = 0.01f;
			Render_Buffer(pEffect, 0);
		}
	}

	pEffect->End();

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

//void CRenderer_Mgr::Render_HDR()
//{
//	CShader*	pShader = static_cast<CShader*>(Clone_ComponentPrototype(L"Proto_Shader_HDRSource"));
//	NULL_CHECK(pShader);
//
//	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
//	pEffect->AddRef();
//
//	pEffect->Begin(NULL, 0);
//
//	// Shade���� �� �ؽ��� ��������
//	Begin_MRT(L"MRT_HDRSource");
//	Engine::Get_RenderTargetTexture(pEffect, L"Shade", "g_HDRSourceTexture");
//	Render_Buffer(pEffect);
//	End_MRT(L"MRT_HDRSource");
//
//	// DownScale
//	Begin_MRT(L"MRT_HDRScaledSource");
//
//	typedef struct
//	{
//		float u0, v0;
//		float u1, v1;
//	}CoordRect;
//
//	CoordRect coords;
//	RECT rectSrc;
//	rectSrc.left = 0;
//	rectSrc.top = 0;
//	rectSrc.right = 800;
//	rectSrc.bottom = 600;
//
//	coords.u0 = 0.f;
//	coords.v0 = 0.f;
//	coords.u1 = 1.f;
//	coords.v1 = 1.f;
//
//	coords.u0 += rectSrc.left / 800;
//	coords.v0 += rectSrc.top / 600;
//	coords.u1 -= (800 - rectSrc.right) / 800;
//	coords.v1 -= (600 - rectSrc.bottom) / 600;
//
//	Engine::Get_RenderTargetTexture(pEffect, L"HDRSource", "g_HDRSourceTexture");
//	_int	index = 0;
//	_vec2	offsets[16];
//
//	for (_int y = 0; y < 4; y++) {
//		for (_int x = 0; x < 4; x++) {
//			offsets[index].x = (x - 1.5f) / 32;
//			offsets[index].y = (y - 1.5f) / 32;
//			index++;
//		}
//	}
//	pEffect->SetValue("g_avSampleOffsets", offsets, sizeof(offsets));
//	pEffect->CommitChanges();
//
//	pEffect->BeginPass(1);
//
//	_float w = 32.f;
//	_float h = 32.f;
//
//	typedef struct {
//		float p[4]; // ��ġ��ǥ
//		float t[2]; // �ؽ�ó��ǥ
//	} ScreenVertex;
//
//	ScreenVertex svQuad[4] = {
//		// x       y      z     w       u       v
//		{ 0 - 0.5f, 0 - 0.5f, 0.5f, 1.0f, coords.u0,  coords.v0, },
//		{ w - 0.5f, 0 - 0.5f, 0.5f, 1.0f, coords.u1, coords.v0, },
//		{ 0 - 0.5f, h - 0.5f, 0.5f, 1.0f, coords.u0,  coords.v1, },
//		{ w - 0.5f, h - 0.5f, 0.5f, 1.0f, coords.u1, coords.v1, },
//	};
//
//	m_pDevice->SetFVF(FVF_SCREEN);
//	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, svQuad, sizeof(ScreenVertex));
//
//	pEffect->EndPass();
//
//	End_MRT(L"MRT_HDRScaledSource");
//
//	// BrightFilter
//	Begin_MRT(L"MRT_HDRBrightFilter");
//	Engine::Get_RenderTargetTexture(pEffect, L"HDRScaledSource", "g_HDRSourceTexture");
//	Render_Buffer(pEffect, 2);
//	End_MRT(L"MRT_HDRBrightFilter");
//
//
//	pEffect->End();
//
//	Safe_Release(pEffect);
//	Safe_Release(pShader);
//}

void CRenderer_Mgr::Render_Buffer(LPD3DXEFFECT _pEffect, _int _iPass)
{
	_pEffect->CommitChanges();
	_pEffect->BeginPass(_iPass);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTX_SCREEN));

	m_pDevice->SetFVF(FVF_SCREEN);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	_pEffect->EndPass();
}

void CRenderer_Mgr::Free(void)
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
	Safe_Release(m_pDecl);
	Safe_Release(m_pDeclInst);
	Safe_Release(m_pDevice);

	Clear_RenderGroup();

	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& tPair : m_mapInstanceData[i])
		{
			tPair.second->vecMatrix.clear();
			Safe_Release(tPair.second->pDB);
			delete tPair.second;
			//vector<INSTANCE_DATA>().swap(tPair.second->vecMatrix);
		}
		m_mapInstanceData[i].clear();
		//map<const wstring, INSTANCE_OBJ*>().swap(m_mapInstanceData[i]);
	}
}