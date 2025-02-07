#include <Windows.h>
#include <iostream>

LRESULT CALLBACK WndProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_QUIT:
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWindow, uMsg, wParam, lParam);
}

int main()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	const char CLASS_NAME[] = "NARIS_SERVER";

	WNDCLASSEX WindowClass = {};
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.lpfnWndProc = WndProc;
	WindowClass.hInstance = hInstance;
	WindowClass.lpszClassName = CLASS_NAME;

	if (!RegisterClassEx(&WindowClass))
		return -1;

	HWND hWindow = CreateWindowEx(
		0,
		CLASS_NAME,
		"NARIS",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWindow)
	{
		UnregisterClass(CLASS_NAME, hInstance);
		return -2;
	}

	ShowWindow(hWindow, SW_SHOWDEFAULT);

	MSG Msg = {};
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return 0;
}
