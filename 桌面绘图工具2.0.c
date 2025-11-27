//项目创建时间2025.11.26
//作者xsq1234,有兴趣可以看看我的GitHub哦。
#include<Windows.h>
LRESULT WINAPI WndPorc(HWND hwnd, UINT msgid, WPARAM wparam, LPARAM lparam) { return DefWindowProc(hwnd, msgid, wparam, lparam); }
HWND Window(LPCWSTR name, int w, int h, int x, int y, char alpha,int mode)
{
	HINSTANCE hinstance = GetModuleHandle(NULL);
	WNDCLASS wndclass = { 0 };
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hInstance = hinstance;
	wndclass.lpfnWndProc = WndPorc;
	wndclass.lpszClassName = name;
	RegisterClass(&wndclass);
	HWND hwnd = CreateWindowEx(WS_EX_LAYERED, name, name, WS_SYSMENU, x, y, w, h, NULL, NULL, hinstance, NULL);
	if(mode)SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
	else SetLayeredWindowAttributes(hwnd, 0, 0, ULW_COLORKEY);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	return hwnd;
}

void RunWindow()
{
	MSG msg = { 0 };
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

int Backdrop(HDC hdc, int x, int y, int w, int h, int color)
{
	RECT rect = { x, y, x + w, y + h };
	HBRUSH hBrush = CreateSolidBrush(color);
	FillRect(hdc, &rect, hBrush);
	DeleteObject(hBrush);
	return color;
}

int Line(HDC hdc, int x1, int y1, int x2, int y2, int width, int color)
{
	HPEN hpen = CreatePen(PS_SOLID, width, color);
	HPEN holdpen = (HPEN)SelectObject(hdc, hpen);
	MoveToEx(hdc, x2, y2, NULL);
	LineTo(hdc, x1, y1);
	SelectObject(hdc, holdpen);
	DeleteObject(hpen);
	return 0;
}

int KeyState(int Key)
{
	static int KEYSTATEbuffer[255];
	int state = GetAsyncKeyState(Key);
	if (!(state & 0x8000))KEYSTATEbuffer[Key] = 0;
	else if (KEYSTATEbuffer[Key] == 0) { KEYSTATEbuffer[Key] = 1; return 1; }
	return 0;
}

void main()
{
	int windowwidth = GetSystemMetrics(SM_CXSCREEN) + 16, windowheight = GetSystemMetrics(SM_CYSCREEN), linewidth = 5, mode = 0, color = 0x0000ff;
	HWND hwnd = Window(L"xsq1234桌面绘图工具2.0", windowwidth, windowheight, 0, 0, 1, 1), hwnd_1 = Window(L"桌面绘图工具2.0", windowwidth, windowheight, 0, 0, 0, 0);
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	HDC hdc = GetDC(hwnd_1);
	SetWindowLong(hwnd_1, GWL_STYLE, GetWindowLong(hwnd_1, GWL_STYLE) & ~WS_CAPTION);
	SetCursor(LoadCursor(NULL, IDC_CROSS));
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		if (GetForegroundWindow() == hwnd || GetForegroundWindow() == hwnd_1)																				//判断窗口焦点
		{
			static POINT mousecoord[2];
			GetCursorPos(&mousecoord[0]);
			SetForegroundWindow(hwnd), SetFocus(hwnd), SetForegroundWindow(hwnd_1), SetFocus(hwnd_1);														//调整窗口顺序
			KeyState(2) && ((mode = !mode), mousecoord[1] = mousecoord[0], 1);																				//更换模式
			GetAsyncKeyState(1)         && (Line(hdc, mousecoord[0].x, mousecoord[0].y, mousecoord[1].x, mousecoord[1].y, linewidth, color), mode = 0, 1);
			GetAsyncKeyState(VK_RETURN) && Backdrop(hdc, 0, 0, windowwidth, windowheight, 0);																//清屏
			KeyState(VK_UP)   && (linewidth = min(max(linewidth + 1, 1), 30));																				//调整画笔大小
			KeyState(VK_DOWN) && (linewidth = min(max(linewidth - 1, 1), 30));
			mode ? 1 : (mousecoord[1] = mousecoord[0], 1);
			Backdrop(hdc, 0, 0, 20, 20,  RGB(255, 0, 0)),     (KeyState(49) || (mousecoord[0].y < 20 && mousecoord[0].x > 0  && mousecoord[0].x < 20  && KeyState(1))) && (color = RGB(255, 0, 0), 1);
			Backdrop(hdc, 20, 0, 20, 20, RGB(0, 255, 0)),     (KeyState(50) || (mousecoord[0].y < 20 && mousecoord[0].x > 20 && mousecoord[0].x < 40  && KeyState(1))) && (color = RGB(0, 255, 0), 1);
			Backdrop(hdc, 40, 0, 20, 20, RGB(0, 0, 255)),     (KeyState(51) || (mousecoord[0].y < 20 && mousecoord[0].x > 40 && mousecoord[0].x < 60  && KeyState(1))) && (color = RGB(0, 0, 255), 1);
			Backdrop(hdc, 60, 0, 20, 20, RGB(255, 255, 255)), (KeyState(52) || (mousecoord[0].y < 20 && mousecoord[0].x > 60 && mousecoord[0].x < 80  && KeyState(1))) && (color = RGB(255, 255, 255), 1);
			Backdrop(hdc, 80, 0, 20, 20, RGB(1, 1, 1)),       (KeyState(48) || (mousecoord[0].y < 20 && mousecoord[0].x > 80 && mousecoord[0].x < 100 && KeyState(1))) && (color = RGB(0, 0, 0), 1);
			Backdrop(hdc, 0, 20, 20, 20,  RGB(100, 100, 100)), (mousecoord[0].y > 20 && mousecoord[0].y < 40 && mousecoord[0].x > 0  && mousecoord[0].x < 20 && KeyState(1)) && (linewidth = min(max(linewidth + 1, 1), 30));
			Backdrop(hdc, 20, 20, 20, 20, RGB(200, 200, 200)), (mousecoord[0].y > 20 && mousecoord[0].y < 40 && mousecoord[0].x > 20 && mousecoord[0].x < 40 && KeyState(1)) && (linewidth = min(max(linewidth - 1, 1), 30));
			Line(hdc, 15, 55, 15, 55, linewidth, color);
		}
		if (!IsWindow(hwnd_1))break;
		RunWindow();
		Sleep(1);
	}
}