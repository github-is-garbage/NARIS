#pragma once

#include "../Features.h"

#include <lmcons.h>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

struct ClientConnectionInfo_t
{
	sockaddr_in ClientAddr;
	int ClientAddrSize;
};

struct ClientSocketInfo_t
{
	char szIdentifier[UNLEN + CNLEN + 1]; // CN\UN
	char szAddr[INET_ADDRSTRLEN];
	int iPort;
	float flPing;
	float flConnectionTime;
	const char* pszOS;
	const char* pszInstallDate;
	const char* pszVersion;
	ClientConnectionInfo_t ConnectionInfo;
};

class Clients : public BaseFeature
{
public:
	std::vector<ClientSocketInfo_t> vecClients;

public:
	void CollectClientInformation(SOCKET pSocketClient, ClientSocketInfo_t& SocketInfo);
	bool SetupSocket();

	void Setup();
	void Destroy();
};
