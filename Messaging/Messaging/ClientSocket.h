#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <string>

struct ServerSocket;

struct ClientSocket
{
	ClientSocket(); // Server side
	ClientSocket(const std::string& _server, int _port); // Client side
	~ClientSocket();

	bool receive(std::vector<unsigned char>& _message);
	bool send(std::vector<unsigned char> _msg, int& _bytes);

	bool closed();
	void close();

private:

	friend struct ServerSocket;

	SOCKET m_socket;
	bool m_closed;

	ClientSocket(const ClientSocket& _copy);
	ClientSocket& operator=(const ClientSocket& _assign);

};