#include "Clients.h"
#include "../Globals.h"

void Clients::Setup()
{
	gpGlobals->GUIManager->AddHook("GUI_ClientList", []()
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

}

void Clients::Destroy()
{

}
