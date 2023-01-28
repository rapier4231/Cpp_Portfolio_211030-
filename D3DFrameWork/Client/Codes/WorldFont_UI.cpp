#include "stdafx.h"
#include "..\Headers\WorldFont_UI.h"

#include "Export_Function.h"

#define WorldFontRed {0.9f,0.1f,0.1f,0.9f}
#define WorldFontGreen {0.1f,0.9f,0.1f,0.9f}
#define WorldFontYellow {0.9f,0.9f,0.1f,0.9f}
#define WorldFontTranslucent_Red {0.9f,0.1f,0.1f,0.7f}
#define WorldFontTranslucent_Green {0.1f,0.9f,0.1f,0.7f}
#define WorldFontTranslucent_Yellow {0.9f,0.9f,0.1f,0.7f}
#define WorldFontBig_Blue {0.2f,0.5f,0.7f,0.9f}
#define WorldFontBig_Green {0.1f,0.8f,0.3f,0.9f}
USING(Engine)

HRESULT CWorldFont_UI::Set_Screen_UI()
{
	return S_OK;
}

CWorldFont_UI::CWorldFont_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CWorldFont_UI::CWorldFont_UI(const CWorldFont_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CWorldFont_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CWorldFont_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	if (_pArg)
	{
		m_tWorldFontInfo = *(static_cast<WFINFO*>(_pArg));
		m_bWFClone = true;
	}
	return S_OK;
}

_int CWorldFont_UI::Update_GameObject(const _double & _dTimeDelta)
{
	if(!m_bWFClone)
		return NO_EVENT;

	COrthoUI::Update_GameObject(_dTimeDelta);
	if (!m_tWorldFontInfo.m_bInfiniteShow) //무한 지속이 아니면
	{
		m_tWorldFontInfo.m_fTime -= (_float)_dTimeDelta;
		if (0 > m_tWorldFontInfo.m_fTime)
			return OBJECT_DEAD;
	}
	return NO_EVENT;
}

_int CWorldFont_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_bWFClone)
		return NO_EVENT;

	COrthoUI::LateUpdate_GameObject(_dTimeDelta);
	Fix_Font_Pos(_dTimeDelta);
	Engine::Add_RenderGroup(RENDER_WolrdPosUI, this);
	return NO_EVENT;
}

void CWorldFont_UI::Render_GameObject(_mat _matOrtho, _mat matOldProj)
{
	_vec2 ScreenPos = ChangePos_World_To_Screen(matOldProj);
	if(ScreenPos.x > 0.f && ScreenPos.y > 0.f)
		Render_Font(_matOrtho, ScreenPos);
}

HRESULT CWorldFont_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	return S_OK;
}

_vec2 CWorldFont_UI::ChangePos_World_To_Screen(_mat matOldProj)
{
	_mat matView, matVP;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);							 //뷰 행렬
	matVP = matView * matOldProj;											 //뷰 * 투영
	_vec3 vPos = m_tWorldFontInfo.m_vPrintWorldPos + m_tWorldFontInfo.m_vAddPos;
	D3DXVec3TransformCoord(&vPos, &vPos, &matVP);			 //로컬 좌표 * 월드 * 뷰 * 투영 -> 투영좌표 -1~1 사이
	if(-1.f >vPos.x || 1.f < vPos.x 
		|| -1.f > vPos.y || 1.f < vPos.y
		|| 0.f > vPos.z || 1.f < vPos.z)
		return _vec2{-1.f,-1.f};
	vPos.x += 1.f; 
	vPos.x *= 0.5f;
	vPos.y += 1.f;
	vPos.y *= 0.5f;
	vPos.y = 1.f - vPos.y;
	vPos.x *= WINCX;
	vPos.y *= WINCY;//투영 좌표를 스크린 좌표로 바꿔주는 식
	return _vec2{ vPos.x  ,vPos.y };
}

void CWorldFont_UI::Fix_Font_Pos(const _double& _dTimeDelta)
{
	switch (m_tWorldFontInfo.m_eFontType)
	{
	case World_FontType_Fix:
		break;
	case World_FontType_Up:
		m_tWorldFontInfo.m_vPrintWorldPos.y += ((_float)_dTimeDelta * 0.2f);
		break;
	case World_FontType_Follow:
		if(m_tWorldFontInfo.m_pvWorldPos)
			m_tWorldFontInfo.m_vPrintWorldPos = *m_tWorldFontInfo.m_pvWorldPos;
		break;
	default:
		break;
	}
}

