#include "stdafx.h"
#include "Text_Input.h"


CText_Input::CText_Input()
{
}


CText_Input::~CText_Input()
{
}

void CText_Input::Key_Input_Update()
{
	//HIMC  HIMC_HanYuong;
	//// hWnd는 윈도우핸들
	//HIMC_HanYuong = ImmGetContext(g_hWnd);
	////한영상태를 얻는다
	////Flag가 true이면 한글, false이면 영문임
	//m_bHangle = ImmGetOpenStatus(HIMC_HanYuong);
	////IMM핸들을 해제 한다.
	//ImmReleaseContext(g_hWnd, HIMC_HanYuong); //Imm.lib 안쓸겨!
	m_bHangle = Engine::Now_HanYoung();

	_bool bCapsLock;
	if (((GetKeyState(VK_CAPITAL) & 0x0001) != 0))
		bCapsLock = true;
	else
		bCapsLock = false;

	if (GetKeyState(VK_SHIFT) & 0x8000)
		bCapsLock = !bCapsLock;

	if (Engine::KeyDown(KeyCheck_A))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			stTemp.m_iText = 6; //ㄱ이 0 //https://d2.naver.com/helloworld/76650?source=post_page---------------------------
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Vowels;
			if (bCapsLock)
				stTemp.m_iText = 0; //A가 0, a가 26
			else
				stTemp.m_iText = 32; //A가 0, a가 32
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_B))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Vowels;
			stTemp.m_iText = 17 /*9*/; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 1; //A가 0, a가 26
			else
				stTemp.m_iText = 33; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_C))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			stTemp.m_iText = 14; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 2; //A가 0, a가 26
			else
				stTemp.m_iText = 34; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_D))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			stTemp.m_iText = 11; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 3; //A가 0, a가 26
			else
				stTemp.m_iText = 35; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_E))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 4;
			else
				stTemp.m_iText = 3; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Vowels;
			if (bCapsLock)
				stTemp.m_iText = 4; //A가 0, a가 26
			else
				stTemp.m_iText = 36; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_F))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
				stTemp.m_iText = 5; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 5; //A가 0, a가 26
			else
				stTemp.m_iText = 37; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_G))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			stTemp.m_iText = 18; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 6; //A가 0, a가 26
			else
				stTemp.m_iText = 38; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_H))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Vowels;
			stTemp.m_iText = 8; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 7; //A가 0, a가 26
			else
				stTemp.m_iText = 39; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_I))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Vowels;
			stTemp.m_iText = 2; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Vowels;
			if (bCapsLock)
				stTemp.m_iText = 8; //A가 0, a가 26
			else
				stTemp.m_iText = 40; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_J))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Vowels;
			stTemp.m_iText = 4; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 9; //A가 0, a가 26
			else
				stTemp.m_iText = 41; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_K))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Vowels;
			stTemp.m_iText = 0; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 10; //A가 0, a가 26
			else
				stTemp.m_iText = 42; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_L))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Vowels;
			stTemp.m_iText = 20; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 11; //A가 0, a가 26
			else
				stTemp.m_iText = 43; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_M))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Vowels;
			stTemp.m_iText = 18; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 12; //A가 0, a가 26
			else
				stTemp.m_iText = 44; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_N))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Vowels;
			stTemp.m_iText = 13; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 13; //A가 0, a가 26
			else
				stTemp.m_iText = 45; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_O))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Vowels;
			if (bCapsLock)
				stTemp.m_iText = 3; //ㄱ이 0, ㅏ가 0
			else
				stTemp.m_iText = 1; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Vowels;
			if (bCapsLock)
				stTemp.m_iText = 14; //A가 0, a가 26
			else
				stTemp.m_iText = 46; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_P))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Vowels;
			if (bCapsLock)
				stTemp.m_iText = 7; //ㄱ이 0, ㅏ가 0
			else
				stTemp.m_iText = 5; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 15; //A가 0, a가 26
			else
				stTemp.m_iText = 47; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_Q))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 8; //ㄱ이 0, ㅏ가 0
			else
				stTemp.m_iText = 7; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 16; //A가 0, a가 26
			else
				stTemp.m_iText = 48; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_R))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 1; //ㄱ이 0, ㅏ가 0
			else
				stTemp.m_iText = 0; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 17; //A가 0, a가 26
			else
				stTemp.m_iText = 49; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_S))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			stTemp.m_iText = 2; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 18; //A가 0, a가 26
			else
				stTemp.m_iText = 50; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_T))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 10; //ㄱ이 0, ㅏ가 0
			else
				stTemp.m_iText = 9; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 19; //A가 0, a가 26
			else
				stTemp.m_iText = 51; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_U))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Vowels;
				stTemp.m_iText = 6; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Vowels;
			if (bCapsLock)
				stTemp.m_iText = 20; //A가 0, a가 26
			else
				stTemp.m_iText = 52; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_V))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			stTemp.m_iText = 17; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 21; //A가 0, a가 26
			else
				stTemp.m_iText = 53; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_W))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 13; //ㄱ이 0, ㅏ가 0
			else
				stTemp.m_iText = 12; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants; //반모음
			if (bCapsLock)
				stTemp.m_iText = 22; //A가 0, a가 26
			else
				stTemp.m_iText = 54; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_X))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			stTemp.m_iText = 16; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants; 
			if (bCapsLock)
				stTemp.m_iText = 23; //A가 0, a가 26
			else
				stTemp.m_iText = 55; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_Y))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Vowels;
			stTemp.m_iText = 12; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants; //반모음
			if (bCapsLock)
				stTemp.m_iText = 24; //A가 0, a가 26
			else
				stTemp.m_iText = 56; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_Z))
	{
		stInputText stTemp;
		if (m_bHangle)
		{
			stTemp.m_eTextType = ETEXT_TYPE_Consonants;
			stTemp.m_iText = 15; //ㄱ이 0, ㅏ가 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 25; //A가 0, a가 26
			else
				stTemp.m_iText = 57; //A가 0, a가 26
		}
		m_vecInputTxt.emplace_back(stTemp);
		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_SpaceBar))
	{
		stInputText stTemp;
		stTemp.m_eTextType = ETEXT_TYPE_Space;
		stTemp.m_iText = 32;
		m_vecInputTxt.emplace_back(stTemp);

		wstrOutPut_Combine();
	}
	else if (Engine::KeyDown(KeyCheck_BackSpaceBar))
	{
		if (0 != m_vecInputTxt.size())
		{
			m_vecInputTxt.erase(--m_vecInputTxt.end());
			wstrOutPut_Combine();
		}
		else
		{
			m_wstrNowCombine = L"";
			m_wstrOutPut = m_wstrOutPut.substr(0, m_wstrOutPut.size() - 1);//맞나?
		}
	}
}

