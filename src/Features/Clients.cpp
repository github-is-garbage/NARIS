#include "Clients.h"
#include "../Globals.h"

#include <stdio.h>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

void Clients::CollectClientInformation(SOCKET pSocketServer, SOCKET pSocketClient, ClientSocketInfo_t& SocketInfo)
{
	strcpy(SocketInfo.szIdentifier, "Unknown");
	strcpy(SocketInfo.szAddr, "Unknown");
	SocketInfo.iPort = 0;
	SocketInfo.flPing = 0.f;
	SocketInfo.flConnectionTime = 0.f;
	SocketInfo.pszOS = "Unknown";
	SocketInfo.pszInstallDate = "Unknown";
	SocketInfo.pszVersion = VERSION;

	// IP Address
	getpeername(pSocketClient, (sockaddr*)&SocketInfo.ConnectionInfo.ClientAddr, &SocketInfo.ConnectionInfo.ClientAddrSize);
	inet_ntop(AF_INET, &SocketInfo.ConnectionInfo.ClientAddr.sin_addr, SocketInfo.szAddr, INET_ADDRSTRLEN);

	// Port
	// SocketInfo.iPort = ntohs(SocketInfo.ConnectionInfo.ClientAddr.sin_port); // This is wrong because ???, So show the port of the socket they connected to instead

	sockaddr_in ServerAddr;
	int ServerAddrSize = sizeof(ServerAddr);

	getsockname(pSocketServer, (sockaddr*)&ServerAddr, &ServerAddrSize);
	SocketInfo.iPort = ntohs(ServerAddr.sin_port);

	// TODO: The rest
}

void ClientHandler(Clients* pClientsFeature, SOCKET pSocketServer, SOCKET pSocketClient, ClientSocketInfo_t SocketInfo)
{
	pClientsFeature->CollectClientInformation(pSocketServer, pSocketClient, SocketInfo);
	pClientsFeature->vecClients.emplace_back(SocketInfo);

	closesocket(pSocketClient);
}

void ListenHandler(SOCKET pSocket)
{
	Clients* pClientsFeature = gpGlobals->FeatureManager->FindFeature<Clients>();

	while (true)
	{
		sockaddr_in ClientAddr;
		int ClientAddrSize = sizeof(ClientAddr);

		SOCKET pSocketClient = accept(pSocket, (sockaddr*)&ClientAddr, &ClientAddrSize);

		if (pSocketClient == INVALID_SOCKET)
		{
			int iError = WSAGetLastError();

			if (iError == WSAEINTR || iError == WSAENOTSOCK)
			{
				MessageBoxW(NULL, L"This should never happen #1", L"", MB_ICONERROR | MB_OK);
				break;
			}

			continue;
		}

		ClientConnectionInfo_t ConnectionInfo;
		ConnectionInfo.ClientAddr = ClientAddr;
		ConnectionInfo.ClientAddrSize = ClientAddrSize;

		ClientSocketInfo_t SocketInfo;
		SocketInfo.ConnectionInfo = ConnectionInfo;

		std::thread(ClientHandler, pClientsFeature, pSocket, pSocketClient, SocketInfo).detach();
	}
}

bool Clients::SetupSocket()
{
	WSADATA wsaData;
	HRESULT Result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (FAILED(Result))
	{
		MessageBoxW(NULL, L"Failed to initialize socket!", L"", MB_ICONERROR | MB_OK);
		return false;
	}

	addrinfo* pInfo = NULL;
	addrinfo Hints;

	ZeroMemory(&Hints, sizeof(Hints));
	Hints.ai_family = AF_INET;
	Hints.ai_socktype = SOCK_STREAM;
	Hints.ai_protocol = IPPROTO_TCP;
	Hints.ai_flags = AI_PASSIVE;

	Result = getaddrinfo(NULL, "27015", &Hints, &pInfo);

	if (FAILED(Result))
	{
		WSACleanup();

		MessageBoxW(NULL, L"Failed to get address info!", L"", MB_ICONERROR | MB_OK);
		return false;
	}

	SOCKET pSocket = socket(pInfo->ai_family, pInfo->ai_socktype, pInfo->ai_protocol);

	if (pSocket == INVALID_SOCKET)
	{
		freeaddrinfo(pInfo);
		WSACleanup();

		MessageBoxW(NULL, L"Failed to initialize socket!", L"", MB_ICONERROR | MB_OK);
		return false;
	}

	Result = bind(pSocket, pInfo->ai_addr, (int)pInfo->ai_addrlen);

	freeaddrinfo(pInfo);

	if (FAILED(Result))
	{
		closesocket(pSocket);
		WSACleanup();

		MessageBoxW(NULL, L"Failed to bind socket!", L"", MB_ICONERROR | MB_OK);
		return false;
	}

	Result = listen(pSocket, SOMAXCONN);

	if (FAILED(Result))
	{
		closesocket(pSocket);
		WSACleanup();

		MessageBoxW(NULL, L"Failed to listen on socket!", L"", MB_ICONERROR | MB_OK);
		return false;
	}

	std::thread(ListenHandler, pSocket).detach();

	return true;
}

void Clients::Setup()
{
	this->vecClients = std::vector<ClientSocketInfo_t>();

	if (!this->SetupSocket())
		return exit(-1);

	gpGlobals->GUIManager->AddHook("GUI_ClientList", []()
	{
		std::vector<ClientSocketInfo_t>& vecClients = gpGlobals->FeatureManager->FindFeature<Clients>()->vecClients;

		for (int i = 0; i < vecClients.size(); ++i)
		{
			ClientSocketInfo_t SocketInfo = vecClients[i];

			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::Text(SocketInfo.szIdentifier);

			ImGui::TableNextColumn();
			ImGui::Text(SocketInfo.szAddr);

			ImGui::TableNextColumn();
			ImGui::Text("%d", SocketInfo.iPort);

			ImGui::TableNextColumn();
			ImGui::Text("%.0f ms", SocketInfo.flPing * 1000.f);

			ImGui::TableNextColumn();
			ImGui::Text("%.2f", SocketInfo.flConnectionTime);

			ImGui::TableNextColumn();
			ImGui::Text(SocketInfo.pszOS);

			ImGui::TableNextColumn();
			ImGui::Text(SocketInfo.pszInstallDate);

			ImGui::TableNextColumn();
			ImGui::Text(SocketInfo.pszVersion);

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
