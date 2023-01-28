#pragma once
#ifndef __Text_Input_h__
#define __Text_Input_h__

#include "Value.h"

class CText_Input abstract
{
public:
	enum ETEXT_TYPE { ETEXT_TYPE_Consonants, ETEXT_TYPE_Vowels ,  ETEXT_TYPE_English_Consonants, ETEXT_TYPE_English_Vowels, ETEXT_TYPE_Number, ETEXT_TYPE_Space, ETEXT_TYPE_Etc, ETEXT_TYPE_END};
	enum EHANGLE_TYPE {EHANGLE_TYPE_ChoSung, EHANGLE_TYPE_JungSung, EHANGLE_TYPE_JongSung, EHANGLE_TYPE_END};
	typedef struct stInputText
	{
		ETEXT_TYPE	m_eTextType = ETEXT_TYPE_END;
		_int		m_iText = NULL;
	}IN_TXT;

protected:
	CText_Input();
	~CText_Input();

public:
	void				Key_Input_Update();
	wstring				Get_NowText();

protected:
	void				wstrOutPut_Combine();
	WCHAR				Make_Hangle(_int _iChoSung = 0, _int _iJungSung = 0, _int _iJongSung = 0);
	WCHAR				Make_Jaso(_int _iText, EHANGLE_TYPE _eHangleType);
	_int				Combine_Moum(_int _iMoum1, _int _iMoum2);
	_int				Making_JongSung(_int _iJaum1 = -1, _int _iJaum2 = -1);

	void				Text_Input_Reset();

protected:
	vector<stInputText>	m_vecInputTxt;
	wstring				m_wstrOutPut = L"";
	wstring				m_wstrNowCombine = L"";
	BOOL				m_bHangle = 1;
};

#endif __Text_Input_h__