void CText_Input::wstrOutPut_Combine()
{
	if (0 == m_vecInputTxt.size())
	{
		m_wstrNowCombine = L"";
		return;
	}

	_int ivecLast = (_int)m_vecInputTxt.size()-1;

	if (!(ETEXT_TYPE_Vowels >= m_vecInputTxt[ivecLast].m_eTextType)) //마지막에 받은 입력이 한글이 아니라면?
	{
		if (m_wstrNowCombine != L"")//만들던 글자가 있다면
			m_wstrOutPut += m_wstrNowCombine;//조합중이던 글 넣어버리고
		m_wstrNowCombine = L""; //만들던 친구 초기화 한다

		if (ETEXT_TYPE_English_Consonants == m_vecInputTxt[ivecLast].m_eTextType || ETEXT_TYPE_English_Vowels == m_vecInputTxt[ivecLast].m_eTextType)//영어라면
		{
			m_vecInputTxt[ivecLast].m_iText += 65;
			WCHAR Temp = (WCHAR)m_vecInputTxt[ivecLast].m_iText;
			m_wstrOutPut += Temp;
		}
		else if (ETEXT_TYPE_Number == m_vecInputTxt[ivecLast].m_eTextType) //숫자라면
		{
			m_vecInputTxt[ivecLast].m_iText += 48;
			WCHAR Temp = (WCHAR)m_vecInputTxt[ivecLast].m_iText;
			m_wstrOutPut += Temp;
		}
		else if (ETEXT_TYPE_Space == m_vecInputTxt[ivecLast].m_eTextType)//공백이라면
			m_wstrOutPut += L" ";

		m_vecInputTxt.clear(); //조합용 벡터 초기화 한다, capacity를 굳이 지울 필요가 없어보이므로 패쓰!
		return;
	}

	//////////////////////////////////////////////예외 처리용
	if (0 == m_vecInputTxt.size())
		return;
	/////////////////////////////////////////////////////////////////////////////
	//한글이 들어왔다.. 후 슈밤
	//첫번째가 모음이라면? 얘는 걍 혼자 놀 친구 ㅇㅇ 근데 모음 두개 연결할 수도 있으니.. 후
	if (ETEXT_TYPE_Vowels == m_vecInputTxt[0].m_eTextType)
	{
		if (1 == m_vecInputTxt.size()) //모음 하나만 있으면 우선 NowCombine에 넣어주고 리턴
		{
			//m_wstrNowCombine = Make_Hangle(0, m_vecInputTxt[0].m_iText); 모음만 나와야 하므로
			m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_JungSung);
			return;
		}

		if (ETEXT_TYPE_Vowels == m_vecInputTxt[1].m_eTextType) //만약 두번째 들어온 놈도 모음이라면..?
		{
			_int iCombineMoum;
			iCombineMoum = Combine_Moum(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText);
			if (-1 == iCombineMoum) //조합이 안되는 모음 두개였다
			{
				m_wstrOutPut += m_wstrNowCombine; //첫번째 모음 자식 아웃풋에 붙여버리고
				m_wstrNowCombine = L""; //초기화 하고
				m_vecInputTxt.erase(m_vecInputTxt.begin()); //첫번째 모음 지워뿌리고
				//m_wstrNowCombine = Make_Hangle(0, m_vecInputTxt[0].m_iText); //만들고 있는거에 새 친구 넣어버리고
				m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_JungSung);
				return; //탈출!
			}
			else //조합이 되는 모음 두개였다면!
			{
				//m_wstrNowCombine = Make_Hangle(0, iCombineMoum);//조합한 모음 한글로 만들어주고
				m_wstrNowCombine = Make_Jaso(iCombineMoum, EHANGLE_TYPE_JungSung);
				m_wstrOutPut += m_wstrNowCombine;//모음 두개면 더 조합할 껀덕지 없으니까 붙여주고
				m_wstrNowCombine = L""; //초기화 하고
				m_vecInputTxt.clear();//날려주고
				return; //탈출!
			}
		}
		else//두번째 들어온 놈이 자음이라면!!
		{
			m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_JungSung);
			m_wstrOutPut += m_wstrNowCombine; //첫번째 모음 자식 아웃풋에 붙여버리고
			m_wstrNowCombine = L""; //초기화 하고
			m_vecInputTxt.erase(m_vecInputTxt.begin()); //첫번째 모음 지워뿌리고
			m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_ChoSung);//들어온 자음 넣어주고!
			return;//탈~! 출~!
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	//한글이 들어왔다.. 후 슈밤
	//첫번째가 자음이라면?
	if (ETEXT_TYPE_Consonants == m_vecInputTxt[0].m_eTextType)
	{

		if (1 == m_vecInputTxt.size()) //자음 하나만 있으면 우선 NowCombine에 넣어주고 리턴
		{
			//m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText);
			m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_ChoSung);
			return;
		}

		if (ETEXT_TYPE_Consonants == m_vecInputTxt[1].m_eTextType) //두번째 들어온 놈도 자음이라면!! 
		{
			m_wstrOutPut += m_wstrNowCombine; //첫번째 자음 자식 아웃풋에 붙여버리고
			m_wstrNowCombine = L""; //초기화 하고
			m_vecInputTxt.erase(m_vecInputTxt.begin()); //첫번째 자음 지워뿌리고
			//m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText); //만들고 있는거에 새 친구 넣어버리고
			m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_ChoSung);
		}
		else if (ETEXT_TYPE_Vowels == m_vecInputTxt[1].m_eTextType)//두번째 들어온 게 모음이라면!! 
		{
			if (2 == m_vecInputTxt.size()) //자음 모음만 있고 아직 더 안들어왔다면 리턴!
			{
				m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText); //만들고 있는 글자 넣고!
				return;
			}

			if (ETEXT_TYPE_Vowels == m_vecInputTxt[2].m_eTextType) //모음이 연속 두개가 들어왔다면!!
			{
				_int iCombineMoum;
				iCombineMoum = Combine_Moum(m_vecInputTxt[1].m_iText, m_vecInputTxt[2].m_iText);
				if (-1 == iCombineMoum) //조합이 안되는 모음 두개였다!
				{
					m_wstrOutPut += m_wstrNowCombine; //만들던거 아웃풋에 붙여버리고
					m_wstrNowCombine = L""; //초기화 하고
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //첫번째 자음 지워뿌리고
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //두번째 모음 지워뿌리고
					//m_wstrNowCombine = Make_Hangle(0, m_vecInputTxt[0].m_iText); //만들고 있는거에 새 친구 넣어버리고
					m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_JungSung);
					return; //탈출!!
				}
				else //조합이 되는 모음 두개였다!
				{
					stInputText stTemp;
					stTemp.m_eTextType = ETEXT_TYPE_Vowels;
					stTemp.m_iText = iCombineMoum;

					m_vecInputTxt.erase(--m_vecInputTxt.end()); //모음 두개를
					m_vecInputTxt.erase(--m_vecInputTxt.end()); //지운 뒤에
					m_vecInputTxt.emplace_back(stTemp); // 조합한 모음을 넣는다!
					m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText); //만들고 있는 글자 넣고!
					return; // 탈~! 출~!!
				}
			}
			else // 세번째 들어온 친구가 종성이라면!!
			{
				m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText, Making_JongSung(m_vecInputTxt[2].m_iText)); //만들고 있는 글자 넣고!
			}

			if (3 == m_vecInputTxt.size()) //초중종만 있다면 
				return; //아직은 여기 올 때가 아니다~

			if (ETEXT_TYPE_Vowels == m_vecInputTxt[3].m_eTextType)//네번째 들어온 넘이 모음이다!
			{
				//사실 종성이 아니였던 거임~!!!!
				m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText); //초 중성으로 한글 만들고!
				m_wstrOutPut += m_wstrNowCombine; //아웃 풋에 더해주고!
				m_vecInputTxt.erase(m_vecInputTxt.begin()); //첫번째 자음 지워뿌리고
				m_vecInputTxt.erase(m_vecInputTxt.begin()); //두번째 모음 지워뿌리고
				m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText); //만들고 있는거에 자음 모음 넣어 뿌고!!
				return;//타아아아아알~~~ 추우웅우욹ㄹ~!
			}
			else //네번째 들어온 애가 자음...이다?~!
			{
				_int iCombineMoum;
				iCombineMoum = Making_JongSung(m_vecInputTxt[2].m_iText, m_vecInputTxt[3].m_iText);

				if (-1 == iCombineMoum) // 둘이 합쳐서 종성인게 아니였던 거임~!!
				{
					//세개 합쳐서 하나로 만들어 버리고!!
					m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText, Making_JongSung(m_vecInputTxt[2].m_iText));
					m_wstrOutPut += m_wstrNowCombine; //아웃 풋에 더해주고!
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //첫번째 자음 지워뿌리고
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //두번째 모음 지워뿌리고
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //세번째 자음 지워뿌리고
					//m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText); //만들고 있는거에 자음 넣어 뿌고!!
					m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_ChoSung);
					return;//타아아아아알~~~ 추우웅우욹ㄹ~!
				}
				else //어찌 종성이 만들어져 버린거임~! 다음에 모음이 들어온다면 모르는 거임~!
				{
					m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText, iCombineMoum);
				}

				if (4 == m_vecInputTxt.size()) //초중종만 있다면 
					return; //아직은 여기 올 때가 아니다~

				if (ETEXT_TYPE_Vowels == m_vecInputTxt[4].m_eTextType)//네번째 친구가 종성 합치기가 아니라 다음 글자였네!!!
				{
					m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText, Making_JongSung(m_vecInputTxt[2].m_iText));
					m_wstrOutPut += m_wstrNowCombine; //아웃 풋에 더해주고!
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //첫번째 자음 지워뿌리고
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //두번째 모음 지워뿌리고
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //세번째 자음 지워뿌리고
					//m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText); //만들고 있는거에 자음 모음 넣어 뿌고!!
					m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_ChoSung);
					return; //타아아아알추우우울~!
				}
				else //종성 합치기가 맞았네!!!
				{
					m_wstrOutPut += m_wstrNowCombine; //아웃 풋에 더해주고!
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //첫번째 자음 지워뿌리고
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //두번째 모음 지워뿌리고
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //세번째 자음 지워뿌리고
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //네번째 자음 지워뿌리고
					//m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText); //만들고 있는거에 자음 모음 넣어 뿌고!!
					m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_ChoSung);
					return; //우와~!! 끝났다아아아아ㅏㅏㅏㅏㅏ
				}
			}

		}
	}
	
}
wstring CText_Input::Get_NowText()
{
	wstring wstrAllText = m_wstrOutPut + m_wstrNowCombine;
	return wstrAllText;
}
WCHAR CText_Input::Make_Hangle(_int _iChoSung, _int _iJungSung, _int _iJongSung)
{
	_int iTemp = 44032 +((_iChoSung * 21) + _iJungSung)* 28 + _iJongSung;
	WCHAR Temp = (WCHAR)iTemp;
	return Temp;
}
WCHAR CText_Input::Make_Jaso(_int _iText, EHANGLE_TYPE _eHangleType)
{
		//각 각의 초성, 중성, 종성을 따로 출력하고 싶을 경우
		//초성 0x1100 ->4352
		//중성 0x1161 ->4449
		//종성 0x11A7 ->4519
		//에 index값을 더하여 출력하면된다.
	_int iTemp;
	switch (_eHangleType)
	{
	case CText_Input::EHANGLE_TYPE_ChoSung:
		iTemp = _iText + 4352;
		break;
	case CText_Input::EHANGLE_TYPE_JungSung:
		iTemp = _iText + 4449;
		break;
	case CText_Input::EHANGLE_TYPE_JongSung:
		iTemp = _iText + 4519;
		break;
	}
	WCHAR Temp = (WCHAR)iTemp;

	return Temp;
}
_int CText_Input::Combine_Moum(_int _iMoum1, _int _iMoum2)
{
	if(_iMoum1 == 8 && _iMoum2 == 0)
		return 9; //ㅘ

	if (_iMoum1 == 8 && _iMoum2 == 1)
		return 10; //ㅙ

	if (_iMoum1 == 8 && _iMoum2 == 20)
		return 11; //ㅚ

	if (_iMoum1 == 13 && _iMoum2 == 4)
		return 14; //ㅝ

	if (_iMoum1 == 13 && _iMoum2 == 5)
		return 15; //ㅞ

	if (_iMoum1 == 13 && _iMoum2 == 20)
		return 16; //ㅟ

	if (_iMoum1 == 18 && _iMoum2 == 20)
		return 19; //ㅢ

	return -1; //실패했다는 뜻
}
_int CText_Input::Making_JongSung(_int _iJaum1, _int _iJaum2)
{
	if (_iJaum1 == -1 && _iJaum2 == -1)
		return 0; //없음

	if (_iJaum1 == 0 && _iJaum2 == -1)
		return 1; //ㄱ

	if (_iJaum1 == 1 && _iJaum2 == -1)
		return 2; //ㄲ

	if (_iJaum1 == 0 && _iJaum2 == 9)
		return 3; //ㄳ

	if (_iJaum1 == 2 && _iJaum2 == -1)
		return 4; //ㄴ

	if (_iJaum1 == 2 && _iJaum2 == 12)
		return 5; //ㄵ

	if (_iJaum1 == 2 && _iJaum2 == 18)
		return 6; //ㄶ

	if (_iJaum1 == 3 && _iJaum2 == -1)
		return 7; //ㄷ

	if (_iJaum1 == 5 && _iJaum2 == -1)
		return 8; //ㄹ

	if (_iJaum1 == 5 && _iJaum2 == 0)
		return 9; //ㄺ

	if (_iJaum1 == 5 && _iJaum2 == 6)
		return 10; //ㄻ

	if (_iJaum1 == 5 && _iJaum2 == 7)
		return 11; //ㄼ

	if (_iJaum1 == 5 && _iJaum2 == 9)
		return 12; //ㄽ

	if (_iJaum1 == 5 && _iJaum2 == 16)
		return 13; //ㄾ

	if (_iJaum1 == 5 && _iJaum2 == 17)
		return 14; //ㄿ

	if (_iJaum1 == 5 && _iJaum2 == 18)
		return 15; //ㅀ

	if (_iJaum1 == 6 && _iJaum2 == -1)
		return 16; //ㅁ

	if (_iJaum1 == 7 && _iJaum2 == -1)
		return 17; //ㅂ

	if (_iJaum1 == 7 && _iJaum2 == 9)
		return 18; //ㅄ

	if (_iJaum1 == 9 && _iJaum2 == -1)
		return 19; //ㅅ

	if (_iJaum1 == 10 && _iJaum2 == -1)
		return 20; //ㅆ

	if (_iJaum1 == 11 && _iJaum2 == -1)
		return 21; //ㅇ

	if (_iJaum1 == 12 && _iJaum2 == -1)
		return 22; //ㅈ

	if (_iJaum1 == 14 && _iJaum2 == -1)
		return 23; //ㅊ

	if (_iJaum1 == 15 && _iJaum2 == -1)
		return 24; //ㅋ

	if (_iJaum1 == 16 && _iJaum2 == -1)
		return 25; //ㅌ

	if (_iJaum1 == 17 && _iJaum2 == -1)
		return 26; //ㅍ

	if (_iJaum1 == 18 && _iJaum2 == -1)
		return 27; //ㅎ

	return -1;
}
void CText_Input::Text_Input_Reset()
{
	m_vecInputTxt.clear();
	vector<stInputText>().swap(m_vecInputTxt);
	m_wstrOutPut = L"";
	m_wstrNowCombine = L"";
	m_bHangle = 1;
}
//
//
//if (1 == m_vecInputTxt.end()) //이제 막 새로운 글자 입력중
//{
//	if (!(ETEXT_TYPE_Vowels >= m_vecInputTxt[0].m_eTextType)) //한글이 아님
//		if (ETEXT_TYPE_English_Consonants == m_vecInputTxt[0].m_eTextType || ETEXT_TYPE_English_Vowels == m_vecInputTxt[0].m_eTextType)//영어라면
//		{
//
//			m_vecInputTxt[0].m_iText += 65;
//			WCHAR Temp = (WCHAR)m_vecInputTxt[0].m_iText;
//			m_wstrOutPut += Temp;
//
//			m_wstrNowCombine = L"";//조합중인거 초기화
//			m_vecInputTxt.clear(); // capacity를 굳이 지울 필요가 없어보여서 안함
//		}
//		else if (ETEXT_TYPE_Number == m_vecInputTxt[0].m_eTextType) //숫자라면
//		{
//			m_vecInputTxt[0].m_iText += 48;
//			WCHAR Temp = (WCHAR)m_vecInputTxt[0].m_iText;
//			m_wstrOutPut += Temp;
//			m_wstrNowCombine = L"";//조합중인거 초기화
//			m_vecInputTxt.clear(); // capacity를 굳이 지울 필요가 없어보여서 안함
//		}
//		else if (ETEXT_TYPE_Space == m_vecInputTxt[0].m_eTextType)//공백이라면
//		{
//			m_wstrOutPut += L" ";
//			m_wstrNowCombine = L"";//조합중인거 초기화
//			m_vecInputTxt.clear(); // capacity를 굳이 지울 필요가 없어보여서 안함
//		}
//}