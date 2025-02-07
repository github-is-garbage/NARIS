#include "GUI.h"
#include <uxtheme.h>
#include <dwmapi.h>
#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_dx9.h>
#include <ImGui/backends/imgui_impl_win32.h>

#include <iostream>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam); // Gay

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
		WS_EX_LAYERED,
		CLASS_NAME,
		"NARIS",
		WS_POPUP,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		this->hInstance,
		NULL
	);

	if (!this->hWindow)
		return this->Destroy();

	if (!ImGui::CreateContext())
		return this->Destroy();

	ImGuiIO &IO = ImGui::GetIO();
	IO.IniFilename = nullptr;

	// More window garbage
	SetLayeredWindowAttributes(this->hWindow, RGB(0, 0, 0), 0, LWA_COLORKEY);

	MARGINS Margins = { -1 };
	DwmExtendFrameIntoClientArea(this->hWindow, &Margins);

	UpdateWindow(this->hWindow);
	ShowWindow(this->hWindow, SW_SHOWDEFAULT);

	this->WindowSetup.Size.x = 640;
	this->WindowSetup.Size.y = 480;

	return true;
}

bool GUI::Destroy()
{
	if (ImGui::GetCurrentContext())
	{
		ImGuiIO& IO = ImGui::GetIO();

		if (IO.BackendPlatformUserData)
		{
			ImGui_ImplDX9_Shutdown();
			ImGui_ImplWin32_Shutdown();
		}

		ImGui::DestroyContext();
	}

	if (this->hWindow)
		DestroyWindow(this->hWindow);

	if (this->Registered)
		UnregisterClass(CLASS_NAME, this->hInstance);

	return false;
}

void GUI::Loop()
{
	LPDIRECT3DDEVICE9 D3DDevice = this->D3DHolder->D3DDevice;

	ImGui_ImplWin32_Init(this->hWindow);
	ImGui_ImplDX9_Init(D3DDevice);

	MSG Msg = {};
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);

			continue;
		}

		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX9_NewFrame();
		ImGui::NewFrame();
		{
			if (!this->WindowSetup.Position.x)
			{
				HWND hDesktop = GetDesktopWindow();

				if (hDesktop)
				{
					RECT DesktopRect;
					GetWindowRect(GetDesktopWindow(), &DesktopRect);

					this->ScreenSetup.Size.x = DesktopRect.right;
					this->ScreenSetup.Size.y = DesktopRect.bottom;

					this->WindowSetup.Position.x = (this->ScreenSetup.Size.x - this->WindowSetup.Size.x) / 2.f;
					this->WindowSetup.Position.y = (this->ScreenSetup.Size.y - this->WindowSetup.Size.y) / 2.f;
				}
			}

			this->Render();
		}
		ImGui::EndFrame();

		D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.f, 0);

		if (D3DDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

			D3DDevice->EndScene();
		}

		HRESULT Result = D3DDevice->Present(NULL, NULL, NULL, NULL);

		if (Result == D3DERR_DEVICELOST && D3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			this->D3DHolder->Reset();
	}
}

void GUI::Render()
{
	ImGui::SetNextWindowPos(this->WindowSetup.Position, ImGuiCond_Once);
	ImGui::SetNextWindowSize(this->WindowSetup.Size, ImGuiCond_Always);

	if (ImGui::Begin("GH", nullptr, ImGuiWindowFlags_NoResize))
	{
		if (ImGui::Button("Yo"))
			std::cout << "prop" << std::endl;

		ImGui::End();
	}
}

LRESULT CALLBACK GUI::WndProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWindow, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
		case WM_SIZE:
			return 0;

		case WM_SYSCOMMAND:
			if ((wParam & 0xFFF0) == SC_KEYMENU)
				return 0;

			break;

		case WM_QUIT:
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWindow, uMsg, wParam, lParam);
}
