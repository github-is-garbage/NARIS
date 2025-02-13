#include "GUI.h"

#include "Globals.h"
#include <uxtheme.h>
#include <dwmapi.h>
#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_dx9.h>
#include <ImGui/backends/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam); // Gay

GUI::GUI()
{
	this->hInstance = GetModuleHandle(NULL);
	this->mapHooks = std::map<std::string, std::vector<std::function<void()>>>();
}

bool GUI::Setup()
{
	WNDCLASSEX WindowClass = {};
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.lpfnWndProc = GUI::WndProc;
	WindowClass.hInstance = this->hInstance;
	WindowClass.lpszClassName = this->pwszWindowClass;

	this->bRegistered = RegisterClassEx(&WindowClass);

	if (!this->bRegistered)
		return this->Destroy();

	this->hWindow = CreateWindowEx(
		WS_EX_TRANSPARENT,
		this->pwszWindowClass,
		L"NARIS",
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

	if (this->bRegistered)
		UnregisterClass(this->pwszWindowClass, this->hInstance);

	return false;
}

void GUI::Loop()
{
	LPDIRECT3DDEVICE9& D3DDevice = gpGlobals->D3DManager->D3DDevice;

	ImGui_ImplWin32_Init(this->hWindow);
	ImGui_ImplDX9_Init(D3DDevice);

	this->Style();

	MSG Msg = {};
	HRESULT Result;

	while (true)
	{
		if (!this->bOpen)
			break;

		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			if (Msg.message == WM_QUIT || Msg.message == WM_DESTROY)
				break;

			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
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

		Result = D3DDevice->Present(NULL, NULL, NULL, NULL);

		if (Result == D3DERR_DEVICELOST && D3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			gpGlobals->D3DManager->Reset();
		else
			this->UpdateRegion();
	}
}

void GUI::Style()
{
	ImGuiStyle& Style = ImGui::GetStyle();

	Style.Colors[ImGuiCol_TableRowBgAlt] = ImColor(100, 100, 100, 50);

	Style.ChildRounding = 0.f;
	Style.FrameRounding = 0.f;
	Style.GrabRounding = 0.f;
	Style.PopupRounding = 0.f;
	Style.ScrollbarRounding = 0.f;
	Style.TabRounding = 0.f;
	Style.WindowRounding = 0.f;
}

void GUI::Render()
{
	ImGui::SetNextWindowPos(this->WindowSetup.Position, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(this->WindowSetup.Size, ImGuiCond_FirstUseEver);

	// Big blob incoming :hahaball:
	if (ImGui::Begin("NARIS v" VERSION, &this->bOpen, ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::BeginTabBar("MainTabs"))
		{
			if (ImGui::BeginTabItem("Clients"))
			{
				if (ImGui::BeginTable("ConnectedClients", 8, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable))
				{
					ImGui::TableSetupColumn("Identifier");
					ImGui::TableSetupColumn("IP Address");
					ImGui::TableSetupColumn("Port");
					ImGui::TableSetupColumn("Ping");
					ImGui::TableSetupColumn("Up Time");
					ImGui::TableSetupColumn("Operating System");
					ImGui::TableSetupColumn("Install Date");
					ImGui::TableSetupColumn("Version");

					ImGui::TableHeadersRow();

					this->RunHook("GUI_ClientList");

					ImGui::EndTable();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Builder"))
			{
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Network"))
			{
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Settings"))
			{
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		gpGlobals->Util->CopyImVec2(ImGui::GetWindowPos(), this->WindowSetup.Position);
		gpGlobals->Util->CopyImVec2(ImGui::GetWindowSize(), this->WindowSetup.Size);

		ImGui::End();
	}
}

void GUI::AddHook(std::string strEvent, std::function<void()> fnCallback)
{
	if (!this->mapHooks.contains(strEvent))
		this->mapHooks.emplace(strEvent, std::vector<std::function<void()>>());

	this->mapHooks.find(strEvent)->second.emplace_back(fnCallback);
}

void GUI::RunHook(std::string strEvent)
{
	if (!this->mapHooks.contains(strEvent))
		return;

	std::vector<std::function<void()>>& Hooks = this->mapHooks.find(strEvent)->second;

	for (std::function<void()>& Hook : Hooks)
		Hook();
}

void GUI::UpdateRegion()
{
	HRGN hRgnInteraction = CreateRectRgn(
		this->WindowSetup.Position.x,
		this->WindowSetup.Position.y,
		this->WindowSetup.Position.x + this->WindowSetup.Size.x,
		this->WindowSetup.Position.y + this->WindowSetup.Size.y
	);

	SetWindowRgn(this->hWindow, hRgnInteraction, TRUE);
	DeleteObject(hRgnInteraction);
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
			if (LOWORD(wParam) == SC_KEYMENU)
				return 0;

			break;

		case WM_QUIT:
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWindow, uMsg, wParam, lParam);
}
