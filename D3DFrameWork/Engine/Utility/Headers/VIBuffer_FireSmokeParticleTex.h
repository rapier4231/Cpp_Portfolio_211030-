#ifndef __VIBUFFER_FIRESMOKEPARTICLETEX_H__
#define __VIBUFFER_FIRESMOKEPARTICLETEX_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_FireSmokeParticleTex : public CVIBuffer
{
public:
	typedef struct
	{
		_vec3		vPostition;
		_vec2		vTexUV;
		//DWORD		dColor = 0xFFFFFFFF;

		enum { FVF = (D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_DIFFUSE*/) };
	}VTXPARTICLE;

	typedef struct
	{
		_vec3			m_vecStartPos;		// �ʱ� ��ġ
		_vec3			m_vecStartSpeed;	// �ʱ� �ӵ�

		_vec3			m_vecPos;			// ���� ��ġ
		_vec3			m_vecSpeed;			// ���� �ӵ�

		_bool			m_bLive;			// Active (Yes/No)
		_float			m_fLife;			// Particle fLife
		_float			m_fFade;			// Fade Speed
		DWORD			m_dColor;			// Color

		_float			m_PrsW;				// Present Billboard W
		_float			m_PrsZ;				// ī�޶��� z��� ��ġ�� ���� ��

		INT				m_PrsImg;	// Image Index
	}PARTICLE;

	static int SortFnc(const PARTICLE* p1, const PARTICLE* p2)
	{
		FLOAT	v1, v2;

		v1 = p1->m_PrsZ;
		v2 = p2->m_PrsZ;

		if (v1 < v2)
			return 1;

		else if (v1 == v2)
			return 0;

		else
			return -1;
	}

private:
	explicit CVIBuffer_FireSmokeParticleTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer_FireSmokeParticleTex(const CVIBuffer_FireSmokeParticleTex& rhs);
	virtual ~CVIBuffer_FireSmokeParticleTex(void);

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_FireSmokeParticleTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent *			Clone(void * pArg = nullptr) override;
	virtual void					Free(void);

public:
	void	SetAni(BOOL bAni = TRUE);		// �ִϸ��̼� Ȱ��ȭ/ ��Ȱ��ȭ
	void	SetAvgTime(_float _fTime);		// ��� �ð� ����
	void	SetPart(_int _iIndex);

	_int	Update_Particle(_float _fTimeDelta);

private:
	_float				m_fTimeAvg;			// Frame ��� �ð�
	_int				m_iParticleCount;	// ������ ��
	PARTICLE*			m_pParticleArr;

	_int				m_iVtx;
	VTXPARTICLE*		m_pVtx;
	BOOL				m_bAni;
	_float				m_fTime = 0.f;
};

END

#endif // __VIBUFFER_FIRESMOKEPARTICLETEX_H__