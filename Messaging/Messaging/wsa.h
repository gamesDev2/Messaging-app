#pragma once
#include <WinSock2.h>
#include <iostream>

struct Wsa
{
	Wsa();
	~Wsa();

private:
	WSADATA m_wsadata;

	Wsa(const Wsa& _copy);
	Wsa& operator=(const Wsa& _assign);
};