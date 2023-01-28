#pragma once
#ifndef __VIBUFFER_H__
#define __VIBUFFER_H__

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 _pDevice);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT				Ready_Component_Prototype() PURE;
	virtual HRESULT				Ready_Component(void * pArg = nullptr) PURE;
	virtual HRESULT				Render_VIBuffer() PURE;

public:
	virtual CComponent *		Clone(void * pArg = nullptr) PURE;
	virtual void				Free() PURE;

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;

	_uint						m_iVertexSize PURE;
	_uint						m_iVertexCount PURE;
	_uint						m_iFVF PURE;

	_uint						m_iIndexSize PURE;
	_uint						m_iTriCount PURE;
	D3DFORMAT					m_IndexFmt = D3DFMT_INDEX16;

	void*						m_pVertices = nullptr;
	void*						m_pConvertVertices = nullptr;
	void*						m_pIndices = nullptr;

	_bool						m_bParticle = false;
};
END

#endif // !__VIBUFFER_H__
