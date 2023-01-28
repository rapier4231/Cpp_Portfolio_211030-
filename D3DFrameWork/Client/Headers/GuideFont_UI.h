#pragma once
#ifndef __GuideFont_UI_H__
#define __GuideFont_UI_H__

#include "Value.h"
#include "OrthoUI.h"

BEGIN(Engine)
class CInven;
class CTexture;
class CShader;
class CVIBuffer_RectTex;
END

enum EGUIDEMESSAGE { EGUIDEMESSAGE_Green , Green_StatUp, Green_GoTaming, Green_TamingSuccess /*안내(그린)문구는 이쪽*/  , 
	EGUIDEMESSAGE_Red , Red_Cold, Red_LessHunger, Red_LessStamina, Red_OverWeight, Red_LessHealth, Red_Sick_AttRock /*경고(레드)문구는 이쪽*/ ,EGUIDEMESSAGE_END};

class CGuideFont_UI : public CGameObject
{
private:
	typedef struct stGuideFont_UI_Resolution_Info
	{
		_float fFont_LeftText_PosX[ESNRT_END] = { 0.07f*ScrnRatio, 0.14f*ScrnRatio };
		_float fFont_LeftText_Name_IntervalPosX[ESNRT_END] = { 0.13f*ScrnRatio, 0.4f*ScrnRatio };
		_float fFont_LeftText_Amount_IntervalPosX[ESNRT_END] = { 0.15f*ScrnRatio, 0.38f*ScrnRatio };
		_float fFont_LeftText_PosY[ESNRT_END] = { 1.45f*ScrnRatio, 4.4f*ScrnRatio };
		_float fFont_LeftText_IntervalPosY[ESNRT_END] = { 0.09f*ScrnRatio, 0.22f*ScrnRatio };
	
		_float fFont_TopText_PosY[ESNRT_END] = { 0.02f*ScrnRatio, 0.1f*ScrnRatio };
		_float fFont_TopText_GreenIntervalPosY[ESNRT_END] = { 0.11f*ScrnRatio, 0.34f*ScrnRatio };
		_float fFont_TopText_RedPosY[ESNRT_END] = { 0.09f*ScrnRatio, 0.3f*ScrnRatio };

		_float fStatus_IconSizeXY[ESNRT_END] = { 0.09f*ScrnRatio, 0.74f*ScrnRatio };
		_float fStatus_IconPosX[ESNRT_END] = { 0.09f*ScrnRatio, 14.9f*ScrnRatio };
		_float fStatus_IconPosY[ESNRT_END] = { 0.09f*ScrnRatio, 8.5f*ScrnRatio };
		_float fStatus_IconIntervalPosX[ESNRT_END] = { 0.09f*ScrnRatio, 1.f*ScrnRatio };
		_float fStatus_IconIntervalPosY[ESNRT_END] = { 0.09f*ScrnRatio, 1.f*ScrnRatio };

		_float fStatus_Icon_Speed[ESNRT_END] = { 0.09f, 20.f};
		_float fStatus_BackGround_Icon_Add_SizeXY[ESNRT_END] = { 0.09f*ScrnRatio , 0.2f*ScrnRatio };

	}STGuideFont_UIURI;

	STGuideFont_UIURI stURI;

	enum ETRANSTYPE { ADD_Green, REMOVE_Red};
private:
	explicit CGuideFont_UI(LPDIRECT3DDEVICE9 _pDevice);
	explicit CGuideFont_UI(const CGuideFont_UI& rhs);
	virtual ~CGuideFont_UI() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject(_mat _matOrtho) override;
	virtual void			Render_GameObject() { return; };
private:
	void					Render_LeftFont(_mat _matOrtho);
	void					Render_TopFont(_mat _matOrtho);
private:
	void					LeftGuideFunc(const _double & _dTimeDelta);
	void					TopGuideFunc(const _double & _dTimeDelta);
	void					LeftGuide_RenderTime_Controller(const _double & _dTimeDelta);
	void					TopGuide_RenderTime_Controller(const _double & _dTimeDelta);

public:
	static CGuideFont_UI*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

public:
	void					Print_LeftGuideFont(ITEMID _eItemID, _int _iAmount, _int _eTransCheck);//마지막 Add면 0, Remove면 1 줄 것!
	void					Print_TopGuideFont(EGUIDEMESSAGE _eMessage);//일반 안내(Green이)면 0, 경고(Red)면 1

private:
	void					Combine_LeftGuideFont();
	void					Combine_TopGuideFont();
	void					FixPos_LeftGuideFont();
	void					FixPos_TopGuideFont();

private:
	virtual HRESULT			Add_Component(void);
	void					Add_Message();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect, _mat _matOrtho, _vec2 _v2Pos, _int _iTexNum, _float _fAlpha, _vec2 _AddSize = {0.f,0.f});

	//CShader*				m_pShaderCom = nullptr;
private:
	list<LEFTGUIDEFONT_INFO> m_listLeftGuideFont;
	list<TOPGUIDEFONT_INFO>  m_listTopGuideFont;

private:
	vector<TCHAR*>			m_vecMessage;

private:
	CTexture*				m_pTextureCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CVIBuffer_RectTex*		m_pBufferCom = nullptr;

	typedef struct stStatust_Icon_Info
	{
		_vec2	v2GoalPos = { 0.f,0.f };
		_vec2	v2NowPos = { 0.f,0.f };
		_int	iTexNum = 0;
	}SII;
	list<SII>				m_listStatusIconInfo;
	void					Add_StatusIcon(EGUIDEMESSAGE _eMessage);
	void					Delete_StatusIcon(EGUIDEMESSAGE _eMessage);
	_int					Switching_Messege_To_Int(EGUIDEMESSAGE _eMessage);
	void					Sort_Status_IconPos(const _double & _dTimeDelta);
};

#endif // !__GuideFont_UI_H__