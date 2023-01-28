// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"
#include "MainApp.h"

#include "Export_System.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE	g_hInst;                                // 현재 인스턴스입니다.
WCHAR		szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR		szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND		g_hWnd;
_double		g_GlobalTime;
_bool		g_bInstancing;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
	g_bInstancing = true;
	srand(unsigned(time(NULL)));

	// 16 : 9 비율의 1280:720 해상도
	//FAILED_CHECK_RETURN(Engine::Set_ScreenSize(CGraphicDevice::ESCREEN_RESOULTION_16To9, 80.f), E_FAIL);

	// 16 : 9 비율의 1366:768 해상도
	FAILED_CHECK_RETURN(Engine::Set_ScreenSize(CGraphicDevice::ESCREEN_RESOULTION_16To9, 85.375f), E_FAIL);

	// 16 : 9 비율의 1600:900 해상도
	//FAILED_CHECK_RETURN(Engine::Set_ScreenSize(CGraphicDevice::ESCREEN_RESOULTION_16To9, 100.f), E_FAIL);

	// 4 : 3 비율의 800:600 해상도
	//FAILED_CHECK_RETURN(Engine::Set_ScreenSize(CGraphicDevice::ESCREEN_RESOULTION_4To3, 200.f), E_FAIL);


    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	MSG msg;
	msg.message = WM_NULL;

	// 메인게임 생성

	CMainApp*		pMainApp = CMainApp::Create();
	NULL_CHECK_RETURN(pMainApp, FALSE);

	// 타이머 생성
	FAILED_CHECK_RETURN(Ready_Timer(L"Timer_Immediate"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Timer(L"Timer_FPS60"), E_FAIL);

	// FPS 생성
	FAILED_CHECK_RETURN(Ready_Frame(L"Frame_FPS60", 60), E_FAIL);


    // 기본 메시지 루프입니다.
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;


			if (msg.message == WM_KEYDOWN)
			{
				switch (msg.wParam)
				{
					case VK_PROCESSKEY:
					{
						/*int ikey_Value_Check = ::MapVirtualKey((msg.lParam >> 16) & 0xff, MAPVK_VSC_TO_VK_EX);
						if (ikey_Value_Check == VK_HANGEUL)
							Engine::HanYoung_Key_Change();*/

						//라이브러리 사용해야 함
						_int ikey_Value_Check = ImmGetVirtualKey(g_hWnd);
						if (ikey_Value_Check == VK_HANGEUL)
						{
							Engine::HanYoung_Key_Change();
						}
					}
				}
			}


			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			Set_TimeDelta(L"Timer_Immediate");

			_double dImmediate = Get_TimeDelta(L"Timer_Immediate");

			if (IsPermit_Call(L"Frame_FPS60", dImmediate))
			{
				Set_TimeDelta(L"Timer_FPS60");
				_double	dFPS60 = Get_TimeDelta(L"Timer_FPS60");

				pMainApp->Update_MainApp(dFPS60);
				//pMainApp->Render_MainApp();

				//_CrtSetBreakAlloc(422160);
			}
		}
	}

	_ulong	dwRef = 0;

	if (Safe_Release(pMainApp))
	{
		MSG_BOX("MainApp Release Failed");
		return FALSE;
	}

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT		rc{ 0, 0, (LONG)WINCX, (LONG)WINCY };
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	//case WM_KEYDOWN:
	//	switch (wParam)
	//	{
	//	case VK_ESCAPE:
	//		DestroyWindow(hWnd);
	//		break;
	//	default:
	//		break;
	//	}
	//	break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}