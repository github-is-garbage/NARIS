#pragma once

#include "DirectXHolder.h"
#include "Features.h"
#include "GUI.h"
#include "Utility.h"

#include <stdlib.h>

class Globals
{
public:
	GUI* GUIManager;
	DirectXHolder* D3DManager;
	Features* FeatureManager;
	Utility* Util;

public:
	bool Setup();
	void Destroy();
	bool Delete();
};

inline Globals* gpGlobals;
