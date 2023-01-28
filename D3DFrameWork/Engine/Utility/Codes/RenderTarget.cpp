#include "RenderTarget.h"
#include "Font_Mgr.h"
USING(Engine)

CRenderTarget::CRenderTarget(LPDIRECT3DDEVICE9 _pDeivce)
	: m_pDevice(_pDeivce)
	, m_pTargetTexture(nullptr)
	, m_pTargetSurface(nullptr)
	, m_pOldTargetSurface(nullptr)
	, m_pTargetZDepth(nullptr)
	, m_pOldTargetZDepth(nullptr)
	, m_bShadow(false)
{
	m_pDevice->AddRef();
}

HRESULT CRenderTarget::Ready_RenderTarget(const _uint & _iWidth, const _uint & _iHeight, D3DFORMAT _Format, D3DXCOLOR _Color, _bool _bShadow)
{
	if (FAILED(D3DXCreateTexture(	m_pDevice, 
									_iWidth, 
									_iHeight, 
									1, 
									D3DUSAGE_RENDERTARGET, 
									_Format, 
									D3DPOOL_DEFAULT, 
									&m_pTargetTexture)))
									return E_FAIL;

	m_bShadow = _bShadow;

	if (m_bShadow)
	{
		if (FAILED(m_pDevice->CreateDepthStencilSurface(
			_iWidth,
			_iHeight,
			D3DFMT_D24X8,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&m_pTargetZDepth,
			NULL)))
			return E_FAIL;
	}

	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);

	m_ClearColor = _Color;


	return S_OK;
}

void CRenderTarget::SetUp_OnGraphicDev(const _uint & _iIndex)
{
	if (m_bShadow)
	{
		m_pDevice->GetRenderTarget(_iIndex, &m_pOldTargetSurface);
		m_pDevice->GetDepthStencilSurface(&m_pOldTargetZDepth);

		m_pDevice->SetRenderTarget(_iIndex, m_pTargetSurface);
		m_pDevice->SetDepthStencilSurface(m_pTargetZDepth);
	}

	else
	{
		m_pDevice->GetRenderTarget(_iIndex, &m_pOldTargetSurface);

		m_pDevice->SetRenderTarget(_iIndex, m_pTargetSurface);
	}
}

void CRenderTarget::Release_OnGraphicDev(const _uint & _iIndex)
{
	if (m_bShadow)
	{
		m_pDevice->SetRenderTarget(_iIndex, m_pOldTargetSurface);
		m_pDevice->SetDepthStencilSurface(m_pOldTargetZDepth);

		Safe_Release(m_pOldTargetSurface);
		Safe_Release(m_pOldTargetZDepth);
	}

	else
	{
		m_pDevice->SetRenderTarget(_iIndex, m_pOldTargetSurface);

		Safe_Release(m_pOldTargetSurface);
	}
}

void CRenderTarget::Clear_RenderTarget(_bool _bMotionBlur)
{
	if (m_bShadow)
	{
		m_pDevice->GetRenderTarget(0, &m_pOldTargetSurface);
		m_pDevice->GetDepthStencilSurface(&m_pOldTargetZDepth);

		m_pDevice->SetRenderTarget(0, m_pTargetSurface);
		m_pDevice->SetDepthStencilSurface(m_pTargetZDepth);

		m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_ClearColor, 1.f, 0);

		m_pDevice->SetRenderTarget(0, m_pOldTargetSurface);
		m_pDevice->SetDepthStencilSurface(m_pOldTargetZDepth);

		Safe_Release(m_pOldTargetSurface);
		Safe_Release(m_pOldTargetZDepth);
	}

	else
	{
		m_pDevice->GetRenderTarget(0, &m_pOldTargetSurface);

		m_pDevice->SetRenderTarget(0, m_pTargetSurface);

		// 모션 블러는 색상값만 초기화
		if(_bMotionBlur)
			m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0);
		else
			m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_ClearColor, 1.f, 0);


		m_pDevice->SetRenderTarget(0, m_pOldTargetSurface);

		Safe_Release(m_pOldTargetSurface);
	}
}

HRESULT CRenderTarget::Ready_DebugBuffer(const wstring & _wstrTargetTag, const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	// 버텍스 퍼버 생성 함수
	FAILED_CHECK_RETURN(m_pDevice->CreateVertexBuffer(sizeof(VTX_SCREEN) * 4,
														0,
														FVF_SCREEN,
														D3DPOOL_MANAGED,
														&m_pVB, NULL), E_FAIL);
	// 인덱스 버퍼 생성 함수
	FAILED_CHECK_RETURN(m_pDevice->CreateIndexBuffer(sizeof(INDEX16) * 2,
														0,
														D3DFMT_INDEX16,
														D3DPOOL_MANAGED,
														&m_pIB, NULL), E_FAIL);

	VTX_SCREEN* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(fX - 0.5f, fY - 0.5f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec4((fX + fSizeX) - 0.5f, fY - 0.5f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4((fX + fSizeX) - 0.5f, (fY + fSizeY) - 0.5f, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec4(fX - 0.5f, (fY + fSizeY) - 0.5f, 0.f, 1.f);
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

	// 디버그 버퍼에 폰트 남기기 위해 필요한 놈들
	m_fX = fX;
	m_fY = fY;
	m_wstrTargetTag = _wstrTargetTag;

	return S_OK;
}

void CRenderTarget::Render_DebugBuffer()
{
	m_pDevice->SetTexture(0, m_pTargetTexture);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTX_SCREEN));
	m_pDevice->SetFVF(FVF_SCREEN);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	CFont_Mgr::GetInstance()->Render_Font(L"Font_Debug_Bold", m_wstrTargetTag, &_vec2(m_fX + 10.f, m_fY + 130.f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
}

void CRenderTarget::Set_RendetTargetTexture(LPD3DXEFFECT & pEffect, const char * pConstantName)
{
	pEffect->SetTexture(pConstantName, m_pTargetTexture);
}

CRenderTarget * CRenderTarget::Create(LPDIRECT3DDEVICE9 _pDeivce, const _uint & _iWidth, const _uint & _iHeight, D3DFORMAT _Format, D3DXCOLOR _Color, _bool _bShadow)
{
	CRenderTarget* pInstance = new CRenderTarget(_pDeivce);

	if (FAILED(pInstance->Ready_RenderTarget(_iWidth, _iHeight, _Format, _Color, _bShadow)))
		Safe_Release(pInstance);

	return pInstance;
}

void CRenderTarget::Free()
{
	CFont_Mgr::GetInstance()->DestroyInstance();

	Safe_Release(m_pOldTargetZDepth);
	Safe_Release(m_pOldTargetSurface);
	Safe_Release(m_pTargetSurface);
	Safe_Release(m_pTargetZDepth);
	Safe_Release(m_pTargetTexture);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	Safe_Release(m_pDevice);
}