#pragma once
#ifndef __VIBUFFER_CUBETEX_H__
#define __VIBUFFER_CUBETEX_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_CubeTex : public CVIBuffer
{
private:
	explicit CVIBuffer_CubeTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_CubeTex(const CVIBuffer_CubeTex& rhs);
	virtual ~CVIBuffer_CubeTex() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_CubeTex* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};
END
#endif // !__VIBUFFER_CUBETEX_H__
