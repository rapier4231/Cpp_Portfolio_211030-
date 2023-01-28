#pragma once
#ifndef __ANICTRL_H__
#define __ANICTRL_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit	CAniCtrl(LPD3DXANIMATIONCONTROLLER _pAniCtrl);
	explicit	CAniCtrl(const CAniCtrl& rhs);
	virtual		~CAniCtrl(void) = default;

public:
	HRESULT						Ready_AnimationSet(void);

public:
	void						Set_Animation(const _uint& iIndex, const _double& _dTime = 0.0, const _double& _dLerp = 0.1);
	void						Play_Animation(const _double& dTimeDelta);
	void						Play_Animation_End(const _double & fTimeDelta);

public:
	LPD3DXANIMATIONCONTROLLER	Get_AnimationCtrl(void) { return m_pAniCtrl; }
	_bool						Is_AnimationsetEnd(_double _dRemoveTime = 0.1);
	_double						Get_AccTime() { return (_double)m_fAccTime; };
	_double						Get_CurrentTime();
	void						Reset_Animation();

public:
	static CAniCtrl*			Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAniCtrl*			Create(const CAniCtrl& rhs);
	virtual void				Free() override;

private:
	LPD3DXANIMATIONCONTROLLER	m_pAniCtrl;

	_uint						m_iCurrentTrack;
	_uint						m_iNewTrack;

	_float						m_fAccTime;
	_uint						m_iOldAniIndex;

	_double						m_dPeriod = 0.0;

	/////////////////////////////
	//_double						m_dCheckTrackPosition = 0.0;
	////////////////////////
};
END
#endif // !__ANICTRL_H__
