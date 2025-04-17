#include "ServerSocket.h"

#include <WS2tcpip.h>

#include <stdexcept>
#include <string>

ServerSocket::ServerSocket(int _port)
	: m_socket(INVALID_SOCKET)
{
	addrinfo hints = { 0 };
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	addrinfo* result = NULL;

	if (getaddrinfo(NULL, std::to_string(_port).c_str(), &hints, &result) != 0) // Get address info from port while conforming to the hints
	{
		throw std::runtime_error("Failed to resolve server address or port");
	}

	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // Create socket from resulting addrinfo

	if (m_socket == INVALID_SOCKET) // Checking that the socket was created properly
	{
		freeaddrinfo(result);
		throw std::runtime_error("Failed to create socket");
	}

	if (bind(m_socket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR) // bind? the socket. Whatever that means. Presumably to the port
	{
		freeaddrinfo(result);
		throw std::runtime_error("Failed to bind socket");
	}

	freeaddrinfo(result); // addrinfo is no longer required so we release it before any potential error can cause a memory leak

	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to listen on socket");
	}

	u_long mode = 1;
	if (ioctlsocket(m_socket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to set non-blocking");
	}
}

ServerSocket::~ServerSocket()
{
	closesocket(m_socket);
}

std::shared_ptr<client> ServerSocket::accept()
{
	SOCKET socket = ::accept(m_socket, NULL, NULL);

	if (socket == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			throw std::runtime_error("Failed to accept socket");
		}
		return std::shared_ptr<client>(); // Same as returning NULL
	}

	std::shared_ptr<client> rtn = std::make_shared<client>();
	rtn->m_socket = socket;

	return rtn;
}
