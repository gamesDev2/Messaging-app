#pragma once
#include "client.h"
#include "ServerSocket.h"
#include "xmlInterface.h"
#include "xmlStringWriter.h"
#include "NetworkFlags.h"

#include <vector>

// This is used to allow the GUI to call the same functions regardless of whether they are the client or the server.
// This is because we want the GUI to not have to think about networking.
// (one could just call it encapsulation and be done with it, but I decided to write an entire paragraph instead)


// This also breaks my personal rule of one class per header-file and as a result it is so so much harder to read even for me... TODO: This! Make this readable.

class netInterface : public xmlInt
{
public:
	netInterface();


	virtual void networkTick();
	virtual void pushData(const std::vector<unsigned char> _msg);
	virtual bool pollData(std::vector<unsigned char>& _msg);

	virtual bool isClosed();
	virtual void close();

	virtual std::string getVersion();

private:
};


class serverInterface : ServerSocket, public client, public netInterface // This causes multiple inheritance of xmlInt, which in this case is actually a good thing since we want two copies that are used for different aspects.
{
public:
	serverInterface(int _port, const char* _uName, const char* _version);

	void networkTick();
	void pushData(const std::vector<unsigned char> _msg);
	bool pollData(std::vector<unsigned char>& _msg);

	void handleMessage(std::vector<unsigned char> _message, size_t _ci);
	void handleOutgoing(size_t _ci);
	bool isClosed();
	void close();

	std::string getVersion();
	
private:
	void acceptConnection();
	std::vector<std::shared_ptr<client>> m_clients;
	size_t m_cid = 0;

	xml_string_writer writer;

	std::string m_version;
};


class clientInterface : public client, public netInterface // This causes multiple of inheritance of xmlInt, this one isn't a good thing... Too Bad!
{
public:
	clientInterface(const std::string& _server, int _port, const char* _uName);

	void networkTick();
	void handleMessage(std::vector<unsigned char> _message);
	void pushData(const std::vector<unsigned char> _msg);
	bool pollData(std::vector<unsigned char>& _msg);
	bool isClosed();
	void close();

	std::string getVersion();

private:
	xml_string_writer writer; // For debugging

	std::string m_version;
};