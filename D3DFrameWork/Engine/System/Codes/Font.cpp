#include "Font.h"
#include "GraphicDevice.h"
USING(Engine)

CFont::CFont(LPDIRECT3DDEVICE9 _pDevice)
	: m_pDevice(_pDevice)
	, m_pFont(nullptr)
	, m_pSprite(nullptr)
{
	m_pDevice->AddRef();
}

HRESULT CFont::Ready_Font(const wstring & _wstrFontType, const _uint & _iWidth, const _uint & _iHeight, const _uint & _iWeight)
{
	D3DXFONT_DESC	Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.CharSet = HANGUL_CHARSET;
	lstrcpy(Font_Desc.FaceName, _wstrFontType.c_str());
	Font_Desc.Width = _iWidth;
	Font_Desc.Height = _iHeight;
	Font_Desc.Weight = _iWeight;

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &Font_Desc, &m_pFont)))
	{
		MSG_BOX("Font Create Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		MSG_BOX("Sprite Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CFont::Render_Font(const wstring & _wstrString, const _vec2 * _pPos, D3DXCOLOR _Color, _int _iFontType /*= 0*/)
{
	RECT	rc{ _long(_pPos->x), _long(_pPos->y) };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	if (1 == _iFontType)
	{
		rc = { _long(_pPos->x), _long(_pPos->y) ,_long(_pPos->x)- (_long)(10.f) };
		m_pFont->DrawTextW(m_pSprite, _wstrString.c_str(), lstrlen(_wstrString.c_str()), &rc, DT_RIGHT | DT_NOCLIP, _Color);
	}
	else if (2 == _iFontType)
	{
		rc = { _long(_pPos->x - 1.f), _long(_pPos->y - 1.f),_long(_pPos->x + 1.f),_long(_pPos->y + 1.f) };
		m_pFont->DrawTextW(m_pSprite, _wstrString.c_str(), lstrlen(_wstrString.c_str()), &rc, DT_CENTER | DT_NOCLIP, _Color);
	}
	else if (3 == _iFontType) //아이템 툴팁 
	{
		if(CGraphicDevice::ESCREEN_RESOULTION_4To3 == CGraphicDevice::GetInstance()->Get_Screen_Resolution())
			rc = { _long(_pPos->x - (0.33f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/*66.f*/), _long(_pPos->y - (0.26f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/*52.f*/),_long(_pPos->x + (0.33f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/*66.f*/),_long(_pPos->y + (0.26f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/*52.f*/) };
		else if (CGraphicDevice::ESCREEN_RESOULTION_16To9 == CGraphicDevice::GetInstance()->Get_Screen_Resolution())
			rc = { _long(_pPos->x - (0.878f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/*75.f*/), _long(_pPos->y - (0.7f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/* 60.f*/),_long(_pPos->x + (0.878f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/*75.f*/),_long(_pPos->y + (0.7f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/* 60.f*/) };
		m_pFont->DrawTextW(m_pSprite, _wstrString.c_str(), lstrlen(_wstrString.c_str()), &rc, DT_LEFT|DT_WORDBREAK/*| DT_EDITCONTROL*/, _Color);
	}
	else if (4 == _iFontType) //이름 짓기 가이드 폰트
	{
		if (CGraphicDevice::ESCREEN_RESOULTION_4To3 == CGraphicDevice::GetInstance()->Get_Screen_Resolution())
			rc = { _long(_pPos->x - (0.33f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/*66.f*/), _long(_pPos->y - (0.26f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/*52.f*/),_long(_pPos->x + (0.33f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/*66.f*/),_long(_pPos->y + (0.26f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/*52.f*/) };
		else if (CGraphicDevice::ESCREEN_RESOULTION_16To9 == CGraphicDevice::GetInstance()->Get_Screen_Resolution())
			rc = { _long(_pPos->x - (2.4f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/*75.f*/), _long(_pPos->y - (0.7f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/* 60.f*/),_long(_pPos->x + (2.4f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/*75.f*/),_long(_pPos->y + (0.7f*CGraphicDevice::GetInstance()->Get_Screen_Ratio())/* 60.f*/) };
		m_pFont->DrawTextW(m_pSprite, _wstrString.c_str(), lstrlen(_wstrString.c_str()), &rc, DT_CENTER | DT_WORDBREAK/*| DT_EDITCONTROL*/, _Color);
	}
	else
		m_pFont->DrawTextW(m_pSprite, _wstrString.c_str(), lstrlen(_wstrString.c_str()), &rc, DT_NOCLIP, _Color);

 	m_pSprite->End();
}

CFont * CFont::Create(LPDIRECT3DDEVICE9 _pDevice, const wstring & _pFontType, const _uint & _iWidth, const _uint & _iHeight, const _uint & _iWeight)
{
	CFont*	pInstance = new CFont(_pDevice);

	if (FAILED(pInstance->Ready_Font(_pFontType, _iWidth, _iHeight, _iWeight)))
	{
		MSG_BOX("Font Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CFont::Render_Font(const list<LEFTGUIDEFONT_INFO>& _vecGuideFont)
{
	RECT	rc = {};
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	for (auto& iter : _vecGuideFont)
	{
		rc = { _long(iter.v2TransPos.x), _long(iter.v2TransPos.y) };
		m_pFont->DrawTextW(m_pSprite, iter.tchTransText, lstrlen(iter.tchTransText), &rc, DT_NOCLIP, iter.tTransColor);
		rc = { _long(iter.v2AmountPos.x), _long(iter.v2AmountPos.y) };
		m_pFont->DrawTextW(m_pSprite, iter.tchAmountText, lstrlen(iter.tchAmountText), &rc, DT_NOCLIP, iter.tAmountColor);
		rc = { _long(iter.v2NamePos.x), _long(iter.v2NamePos.y) };
		m_pFont->DrawTextW(m_pSprite, iter.tchNameText, lstrlen(iter.tchNameText), &rc, DT_NOCLIP, iter.tNameColor);
	}

	m_pSprite->End();
}

void CFont::Render_Font(const list<TOPGUIDEFONT_INFO>& _vecGuideFont, _int _iRedboundary, _bool _bGreenGuide)
{
	RECT	rc = {};
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	if (_bGreenGuide)
	{
		for (auto& iter : _vecGuideFont)
		{
			if (_iRedboundary > iter.eMessageEnum)
			{
				rc = { _long(iter.v2Pos.x - 1.f), _long(iter.v2Pos.y - 1.f),_long(iter.v2Pos.x + 1.f),_long(iter.v2Pos.y + 1.f) };
				m_pFont->DrawTextW(m_pSprite, iter.tchText, lstrlen(iter.tchText), &rc, DT_CENTER | DT_NOCLIP, iter.tColor);
			}
		}
	}
	else
	{
		for (auto& iter : _vecGuideFont)
		{
			if (_iRedboundary < iter.eMessageEnum)
			{
				rc = { _long(iter.v2Pos.x - 1.f), _long(iter.v2Pos.y - 1.f),_long(iter.v2Pos.x + 1.f),_long(iter.v2Pos.y + 1.f) };
				m_pFont->DrawTextW(m_pSprite, iter.tchText, lstrlen(iter.tchText), &rc, DT_CENTER | DT_NOCLIP, iter.tColor);
			}
		}
	}

	m_pSprite->End();
}


void CFont::Free()
{
	Engine::Safe_Release(m_pFont);
	Engine::Safe_Release(m_pSprite);
	Engine::Safe_Release(m_pDevice);
}
