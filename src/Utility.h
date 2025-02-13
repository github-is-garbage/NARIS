#pragma once

#include <ImGui/imgui.h>

#define DeletePtr(Ptr) 	\
if (Ptr)				\
{						\
	free(Ptr);			\
	Ptr = nullptr;		\
}

class Utility
{
public:
};
