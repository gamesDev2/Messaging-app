#include "client.h"


client::client()
	:ClientSocket(),
	xmlInt(),
	zInt(),
	encryption(),
	m_id(0)
{
}

client::client(const std::string& _server, int _port)
	:ClientSocket(_server, _port),
	xmlInt(),
	m_id(0)
{

}

void client::sendData()
{
	int bytes = 0;
	if (!m_outgoing.size())
	{
		
		return;
	}

	while (send(m_outgoing, bytes))
	{
		m_outgoing.erase(m_outgoing.begin(), m_outgoing.begin() + bytes);
	}
}

void client::pushData(const std::vector<unsigned char> _msg)
{
	// Compress, encrypt and Cobify data
	std::vector<unsigned char> compressed = myCompress(_msg);
	// Encrypt
	encrypt_Symetric(compressed);
	std::vector<unsigned char> cobsedMsg = cobify(preCob(compressed)); // Applies C.O.B.S to the message(after applying preCob)

	m_outgoing.insert(m_outgoing.end(), cobsedMsg.begin(), cobsedMsg.end());
}

void client::recvData()
{
	std::vector<unsigned char> incomingMsg;
	if (receive(incomingMsg)) // If a message has been recieved then it is pushed into the stream. The reason this isn't done during polling is because we don't know if we have a complete data packet yet, even if we successfully recieved data from the socket
	{	
		
		m_incoming.insert(m_incoming.end(), incomingMsg.begin(), incomingMsg.end()); // We don't decobify the message here as we still need usage of the overhead and delimiter byte from C.O.B.S
	}
}

bool client::pollData(std::vector<unsigned char>& _msg)
{
	for (size_t i = 0; i < m_incoming.size(); ++i)
	{
		if (m_incoming.at(i) == 0)
		{
			std::vector<unsigned char> msg(m_incoming.begin(), m_incoming.begin() + i + 1);
			m_incoming.erase(m_incoming.begin(), m_incoming.begin() + i + 1);

			msg = deCobify(msg); // This is where we finally decobify the message since we have seperated it out from the data stream

			msg = postCob(msg);

			// Decrypt
			encrypt_Symetric(msg);

			// Decompress and return data
			_msg = myDecompress(msg);
			

			return true;
		}
	}

	return false;
}

int& client::id()
{
	return m_id;
}

void client::close()
{
	ClientSocket::close();
}

// Used to handle data too large for cobs to handle on it's own (Still doesn't account for sending files larger than what can be fit in memory)
std::vector<unsigned char> client::preCob(std::vector<unsigned char> _msg)
{
	// I try to avoid using vector.insert() in this function since that's a O(n) complexity function, and the whole point of this function is to deal with the case that the message is very large.
	// So it could result in a non-trivial slow down.
	std::vector<unsigned char> newMsg(_msg.size() + (_msg.size() / 254) + 1); // As a result we allocate the entire array at the start. Also O(n) but we only have to do it once instead of everytime we want to insert a Null.

	size_t msgIdx = 0;
	for (size_t i = 0; msgIdx < _msg.size(); i++)
	{
		if (msgIdx % 254 != 0)
		{
			newMsg.at(i) = _msg.at(msgIdx);
		}
		else
		{
			newMsg.at(i) = 0;
			newMsg.at(++i) = _msg.at(msgIdx);

		}
		msgIdx++;
	}

	return newMsg;
}

std::vector<unsigned char> client::postCob(std::vector<unsigned char> _msg)
{
	std::vector<unsigned char> newMsg;

	for (size_t i = 0; i < _msg.size(); i++)
	{
		if (i  % 255 != 0)
		{
			newMsg.push_back(_msg.at(i));
		}
	}

	return newMsg;
}

std::vector<unsigned char> client::cobify(std::vector<unsigned char> _msg)
{
	std::vector<unsigned char> newMsg(_msg.size() + 2);

	size_t idx = 0;
	for (size_t i = 1; i <= _msg.size(); i++)
	{
		if (_msg.at(i - 1) == 0) // If byte is NULL set the previous NULL byte to point to it
		{
			newMsg.at(idx) = i - idx;
			idx = i;
		}
		else // Otherwise simply set the byte to itself
		{
			newMsg.at(i) = _msg.at(i - 1);
		}
	}


	// Add the Delimiter byte and have the last NULL byte point to it
	newMsg.at(idx) = newMsg.size() - (idx + 1); // +1 to the idx since size doesn't return the index to the final element in the vector but rather the total number of elements (a mistake I took a while to find (god damn off-by-one errors))
	newMsg.back() = 0;

	return newMsg;
}

std::vector<unsigned char> client::deCobify(std::vector<unsigned char> _message)
{
	size_t idx = 0;
	unsigned char cByte = _message.at(idx);
	while (cByte != 0)
	{
		idx += cByte;
		cByte = _message.at(idx);
		_message.at(idx) = 0;
	}

	if (_message.size() >= 2)
	{
		_message.erase(_message.begin());
		_message.pop_back();
	}

	return _message;
}
