#pragma once
#ifndef __MiniMap_UI_H__
#define __MiniMap_UI_H__

#include "Value.h"
#include "OrthoUI.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END
class CUnit;

class CMiniMap_UI : public COrthoUI
{
private:
	typedef struct stMiniMap_UI_Resolution_Info : STOURI
	{
		_float fPosX[ESNRT_END] = { 0.35f*ScrnRatio,  1.f*ScrnRatio };
		_float fPosY[ESNRT_END] = { 2.65f*ScrnRatio ,8.f*ScrnRatio };
		_float fSizeXY[ESNRT_END] = { 0.6f* ScrnRatio,  1.8f* ScrnRatio };

		_float fCompassSizeXY[ESNRT_END] = { 0.14f* ScrnRatio,  0.3f* ScrnRatio };

	}STMiniMap_UIURI;

	STMiniMap_UIURI stURI;
	virtual HRESULT			Set_Screen_UI()override;

public:
	typedef struct MiniMap_BedPos
	{
		_vec3 vRealPos = {};
		_vec2 v2MapPos = {};
	}MAP_BED_POS;

private:
	explicit CMiniMap_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CMiniMap_UI(const CMiniMap_UI& rhs);
	virtual ~CMiniMap_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;


public:
	static	CMiniMap_UI* Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CUI*			Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	virtual HRESULT			Add_Component(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _int _bTwo = 0, _vec2 _BedMapPos = _vec2{0.f,0.f}, _int _iBedKey = 0);

public:
	void					MiniMap_Move(_bool _bCenter);
	_int					Bed_Create(_vec3 _vBedWorldPos);
	_bool					Bed_Delete(_int _iBedKeyValue);
	size_t					Get_BedCount();	//등록되어있는 침대 개수 반환
private:
	void					MiniMap_Center_Move(const _double& _dTimeDelta);
	void					Bed_Select(_int _iBedKey = 0);
	void					Select_Bed_Erase();
private:
	void					Calculate_Compass_World();
	void					Calculate_Player_Pos();
	_vec2					Calculate_Bed_Pos(_vec3 _vRealBedPos);
	_bool					Now_Mouse_On_BedIcon(_vec2 _vRealBedPos, _vec2 _vRealSize);

private:
	CTexture*				m_pMiniMapTex_Com = nullptr;
	CTexture*				m_pPingTex_Com = nullptr;
	CTexture*				m_pCompassTex_Com = nullptr;

	//침대
	CTexture*				m_pBedUnSelecTex_Com = nullptr;
	CTexture*				m_pBedSelecTex_Com = nullptr;

	CShader*				m_pShaderCom = nullptr; 
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

private:
	_mat					m_matCompassWorld;
	_mat					m_matMiniMapWorld;
	_vec2					m_v2PlayerPos;

	//디버그용
#ifdef _DEBUG
	_float					m_fa;
#endif

private:
	map<_int, MAP_BED_POS>		m_mapBedPos;
	_int						m_iBedNum=0;
	_int						m_iEraseBedNum = -1;
	_int						m_iCenter = 2; //0 -> 작은 상태 // 1-> 큰 상태 // 2-> 가만히
	_bool						m_bCenter = false;
};

#endif // !__MiniMap_UI_H__