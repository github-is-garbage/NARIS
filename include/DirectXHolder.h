#pragma once

#include <d3d9.h>

class DirectXHolder
{
public:
	DirectXHolder();

	bool Setup(HWND hWindow);
	void Destroy();

	LPDIRECT3D9 D3D9;
	LPDIRECT3DDEVICE9 D3DDevice;
	D3DPRESENT_PARAMETERS PresentParams;
};
