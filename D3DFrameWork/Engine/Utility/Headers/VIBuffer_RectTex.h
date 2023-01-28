#pragma once
#ifndef __VIBUFFER_RECTTEX_H__
#define __VIBUFFER_RECTTEX_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectTex : public CVIBuffer
{
private:
	explicit CVIBuffer_RectTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_RectTex(const CVIBuffer_RectTex& rhs);
	virtual ~CVIBuffer_RectTex() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_RectTex* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};
END
#endif // !__VIBUFFER_RECTTEX_H__
