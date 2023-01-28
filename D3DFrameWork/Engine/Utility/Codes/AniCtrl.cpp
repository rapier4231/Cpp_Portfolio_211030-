#include "AniCtrl.h"

USING(Engine)

CAniCtrl::CAniCtrl(LPD3DXANIMATIONCONTROLLER _pAniCtrl)
	: m_pAniCtrl(_pAniCtrl)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
	, m_fAccTime(0.f)
	, m_iOldAniIndex(999)
{
	m_pAniCtrl->AddRef();
}

CAniCtrl::CAniCtrl(const CAniCtrl & rhs)
	: m_iCurrentTrack(rhs.m_iCurrentTrack)
	, m_iNewTrack(rhs.m_iNewTrack)
	, m_fAccTime(rhs.m_fAccTime)
	, m_iOldAniIndex(rhs.m_iOldAniIndex)
{
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),	// 복제 시 원본 객체에서 제공되는 애니메이션 개수
		rhs.m_pAniCtrl->GetMaxNumAnimationSets(),		// 구동 가능한 애니메이션 개수(대분과 1과 2의 값이 같음)
		rhs.m_pAniCtrl->GetMaxNumTracks(),				// 구동 가능한 최대 트랙의 개수(정말 많이 써야 두 개를 사용함)
		rhs.m_pAniCtrl->GetMaxNumEvents(),				// 우리는 못씀
		&m_pAniCtrl);
}

HRESULT CAniCtrl::Ready_AnimationSet(void)
{
	return S_OK;
}

void CAniCtrl::Set_Animation(const _uint & iIndex, const _double& _dTime/* = 0.0*/, const _double& _dLerp/* = 0.1*/)
{
	if (m_iOldAniIndex == iIndex && _dTime == 0.0)
		return;

	///////////////////////////////////////////
	//m_dCheckTrackPosition = 0.0;
	///////////////////////////////////////////

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET		pAS = NULL;

	// 인덱스 값에 해당하는 애니메이션 셋을 얻어는 함수
	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);

	// m_pAniCtrl->GetAnimationSetByName()

	m_dPeriod = pAS->GetPeriod(); // 애니메이션 셋의 재생 시간을 반환하는 함수

								  // 0번 트랙에 애니메이션 셋 세팅
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// 애니메이션 정보 안에 삽입되어 있는 이벤트 정보를 해제하는 함수(이벤트가 없는 것으로 처리)
	// : 우리가 이벤트를 사용할 수 없는 상황인데, 정보가 삽입되어 있는 경우 가끔 애니메이션이 보간 되지 않는 문제가 발생
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// 현재 설정된 트랙을 재생 또는 종료 시키기 위한 함수(3인자 : 언제부터 현재 트랙을 해제할 것인가)
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + _dLerp);

	// 인자값으로 들어오는 트랙에 세팅된 애니메이션 셋을 어떤 속도로 움직일 것인지 설정하는 함수(속도의 상수 값은 1)
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, _dLerp, D3DXTRANSITION_LINEAR);

	// 인자값으로 들어오는 트랙의 가중치를 설정하는 함수
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, _dLerp, D3DXTRANSITION_LINEAR);


	// 0번 트랙의 활성화를 지시하는 함수
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, _dLerp, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, _dLerp, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime(); // AdvanceTime 호출 시 내부적으로 누적되던 시간을 초기화하는 함수
	m_fAccTime = 0.f;

	// 기존의 재생 중이던 트랙에서 새로운 트랙으로 변경을 했기 때문에 재생 시점을 0초(0의 위치로 초기화)로 시작하도록 지시하는 함수
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, _dTime);

	m_iOldAniIndex = iIndex;
	m_iCurrentTrack = m_iNewTrack;
}

void CAniCtrl::Play_Animation(const _double& dTimeDelta)
{
	m_pAniCtrl->AdvanceTime(dTimeDelta, NULL);	// 2인자 : 애니메이션 재생에 따라 사운드나, 이펙트를 구동 가능, 하지만 안씀.
												// AdvanceTime 호출 시 내부적으로 누적되는 시간 값이 있음

	m_fAccTime += (_float)dTimeDelta;
}

void CAniCtrl::Play_Animation_End(const _double & dTimeDelta)
{
	m_fAccTime += (_float)dTimeDelta;

	if (m_dPeriod <= (_double)m_fAccTime)
	{
		m_fAccTime = (_float)m_dPeriod;
		m_pAniCtrl->AdvanceTime(0.f, NULL);
	}
	else
		m_pAniCtrl->AdvanceTime(dTimeDelta, NULL);
}


_bool CAniCtrl::Is_AnimationsetEnd(_double _dRemoveTime/* = 0.1*/)
{
	D3DXTRACK_DESC			TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	///////////////////////////////////////////
	//if (m_dCheckTrackPosition > (TrackInfo.Position+ 0.01))
	//	return true;
	//else
	//	m_dCheckTrackPosition = TrackInfo.Position;
	//
	//if (m_dCheckTrackPosition >= m_dPeriod - _dRemoveTime)
	//	return true;
	///////////////////////////////////////////////

	if (TrackInfo.Position >= m_dPeriod - _dRemoveTime)
		return true;

	return false;
}

_double CAniCtrl::Get_CurrentTime()
{
	D3DXTRACK_DESC			TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));
	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);


	return TrackInfo.Position / m_dPeriod * 1;
}

void CAniCtrl::Reset_Animation()
{
	D3DXTRACK_DESC			TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));
	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	TrackInfo.Position = 0;

	m_pAniCtrl->SetTrackDesc(m_iCurrentTrack, &TrackInfo);
}

CAniCtrl * CAniCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAniCtrl*		pInstance = new CAniCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AnimationSet()))
		Safe_Release(pInstance);

	return pInstance;
}

CAniCtrl * CAniCtrl::Create(const CAniCtrl & rhs)
{
	CAniCtrl*		pInstance = new CAniCtrl(rhs);

	if (FAILED(pInstance->Ready_AnimationSet()))
		Safe_Release(pInstance);

	return pInstance;
}

void CAniCtrl::Free()
{
	Safe_Release(m_pAniCtrl);
}