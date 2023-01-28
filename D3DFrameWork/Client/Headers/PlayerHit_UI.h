#pragma once
#ifndef __PlayerHit_UI_H__
#define __PlayerHit_UI_H__

#include "Value.h"
#include "OrthoUI.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END
class CPlayer;
class CPlayerHit_UI : public COrthoUI
{
private:
	typedef struct stPlayerHit_UI_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { WINCX*FHALF, WINCX*FHALF };
		_float fPosY[ESNRT_END] = { WINCY*FHALF ,WINCY*FHALF };
		_float fSizeX[ESNRT_END] = { WINCX,  WINCX };
		_float fSizeY[ESNRT_END] = { WINCY, WINCY };
		
	}STPlayerHit_UIURI;

	STPlayerHit_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

private:
	explicit CPlayerHit_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CPlayerHit_UI(const CPlayerHit_UI& rhs);
	virtual ~CPlayerHit_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;

public:
	static	CPlayerHit_UI* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _int _bTwo = 0);

public:
	void					Player_Hit_On(_vec3 _vPos,_vec3 _vAtkPos);
private:
	CTexture*				m_pHitTexCom = nullptr;
	CTexture*				m_pHitDirTexCom = nullptr;
	CShader*				m_pShaderCom = nullptr; 
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

private:
	_float					m_fHeartbeat_UI_Time = 0.f;
	_float					m_fTime = 0.f;
	_float					m_fPlayerHpRatio = 0.f;
	CPlayer*				m_pPlayer = nullptr;
	_bool					m_pHeartbeatOn = false;
	_float					m_fHeartBeatAlpha = 0.f;
	_bool					m_pHitOn = false;
	_float					m_fHitOnAlpha = 0.f;
	_bool					m_pHitOn2 = false;
	_float					m_fHitOn2Alpha = 0.f;
	_float					m_fAtkGackDo = 0.f;
	_float					m_fAtkGackDo2 = 0.f;
	_float					m_fHit_Timer = 0.f;
	_float					m_fHit_Timer2 = 0.f;
	_float					m_fShortHeartBeatTime = 0.f;
};

#endif // !__PlayerHit_UI_H__