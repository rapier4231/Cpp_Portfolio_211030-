// Client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Client.h"
#include "MainApp.h"

#include "Export_System.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE	g_hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR		szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR		szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
HWND		g_hWnd;
_double		g_GlobalTime;
_bool		g_bInstancing;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.
	g_bInstancing = true;
	srand(unsigned(time(NULL)));

	// 16 : 9 ������ 1280:720 �ػ�
	//FAILED_CHECK_RETURN(Engine::Set_ScreenSize(CGraphicDevice::ESCREEN_RESOULTION_16To9, 80.f), E_FAIL);

	// 16 : 9 ������ 1366:768 �ػ�
	FAILED_CHECK_RETURN(Engine::Set_ScreenSize(CGraphicDevice::ESCREEN_RESOULTION_16To9, 85.375f), E_FAIL);

	// 16 : 9 ������ 1600:900 �ػ�
	//FAILED_CHECK_RETURN(Engine::Set_ScreenSize(CGraphicDevice::ESCREEN_RESOULTION_16To9, 100.f), E_FAIL);

	// 4 : 3 ������ 800:600 �ػ�
	//FAILED_CHECK_RETURN(Engine::Set_ScreenSize(CGraphicDevice::ESCREEN_RESOULTION_4To3, 200.f), E_FAIL);


    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	MSG msg;
	msg.message = WM_NULL;

	// ���ΰ��� ����

	CMainApp*		pMainApp = CMainApp::Create();
	NULL_CHECK_RETURN(pMainApp, FALSE);

	// Ÿ�̸� ����
	FAILED_CHECK_RETURN(Ready_Timer(L"Timer_Immediate"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Timer(L"Timer_FPS60"), E_FAIL);

	// FPS ����
	FAILED_CHECK_RETURN(Ready_Frame(L"Frame_FPS60", 60), E_FAIL);


    // �⺻ �޽��� �����Դϴ�.
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

						//���̺귯�� ����ؾ� ��
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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