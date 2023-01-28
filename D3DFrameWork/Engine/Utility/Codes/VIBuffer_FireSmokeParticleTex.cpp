#include "VIBuffer_FireSmokeParticleTex.h"

USING(Engine)

Engine::CVIBuffer_FireSmokeParticleTex::CVIBuffer_FireSmokeParticleTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
	m_iParticleCount = 0;
	m_pParticleArr = NULL;

	m_fTimeAvg = 0;
	m_bAni = FALSE;

	m_pVtx = NULL;

	m_bParticle = true;
}

Engine::CVIBuffer_FireSmokeParticleTex::CVIBuffer_FireSmokeParticleTex(const CVIBuffer_FireSmokeParticleTex& rhs)
	: CVIBuffer(rhs)
{
	m_fTimeAvg = rhs.m_fTimeAvg;
	m_bAni = rhs.m_bAni;

	m_iParticleCount = 30;
	m_pParticleArr = new PARTICLE[m_iParticleCount];

	// 입자의 운동 출력용 설정
	m_iVtx = m_iParticleCount * 2 * 3;
	m_pVtx = new VTXPARTICLE[m_iVtx];
}

Engine::CVIBuffer_FireSmokeParticleTex::~CVIBuffer_FireSmokeParticleTex(void)
{

}

HRESULT CVIBuffer_FireSmokeParticleTex::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_FireSmokeParticleTex::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_FireSmokeParticleTex::Render_VIBuffer()
{
	if (!m_bAni)
		return S_OK;

	m_pDevice->SetFVF(VTXPARTICLE::FVF);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_iParticleCount * 2, m_pVtx, sizeof(VTXPARTICLE));

	return S_OK;
}

_int CVIBuffer_FireSmokeParticleTex::Update_Particle(_float _fTimeDelta)
{
	if (!m_bAni)
		return 0;

	// 1. 운동을 갱신한다.
	_float	dtime = m_fTimeAvg * _fTimeDelta;
	
	for (_int i = 0; i<m_iParticleCount; ++i)
	{
		PARTICLE* pPrt = &m_pParticleArr[i];

		// 3. 현재 위치 갱신
		pPrt->m_vecPos += pPrt->m_vecSpeed * dtime;

		// 4. 경계값 설정. 벗어나면 죽은 상태로 설정.
		if (pPrt->m_vecPos.y > 4.f)
		{
			pPrt->m_bLive = FALSE;
		}

		if (FALSE == pPrt->m_bLive)
			continue;

		++pPrt->m_PrsImg;
		pPrt->m_PrsImg %= 16;

		//D3DXCOLOR	xc = pPrt->m_dColor;

		//pPrt->m_fLife -= pPrt->m_fFade * _fTimeDelta;

		//if (pPrt->m_fLife <= 0.f)
		//{
		//	pPrt->m_bLive = FALSE;
		//	continue;
		//}

		//xc.a = pPrt->m_fLife;
		//pPrt->m_dColor = xc;
	}

	// 3. 죽은 파티클을 재생한다.
	for (_int i = 0; i < m_iParticleCount; ++i)
	{
		PARTICLE* pPrt = &m_pParticleArr[i];

		if (TRUE == pPrt->m_bLive)
			continue;

		this->SetPart(i);
	}
	
	// 4. 출력을 설정한다.

	// 카메라의 정보
	D3DXMATRIX mtView;
	m_pDevice->GetTransform(D3DTS_VIEW, &mtView);

	D3DXVECTOR3 vcCamX(mtView._11, mtView._21, mtView._31);
	D3DXVECTOR3 vcCamY(mtView._12, mtView._22, mtView._32);
	D3DXVECTOR3 vcCamZ(mtView._13, mtView._23, mtView._33);

	for (_int i = 0; i < m_iParticleCount; ++i)
	{
		PARTICLE* pPrt = &m_pParticleArr[i];

		D3DXVECTOR3	vcP = pPrt->m_vecPos;

		// 카메라의 Z축과 파티클의 위치와 내적
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
		//D3DXCOLOR	xcC = pPrt->m_dColor;

		VTXPARTICLE* pVtx = &m_pVtx[i * 6 + 0];

		int			nIdxX = pPrt->m_PrsImg % 4;
		int			nIdxY = pPrt->m_PrsImg / 4;
		D3DXVECTOR2	uv00((nIdxX + 0) / 4.f, (nIdxY + 0) / 4.f);
		D3DXVECTOR2	uv11((nIdxX + 1) / 4.f, (nIdxY + 1) / 4.f);

		(pVtx + 0)->vPostition.x = vcP.x - (vcCamX.x - vcCamY.x) * fW;
		(pVtx + 0)->vPostition.y = vcP.y - (vcCamX.y - vcCamY.y) * fW;
		(pVtx + 0)->vPostition.z = vcP.z - (vcCamX.z - vcCamY.z) * fW;
		(pVtx + 0)->vTexUV.x = uv00.x;
		(pVtx + 0)->vTexUV.y = uv00.y;
		//(pVtx + 0)->dColor = xcC;

		(pVtx + 1)->vPostition.x = vcP.x + (vcCamX.x + vcCamY.x) * fW;
		(pVtx + 1)->vPostition.y = vcP.y + (vcCamX.y + vcCamY.y) * fW;
		(pVtx + 1)->vPostition.z = vcP.z + (vcCamX.z + vcCamY.z) * fW;
		(pVtx + 1)->vTexUV.x = uv11.x;
		(pVtx + 1)->vTexUV.y = uv00.y;
		//(pVtx + 1)->dColor = xcC;

		(pVtx + 2)->vPostition.x = vcP.x - (vcCamX.x + vcCamY.x) * fW;
		(pVtx + 2)->vPostition.y = vcP.y - (vcCamX.y + vcCamY.y) * fW;
		(pVtx + 2)->vPostition.z = vcP.z - (vcCamX.z + vcCamY.z) * fW;
		(pVtx + 2)->vTexUV.x = uv00.x;
		(pVtx + 2)->vTexUV.y = uv11.y;
		//(pVtx + 2)->dColor = xcC;

		(pVtx + 3)->vPostition.x = vcP.x + (vcCamX.x - vcCamY.x) * fW;
		(pVtx + 3)->vPostition.y = vcP.y + (vcCamX.y - vcCamY.y) * fW;
		(pVtx + 3)->vPostition.z = vcP.z + (vcCamX.z - vcCamY.z) * fW;
		(pVtx + 3)->vTexUV.x = uv11.x;
		(pVtx + 3)->vTexUV.y = uv11.y;
		//(pVtx + 3)->dColor = xcC;

		m_pVtx[i * 6 + 4] = m_pVtx[i * 6 + 2];
		m_pVtx[i * 6 + 5] = m_pVtx[i * 6 + 1];
	}

	return 0;
}

