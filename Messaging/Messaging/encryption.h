#pragma once
#include <vector>

class encryption
{
public:
	encryption();

	void encrypt_Symetric(std::vector<unsigned char>& _data);

private:
	std::vector<unsigned char> m_key;
};