void CWorldFont_UI::Render_Font(_mat _matOrtho, _vec2 _v2ScreenPos)
{
	if(World_Font_Translucent_Red > m_tWorldFontInfo.m_eFontColor)
		Engine::Render_Font(L"Font_WorldFont", m_tWorldFontInfo.m_wstrFont, &_vec2(_v2ScreenPos.x + m_tWorldFontInfo.m_v2AddScreenPos.x, _v2ScreenPos.y + m_tWorldFontInfo.m_v2AddScreenPos.y), D3DXCOLOR(m_tWorldFontInfo.m_v4FontColor), 2);
	else if ((World_Font_Translucent_Red <= m_tWorldFontInfo.m_eFontColor) &&(World_Font_Translucent_Yellow >= m_tWorldFontInfo.m_eFontColor)) 
		Engine::Render_Font(L"Font_WorldFont_Translucent", m_tWorldFontInfo.m_wstrFont, &_vec2(_v2ScreenPos.x + m_tWorldFontInfo.m_v2AddScreenPos.x, _v2ScreenPos.y + m_tWorldFontInfo.m_v2AddScreenPos.y), D3DXCOLOR(m_tWorldFontInfo.m_v4FontColor), 2);
	else if ((World_Font_Big_Blue <= m_tWorldFontInfo.m_eFontColor) && (World_Font_Color_END >= m_tWorldFontInfo.m_eFontColor))
		Engine::Render_Font(L"Font_WorldFont_Big", m_tWorldFontInfo.m_wstrFont, &_vec2(_v2ScreenPos.x + m_tWorldFontInfo.m_v2AddScreenPos.x, _v2ScreenPos.y + m_tWorldFontInfo.m_v2AddScreenPos.y), D3DXCOLOR(m_tWorldFontInfo.m_v4FontColor), 2);
}

CWorldFont_UI * CWorldFont_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CWorldFont_UI*	pInstance = new CWorldFont_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CWorldFont_UI::Clone(void * _pArg)
{
	CWorldFont_UI*	pClone = new CWorldFont_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CWorldFont_UI::Free()
{
	COrthoUI::Free();
}

_int CWorldFont_UI::Print_WorldFont_UI(const _vec3* _pWorldPos, const wstring & _wstrFont, EWorld_Font_Color _eColor, EWorld_FontType _eFontType, const _vec3& _vAddPos, _float _fTime, _vec2 _v2AddSrceenPos)
{
	++m_iCteateWorldFontNum;
	WFINFO tWFInfo;
	tWFInfo.m_eFontType = _eFontType;
	tWFInfo.m_pvWorldPos = _pWorldPos;
	tWFInfo.m_vPrintWorldPos = *_pWorldPos;
	tWFInfo.m_wstrFont = _wstrFont;
	tWFInfo.m_eFontColor = _eColor;
	tWFInfo.m_vAddPos = _vAddPos;
	tWFInfo.m_v2AddScreenPos = _v2AddSrceenPos;
	if (0.f < _fTime)
		tWFInfo.m_fTime = _fTime;
	else //무한 지속
	{
		tWFInfo.m_fTime = 10.f;
		tWFInfo.m_bInfiniteShow = true;
	}
	tWFInfo.m_iWF_ID = m_iCteateWorldFontNum;
	switch (_eColor)
	{
	case CWorldFont_UI::World_Font_Red:
		tWFInfo.m_v4FontColor = WorldFontRed;
		break;
	case CWorldFont_UI::World_Font_Green:
		tWFInfo.m_v4FontColor = WorldFontGreen;
		break;
	case CWorldFont_UI::World_Font_Yellow:
		tWFInfo.m_v4FontColor = WorldFontYellow;
		break;
	case CWorldFont_UI::World_Font_Translucent_Red:
		tWFInfo.m_v4FontColor = WorldFontTranslucent_Red;
		break;
	case CWorldFont_UI::World_Font_Translucent_Green:
		tWFInfo.m_v4FontColor = WorldFontTranslucent_Green;
		break;
	case CWorldFont_UI::World_Font_Translucent_Yellow:
		tWFInfo.m_v4FontColor = WorldFontTranslucent_Yellow;
		break;
	case CWorldFont_UI::World_Font_Big_Blue:
		tWFInfo.m_v4FontColor = WorldFontBig_Blue;
		break;
	case CWorldFont_UI::World_Font_Big_Green:
		tWFInfo.m_v4FontColor = WorldFontBig_Green;
		break;
	case CWorldFont_UI::World_Font_Color_END:
		break;
	default:
		break;
	}
	Engine::Get_Layer(L"WorldFont_UI")->Add_GameObject(Engine::Clone_GameObjectPrototype(L"Proto_WorldFont_UI",&tWFInfo));

	return m_iCteateWorldFontNum;
}

_bool CWorldFont_UI::Remove_WorldFont_UI(_int _iFontNum)
{
	for (auto& iter : *Engine::Get_Layer(L"WorldFont_UI")->Get_ListGameObject())
	{
		if (_iFontNum == static_cast<CWorldFont_UI*>(iter)->Get_Font_ID())
		{
			static_cast<CWorldFont_UI*>(iter)->Dead_Font();
			return true;
		}
	}
	return false;
}
