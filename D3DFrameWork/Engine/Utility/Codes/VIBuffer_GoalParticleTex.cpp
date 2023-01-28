#include "VIBuffer_GoalParticleTex.h"

USING(Engine)

Engine::CVIBuffer_GoalParticleTex::CVIBuffer_GoalParticleTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
	m_iParticleCount = 0;
	m_pParticleArr = NULL;

	m_fTimeAvg = 0;
	m_bAni = FALSE;

	m_pVtx = NULL;

	m_bParticle = true;
}

Engine::CVIBuffer_GoalParticleTex::CVIBuffer_GoalParticleTex(const CVIBuffer_GoalParticleTex& rhs)
	: CVIBuffer(rhs)
{
	m_fTimeAvg = rhs.m_fTimeAvg;
	m_bAni = rhs.m_bAni;

	m_iParticleCount = 200;
	m_pParticleArr = new PARTICLE[m_iParticleCount];

	// ������ � ��¿� ����
	m_iVtx = m_iParticleCount * 2 * 3;
	m_pVtx = new VTXPARTICLE[m_iVtx];

	m_fImageWidth = 1.f;
	m_fImageHeight = 1.f;
}

Engine::CVIBuffer_GoalParticleTex::~CVIBuffer_GoalParticleTex(void)
{

}

HRESULT CVIBuffer_GoalParticleTex::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_GoalParticleTex::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_GoalParticleTex::Render_VIBuffer()
{
	if (!m_bAni)
		return S_OK;

	m_pDevice->SetFVF(VTXPARTICLE::FVF);

	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_iParticleCount * 2, m_pVtx, sizeof(VTXPARTICLE));

	return S_OK;
}

