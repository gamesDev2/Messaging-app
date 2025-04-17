#pragma once
#include <WinSock2.h>
#include <memory>

#include "ClientSocket.h"
#include "client.h"

struct ServerSocket
{
	ServerSocket(int _port);
	~ServerSocket();

	std::shared_ptr<client> accept(); 

private:
	SOCKET m_socket;

	ServerSocket(const ServerSocket& _copy);
	ServerSocket& operator=(const ServerSocket& _assign);
};