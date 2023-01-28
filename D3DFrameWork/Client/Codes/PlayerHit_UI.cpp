#include "stdafx.h"
#include "..\Headers\PlayerHit_UI.h"

#include "Export_Function.h"
#include "Player.h"

#define	HBeat_1Step_Hp_Ratio 0.3f
#define	HBeat_2Step_Hp_Ratio 0.2f
#define	HBeat_3Step_Hp_Ratio 0.1f
#define	HBeat_1Step_OnOff_Time 1.2f
#define	HBeat_2Step_OnOff_Time 0.9f
#define	HBeat_3Step_OnOff_Time 0.6f
#define	HBeat_Time 0.2f
#define HBeat_Alpha 0.6f
#define HBeat_NomalAlpha 0.2f
#define Hit_Timer	1.5f
USING(Engine)

HRESULT CPlayerHit_UI::Set_Screen_UI()
{
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	return S_OK;
}

CPlayerHit_UI::CPlayerHit_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CPlayerHit_UI::CPlayerHit_UI(const CPlayerHit_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CPlayerHit_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CPlayerHit_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);
	
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];

	return S_OK;
}

_int CPlayerHit_UI::Update_GameObject(const _double & _dTimeDelta)
{
	if (!m_pPlayer)
		m_pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0));

	m_fPlayerHpRatio = m_pPlayer->Get_Stat(CUnit::STAT_HP) / m_pPlayer->Get_Stat(CUnit::STAT_MAXHP);
	if (HBeat_3Step_Hp_Ratio <= m_fPlayerHpRatio)
	{
		m_fTime = 0.f;
		m_fHeartBeatAlpha = HBeat_NomalAlpha;
		m_fShortHeartBeatTime = 0.f;
		return NO_EVENT;
	}
	else
		m_pHeartbeatOn = true;

	COrthoUI::Update_GameObject(_dTimeDelta);

	if (HBeat_3Step_Hp_Ratio > m_fPlayerHpRatio)
	{
		if (HBeat_3Step_Hp_Ratio > m_fPlayerHpRatio)
			m_fHeartbeat_UI_Time = HBeat_3Step_OnOff_Time;
		else if (HBeat_2Step_Hp_Ratio > m_fPlayerHpRatio)
			m_fHeartbeat_UI_Time = HBeat_2Step_OnOff_Time;
		else if (HBeat_1Step_Hp_Ratio > m_fPlayerHpRatio)
			m_fHeartbeat_UI_Time = HBeat_1Step_OnOff_Time;
		else
			m_fHeartbeat_UI_Time = FNULL;

		m_fTime += (_float)_dTimeDelta;
		if (m_fTime > m_fHeartbeat_UI_Time)
		{
			m_fShortHeartBeatTime += (_float)_dTimeDelta;
			m_fHeartBeatAlpha = HBeat_Alpha;
		}

		if (HBeat_Time < m_fShortHeartBeatTime)
		{
			m_fTime = 0.f;
			m_fHeartBeatAlpha = HBeat_NomalAlpha;
		}
	}
	//////////////////////////////////////////////////////////////
	return NO_EVENT;
}

_int CPlayerHit_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_pHeartbeatOn && !m_pHitOn && !m_pHitOn2)
		return NO_EVENT;

	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	Engine::Add_RenderGroup(RENDER_ORTHOBACKUI, this);

	if(m_pHitOn)
	{
		m_fHit_Timer -= (_float)_dTimeDelta;

		if(Hit_Timer*2.f / 5.f > m_fHit_Timer)
			m_fHitOnAlpha -= (_float)_dTimeDelta;
		else if (Hit_Timer*4.f / 5.f < m_fHit_Timer)
			m_fHitOnAlpha += (_float)_dTimeDelta*3.f;

		if (0.f > m_fHit_Timer)
		{
			m_pHitOn = false;
			m_fHitOnAlpha = 0.f;
		}
	}
	if(m_pHitOn2)
	{
		m_fHit_Timer2 -= (_float)_dTimeDelta;

		if (Hit_Timer*2.f / 5.f > m_fHit_Timer2)
			m_fHitOn2Alpha -= (_float)_dTimeDelta;
		else if (Hit_Timer*4.f / 5.f < m_fHit_Timer2)
			m_fHitOn2Alpha += (_float)_dTimeDelta*3.f;

		if (0.f > m_fHit_Timer2)
		{
			m_pHitOn2 = false;
			m_fHitOn2Alpha = 0.f;
		}
	}
	return NO_EVENT;
}

