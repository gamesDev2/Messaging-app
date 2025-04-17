#pragma once
#include "ClientSocket.h"
#include "xmlInterface.h"
#include "miniz.h"
#include "compression.h"
#include "encryption.h"
#include <vector>
#include <iostream>

class client : zInt, encryption, public xmlInt, public ClientSocket
{
public:
	client(); // Server side
	client(const std::string& _server, int _port); // Client side

	void sendData(); // Send as much of the outgoing stream as can be done in a single go
	void pushData(const std::vector<unsigned char> _msg); // push data into the outgoing stream

	void recvData(); // Take incoming data and push it onto the incoming stream
	bool pollData(std::vector<unsigned char>& _msg); // Pull data from the incoming stream assuming there is a complete packet to pull

	int& id();

	void close();

private:
	// Data streams
	std::vector<unsigned char> m_outgoing;
	std::vector<unsigned char> m_incoming;

	int m_id;

	// We need to run a preCobs to account for messages larger than 254 bytes long.
	// Since unsigned char's have a max value of 255, any message longer than that
	// is at risk of causing an overflow error in the COBS algorithm.
	// The postCob is to convert it back to readable form.
	std::vector<unsigned char> preCob(std::vector<unsigned char> _msg);
	std::vector<unsigned char> postCob(std::vector<unsigned char> _msg);

	std::vector<unsigned char> cobify(std::vector<unsigned char> _msg); // Apply C.O.B.S (Consistent overhead Byte Stuffing) to the message
	std::vector<unsigned char> deCobify(std::vector<unsigned char> _message); // Convert a message that has had COBS applied to it back into the original message
};