// Traffic Simulator.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Traffic Simulator.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
TrafficLight trafficLight(0, 1, 50, 50, 50, 100, 50, 150);
TrafficLight trafficLight2(2, 2, 200, 360, 250, 360, 300, 360);
RECT rect = { 50, 50, 100, 200 };
COLORREF trafficLightColors[4][3] = {
    {RGB(255, 0, 0), RGB(50, 50, 50), RGB(50, 50, 50)},    // red
    {RGB(255, 0, 0), RGB(255, 255, 0), RGB(50, 50, 50)},  // red + yellow
    {RGB(50, 50, 50), RGB(50, 50, 50), RGB(0, 255, 0)},   // green
	{RGB(50, 50, 50), RGB(255, 255, 0), RGB(50, 50, 50)}  // yellow
};
std::vector<Car> cars;
#define TIMER_CARS 3
#define TIMER_AUTO 4
#define CAR_SIZE 30
#define CAR_SPEED 4
#define CAR_GAP 15

// probability
int pw = 50; 
int pn = 50; 


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Settings(HWND, UINT, WPARAM, LPARAM);

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
    case WM_CREATE:
        {
        SetTimer(hWnd, trafficLight.m_id, 3000, NULL);
        SetTimer(hWnd, trafficLight2.m_id, 3000, NULL);
        SetTimer(hWnd, TIMER_CARS, 30, NULL);
        SetTimer(hWnd, TIMER_AUTO, 1000, NULL);
        break;
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_SETTINGS:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTINGS), hWnd, Settings);
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

            RECT windowRc;
            GetClientRect(hWnd, &windowRc);

			// Double buffer to fix flickering
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, windowRc.right, windowRc.bottom);
            HGDIOBJ oldBitmap = SelectObject(memDC, memBitmap);
            FillRect(memDC, &windowRc, (HBRUSH)(COLOR_WINDOW + 1));

            // Remove borders from shapes
            HPEN pen = CreatePen(PS_NULL, 0, 0);
            HGDIOBJ oldPen = SelectObject(memDC, pen);

            // Draw roads
            HBRUSH roadBrush = CreateSolidBrush(RGB(80, 80, 80));
            HGDIOBJ oldBrush = SelectObject(memDC, roadBrush);
            Rectangle(memDC, 350, windowRc.top, 450, windowRc.bottom);
            Rectangle(memDC, windowRc.left, 250, windowRc.right, 350);
            SelectObject(memDC, oldBrush);
            DeleteObject(roadBrush);

            // Traffic light boxes 
            HBRUSH blackBrush = CreateSolidBrush(RGB(35, 35, 35));
            oldBrush = SelectObject(memDC, blackBrush);
            RoundRect(memDC, 295, 95, 345, 245, 10, 10);
            RoundRect(memDC, 195, 355, 345, 405, 10, 10);
            SelectObject(memDC, oldBrush);
            DeleteObject(blackBrush);

            // Grey circles
            HBRUSH greyBrush = CreateSolidBrush(RGB(125, 125, 125));
            oldBrush = SelectObject(memDC, greyBrush);
            Ellipse(memDC, 300, 100, 340, 140);
            Ellipse(memDC, 300, 150, 340, 190);
            Ellipse(memDC, 300, 200, 340, 240);
            Ellipse(memDC, 200, 360, 240, 400);
            Ellipse(memDC, 250, 360, 290, 400);
            Ellipse(memDC, 300, 360, 340, 400);
            SelectObject(memDC, oldBrush);
            DeleteObject(greyBrush);

            // Traffic light 1 
            int lightPositions1[4] = { 300, 100, 340, 140 };
            for (auto& color : trafficLightColors[trafficLight.m_index]) {
                HBRUSH hBrush = CreateSolidBrush(color);
                oldBrush = SelectObject(memDC, hBrush);
                Ellipse(memDC, lightPositions1[0], lightPositions1[1], lightPositions1[2], lightPositions1[3]);
                lightPositions1[1] += 50;
                lightPositions1[3] += 50;
                SelectObject(memDC, oldBrush);
                DeleteObject(hBrush);
            }

            // Traffic light 2 
            int lightPositions2[4] = { 200, 360, 240, 400 };
            for (auto& color : trafficLightColors[trafficLight2.m_index]) {
                HBRUSH hBrush = CreateSolidBrush(color);
                oldBrush = SelectObject(memDC, hBrush);
                Ellipse(memDC, lightPositions2[0], lightPositions2[1], lightPositions2[2], lightPositions2[3]);
                lightPositions2[0] += 50;
                lightPositions2[2] += 50;
                SelectObject(memDC, oldBrush);
                DeleteObject(hBrush);
            }

            // Draw cars
            for (auto& car : cars) {
                HBRUSH carBrush = CreateSolidBrush(car.color);
                HGDIOBJ oldBRush = SelectObject(memDC, carBrush);

                if (car.dir == SOUTH) {
                    Rectangle(memDC, car.x, car.y, car.x + CAR_SIZE, car.y + CAR_SIZE + 10);
                }
                else {
                    Rectangle(memDC, car.x, car.y, car.x + CAR_SIZE + 10, car.y + CAR_SIZE);
                }

                SelectObject(memDC, oldBRush);
                DeleteObject(carBrush);
            }

            SelectObject(memDC, oldPen);
            DeleteObject(pen);

            // Copy buffer to screen
            BitBlt(hdc, 0, 0, windowRc.right, windowRc.bottom, memDC, 0, 0, SRCCOPY);

            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER:
        {
            if (wParam == TIMER_CARS) {
                for (size_t i = 0; i < cars.size(); i++) {
                    bool canDrive = true;

                    // Check for traffic light
                    if (cars[i].dir == SOUTH) {
                        if (trafficLight.m_index != 2 && cars[i].y + CAR_SIZE >= 220 && cars[i].y < 220) {
                            canDrive = false;
                        }
                    }
                    else {
                        if (trafficLight2.m_index != 2 && cars[i].x + CAR_SIZE >= 320 && cars[i].x < 320) {
                            canDrive = false;
                        }
                    }

                    // Checking for cars in front
                    for (size_t j = 0; j < cars.size(); j++) {
                        if (i == j) continue;
                        if (cars[i].dir == cars[j].dir) {
                            int dist = (cars[i].dir == SOUTH) ? cars[j].y - cars[i].y : cars[j].x - cars[i].x;
                            if (dist > 0 && dist < CAR_SIZE + CAR_GAP) {
                                canDrive = false;
                            }
                        }
                    }

                    // Move car if possible
                    if (canDrive) {
                        if (cars[i].dir == SOUTH) cars[i].y += CAR_SPEED;
                        else cars[i].x += CAR_SPEED;
                    }
                }

                // Remove off-screen cars
                std::erase_if(cars, [](const Car& c) {
                    return c.x > 1500 || c.y > 1500;
                });
            }
            else if (wParam == TIMER_AUTO) {
                if (rand() % 100 < pw)
                    cars.push_back(Car(-CAR_SIZE, 285, EAST));
                if (rand() % 100 < pn)
                    cars.push_back(Car(385, -CAR_SIZE, SOUTH));
            }
            else if (wParam == trafficLight.m_id) {
                trafficLight.m_index = (trafficLight.m_index + 1) % 4;
                int nextInterval = (trafficLight.m_index % 2 == 1) ? 2000 : 4000;
                SetTimer(hWnd, trafficLight.m_id, nextInterval, NULL);
            }
            else if (wParam == trafficLight2.m_id) {
                trafficLight2.m_index = (trafficLight2.m_index + 1) % 4;
                int nextInterval = (trafficLight2.m_index % 2 == 1) ? 2000 : 4000;
                SetTimer(hWnd, trafficLight2.m_id, nextInterval, NULL);
            }

            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_LBUTTONDOWN:
        {
            cars.push_back(Car(-CAR_SIZE, 285, EAST));
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_RBUTTONDOWN:
        {
            cars.push_back(Car(385, -CAR_SIZE, SOUTH));
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
        //Arrow keys for probabilty
    case WM_KEYDOWN:
        {
            switch (wParam)
            {
            case VK_RIGHT:
                pw = min(100, pw + 10);
                break;
            case VK_LEFT:
                pw = max(0, pw - 10);
                break;
            case VK_UP:
                pn = min(100, pn + 10);
                break;
            case VK_DOWN:
                pn = max(0, pn - 10);
                break;
            }
        }
        break;
    case WM_ERASEBKGND:
        return 1;
    case WM_DESTROY:
        KillTimer(hWnd, trafficLight.m_id);
        KillTimer(hWnd, trafficLight2.m_id);
        KillTimer(hWnd, TIMER_CARS);
        KillTimer(hWnd, TIMER_AUTO);
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

// Message handler for settings dialog
INT_PTR CALLBACK Settings(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        SetDlgItemInt(hDlg, IDC_EDIT_PW, pw, FALSE);
        SetDlgItemInt(hDlg, IDC_EDIT_PN, pn, FALSE);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            pw = GetDlgItemInt(hDlg, IDC_EDIT_PW, NULL, FALSE);
            pn = GetDlgItemInt(hDlg, IDC_EDIT_PN, NULL, FALSE);
            EndDialog(hDlg, IDOK);
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, IDCANCEL);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
