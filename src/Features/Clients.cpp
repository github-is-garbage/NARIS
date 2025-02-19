#include "Clients.h"
#include "../Globals.h"

#include <chrono>
#include <iostream>
#include <thread>

void ListenHandler(zmq::socket_t* pSocket, zmq::context_t* pSocketContext)
{
	// TODO: This is not final at all (it's stolen from the internet) do not do this it's just test code
	std::vector<zmq::pollitem_t> Items = {
		{ *pSocket, 0, ZMQ_POLLIN, 0 }
	};

	zmq_socket_monitor(pSocket->handle(), "inproc://monitor", ZMQ_EVENT_CONNECTED | ZMQ_EVENT_CONNECT_DELAYED);

	zmq::socket_t monitor(*pSocketContext, zmq::socket_type::pair);
	monitor.connect("inproc://monitor");

	std::vector<zmq::pollitem_t> items = {
		{ *pSocket, 0, ZMQ_POLLIN, 0 },
		{ monitor, 0, ZMQ_POLLIN, 0 }
	};

	while (true)
	{
		zmq::poll(items, std::chrono::milliseconds(100));

		if (items[0].revents & ZMQ_POLLIN)
		{
			zmq::message_t request;
			zmq::recv_result_t _ = pSocket->recv(request, zmq::recv_flags::none);

			std::cout << "Received: " << request.to_string() << std::endl;
		}

		if (items[1].revents & ZMQ_POLLIN)
		{
			zmq::message_t eventMsg;
			zmq::message_t addrMsg;
			if (monitor.recv(eventMsg, zmq::recv_flags::none) && monitor.recv(addrMsg, zmq::recv_flags::none))
			{
				uint16_t event = *static_cast<uint16_t*>(eventMsg.data());
				std::string endpoint = addrMsg.to_string();

				if (event == ZMQ_EVENT_CONNECTED)
					std::cout << "Client connected: " << endpoint << std::endl;
				else if (event == ZMQ_EVENT_CONNECT_DELAYED)
					std::cout << "Connection delayed: " << endpoint << std::endl;
				else
					std::cout << "Monitor event " << event << " from " << endpoint << std::endl;
			}
		}
	}
}

bool Clients::SetupSocket()
{
	this->pSocketContext = new zmq::context_t();

	this->pSocket = new zmq::socket_t(*this->pSocketContext, zmq::socket_type::rep);
	this->pSocket->bind("tcp://*:5555");

	std::thread(ListenHandler, this->pSocket, this->pSocketContext).detach();

	return true;
}

void Clients::Setup()
{
	if (!this->SetupSocket())
		return exit(-1);

	// gpGlobals->GUIManager->AddHook("GUI_ClientList", []()
	// {
	// 	std::vector<ClientSocketInfo_t>& vecClients = gpGlobals->FeatureManager->FindFeature<Clients>()->vecClients;

	// 	for (int i = 0; i < vecClients.size(); ++i)
	// 	{
	// 		ClientSocketInfo_t SocketInfo = vecClients[i];

	// 		ImGui::TableNextRow();

	// 		ImGui::TableNextColumn();
	// 		ImGui::Text(SocketInfo.szIdentifier);

	// 		ImGui::TableNextColumn();
	// 		ImGui::Text(SocketInfo.szAddr);

	// 		ImGui::TableNextColumn();
	// 		ImGui::Text("%d", SocketInfo.iPort);

	// 		ImGui::TableNextColumn();
	// 		ImGui::Text("%.0f ms", SocketInfo.flPing * 1000.f);

	// 		ImGui::TableNextColumn();
	// 		ImGui::Text("%.2f", SocketInfo.flConnectionTime);

	// 		ImGui::TableNextColumn();
	// 		ImGui::Text(SocketInfo.pszOS);

	// 		ImGui::TableNextColumn();
	// 		ImGui::Text(SocketInfo.pszInstallDate);

	// 		ImGui::TableNextColumn();
	// 		ImGui::Text(SocketInfo.pszVersion);

	// 		if (ImGui::TableGetHoveredRow() - 1 == i)
	// 			ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, ImGui::GetColorU32(ImGuiCol_HeaderHovered));
	// 		else if (i % 2)
	// 			ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, ImGui::GetColorU32(ImGuiCol_TableRowBgAlt));
	// 	}
	// });
}

void Clients::Destroy()
{

}
