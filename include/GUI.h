#pragma once

#include <Windows.h>

class GUI
{
public:
	ATOM Registered;
	HINSTANCE hInstance;
	HWND hWindow;

public:
	GUI();

	bool Setup();
	bool Destroy();
	void Loop();

	static LRESULT CALLBACK WndProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