_int CVIBuffer_GoalParticleTex::Update_Particle(_float _fTimeDelta)
{
	if (!m_bAni)
		return 0;

	// 1. ��� �����Ѵ�.
	_float	dtime = m_fTimeAvg * _fTimeDelta;
	
	for (_int i = 0; i<m_iParticleCount; ++i)
	{
		PARTICLE* pPrt = &m_pParticleArr[i];

		// ���������� ���Ѵ�.
		D3DXVECTOR3	vcAirR = pPrt->m_vecSpeed;					// ���������� ���� ����
		_float		fLenV = D3DXVec3LengthSq(&vcAirR);		// �ӵ��� ����(Vx*Vx + Vy*Vy + Vz*Vz)ũ�� ����

		// ���������� ���� ���͸� ���Ѵ�.
		D3DXVec3Normalize(&vcAirR, &vcAirR);

		// �̵� �ӵ��� �ݴ�� ����
		vcAirR *= -1.F;

		// �ӷ����� * ���� ���� ����� ����.
		vcAirR *= fLenV * pPrt->m_fDamp;

		// 1. ���ӵ��� ���������� ���Ѵ�.
		pPrt->m_vecAccel = pPrt->m_vecStartAccel + vcAirR;

		// 2. ���� �ӵ� ����
		pPrt->m_vecSpeed += pPrt->m_vecAccel * dtime;

		// 3. ���� ��ġ ����
		pPrt->m_vecPos += pPrt->m_vecSpeed * dtime;

		// 4. ��谪 ����. ����� ���� ���·� ����.
		//if (pPrt->m_vecPos.y < 0.f)
		//{
		//	pPrt->m_bLive = FALSE;
		//}
	}

	// 2. ��ƼŬ�� ������ �����Ѵ�.
	for (_int i = 0; i < m_iParticleCount; ++i)
	{
		PARTICLE* pPrt = &m_pParticleArr[i];

		if (FALSE == pPrt->m_bLive)
			continue;


		pPrt->m_fLife -= pPrt->m_fFade * dtime;

		if (pPrt->m_fLife <= 0.f)
		{
			pPrt->m_bLive = FALSE;
			continue;
		}
	}

	// 3. �ؽ�ó �ִϸ��̼ǿ� ���� ��ƼŬ�� �����Ѵ�.
	//m_fTime = _fTimeDelta * 48.f;

	////if (1.f < m_fTime)
	//{
	//	//m_fTime = 0.f;

	//	for (_int i = 0; i < m_iParticleCount; ++i)
	//	{
	//		PARTICLE* pPrt = &m_pParticleArr[i];

	//		if (FALSE == pPrt->m_bLive)
	//			continue;

	//		pPrt->m_fImageIndex += m_fTime;
	//		if (31.f < pPrt->m_fImageIndex)
	//		{
	//			pPrt->m_bLive = FALSE;
	//			//pPrt->m_fImageIndex = 31.f;
	//		}

	//		//pPrt->m_fImageIndex %= 16;
	//	}
	//}

	//// 3. ���� ��ƼŬ�� ����Ѵ�.
	//if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000)
	//{
	//	for (_int i = 0; i < m_iParticleCount; ++i)
	//	{
	//		PARTICLE* pPrt = &m_pParticleArr[i];

	//		if (TRUE == pPrt->m_bLive)
	//			continue;

	//		this->SetPart(i);
	//	}
	//}

	// 4. ����� �����Ѵ�.

	// ī�޶��� ����
	D3DXMATRIX mtView;
	m_pDevice->GetTransform(D3DTS_VIEW, &mtView);

	D3DXVECTOR3 vcCamX(mtView._11, mtView._21, mtView._31);
	D3DXVECTOR3 vcCamY(mtView._12, mtView._22, mtView._32);
	D3DXVECTOR3 vcCamZ(mtView._13, mtView._23, mtView._33);

	for (_int i = 0; i < m_iParticleCount; ++i)
	{
		PARTICLE* pPrt = &m_pParticleArr[i];

		D3DXVECTOR3	vcP = pPrt->m_vecPos;

		// ī�޶��� Z��� ��ƼŬ�� ��ġ�� ����
		pPrt->m_PrsZ = D3DXVec3Dot(&vcP, &vcCamZ);
	}

	// Sorting
	qsort(m_pParticleArr
		, m_iParticleCount
		, sizeof(PARTICLE)
		, (int(*) (const void *, const void *)) SortFnc);


	for (_int i = 0; i < m_iParticleCount; ++i)
	{
		PARTICLE* pPrt = &m_pParticleArr[i];

		D3DXVECTOR3	vcP = pPrt->m_vecPos;

		_float		fW = pPrt->m_PrsW;
		_float		fH = pPrt->m_PrsH;
		_float		fD = min(fW, fH);

		VTXPARTICLE* pVtx = &m_pVtx[i * 6 + 0];

		int			_iIndexX = 0;
		int			_iIndexY = 0;

		D3DXVECTOR2	uv00(0.f, 0.f);
		D3DXVECTOR2	uv11(1.f, 1.f);

		(pVtx + 0)->vPostition.x = vcP.x - (vcCamX.x - vcCamY.x) * fW;
		(pVtx + 0)->vPostition.y = vcP.y - (vcCamX.y - vcCamY.y) * fH;
		(pVtx + 0)->vPostition.z = vcP.z - (vcCamX.z - vcCamY.z) * fD;
		(pVtx + 0)->vTexUV = { uv00.x, uv00.y };

		(pVtx + 1)->vPostition.x = vcP.x + (vcCamX.x + vcCamY.x) * fW;
		(pVtx + 1)->vPostition.y = vcP.y + (vcCamX.y + vcCamY.y) * fH;
		(pVtx + 1)->vPostition.z = vcP.z + (vcCamX.z + vcCamY.z) * fD;
		(pVtx + 1)->vTexUV = { uv11.x, uv00.y };

		(pVtx + 2)->vPostition.x = vcP.x - (vcCamX.x + vcCamY.x) * fW;
		(pVtx + 2)->vPostition.y = vcP.y - (vcCamX.y + vcCamY.y) * fH;
		(pVtx + 2)->vPostition.z = vcP.z - (vcCamX.z + vcCamY.z) * fD;
		(pVtx + 2)->vTexUV = { uv00.x, uv11.y };

		(pVtx + 3)->vPostition.x = vcP.x + (vcCamX.x - vcCamY.x) * fW;
		(pVtx + 3)->vPostition.y = vcP.y + (vcCamX.y - vcCamY.y) * fH;
		(pVtx + 3)->vPostition.z = vcP.z + (vcCamX.z - vcCamY.z) * fD;
		(pVtx + 3)->vTexUV = { uv11.x, uv11.y };

		m_pVtx[i * 6 + 4] = m_pVtx[i * 6 + 2];
		m_pVtx[i * 6 + 5] = m_pVtx[i * 6 + 1];
	}

	return 0;
}

