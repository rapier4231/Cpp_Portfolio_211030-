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
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),	// ���� �� ���� ��ü���� �����Ǵ� �ִϸ��̼� ����
		rhs.m_pAniCtrl->GetMaxNumAnimationSets(),		// ���� ������ �ִϸ��̼� ����(��а� 1�� 2�� ���� ����)
		rhs.m_pAniCtrl->GetMaxNumTracks(),				// ���� ������ �ִ� Ʈ���� ����(���� ���� ��� �� ���� �����)
		rhs.m_pAniCtrl->GetMaxNumEvents(),				// �츮�� ����
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

	// �ε��� ���� �ش��ϴ� �ִϸ��̼� ���� ���� �Լ�
	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);

	// m_pAniCtrl->GetAnimationSetByName()

	m_dPeriod = pAS->GetPeriod(); // �ִϸ��̼� ���� ��� �ð��� ��ȯ�ϴ� �Լ�

								  // 0�� Ʈ���� �ִϸ��̼� �� ����
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// �ִϸ��̼� ���� �ȿ� ���ԵǾ� �ִ� �̺�Ʈ ������ �����ϴ� �Լ�(�̺�Ʈ�� ���� ������ ó��)
	// : �츮�� �̺�Ʈ�� ����� �� ���� ��Ȳ�ε�, ������ ���ԵǾ� �ִ� ��� ���� �ִϸ��̼��� ���� ���� �ʴ� ������ �߻�
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// ���� ������ Ʈ���� ��� �Ǵ� ���� ��Ű�� ���� �Լ�(3���� : �������� ���� Ʈ���� ������ ���ΰ�)
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + _dLerp);

	// ���ڰ����� ������ Ʈ���� ���õ� �ִϸ��̼� ���� � �ӵ��� ������ ������ �����ϴ� �Լ�(�ӵ��� ��� ���� 1)
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, _dLerp, D3DXTRANSITION_LINEAR);

	// ���ڰ����� ������ Ʈ���� ����ġ�� �����ϴ� �Լ�
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, _dLerp, D3DXTRANSITION_LINEAR);


	// 0�� Ʈ���� Ȱ��ȭ�� �����ϴ� �Լ�
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, _dLerp, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, _dLerp, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime(); // AdvanceTime ȣ�� �� ���������� �����Ǵ� �ð��� �ʱ�ȭ�ϴ� �Լ�
	m_fAccTime = 0.f;

	// ������ ��� ���̴� Ʈ������ ���ο� Ʈ������ ������ �߱� ������ ��� ������ 0��(0�� ��ġ�� �ʱ�ȭ)�� �����ϵ��� �����ϴ� �Լ�
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, _dTime);

	m_iOldAniIndex = iIndex;
	m_iCurrentTrack = m_iNewTrack;
}

void CAniCtrl::Play_Animation(const _double& dTimeDelta)
{
	m_pAniCtrl->AdvanceTime(dTimeDelta, NULL);	// 2���� : �ִϸ��̼� ����� ���� ���峪, ����Ʈ�� ���� ����, ������ �Ⱦ�.
												// AdvanceTime ȣ�� �� ���������� �����Ǵ� �ð� ���� ����

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