#include <Windows.h>

#include "Globals.h"

int main()
{
	gpGlobals = new Globals();

	if (!gpGlobals->Setup())
		return -1;

	gpGlobals->GUIManager->Loop();

	gpGlobals->Destroy();

	return 0;
}
