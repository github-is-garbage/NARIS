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
	void CopyImVec2(const ImVec2& Source, ImVec2& Destination);
};
