#ifndef __RENDERTARGET_H__
#define __RENDERTARGET_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget : public CBase
{
private:
	explicit CRenderTarget(LPDIRECT3DDEVICE9 _pDeivce);
	virtual ~CRenderTarget() = default;

public:
	HRESULT			Ready_RenderTarget(const _uint& _iWidth, const _uint& _iHeight, D3DFORMAT _Format, D3DXCOLOR _Color, _bool _bShadow);
	void			SetUp_OnGraphicDev(const _uint& _iIndex);	// 내가 원하는 서피스를 장치에 연결하는 함수
	void			Release_OnGraphicDev(const _uint& _iIndex);	// 그리기가 종료되어 임시적으로 저장해놨던 기존 서피스를 다시 연결해 놓는 함수
	void			Clear_RenderTarget(_bool _bMotionBlur);

	HRESULT			Ready_DebugBuffer(const wstring & _wstrTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void			Render_DebugBuffer();

	void			Set_RendetTargetTexture(LPD3DXEFFECT& pEffect, const char* pConstantName);

public:
	static CRenderTarget* Create(	LPDIRECT3DDEVICE9 _pDeivce,
									const _uint& _iWidth,
									const _uint& _iHeight,
									D3DFORMAT _Format,
									D3DXCOLOR _Color,
									_bool _bShadow);
	virtual void Free() override;

private:


private:
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;

	LPDIRECT3DDEVICE9				m_pDevice = nullptr;

	LPDIRECT3DTEXTURE9				m_pTargetTexture = nullptr;
	LPDIRECT3DSURFACE9				m_pTargetSurface = nullptr;
	LPDIRECT3DSURFACE9				m_pTargetZDepth = nullptr;

	LPDIRECT3DSURFACE9				m_pOldTargetSurface = nullptr;
	LPDIRECT3DSURFACE9				m_pOldTargetZDepth = nullptr;

	D3DXCOLOR						m_ClearColor;

	_float							m_fX, m_fY;
	wstring							m_wstrTargetTag;

	_bool							m_bShadow;
};
END
#endif // !__RENDERTARGET_H__
