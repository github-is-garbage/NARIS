#include <Windows.h>

#include "DirectXHolder.h"
#include "GUI.h"

int main()
{
	// FreeConsole(); //  TODO:

	GUI* GUIManager = new GUI();

	if (!GUIManager->Setup())
		return -1;

	DirectXHolder* D3DHolder = new DirectXHolder();

	if (!D3DHolder->Setup(GUIManager->hWindow))
	{
		GUIManager->Destroy();
		return -2;
	}
	else
		GUIManager->D3DHolder = D3DHolder;

	GUIManager->Loop();

	D3DHolder->Destroy();
	GUIManager->Destroy();

	return 0;
}
