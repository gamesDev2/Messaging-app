#include "ClientSocket.h"

// Server Side Client socket constructor. serverSocket.h handles the construction for this upon the acceptance of a connection
ClientSocket::ClientSocket()
	: m_socket(INVALID_SOCKET),
	m_closed(false)
{

}

// Client side Client socket constructor. Used by the Client to connect and act as an interface to the server, therefore it needs it's own constructor
ClientSocket::ClientSocket(const std::string& _server, int _port)
	: m_socket(INVALID_SOCKET),
	m_closed(false)
{
	addrinfo hints = { 0 };
	addrinfo* result = NULL;
	addrinfo* ptr = NULL;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(_server.c_str(), std::to_string(_port).c_str(), &hints, &result) != 0) // Get address info from port while conforming to the hints
	{
		throw std::runtime_error("Failed to resolve server address or port");
	}

	ptr = result;
	while (ptr != NULL) // Even with the address there are multiple possible 
	{
		m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol); // Create socket from resulting addrinfo

		if (m_socket == INVALID_SOCKET) // Checking that the socket was created properly
		{
			freeaddrinfo(result);
			throw std::runtime_error("Failed to create socket");
		}

		if (connect(m_socket, ptr->ai_addr, ptr->ai_addrlen) == SOCKET_ERROR)
		{
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}

		ptr = ptr->ai_next;

		if (m_socket != INVALID_SOCKET)
		{
			ptr = NULL;
		}
	}

	if ((m_socket == INVALID_SOCKET))
	{
		throw std::runtime_error("Failed to connect to server");
	}

	freeaddrinfo(result); // addrinfo is no longer required so we release it before any potential error can cause a memory leak TODO: (Should be put into a self terminating struct)

	u_long mode = 1;
	if (ioctlsocket(m_socket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to set non-blocking");
	}
}

ClientSocket::~ClientSocket()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
	}
}

bool ClientSocket::receive(std::vector<unsigned char>& _message) 
{
	char buffer[1024] = { 0 };
	int bytes = recv(m_socket, buffer, sizeof(buffer) - 1, 0);

	if (bytes == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			m_closed = true;

			return false;
		}
		return false;
	}
	else if(bytes == 0)
	{
		m_closed = true;

		return false;
	}


	// Put the received buffer into a vector making sure to only include bytes received instead of the whole buffer(which may include whitespace zeroes)
	_message.resize(bytes);

	for (int i = 0; i < bytes; ++i) 
	{
		_message.at(i) = (buffer[i]);
	}
	return true;
}

bool ClientSocket::send(std::vector<unsigned char> _msg, int& _bytes)
{
	int bytes = ::send(m_socket, reinterpret_cast<char*>(_msg.data()), min(_msg.size(), 1024 * 1024), 0); // Send a maximum of 1KB at a time(This is slightly unnecessary for this program since we never send message large enough that we need to limit the size)
	_bytes = bytes;
	if (bytes <= 0)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}

		return false;
	}
	return true;
}

bool ClientSocket::closed()
{
	return m_closed;
}

void ClientSocket::close()
{
	m_closed = true;
}
