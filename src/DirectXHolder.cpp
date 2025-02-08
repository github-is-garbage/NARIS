#include "DirectXHolder.h"

#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_dx9.h>

DirectXHolder::DirectXHolder()
{
	this->D3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	ZeroMemory(&this->PresentParams, sizeof(this->PresentParams));

	this->PresentParams.Windowed = true;
	this->PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	this->PresentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	this->PresentParams.EnableAutoDepthStencil = TRUE;
	this->PresentParams.AutoDepthStencilFormat = D3DFMT_D16;
	this->PresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

bool DirectXHolder::Setup(HWND hWindow)
{
	HRESULT Result = this->D3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &this->PresentParams, &this->D3DDevice);

	return SUCCEEDED(Result);
}

void DirectXHolder::Destroy()
{
	this->D3DDevice->Release();
	this->D3D9->Release();
}

void DirectXHolder::Reset()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT Result = this->D3DDevice->Reset(&this->PresentParams);

	assert(SUCCEEDED(Result));

	ImGui_ImplDX9_CreateDeviceObjects();
}
