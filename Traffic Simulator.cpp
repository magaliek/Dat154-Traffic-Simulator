// Traffic Simulator.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Traffic Simulator.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
TrafficLight trafficLight(0, 50, 50, 50, 100, 50, 150);
TrafficLight trafficLight2(2, 200, 360, 250, 360, 300, 360);
RECT rect = { 50, 50, 100, 200 };

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TRAFFICSIMULATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRAFFICSIMULATOR));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRAFFICSIMULATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TRAFFICSIMULATOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // Draw roads 
            HBRUSH roadBrush = CreateSolidBrush(RGB(80, 80, 80));
            HGDIOBJ oldRoadBrush = SelectObject(hdc, roadBrush);
            Rectangle(hdc, 350, 0, 450, 800);   // Vertical road 
            Rectangle(hdc, 0, 250, 800, 350);   // Horizontal road 
            SelectObject(hdc, oldRoadBrush);
            DeleteObject(roadBrush);

            // Traffic light 1 
            Rectangle(hdc, 295, 95, 345, 245);
            HBRUSH hBrush = CreateSolidBrush(trafficLight.trafficLightColors[trafficLight.m_index]);

            if (trafficLight.m_index == 0) {
                // Red
                Ellipse(hdc, 300, 150, 340, 190);
                Ellipse(hdc, 300, 195, 340, 235);
                HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
                Ellipse(hdc, 300, 100, 340, 140);
                SelectObject(hdc, oldBrush);
            }   
            else if (trafficLight.m_index == 1) {
                // Red + Yellow
                Ellipse(hdc, 300, 195, 340, 235);
                HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
                Ellipse(hdc, 300, 100, 340, 140);
                HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
                SelectObject(hdc, yellowBrush);
                Ellipse(hdc, 300, 150, 340, 190);
                SelectObject(hdc, oldBrush);
                DeleteObject(yellowBrush);
            }
            else if (trafficLight.m_index == 2) {
                // Green
                Ellipse(hdc, 300, 100, 340, 140);
                Ellipse(hdc, 300, 150, 340, 190);
                HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
                Ellipse(hdc, 300, 195, 340, 235);
                SelectObject(hdc, oldBrush);
            }
            else if (trafficLight.m_index == 3) {
                // Yellow
                Ellipse(hdc, 300, 100, 340, 140);
                Ellipse(hdc, 300, 195, 340, 235);
                HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
                Ellipse(hdc, 300, 150, 340, 190);
                SelectObject(hdc, oldBrush);
            }

            DeleteObject(hBrush);

            // Traffic light 2 
            Rectangle(hdc, 195, 355, 345, 405);
            HBRUSH hBrush2 = CreateSolidBrush(trafficLight2.trafficLightColors[trafficLight2.m_index]);

            if (trafficLight2.m_index == 0) {
                // Red
                Ellipse(hdc, 250, 360, 290, 400);
                Ellipse(hdc, 300, 360, 340, 400);
                HGDIOBJ oldBrush = SelectObject(hdc, hBrush2);
                Ellipse(hdc, 200, 360, 240, 400);
                SelectObject(hdc, oldBrush);
            }
            else if (trafficLight2.m_index == 1) {
                // Red + Yellow
                Ellipse(hdc, 300, 360, 340, 400);
                HGDIOBJ oldBrush = SelectObject(hdc, hBrush2);
                Ellipse(hdc, 200, 360, 240, 400);
                HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
                SelectObject(hdc, yellowBrush);
                Ellipse(hdc, 250, 360, 290, 400);
                SelectObject(hdc, oldBrush);
                DeleteObject(yellowBrush);
            }
            else if (trafficLight2.m_index == 2) {
                // Green
                Ellipse(hdc, 200, 360, 240, 400);
                Ellipse(hdc, 250, 360, 290, 400);
                HGDIOBJ oldBrush = SelectObject(hdc, hBrush2);
                Ellipse(hdc, 300, 360, 340, 400);
                SelectObject(hdc, oldBrush);
            }
            else if (trafficLight2.m_index == 3) {
                // Yellow
                Ellipse(hdc, 200, 360, 240, 400);
                Ellipse(hdc, 300, 360, 340, 400);
                HGDIOBJ oldBrush = SelectObject(hdc, hBrush2);
                Ellipse(hdc, 250, 360, 290, 400);
                SelectObject(hdc, oldBrush);
            }

            DeleteObject(hBrush2);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
        {
            trafficLight.m_index = (trafficLight.m_index + 1) % 4;
            trafficLight2.m_index = (trafficLight.m_index + 2) % 4;

            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