void CVIBuffer_FireSmokeParticleTex::SetAni(BOOL bAni)
{
	m_bAni = bAni;

	if (!m_bAni)
		return;

	for (int i = 0; i<m_iParticleCount; ++i)
	{
		SetPart(i);
	}
}

void CVIBuffer_FireSmokeParticleTex::SetAvgTime(_float _fTime)
{
	m_fTimeAvg = _fTime;
}

void CVIBuffer_FireSmokeParticleTex::SetPart(_int _iIndex)
{
	PARTICLE* pPrt = &m_pParticleArr[_iIndex];

	_float	fTheta;		// 각도 θ
	_float	fPhi;		// 각도 φ
	_float	fSpdR;		// 속도 크기

	//초기 속도와 위치를 설정하기 위한 변수
	fTheta = _float(rand() % 31);
	fTheta -= 30.f;

	fPhi = _float(rand() % 360);

	fSpdR = 10.f + rand() % 41;
	fSpdR *= 0.03f;

	// 라디안으로 변경
	fTheta = D3DXToRadian(fTheta);
	fPhi = D3DXToRadian(fPhi);

	// 초기 속도
	pPrt->m_vecStartSpeed.x = fSpdR * sinf(fTheta) * sinf(fPhi);
	pPrt->m_vecStartSpeed.y = fSpdR * cosf(fTheta);
	pPrt->m_vecStartSpeed.z = fSpdR * sinf(fTheta) * cosf(fPhi);

	// 초기 위치		
	pPrt->m_vecStartPos.x = 0.f;
	pPrt->m_vecStartPos.y = 0.f;
	pPrt->m_vecStartPos.z = 0.f;
	pPrt->m_vecStartPos += 1.5f * D3DXVECTOR3(cosf(fPhi), 0, sinf(fPhi));

	// 초기 위치, 속도, 가속도를 현재의 값들의 초기 값으로 설정
	pPrt->m_vecPos = pPrt->m_vecStartPos;
	pPrt->m_vecSpeed = pPrt->m_vecStartSpeed;

	// 입자의 생명 요소
	pPrt->m_bLive = TRUE;
	pPrt->m_fLife = 30.f + rand() % 71;
	pPrt->m_fLife *= 0.01f;

	pPrt->m_fFade = (100 + rand() % 101) * 0.0001f;
	//pPrt->m_dColor = D3DXCOLOR(1.f, 1.f, 1.f, pPrt->m_fLife);

	// 입자의 표현 요소
	pPrt->m_PrsW = 50.f + rand() % 51;
	pPrt->m_PrsW *= 0.02f;

	pPrt->m_PrsImg = rand() % 16;
}

CVIBuffer_FireSmokeParticleTex* Engine::CVIBuffer_FireSmokeParticleTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVIBuffer_FireSmokeParticleTex*	pInstance = new CVIBuffer_FireSmokeParticleTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Component_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CVIBuffer_FireSmokeParticleTex::Clone(void * pArg)
{
	CVIBuffer_FireSmokeParticleTex* pClone = new CVIBuffer_FireSmokeParticleTex(*this);
	if (FAILED(pClone->Ready_Component()))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void Engine::CVIBuffer_FireSmokeParticleTex::Free(void)
{
	if (m_bIsClone)
	{
		Safe_Delete_Array(m_pParticleArr);
		Safe_Delete_Array(m_pVtx);
	}

	CVIBuffer::Free();
}