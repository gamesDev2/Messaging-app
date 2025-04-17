#pragma once
#include "pugixml.hpp"
#include <iostream>
class xmlInt
{
public:
	xmlInt();
	void addUser(int _id, const char* _name);
	void editUser(int _id, const char* _name);

	void addMessage(int _id, const char* _message);
	size_t getLastMessagePos();

	const pugi::xml_document* getDocument();

private:
	pugi::xml_document m_doc;
};