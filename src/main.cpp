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

	GUIManager->AddHook("GUI_ClientList", []()
	{
		for (int i = 0; i < 10; ++i)
		{
			ImGui::TableNextRow();

			for (int ii = 0; ii < 8; ++ii)
			{
				ImGui::TableNextColumn();
				ImGui::Text("%d %d", i, ii);
			}

			if (ImGui::TableGetHoveredRow() - 1 == i)
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, ImGui::GetColorU32(ImGuiCol_HeaderHovered));
			else if (i % 2)
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, ImGui::GetColorU32(ImGuiCol_TableRowBgAlt));
		}
	});

	GUIManager->Loop();

	D3DHolder->Destroy();
	GUIManager->Destroy();

	return 0;
}
