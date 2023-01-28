#ifndef __VIBUFFER_PARTICLETEX_H__
#define __VIBUFFER_PARTICLETEX_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_ParticleTex : public CVIBuffer
{
public:
	typedef struct
	{
		_vec3		vPostition;
		_vec2		vTexUV;

		enum { FVF = (D3DFVF_XYZ | D3DFVF_TEX1) };
	}VTXPARTICLE;

	typedef struct
	{
		// 입자의 운동 요소
		_vec3			m_vecStartPos;		// 초기 위치
		_vec3			m_vecStartSpeed;	// 초기 속도
		_vec3			m_vecStartAccel;	// 초기 가속도

		_vec3			m_vecPos;			// 현재 위치
		_vec3			m_vecSpeed;			// 현재 속도
		_vec3			m_vecAccel;			// 현재 가속도

		_float			m_fElst;			// 탄성(Elastic) 계수
		_float			m_fDamp;			// 공기 저항(Air Registance) 계수

		// 입자의 생명 요소
		_bool			m_bLive;			// Active (Yes/No)
		_float			m_fLife;			// Particle fLife
		_float			m_fFade;			// Fade Speed

		// 입자의 표현 요소
		_float			m_PrsW;				// Present Billboard W
		_float			m_PrsH;				// Present Billboard H
		_float			m_PrsZ;				// 카메라의 z축과 위치의 내적 값

		_int 			m_iImageIndex;		// Image Index
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
	explicit CVIBuffer_ParticleTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer_ParticleTex(const CVIBuffer_ParticleTex& rhs);
	virtual ~CVIBuffer_ParticleTex(void);

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_ParticleTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent *			Clone(void * pArg = nullptr) override;
	virtual void					Free(void);

public:
	void	SetAni(BOOL bAni = TRUE);		// 애니메이션 활성화/ 비활성화
	void	SetAvgTime(_float _fTime);		// 평균 시간 설정
	void	SetPart(_int _iIndex);

	_int	Update_Particle(_float _fTimeDelta);

private:
	_float				m_fTimeAvg;			// Frame 평균 시간
	_int				m_iParticleCount;	// 입자의 수
	PARTICLE*			m_pParticleArr;

	_int				m_iVtx;
	VTXPARTICLE*		m_pVtx;
	BOOL				m_bAni;
	_float				m_fTime = 0.f;
};

END

#endif // __VIBUFFER_PARTICLETEX_H__