#include "encryption.h"

encryption::encryption()
{
	srand(100);

	for (unsigned int i = 0; i < 1024; i++)
	{
		m_key.push_back(rand() % 256);
	}
}

void encryption::encrypt_Symetric(std::vector<unsigned char>& _data)
{
	for (size_t i = 0; i < _data.size(); ++i)
	{
		_data.at(i) ^= m_key.at(i % m_key.size());
	}
}
