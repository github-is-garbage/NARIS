#pragma once

#include "DirectXHolder.h"

#include <functional>
#include <ImGui/imgui.h>
#include <map>
#include <string>
#include <vector>
#include <Windows.h>

struct WindowSetup_t
{
	ImVec2 Size;
	ImVec2 Position;
};

class GUI
{
private:
	bool bOpen = true;

	std::map<std::string, std::vector<std::function<void()>>> mapHooks;

public:
	DirectXHolder* D3DHolder;

	ATOM bRegistered;
	HINSTANCE hInstance;
	HWND hWindow;

	WindowSetup_t ScreenSetup;
	WindowSetup_t WindowSetup;

public:
	GUI();

	bool Setup();
	bool Destroy();
	void Loop();
	void Style();
	void Render();
	void AddHook(std::string strEvent, std::function<void()> fnCallback);

private:
	void RunHook(std::string strEvent);

	static LRESULT CALLBACK WndProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
