#include <GUI.h>

const char CLASS_NAME[] = "NARIS_SERVER";

GUI::GUI()
{
	this->hInstance = GetModuleHandle(NULL);
}

bool GUI::Setup()
{
	WNDCLASSEX WindowClass = {};
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.lpfnWndProc = GUI::WndProc;
	WindowClass.hInstance = this->hInstance;
	WindowClass.lpszClassName = CLASS_NAME;

	this->Registered = RegisterClassEx(&WindowClass);

	if (!this->Registered)
		return this->Destroy();

	this->hWindow = CreateWindowEx(
		0,
		CLASS_NAME,
		"NARIS",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		this->hInstance,
		NULL
	);

	if (!this->hWindow)
		return this->Destroy();

	return true;
}

bool GUI::Destroy()
{
	if (this->hWindow)
		DestroyWindow(this->hWindow);

	if (this->Registered)
		UnregisterClass(CLASS_NAME, this->hInstance);

	return false;
}

void GUI::Loop()
{
	MSG Msg = {};
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

LRESULT CALLBACK GUI::WndProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
