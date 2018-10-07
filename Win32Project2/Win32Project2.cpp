// Win32Project2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32Project2.h"

#define MAX_LOADSTRING 100

static COLORREF greenColor = RGB(0, 255, 0);//some other changes
static COLORREF blackColor = RGB(0, 0, 0);
HBRUSH blackBG = CreateSolidBrush(blackColor);

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//////////////////////paint functions/////////////////

///////////optimal line///////////////
void line(int x0, int y0, int x1, int y1, HDC hdc, COLORREF color) 
{
	bool steep = false;
	if ((x0 - x1) < (y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = (dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			SetPixel(hdc, y, x, color);
		}
		else {
			SetPixel(hdc, x, y, color);
		}
		error2 += derror2;

		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}
/////////////////////////////////////

///////////////triangle//////////////
void triangle(std::vector<int>& v0, std::vector<int>& v1, std::vector<int>& v2, HDC hdc, COLORREF color)
{
	/*
	if (v0.at(0) > v1.at(1)) std::swap(v0, v1);
	if (v0.at(0) > v2.at(1)) std::swap(v0, v2);
	if (v1.at(0) > v2.at(1)) std::swap(v1, v2);
	*/
	int xa = v0.at(0);
	int ya = v0.at(1);
	int xb = v1.at(0);
	int yb = v1.at(1);
	int xc = v2.at(0);
	int yc = v2.at(1);

	line(v0.at(0), v0.at(1), v1.at(0), v1.at(1), hdc, color);
	line(v1.at(0), v1.at(1), v2.at(0), v2.at(1), hdc, color);
	line(v2.at(0), v2.at(1), v0.at(0), v0.at(1), hdc, color);
}
/////////////////////////////////////

/////////////////////////////////////////////////////////

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
    LoadStringW(hInstance, IDC_WIN32PROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT2));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = blackBG;                 //(HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT2);
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
//  PURPOSE:  Processes messages for the main window.
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
            // TODO: Add any drawing code that uses hdc here...

			//1st test line
			for (int i = 0; i < 100; i++) {
				SetPixel(hdc, 50+i, 50+i, greenColor);
			}


			//optimal circle
			/*
			int x0 = 400;
			int y0 = 300;
			int radius = 200;
			int x = radius - 1;
			int y = 0;
			int dx = 1;
			int dy = 1;
			int err = dx - (radius << 1);

			while (x >= y)
			{
				SetPixel(hdc, x0 + x, y0 + y, greenColor);
				SetPixel(hdc, x0 + y, y0 + x, greenColor);
				SetPixel(hdc, x0 - y, y0 + x, greenColor);
				SetPixel(hdc, x0 - x, y0 + y, greenColor);
				SetPixel(hdc, x0 - x, y0 - y, greenColor);
				SetPixel(hdc, x0 - y, y0 - x, greenColor);
				SetPixel(hdc, x0 + y, y0 - x, greenColor);
				SetPixel(hdc, x0 + x, y0 - y, greenColor);

				if (err <= 0)
				{
					y++;
					err += dy;
					dy += 2;
				}

				if (err > 0)
				{
					x--;
					dx += 2;
					err += dx - (radius << 1);
				}
			}
			*/

			//bad line
			int lx0 = 80;
			int ly0 = 50;
			int lx1 = 180;
			int ly1 = 150;
			for (float t = 0.; t<1.; t += .01) {
				int lx = lx0*(1. - t) + lx1*t;
				int ly = ly0*(1. - t) + ly1*t;
				SetPixel(hdc, lx, ly, greenColor);
			}

			//optimal line
			line(410, 50, 510, 150, hdc, greenColor);

			//triangle test
			std::vector<int> v0 = { 200, 200 };
			std::vector<int> v1 = { 270, 220 };
			std::vector<int> v2 = { 220, 300 };

			triangle(v0, v1, v2, hdc, greenColor);


			//
            EndPaint(hWnd, &ps);
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
