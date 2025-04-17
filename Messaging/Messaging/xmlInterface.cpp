#include "xmlInterface.h"

xmlInt::xmlInt()
{
	m_doc.append_child("messages");
	m_doc.append_child("users");
}

void xmlInt::addUser(int _id, const char* _name) // Adds a username to document. WARNING: Does not check for duplicate ID's
{
	pugi::xml_node users = m_doc.child("users");
	pugi::xml_node user = users.append_child("user");
	
	user.append_attribute("id").set_value(_id);
	user.append_attribute("name").set_value(_name);

}

void xmlInt::editUser(int _id, const char* _name) // Changes the associated user name of a specified user ID
{
	std::string b = std::to_string(_id);
	pugi::xml_node users = m_doc.child("users");
	pugi::xml_node user = users.find_child_by_attribute("user", "id", b.c_str());
	user.attribute("name").set_value(_name);
}

void xmlInt::addMessage(int _id, const char* _message) // Adds a new message to document and associates with specified ID. WARNING: Does not check if specified ID exists
{
	pugi::xml_node messages = m_doc.child("messages");
	pugi::xml_node message = messages.append_child("message");

	message.append_attribute("id").set_value(_id);
	message.append_attribute("msg").set_value(_message);
}

size_t xmlInt::getLastMessagePos() // Finds the location of the last message in the messages node. This is used so we can create a diff between the Differencing and Master copy
{
	pugi::xml_node messages = m_doc.child("messages");
	pugi::xml_node lastMessage = messages.last_child();
	
	return std::distance(messages.begin(), messages.end());
}

const pugi::xml_document* xmlInt::getDocument() // What do you think this does?
{
	return &m_doc;
}