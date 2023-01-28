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
	//// hWnd�� �������ڵ�
	//HIMC_HanYuong = ImmGetContext(g_hWnd);
	////�ѿ����¸� ��´�
	////Flag�� true�̸� �ѱ�, false�̸� ������
	//m_bHangle = ImmGetOpenStatus(HIMC_HanYuong);
	////IMM�ڵ��� ���� �Ѵ�.
	//ImmReleaseContext(g_hWnd, HIMC_HanYuong); //Imm.lib �Ⱦ���!
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
			stTemp.m_iText = 6; //���� 0 //https://d2.naver.com/helloworld/76650?source=post_page---------------------------
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Vowels;
			if (bCapsLock)
				stTemp.m_iText = 0; //A�� 0, a�� 26
			else
				stTemp.m_iText = 32; //A�� 0, a�� 32
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
			stTemp.m_iText = 17 /*9*/; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 1; //A�� 0, a�� 26
			else
				stTemp.m_iText = 33; //A�� 0, a�� 26
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
			stTemp.m_iText = 14; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 2; //A�� 0, a�� 26
			else
				stTemp.m_iText = 34; //A�� 0, a�� 26
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
			stTemp.m_iText = 11; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 3; //A�� 0, a�� 26
			else
				stTemp.m_iText = 35; //A�� 0, a�� 26
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
				stTemp.m_iText = 3; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Vowels;
			if (bCapsLock)
				stTemp.m_iText = 4; //A�� 0, a�� 26
			else
				stTemp.m_iText = 36; //A�� 0, a�� 26
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
				stTemp.m_iText = 5; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 5; //A�� 0, a�� 26
			else
				stTemp.m_iText = 37; //A�� 0, a�� 26
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
			stTemp.m_iText = 18; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 6; //A�� 0, a�� 26
			else
				stTemp.m_iText = 38; //A�� 0, a�� 26
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
			stTemp.m_iText = 8; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 7; //A�� 0, a�� 26
			else
				stTemp.m_iText = 39; //A�� 0, a�� 26
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
			stTemp.m_iText = 2; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Vowels;
			if (bCapsLock)
				stTemp.m_iText = 8; //A�� 0, a�� 26
			else
				stTemp.m_iText = 40; //A�� 0, a�� 26
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
			stTemp.m_iText = 4; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 9; //A�� 0, a�� 26
			else
				stTemp.m_iText = 41; //A�� 0, a�� 26
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
			stTemp.m_iText = 0; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 10; //A�� 0, a�� 26
			else
				stTemp.m_iText = 42; //A�� 0, a�� 26
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
			stTemp.m_iText = 20; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 11; //A�� 0, a�� 26
			else
				stTemp.m_iText = 43; //A�� 0, a�� 26
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
			stTemp.m_iText = 18; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 12; //A�� 0, a�� 26
			else
				stTemp.m_iText = 44; //A�� 0, a�� 26
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
			stTemp.m_iText = 13; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 13; //A�� 0, a�� 26
			else
				stTemp.m_iText = 45; //A�� 0, a�� 26
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
				stTemp.m_iText = 3; //���� 0, ���� 0
			else
				stTemp.m_iText = 1; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Vowels;
			if (bCapsLock)
				stTemp.m_iText = 14; //A�� 0, a�� 26
			else
				stTemp.m_iText = 46; //A�� 0, a�� 26
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
				stTemp.m_iText = 7; //���� 0, ���� 0
			else
				stTemp.m_iText = 5; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 15; //A�� 0, a�� 26
			else
				stTemp.m_iText = 47; //A�� 0, a�� 26
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
				stTemp.m_iText = 8; //���� 0, ���� 0
			else
				stTemp.m_iText = 7; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 16; //A�� 0, a�� 26
			else
				stTemp.m_iText = 48; //A�� 0, a�� 26
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
				stTemp.m_iText = 1; //���� 0, ���� 0
			else
				stTemp.m_iText = 0; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 17; //A�� 0, a�� 26
			else
				stTemp.m_iText = 49; //A�� 0, a�� 26
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
			stTemp.m_iText = 2; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 18; //A�� 0, a�� 26
			else
				stTemp.m_iText = 50; //A�� 0, a�� 26
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
				stTemp.m_iText = 10; //���� 0, ���� 0
			else
				stTemp.m_iText = 9; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 19; //A�� 0, a�� 26
			else
				stTemp.m_iText = 51; //A�� 0, a�� 26
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
				stTemp.m_iText = 6; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Vowels;
			if (bCapsLock)
				stTemp.m_iText = 20; //A�� 0, a�� 26
			else
				stTemp.m_iText = 52; //A�� 0, a�� 26
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
			stTemp.m_iText = 17; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 21; //A�� 0, a�� 26
			else
				stTemp.m_iText = 53; //A�� 0, a�� 26
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
				stTemp.m_iText = 13; //���� 0, ���� 0
			else
				stTemp.m_iText = 12; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants; //�ݸ���
			if (bCapsLock)
				stTemp.m_iText = 22; //A�� 0, a�� 26
			else
				stTemp.m_iText = 54; //A�� 0, a�� 26
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
			stTemp.m_iText = 16; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants; 
			if (bCapsLock)
				stTemp.m_iText = 23; //A�� 0, a�� 26
			else
				stTemp.m_iText = 55; //A�� 0, a�� 26
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
			stTemp.m_iText = 12; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants; //�ݸ���
			if (bCapsLock)
				stTemp.m_iText = 24; //A�� 0, a�� 26
			else
				stTemp.m_iText = 56; //A�� 0, a�� 26
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
			stTemp.m_iText = 15; //���� 0, ���� 0
		}
		else
		{
			stTemp.m_eTextType = ETEXT_TYPE_English_Consonants;
			if (bCapsLock)
				stTemp.m_iText = 25; //A�� 0, a�� 26
			else
				stTemp.m_iText = 57; //A�� 0, a�� 26
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
			m_wstrOutPut = m_wstrOutPut.substr(0, m_wstrOutPut.size() - 1);//�³�?
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

	if (!(ETEXT_TYPE_Vowels >= m_vecInputTxt[ivecLast].m_eTextType)) //�������� ���� �Է��� �ѱ��� �ƴ϶��?
	{
		if (m_wstrNowCombine != L"")//����� ���ڰ� �ִٸ�
			m_wstrOutPut += m_wstrNowCombine;//�������̴� �� �־������
		m_wstrNowCombine = L""; //����� ģ�� �ʱ�ȭ �Ѵ�

		if (ETEXT_TYPE_English_Consonants == m_vecInputTxt[ivecLast].m_eTextType || ETEXT_TYPE_English_Vowels == m_vecInputTxt[ivecLast].m_eTextType)//������
		{
			m_vecInputTxt[ivecLast].m_iText += 65;
			WCHAR Temp = (WCHAR)m_vecInputTxt[ivecLast].m_iText;
			m_wstrOutPut += Temp;
		}
		else if (ETEXT_TYPE_Number == m_vecInputTxt[ivecLast].m_eTextType) //���ڶ��
		{
			m_vecInputTxt[ivecLast].m_iText += 48;
			WCHAR Temp = (WCHAR)m_vecInputTxt[ivecLast].m_iText;
			m_wstrOutPut += Temp;
		}
		else if (ETEXT_TYPE_Space == m_vecInputTxt[ivecLast].m_eTextType)//�����̶��
			m_wstrOutPut += L" ";

		m_vecInputTxt.clear(); //���տ� ���� �ʱ�ȭ �Ѵ�, capacity�� ���� ���� �ʿ䰡 ����̹Ƿ� �о�!
		return;
	}

	//////////////////////////////////////////////���� ó����
	if (0 == m_vecInputTxt.size())
		return;
	/////////////////////////////////////////////////////////////////////////////
	//�ѱ��� ���Դ�.. �� ����
	//ù��°�� �����̶��? ��� �� ȥ�� �� ģ�� ���� �ٵ� ���� �ΰ� ������ ���� ������.. ��
	if (ETEXT_TYPE_Vowels == m_vecInputTxt[0].m_eTextType)
	{
		if (1 == m_vecInputTxt.size()) //���� �ϳ��� ������ �켱 NowCombine�� �־��ְ� ����
		{
			//m_wstrNowCombine = Make_Hangle(0, m_vecInputTxt[0].m_iText); ������ ���;� �ϹǷ�
			m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_JungSung);
			return;
		}

		if (ETEXT_TYPE_Vowels == m_vecInputTxt[1].m_eTextType) //���� �ι�° ���� �� �����̶��..?
		{
			_int iCombineMoum;
			iCombineMoum = Combine_Moum(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText);
			if (-1 == iCombineMoum) //������ �ȵǴ� ���� �ΰ�����
			{
				m_wstrOutPut += m_wstrNowCombine; //ù��° ���� �ڽ� �ƿ�ǲ�� �ٿ�������
				m_wstrNowCombine = L""; //�ʱ�ȭ �ϰ�
				m_vecInputTxt.erase(m_vecInputTxt.begin()); //ù��° ���� �����Ѹ���
				//m_wstrNowCombine = Make_Hangle(0, m_vecInputTxt[0].m_iText); //����� �ִ°ſ� �� ģ�� �־������
				m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_JungSung);
				return; //Ż��!
			}
			else //������ �Ǵ� ���� �ΰ����ٸ�!
			{
				//m_wstrNowCombine = Make_Hangle(0, iCombineMoum);//������ ���� �ѱ۷� ������ְ�
				m_wstrNowCombine = Make_Jaso(iCombineMoum, EHANGLE_TYPE_JungSung);
				m_wstrOutPut += m_wstrNowCombine;//���� �ΰ��� �� ������ ������ �����ϱ� �ٿ��ְ�
				m_wstrNowCombine = L""; //�ʱ�ȭ �ϰ�
				m_vecInputTxt.clear();//�����ְ�
				return; //Ż��!
			}
		}
		else//�ι�° ���� ���� �����̶��!!
		{
			m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_JungSung);
			m_wstrOutPut += m_wstrNowCombine; //ù��° ���� �ڽ� �ƿ�ǲ�� �ٿ�������
			m_wstrNowCombine = L""; //�ʱ�ȭ �ϰ�
			m_vecInputTxt.erase(m_vecInputTxt.begin()); //ù��° ���� �����Ѹ���
			m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_ChoSung);//���� ���� �־��ְ�!
			return;//Ż~! ��~!
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	//�ѱ��� ���Դ�.. �� ����
	//ù��°�� �����̶��?
	if (ETEXT_TYPE_Consonants == m_vecInputTxt[0].m_eTextType)
	{

		if (1 == m_vecInputTxt.size()) //���� �ϳ��� ������ �켱 NowCombine�� �־��ְ� ����
		{
			//m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText);
			m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_ChoSung);
			return;
		}

		if (ETEXT_TYPE_Consonants == m_vecInputTxt[1].m_eTextType) //�ι�° ���� �� �����̶��!! 
		{
			m_wstrOutPut += m_wstrNowCombine; //ù��° ���� �ڽ� �ƿ�ǲ�� �ٿ�������
			m_wstrNowCombine = L""; //�ʱ�ȭ �ϰ�
			m_vecInputTxt.erase(m_vecInputTxt.begin()); //ù��° ���� �����Ѹ���
			//m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText); //����� �ִ°ſ� �� ģ�� �־������
			m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_ChoSung);
		}
		else if (ETEXT_TYPE_Vowels == m_vecInputTxt[1].m_eTextType)//�ι�° ���� �� �����̶��!! 
		{
			if (2 == m_vecInputTxt.size()) //���� ������ �ְ� ���� �� �ȵ��Դٸ� ����!
			{
				m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText); //����� �ִ� ���� �ְ�!
				return;
			}

			if (ETEXT_TYPE_Vowels == m_vecInputTxt[2].m_eTextType) //������ ���� �ΰ��� ���Դٸ�!!
			{
				_int iCombineMoum;
				iCombineMoum = Combine_Moum(m_vecInputTxt[1].m_iText, m_vecInputTxt[2].m_iText);
				if (-1 == iCombineMoum) //������ �ȵǴ� ���� �ΰ�����!
				{
					m_wstrOutPut += m_wstrNowCombine; //������� �ƿ�ǲ�� �ٿ�������
					m_wstrNowCombine = L""; //�ʱ�ȭ �ϰ�
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //ù��° ���� �����Ѹ���
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //�ι�° ���� �����Ѹ���
					//m_wstrNowCombine = Make_Hangle(0, m_vecInputTxt[0].m_iText); //����� �ִ°ſ� �� ģ�� �־������
					m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_JungSung);
					return; //Ż��!!
				}
				else //������ �Ǵ� ���� �ΰ�����!
				{
					stInputText stTemp;
					stTemp.m_eTextType = ETEXT_TYPE_Vowels;
					stTemp.m_iText = iCombineMoum;

					m_vecInputTxt.erase(--m_vecInputTxt.end()); //���� �ΰ���
					m_vecInputTxt.erase(--m_vecInputTxt.end()); //���� �ڿ�
					m_vecInputTxt.emplace_back(stTemp); // ������ ������ �ִ´�!
					m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText); //����� �ִ� ���� �ְ�!
					return; // Ż~! ��~!!
				}
			}
			else // ����° ���� ģ���� �����̶��!!
			{
				m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText, Making_JongSung(m_vecInputTxt[2].m_iText)); //����� �ִ� ���� �ְ�!
			}

			if (3 == m_vecInputTxt.size()) //�������� �ִٸ� 
				return; //������ ���� �� ���� �ƴϴ�~

			if (ETEXT_TYPE_Vowels == m_vecInputTxt[3].m_eTextType)//�׹�° ���� ���� �����̴�!
			{
				//��� ������ �ƴϿ��� ����~!!!!
				m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText); //�� �߼����� �ѱ� �����!
				m_wstrOutPut += m_wstrNowCombine; //�ƿ� ǲ�� �����ְ�!
				m_vecInputTxt.erase(m_vecInputTxt.begin()); //ù��° ���� �����Ѹ���
				m_vecInputTxt.erase(m_vecInputTxt.begin()); //�ι�° ���� �����Ѹ���
				m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText); //����� �ִ°ſ� ���� ���� �־� �Ѱ�!!
				return;//Ÿ�ƾƾƾƾ�~~~ �߿�����~!
			}
			else //�׹�° ���� �ְ� ����...�̴�?~!
			{
				_int iCombineMoum;
				iCombineMoum = Making_JongSung(m_vecInputTxt[2].m_iText, m_vecInputTxt[3].m_iText);

				if (-1 == iCombineMoum) // ���� ���ļ� �����ΰ� �ƴϿ��� ����~!!
				{
					//���� ���ļ� �ϳ��� ����� ������!!
					m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText, Making_JongSung(m_vecInputTxt[2].m_iText));
					m_wstrOutPut += m_wstrNowCombine; //�ƿ� ǲ�� �����ְ�!
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //ù��° ���� �����Ѹ���
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //�ι�° ���� �����Ѹ���
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //����° ���� �����Ѹ���
					//m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText); //����� �ִ°ſ� ���� �־� �Ѱ�!!
					m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_ChoSung);
					return;//Ÿ�ƾƾƾƾ�~~~ �߿�����~!
				}
				else //���� ������ ������� ��������~! ������ ������ ���´ٸ� �𸣴� ����~!
				{
					m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText, iCombineMoum);
				}

				if (4 == m_vecInputTxt.size()) //�������� �ִٸ� 
					return; //������ ���� �� ���� �ƴϴ�~

				if (ETEXT_TYPE_Vowels == m_vecInputTxt[4].m_eTextType)//�׹�° ģ���� ���� ��ġ�Ⱑ �ƴ϶� ���� ���ڿ���!!!
				{
					m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText, Making_JongSung(m_vecInputTxt[2].m_iText));
					m_wstrOutPut += m_wstrNowCombine; //�ƿ� ǲ�� �����ְ�!
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //ù��° ���� �����Ѹ���
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //�ι�° ���� �����Ѹ���
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //����° ���� �����Ѹ���
					//m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText, m_vecInputTxt[1].m_iText); //����� �ִ°ſ� ���� ���� �־� �Ѱ�!!
					m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_ChoSung);
					return; //Ÿ�ƾƾƾ��߿���~!
				}
				else //���� ��ġ�Ⱑ �¾ҳ�!!!
				{
					m_wstrOutPut += m_wstrNowCombine; //�ƿ� ǲ�� �����ְ�!
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //ù��° ���� �����Ѹ���
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //�ι�° ���� �����Ѹ���
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //����° ���� �����Ѹ���
					m_vecInputTxt.erase(m_vecInputTxt.begin()); //�׹�° ���� �����Ѹ���
					//m_wstrNowCombine = Make_Hangle(m_vecInputTxt[0].m_iText); //����� �ִ°ſ� ���� ���� �־� �Ѱ�!!
					m_wstrNowCombine = Make_Jaso(m_vecInputTxt[0].m_iText, EHANGLE_TYPE_ChoSung);
					return; //���~!! �����پƾƾƾƤ���������
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
		//�� ���� �ʼ�, �߼�, ������ ���� ����ϰ� ���� ���
		//�ʼ� 0x1100 ->4352
		//�߼� 0x1161 ->4449
		//���� 0x11A7 ->4519
		//�� index���� ���Ͽ� ����ϸ�ȴ�.
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
		return 9; //��

	if (_iMoum1 == 8 && _iMoum2 == 1)
		return 10; //��

	if (_iMoum1 == 8 && _iMoum2 == 20)
		return 11; //��

	if (_iMoum1 == 13 && _iMoum2 == 4)
		return 14; //��

	if (_iMoum1 == 13 && _iMoum2 == 5)
		return 15; //��

	if (_iMoum1 == 13 && _iMoum2 == 20)
		return 16; //��

	if (_iMoum1 == 18 && _iMoum2 == 20)
		return 19; //��

	return -1; //�����ߴٴ� ��
}
_int CText_Input::Making_JongSung(_int _iJaum1, _int _iJaum2)
{
	if (_iJaum1 == -1 && _iJaum2 == -1)
		return 0; //����

	if (_iJaum1 == 0 && _iJaum2 == -1)
		return 1; //��

	if (_iJaum1 == 1 && _iJaum2 == -1)
		return 2; //��

	if (_iJaum1 == 0 && _iJaum2 == 9)
		return 3; //��

	if (_iJaum1 == 2 && _iJaum2 == -1)
		return 4; //��

	if (_iJaum1 == 2 && _iJaum2 == 12)
		return 5; //��

	if (_iJaum1 == 2 && _iJaum2 == 18)
		return 6; //��

	if (_iJaum1 == 3 && _iJaum2 == -1)
		return 7; //��

	if (_iJaum1 == 5 && _iJaum2 == -1)
		return 8; //��

	if (_iJaum1 == 5 && _iJaum2 == 0)
		return 9; //��

	if (_iJaum1 == 5 && _iJaum2 == 6)
		return 10; //��

	if (_iJaum1 == 5 && _iJaum2 == 7)
		return 11; //��

	if (_iJaum1 == 5 && _iJaum2 == 9)
		return 12; //��

	if (_iJaum1 == 5 && _iJaum2 == 16)
		return 13; //��

	if (_iJaum1 == 5 && _iJaum2 == 17)
		return 14; //��

	if (_iJaum1 == 5 && _iJaum2 == 18)
		return 15; //��

	if (_iJaum1 == 6 && _iJaum2 == -1)
		return 16; //��

	if (_iJaum1 == 7 && _iJaum2 == -1)
		return 17; //��

	if (_iJaum1 == 7 && _iJaum2 == 9)
		return 18; //��

	if (_iJaum1 == 9 && _iJaum2 == -1)
		return 19; //��

	if (_iJaum1 == 10 && _iJaum2 == -1)
		return 20; //��

	if (_iJaum1 == 11 && _iJaum2 == -1)
		return 21; //��

	if (_iJaum1 == 12 && _iJaum2 == -1)
		return 22; //��

	if (_iJaum1 == 14 && _iJaum2 == -1)
		return 23; //��

	if (_iJaum1 == 15 && _iJaum2 == -1)
		return 24; //��

	if (_iJaum1 == 16 && _iJaum2 == -1)
		return 25; //��

	if (_iJaum1 == 17 && _iJaum2 == -1)
		return 26; //��

	if (_iJaum1 == 18 && _iJaum2 == -1)
		return 27; //��

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
//if (1 == m_vecInputTxt.end()) //���� �� ���ο� ���� �Է���
//{
//	if (!(ETEXT_TYPE_Vowels >= m_vecInputTxt[0].m_eTextType)) //�ѱ��� �ƴ�
//		if (ETEXT_TYPE_English_Consonants == m_vecInputTxt[0].m_eTextType || ETEXT_TYPE_English_Vowels == m_vecInputTxt[0].m_eTextType)//������
//		{
//
//			m_vecInputTxt[0].m_iText += 65;
//			WCHAR Temp = (WCHAR)m_vecInputTxt[0].m_iText;
//			m_wstrOutPut += Temp;
//
//			m_wstrNowCombine = L"";//�������ΰ� �ʱ�ȭ
//			m_vecInputTxt.clear(); // capacity�� ���� ���� �ʿ䰡 ������� ����
//		}
//		else if (ETEXT_TYPE_Number == m_vecInputTxt[0].m_eTextType) //���ڶ��
//		{
//			m_vecInputTxt[0].m_iText += 48;
//			WCHAR Temp = (WCHAR)m_vecInputTxt[0].m_iText;
//			m_wstrOutPut += Temp;
//			m_wstrNowCombine = L"";//�������ΰ� �ʱ�ȭ
//			m_vecInputTxt.clear(); // capacity�� ���� ���� �ʿ䰡 ������� ����
//		}
//		else if (ETEXT_TYPE_Space == m_vecInputTxt[0].m_eTextType)//�����̶��
//		{
//			m_wstrOutPut += L" ";
//			m_wstrNowCombine = L"";//�������ΰ� �ʱ�ȭ
//			m_vecInputTxt.clear(); // capacity�� ���� ���� �ʿ䰡 ������� ����
//		}
//}