void CVIBuffer_GoalParticleTex::SetAni(BOOL bAni)
{
	m_bAni = bAni;

	if (!m_bAni)
		return;

	for (int i = 0; i<m_iParticleCount; ++i)
	{
		SetPart(i);
	}
}

void CVIBuffer_GoalParticleTex::SetAvgTime(_float _fTime)
{
	m_fTimeAvg = _fTime;
}

void CVIBuffer_GoalParticleTex::SetPart(_int _iIndex)
{
	PARTICLE* pPrt = &m_pParticleArr[_iIndex];

	_float	fTheta;		// ���� ��
	_float	fPhi;		// ���� ��
	_float	fSpdR;		// �ӵ� ũ��

	// �ʱ� ���ӵ�
	pPrt->m_vecStartAccel = D3DXVECTOR3(0, -0.2F, 0);

	//�ʱ� �ӵ��� ��ġ�� �����ϱ� ���� ����
	fTheta = _float(rand() % 120);
	fTheta -= 60.f;

	fPhi = _float(rand() % 360);

	fSpdR = 5.f + (_float)(rand() % 30) - (_float)(rand() % 30);
	//fSpdR *= 0.5f;

	// �������� ����
	fTheta = D3DXToRadian(fTheta);
	fPhi = D3DXToRadian(fPhi);

	// �ʱ� �ӵ�
	pPrt->m_vecStartSpeed.x = fSpdR * sinf(fTheta) * sinf(fPhi);
	pPrt->m_vecStartSpeed.y = fSpdR * cosf(fTheta) * 0.5f;
	pPrt->m_vecStartSpeed.z = fSpdR * sinf(fTheta) * cosf(fPhi);

	// �ʱ� ��ġ		
	pPrt->m_vecStartPos.x = 0.f;
	pPrt->m_vecStartPos.y = 0.f;
	pPrt->m_vecStartPos.z = 0.f;

	// ź�� ��� ����
	pPrt->m_fElst = (50 + rand() % 51) * 0.01f;

	// �������� ���
	pPrt->m_fDamp = (100 + rand() % 101) * 0.000005F;

	// �ʱ� ��ġ, �ӵ�, ���ӵ��� ������ ������ �ʱ� ������ ����
	pPrt->m_vecPos = pPrt->m_vecStartPos;
	pPrt->m_vecSpeed = pPrt->m_vecStartSpeed;
	pPrt->m_vecAccel = pPrt->m_vecStartAccel;

	// ������ ���� ���
	pPrt->m_bLive = TRUE;
	pPrt->m_fLife = 30.f /*+ rand() % 71*/;
	//pPrt->m_fLife *= 0.01f;

	pPrt->m_fFade = (100 + rand() % 101) *0.0001f;

	// ������ ǥ�� ���
	//pPrt->m_PrsW = (_float)(rand() % 50);
	//pPrt->m_PrsH = (_float)(rand() % 50);

	pPrt->m_PrsW = 25.f;
	pPrt->m_PrsH = 25.f;

	//pPrt->m_fImageIndex = (_float)(rand() % (_int)(m_fImageWidth * m_fImageHeight * 0.5f));
	pPrt->m_fImageIndex = 0;
}

CVIBuffer_GoalParticleTex* Engine::CVIBuffer_GoalParticleTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVIBuffer_GoalParticleTex*	pInstance = new CVIBuffer_GoalParticleTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CVIBuffer_GoalParticleTex::Clone(void * pArg)
{
	CVIBuffer_GoalParticleTex* pClone = new CVIBuffer_GoalParticleTex(*this);
	if (FAILED(pClone->Ready_Component()))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void Engine::CVIBuffer_GoalParticleTex::Free(void)
{
	if (m_bIsClone)
	{
		Safe_Delete_Array(m_pParticleArr);
		Safe_Delete_Array(m_pVtx);
	}

	CVIBuffer::Free();
}