#pragma once

#include "../Features.h"

#include <zmq.hpp>

class Clients : public BaseFeature
{
public:
	zmq::context_t* pSocketContext;
	zmq::socket_t* pSocket;

public:
	bool SetupSocket();

	void Setup();
	void Destroy();
};
