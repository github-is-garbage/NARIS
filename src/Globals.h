#pragma once

#include "DirectXHolder.h"
#include "GUI.h"
#include "Features.h"

class Globals
{
public:
	GUI* GUIManager;
	DirectXHolder* D3DManager;
	Features* FeatureManager;

public:
	bool Setup();
	void Destroy();
};

inline Globals* gpGlobals;
