#pragma once
#ifndef __InfoView_UI_H__
#define __InfoView_UI_H__

#include "Value.h"
#include "OrthoUI.h"
#define DinoInfoViewFixWorldPos {0.f,-0.1f,0.f}

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END
class CUnit;

class CInfoView_UI : public COrthoUI
{
private:
	typedef struct stInfoView_UI_Monster_Resolution_Info : STOURI
	{
		_float fSizeX[ESNRT_END] = { 1.2f*ScrnRatio, 3.f*ScrnRatio };
		_float fSizeY[ESNRT_END] = { 0.75f*ScrnRatio, 1.7f*ScrnRatio };
		_vec4  v4EdgeSize[ESNRT_END] = { {  },
										 {  } };

		_float fStatIcon_SizeXY[ESNRT_END] = { 0.14f*ScrnRatio, 0.4f*ScrnRatio };
		_float fStatIcon_IntervalPosX[ESNRT_END] = { 0.03f*ScrnRatio, 0.1f*ScrnRatio };
		_float fStatIcon_FixPosX[ESNRT_END] = { 0.025f*ScrnRatio, 0.28f*ScrnRatio };
		_float fStatIcon_FixPosY[ESNRT_END] = { 0.01f*ScrnRatio, 0.08f*ScrnRatio };

		_float fTamingBar_SizeY[ESNRT_END] = { 0.1f*ScrnRatio, 0.27f*ScrnRatio };
		_float fTamingBar_FixPosY[ESNRT_END] = { 0.07f*ScrnRatio, 0.19f*ScrnRatio };

		_float fDinoFace_SizeXY[ESNRT_END] = { 0.3f*ScrnRatio, 1.f*ScrnRatio };
		_float fDinoFace_FixPosX[ESNRT_END] = { 0.35f*ScrnRatio, 1.f*ScrnRatio };
		_float fDinoFace_FixPosY[ESNRT_END] = { 0.15f*ScrnRatio, 0.36f*ScrnRatio };

		_float fHorizontalLine_SizeX[ESNRT_END] = { 1.f*ScrnRatio, 4.f*ScrnRatio };
		_float fHorizontalLine_SizeY[ESNRT_END] = { 0.03f*ScrnRatio, 0.08f*ScrnRatio };
		_float fHorizontalLine_FixPosX[ESNRT_END] = { 0.03f*ScrnRatio, 0.12f*ScrnRatio };
		_float fHorizontalLine_FixPosY[ESNRT_END] = { 0.11f*ScrnRatio, 0.154f*ScrnRatio };

		_float fFont_Monster_Name_IntervalPosY[ESNRT_END] = { 0.325f*ScrnRatio, 0.76f*ScrnRatio };
		_float fFont_Player_Name_IntervalPosY[ESNRT_END] = { 0.23f*ScrnRatio, 0.5f*ScrnRatio };
		_float fFont_Player_Gender_Name_IntervalPosY[ESNRT_END] = { 0.16f*ScrnRatio, 0.3f*ScrnRatio };
		_float fFont_Taming_Progress_IntervalPosY[ESNRT_END] = { 0.425f*ScrnRatio, 0.96f*ScrnRatio };

		_float fFixPosY[ESNRT_END] = { 0.75f*ScrnRatio , 4.f*ScrnRatio };
		//_vec3  vecLine[ESNRT_END][3] = { {{ },{},{} }, {{   },{ 0.f,8.f * ScrnRatio,0.f },{ 0.f,8.f * ScrnRatio,0.f } } };
		_vec2 vLine1_Left[ESNRT_END] = { {},{ 5.06f * ScrnRatio,7.715f * ScrnRatio } };
		//_vec3 vLine1_Right[ESNRT_END] = { {} ,{WINCX - 5.f * ScrnRatio,7.8f * ScrnRatio,0.f } };
		_vec2 vLine2_Left[ESNRT_END] = { {},{3.51f * ScrnRatio,7.715f * ScrnRatio } };
		//_vec3 vLine2_Right[ESNRT_END] = { {} , { WINCX - 3.4f * ScrnRatio,7.8f * ScrnRatio,0.f} };
	}STInfoView_UIURI;

	STInfoView_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

	enum EStatType {EStatType_HP = 10, EStatType_Stamina, EStatType_Food, EStatType_Weight, EStatType_END};
private:
	explicit CInfoView_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CInfoView_UI(const CInfoView_UI& rhs);
	virtual ~CInfoView_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) { return; };
	virtual void			Render_GameObject(_mat _matOrtho, _mat matOldProj) override;


public:
	static	CInfoView_UI*   Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _mat matOldProj, _vec2 _v2ScreenPos, _int _bTwo = 0);

private:
	_vec2					ChangePos_World_To_Screen(_mat matOldProj);
	CTexture*				Get_DinoEmptyTex();

private:
	CTexture*				m_pInfoView_BgCom = nullptr;
	CTexture*				m_pHpTexCom = nullptr;
	CTexture*				m_pStaminaTexCom = nullptr;
	CTexture*				m_pFoodTexCom = nullptr;
	CTexture*				m_pWeightTexCom = nullptr;
	CTexture*				m_pHpFillTexCom = nullptr;
	CTexture*				m_pStaminaFillTexCom = nullptr;
	CTexture*				m_pFoodFillTexCom = nullptr;
	CTexture*				m_pWeightFillTexCom = nullptr;
	CTexture*				m_pLineTexCom = nullptr;
	CTexture*				m_pInventoryBgTexCom = nullptr;
	CTexture*				m_pInventoryBgFillterTexCom = nullptr;
	_float					m_fGlowLineFallTime = 0.f;

	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;
	CUnit*					m_pPlayer = nullptr;

	CTexture*				m_pEmptyDinoTexCom = nullptr;
private:
	CUnit*					m_pMonster = nullptr;
	_float					m_fMaxMonsterStat = FNULL;
	_float					m_fNowMonsterStat = FNULL;
	_float					m_fPerMonsterStat = FNULL;
	_float					m_fPerMonsterTamingRate = FNULL;

private:
	void					Render_Font(_mat _matOrtho, _vec2 _v2ScreenPos);

private:
	_int					m_iDinoNameFontID = NULL;

private:
	_vec2					m_vLine[3] = {};//공룡 포스 0, 내가 원하는 Y지점까지 끌어내리는 포스1, 옆으로 수평하게 빼주는 포스 2
	_float					m_fLineLenth1 = 0.f;
	_float					m_fLineLenth2 = 0.f;
	_int					m_iLineReady = 0;

	_float					m_fOpenDelay = 0.f;
	//_bool					m_bLeftView = false;
	//_bool					m_bLeftView_Check = false;
	void					Reset_Line();
};

#endif // !__InfoView_UI_H__