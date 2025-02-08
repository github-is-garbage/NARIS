#pragma once

#include <Windows.h>
#include "DirectXHolder.h"
#include <ImGui/imgui.h>

struct WindowSetup_t
{
	ImVec2 Size;
	ImVec2 Position;
};

class GUI
{
private:
	bool Open = true;

public:
	DirectXHolder* D3DHolder;

	ATOM Registered;
	HINSTANCE hInstance;
	HWND hWindow;

	WindowSetup_t ScreenSetup;
	WindowSetup_t WindowSetup;

public:
	GUI();

	bool Setup();
	bool Destroy();
	void Loop();
	void Render();

	static LRESULT CALLBACK WndProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
