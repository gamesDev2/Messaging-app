#include "netInt.h"

netInterface::netInterface() :
	xmlInt()
{
}

void netInterface::networkTick()
{
}

void netInterface::pushData(const std::vector<unsigned char> _msg)
{
}

bool netInterface::pollData(std::vector<unsigned char>& _msg)
{
	return false;
}

bool netInterface::isClosed()
{
	return false;
}

void netInterface::close()
{
}

std::string netInterface::getVersion()
{
	return std::string();
}

// Server class
serverInterface::serverInterface(int _port, const char* _uName, const char* _version) :
	netInterface(),
	ServerSocket(_port),
	client("localhost", _port),
	m_version(_version)
{
	std::string strMsg = _uName;
	std::vector<unsigned char> msg(strMsg.begin(), strMsg.end());
	msg.insert(msg.begin(), NET_USER); // Flag message as a username

	pushData(msg);

	acceptConnection(); // Accept connection from self, this allows us to treat the host the exact same as all the other clients. This makes writing the GUI a bit easier
}



void serverInterface::networkTick()
{
	acceptConnection();

	client::sendData();
	client::recvData();

	for (size_t ci = 0; ci < m_clients.size(); ++ci)
	{
		m_clients.at(ci)->recvData();

		// Handling incoming data
		std::vector<unsigned char> message;
		while (m_clients.at(ci)->pollData(message))
		{
			handleMessage(message, ci);
		}

		// Handling outgoing data


		handleOutgoing(ci);

		
		m_clients.at(ci)->sendData();

		if (m_clients.at(ci)->closed())
		{
			m_clients.erase(m_clients.begin() + ci);
			--ci;
		}
	}
}

void serverInterface::handleMessage(std::vector<unsigned char> _message, size_t _ci)
{
	std::vector<unsigned char> msg;
	switch (_message.at(0))
	{
	case NET_TEXT: // If the message is a text message we simply add it to the xml tree
		_message.push_back('\0');
		netInterface::addMessage(m_clients.at(_ci)->id(), (const char*)_message.data());
		break;

	case NET_USER: // If the message is a edit to the username then call the relevent xmlInterface func
		_message.push_back('\0');

		netInterface::editUser(m_clients.at(_ci)->id(), (const char*)_message.data() + 1);
		break;
	case NET_ABOUT:
		msg.insert(msg.begin(), m_version.begin(), m_version.end());
		msg.insert(msg.begin(), NET_ABOUT);
		m_clients.at(_ci)->pushData(msg);
		break;
	default:
		m_clients.at(_ci)->close();
	}
	
}

void serverInterface::handleOutgoing(size_t _ci) // Finds the difference between the differencing copy and the master copy and sends the missing data to the remote copy
{

	// First we send all the unsent messages
	// I don't really like this solution since it looks through the entire xml tree everytime we make a diff, but I can't think of a better one
	const pugi::xml_document* masterDoc = netInterface::getDocument();
	pugi::xml_node messages = masterDoc->child("messages");

	const pugi::xml_document* diffDoc = m_clients.at(_ci)->getDocument();
	pugi::xml_node diffMessage = diffDoc->child("messages");

	std::vector<unsigned char> msg;

	// Send the differencing data in the form of a xml string
	pugi::xml_node diffI = diffMessage.child("message");
	for (pugi::xml_node i = messages.child("message"); i; i = i.next_sibling())
	{

		if (!diffI)
		{
			i.print(writer);
			msg.clear();
			msg.insert(msg.begin(), writer.result.begin(), writer.result.end());
			writer.result.clear();
			msg.insert(msg.begin(), NET_MESSAGE); // Flag indicating that it belongs to the messages node

			m_clients.at(_ci)->pushData(msg);
			diffMessage.append_copy(i);
		}
		diffI = diffI.next_sibling();
	}

	// Second we make all the username edits/additions

	pugi::xml_node mUNames = masterDoc->child("users");
	pugi::xml_node dUNames = diffDoc->child("users");
	bool same = false;
	for (pugi::xml_node mUser = mUNames.child("user"); mUser; mUser = mUser.next_sibling())
	{
		pugi::xml_node dUser = dUNames.find_child_by_attribute("user", "id", mUser.attribute("id").value());

		same = !strcmp(dUser.attribute("name").value(), mUser.attribute("name").value());
		if (!same or !dUser)
		{
			mUser.print(writer);
			msg.clear();
			msg.insert(msg.begin(), writer.result.begin(), writer.result.end());
			writer.result.clear();

			msg.insert(msg.begin(), NET_USER); // Flag indicating that it's a user
			m_clients.at(_ci)->pushData(msg);

			if (dUser)
			{
				dUser.attribute("name").set_value(mUser.attribute("name").value());
			}
			else
			{
				dUNames.append_copy(mUser);
			}
		}
	}
}