void CPlayerHit_UI::Render_GameObject(_mat _matOrtho)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 6;

	pEffect->Begin(&iPassMax, NULL);

	pEffect->CommitChanges();

	////////////////////////////////////////
	if (m_pHeartbeatOn)
	{
		pEffect->BeginPass(1);
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
	}
	//////////////////////////////////////
	if (m_pHitOn)
	{
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, 1);
		pEffect->BeginPass(2);
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
	}
	////////////////////////////////////
	//////////////////////////////////////
	if (m_pHitOn2)
	{
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, 2);
		pEffect->BeginPass(2);
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass(); 
	}
	////////////////////////////////////

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CPlayerHit_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pHitTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Player_Hit_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture", pComponent);

	pComponent = m_pHitDirTexCom = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Player_Hit_Dir_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture2", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Loading"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CPlayerHit_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	if (1 == _bTwo)
	{
		matWorld._11 = WINCX;
		matWorld._22 = WINCY;
		matWorld._33 = FTRUE;

		matWorld._11 = cos(m_fAtkGackDo);
		matWorld._12 = -sin(m_fAtkGackDo);
		matWorld._21 = sin(m_fAtkGackDo);
		matWorld._22 = cos(m_fAtkGackDo);

		matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pHitDirTexCom->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_fAlpha", m_fHitOnAlpha);
	}
	else if (2 == _bTwo)
	{
		matWorld._11 = WINCX;
		matWorld._22 = WINCY;
		matWorld._33 = FTRUE;

		matWorld._11 = cos(m_fAtkGackDo2);
		matWorld._12 = -sin(m_fAtkGackDo2);
		matWorld._21 = sin(m_fAtkGackDo2);
		matWorld._22 = cos(m_fAtkGackDo2);

		matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pHitDirTexCom->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_fAlpha", m_fHitOn2Alpha);
	}
	else
	{
		matWorld._11 = WINCX;
		matWorld._22 = WINCY;
		matWorld._33 = FTRUE;

		matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;

		m_pHitTexCom->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_fAlpha", m_fHeartBeatAlpha);
	}

	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);
	return S_OK;
}

void CPlayerHit_UI::Player_Hit_On(_vec3 _vPos,_vec3 _vAtkPos)
{
	//이거는 이제 방향 띄워주기
	if (!m_pHitOn)
	{
		m_pHitOn = true;
		m_fHit_Timer = Hit_Timer;
		_vec2 v2PlayerPos, v2AtkPos = {};
		v2PlayerPos.x = _vPos.x;
		v2PlayerPos.y = _vPos.y;
		v2AtkPos.x = _vAtkPos.x;
		v2AtkPos.y = _vAtkPos.y;

		m_fAtkGackDo = asin((v2PlayerPos.x*v2AtkPos.y - v2PlayerPos.y*v2AtkPos.x) /
			sqrt(v2PlayerPos.x*v2PlayerPos.x + v2PlayerPos.y*v2PlayerPos.y)*sqrt(v2AtkPos.x*v2AtkPos.x + v2AtkPos.y*v2AtkPos.y));

		if (0 > m_fAtkGackDo)
			m_fAtkGackDo += D3DXToRadian(360.f);
	}
	else if(!m_pHitOn2)
	{
		m_pHitOn2 = true;
		m_fHit_Timer2 = Hit_Timer;

		_vec2 v2PlayerPos, v2AtkPos = {};
		v2PlayerPos.x = _vPos.x;
		v2PlayerPos.y = _vPos.y;
		v2AtkPos.x = _vAtkPos.x;
		v2AtkPos.y = _vAtkPos.y;

		m_fAtkGackDo2 = asin((v2PlayerPos.x*v2AtkPos.y - v2PlayerPos.y*v2AtkPos.x) /
			sqrt(v2PlayerPos.x*v2PlayerPos.x + v2PlayerPos.y*v2PlayerPos.y)*sqrt(v2AtkPos.x*v2AtkPos.x + v2AtkPos.y*v2AtkPos.y));

		if (0 > m_fAtkGackDo2)
			m_fAtkGackDo2 += D3DXToRadian(360.f);
	}
}

CPlayerHit_UI * CPlayerHit_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CPlayerHit_UI*	pInstance = new CPlayerHit_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CPlayerHit_UI::Clone(void * _pArg)
{
	CPlayerHit_UI*	pClone = new CPlayerHit_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CPlayerHit_UI::Free()
{
	COrthoUI::Free();
}