bool serverInterface::isClosed()
{
	return closed();
}

void serverInterface::close()
{
	client::close();
}

std::string serverInterface::getVersion()
{
	return m_version;
}

void serverInterface::pushData(const std::vector<unsigned char> _msg)
{
	client::pushData(_msg);
}

bool serverInterface::pollData(std::vector<unsigned char>& _msg)
{
	return client::pollData(_msg);
}

void serverInterface::acceptConnection()
{
	std::shared_ptr<client> client = accept();
	std::vector<unsigned char> msg;
	std::string defaultName = "new";
	if (!client)
	{
		return;
	}
	m_clients.push_back(client);
	client->recvData();

	msg.insert(msg.begin(), defaultName.begin(), defaultName.end());
	msg.push_back('\0');

	netInterface::addUser(m_cid, (const char*)(msg.data())); // Since we have multiple xmlInterfaces we need to specify which one we mean.
	client->id() = m_cid;
		
	m_cid++;
}




// Client class
clientInterface::clientInterface(const std::string& _server, int _port, const char* _uName) :
	netInterface(),
	client(_server, _port),
	m_version("\0")
{
	std::string strMsg = _uName;
	std::vector<unsigned char> msg(strMsg.begin(), strMsg.end());
	msg.insert(msg.begin(), NET_USER); // Flag message as a username

	pushData(msg);
	msg.clear();
	msg.push_back(NET_ABOUT);
	pushData(msg);
}

void clientInterface::networkTick()
{
	client::sendData();
	client::recvData();

	std::vector<unsigned char> message;
	
	while (pollData(message))
	{
		handleMessage(message);
	}
}

void clientInterface::handleMessage(std::vector<unsigned char> _message)
{
	_message.push_back('\0');
	pugi::xml_document doc;

	const pugi::xml_document* remote = netInterface::getDocument();
	pugi::xml_node messages;

	pugi::xml_node update;

	pugi::xml_node users;
	pugi::xml_node user;

	std::string newStr(_message.begin() + 1, _message.end());

	switch (_message.at(0))
	{
	case (NET_MESSAGE):

		if (!doc.load_buffer(_message.data() + 1, _message.size()))
		{
			client::close();
			return;
		}

		update = doc.child("message");
        messages = remote->child("messages");
		messages.append_copy(update);

		break;

	case(NET_USER):

		if (!doc.load_buffer(_message.data() + 1, _message.size()))
		{
			client::close();
			return;
		}

		update = doc.child("user");
		users = remote->child("users");
		user = users.find_child_by_attribute("user", "id", update.attribute("id").value());
		
		if (user)
		{
			user.attribute("name").set_value(update.attribute("name").value());
		}
		else
		{
			users.append_copy(update);
		}

		break;
	case(NET_ABOUT):
		m_version = newStr;
		break;
	default:
		break;
	}
}

void clientInterface::pushData(const std::vector<unsigned char> _msg)
{
	client::pushData(_msg);
}

bool clientInterface::pollData(std::vector<unsigned char>& _msg)
{
	return client::pollData(_msg);
}

bool clientInterface::isClosed()
{
	return closed();
}

void clientInterface::close()
{
	client::close();
}

std::string clientInterface::getVersion()
{
	return m_version;
}
