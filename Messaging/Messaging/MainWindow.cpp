#include "MainWindow.h"

MainWindow::MainWindow(int x, int y, int width, int height, const char(*title), const char(*version)) :
	Fl_Double_Window(x, y, width, height, title),
	Timer(0.1),
	Wsa(),
	m_version(version),
	// Fl_Flex's
	m_mainrow(0, 0, w(), h()),
	m_maincol(0, 0, 0, 0),
	m_secondarycol(0, 0, 0, 0),
	m_logcol(0, 0, 0, 0),
	m_secondaryBottomrow(0, 0, 0, 0),
	m_bottomrow(0, 0, 0, 0),
	m_bottomcolumns(0, 0, 0, 0),
	// Menubar
	m_menubar(0, 0, 0, 0),
	// Text displays
	m_content(0, 0, 0, 0),
	m_users(0, 0, 0, 0),
	m_log(0, 0, 0, 0),
	// Text inputs
	m_message(0, 0, 0, 0),
	m_UsernameInput(0, 0, 0, 0, "Username: "),
	// Buttons
	m_send(0, 0, 0, 0, "Send"),
	m_changeUsername(0, 0, 0, 0, "^"),
	// Format spacers
	m_spacer(0, 0, 0, 0),
	m_sendSpacer1(0, 0, 0, 0),
	m_sendSpacer2(0, 0, 0, 0)
{
	this->add_resizable(m_mainrow);

	// Adding Widgets to the main row
	m_mainrow.type(Fl_Flex::ROW);
	m_mainrow.add_resizable(m_secondarycol);
	m_mainrow.add_resizable(m_maincol);
	m_mainrow.add_resizable(m_logcol);

	m_mainrow.fixed(m_secondarycol, 320);
	m_mainrow.fixed(m_logcol, 320);

	// Adding widgets to the secondary flex column
	m_secondarycol.add_resizable(m_users);
	m_secondarycol.add_resizable(m_secondaryBottomrow);

	m_secondarycol.fixed(m_secondaryBottomrow, 40);

	m_secondaryBottomrow.type(Fl_Flex::ROW);
	m_secondaryBottomrow.add_resizable(m_UsernameInput);
	m_secondaryBottomrow.add_resizable(m_changeUsername);

	m_secondaryBottomrow.fixed(m_changeUsername, 30);

	// Adding widgets to the main flex column
	m_maincol.add_resizable(m_menubar);
	m_maincol.add_resizable(m_content);
	m_maincol.add_resizable(m_bottomrow);

	// Adding widgets to the log column
	m_logcol.add_resizable(m_log);

	// Freezing the menubar and bottom row to a specific pixel count
	m_maincol.fixed(m_menubar, 30);
	m_maincol.fixed(m_bottomrow, 50);

	// We set the bottom row flex to ROW which makes the widgets that are placed in it appear side by side. We also add a margin between them.
	m_bottomrow.type(Fl_Flex::ROW);
	m_bottomrow.margin(5);
	m_bottomrow.add_resizable(m_message);
	m_bottomrow.add_resizable(m_spacer);
	m_bottomrow.add_resizable(m_bottomcolumns);

	m_bottomrow.fixed(m_bottomcolumns, 120);
	m_bottomrow.fixed(m_spacer, 5);

	m_bottomcolumns.add_resizable(m_sendSpacer1);
	m_bottomcolumns.add_resizable(m_send);
	m_bottomcolumns.add_resizable(m_sendSpacer2);
	
	
	m_bottomcolumns.fixed(m_send, 30);

	// We set the font to a mono-spaced font in order to make sure the estimation of the text length is accurate
	m_message.textfont(FL_COURIER);


	// Styalizing widgets
	m_menubar.box(FL_BORDER_BOX);
	m_menubar.color(DISCORD_DARK);
	m_menubar.color2(NAVY_BLUE);
	m_menubar.textcolor(DIRTY_WHITE);

	m_maincol.box(FL_BORDER_BOX);
	m_maincol.color(DISCORD_DARK);

	m_bottomrow.box(FL_BORDER_BOX);
	m_bottomrow.color(DISCORD_DARK);

	m_secondaryBottomrow.margin(5);

	this->color(DISCORD_DARK);

	// Adding menu widgets to the menu bar
	// Adding a radio drop down for the File tab
	m_menubar.add("&Connect/&Join", NULL, on_join, this);
	m_menubar.add("&Connect/&Host", NULL, on_create, this);
	m_menubar.add("&Connect/&Disconnect", NULL, on_disconnect, this);


	m_menubar.add("&Help/&About", NULL, see_about, this);

	m_send.callback(send_data, this);
	m_message.callback(send_data, this);

	m_changeUsername.callback(send_username, this);

	this->end();
	this->show();

	m_serverPopup = std::make_shared<createServerPopUp>(0, 0, 400, 90, "Host Server");
	m_clientPopup = std::make_shared<joinServerPopUp>(0, 0, 400, 120, "Join Server");
	m_aboutPopup = std::make_shared<aboutPopup>(0, 0, 320, 200, "About");
}

void MainWindow::tick()
{
	try
	{
		if (m_network == nullptr)
		{
			return;
		}
		m_network->networkTick();

		xmlToText();

		if (m_network->isClosed())
		{
			m_network = nullptr;
			m_content.text("");
			m_users.text("");
		}
	}
	catch (std::runtime_error _error) // This logs any error that occurs when ticking
	{
		m_log.append(_error.what());
		m_log.append("\n");
	}
	
}


void MainWindow::on_close(Fl_Widget* _widget, void* _userdata)
{
	MainWindow* window = (MainWindow*)_userdata;
	window->hide();
	
}

// Host and join a server
void MainWindow::on_create(Fl_Widget* _widget, void* _userdata)
{
	MainWindow* window = (MainWindow*)_userdata;
	std::shared_ptr<netInterface>* network = &window->m_network;
	std::shared_ptr<createServerPopUp> popUp = window->m_serverPopup;

	if (*network != nullptr)
	{
		window->m_log.append("Connection already exists! Close it before creating another.");
		window->m_log.append("\n");
		return;
	}

	// Finding the position to put the popup in the centre of the main window
	int newX = window->x() + (window->w() / 2) - (popUp->w() / 2);
	int newY = window->y() + (window->h() / 2) - (popUp->h() / 2);

	popUp->position(newX, newY);
	popUp->show();

	while (popUp->shown()) // Turns the new window into a dialog box by pausing the function but not the fltk engine until the user is done with the box
	{
		Fl::wait();
	}

	try
	{
		if (popUp->dataSubmitted())
		{
			*network = std::make_shared<serverInterface>(popUp->port(), popUp->userName(), window->m_version.c_str());
			window->m_log.append("Server Created!");
			window->m_log.append("\n");
		}
	}
	catch (std::runtime_error _error) // This logs any error that occurs when validating the port or when trying to make the socket server
	{
		window->m_log.append(_error.what());
		window->m_log.append("\n");
	}
}

// Join a server
void MainWindow::on_join(Fl_Widget* _widget, void* _userdata)
{
	MainWindow* window = (MainWindow*)_userdata;
	std::shared_ptr<netInterface>* network = &window->m_network;
	std::shared_ptr<joinServerPopUp> popUp = window->m_clientPopup;

	if (*network != nullptr)
	{
		window->m_log.append("Connection already exists! Close it before creating another.");
		window->m_log.append("\n");
		return;
	}

	// Finding the position to put the popup in the centre of the main window
	int newX = window->x() + (window->w() / 2) - (popUp->w() / 2);
	int newY = window->y() + (window->h() / 2) - (popUp->h() / 2);

	popUp->position(newX, newY);
	popUp->show();

	while (popUp->shown()) // Turns the new window into a dialog box by pausing the function but not the fltk engine until the user is done with the box
	{
		Fl::wait();
	}

	try
	{
		if (popUp->dataSubmitted()) // Determines if the user cancelled or not
		{
			*network = std::make_shared<clientInterface>(popUp->address(), popUp->port(), popUp->userName());
			window->m_log.append("Client Created!");
			window->m_log.append("\n");
		}
	}
	catch(std::runtime_error _error) // This logs any error that occurs when validating the port or when trying to make the socket connection
	{
		window->m_log.append(_error.what());
		window->m_log.append("\n");
	}
}

void MainWindow::on_disconnect(Fl_Widget* _widget, void* _userdata)
{
	MainWindow* window = (MainWindow*)_userdata;
	std::shared_ptr<netInterface> network = window->m_network;
	if (network != nullptr)
	{
		network->close();
	}
}

// Used as a call back function to send data from the input buffer to the network send stream (This one is for text messages)
void MainWindow::send_data(Fl_Widget* _widget, void* _userdata)
{
	MainWindow* window = (MainWindow*)_userdata;

	if (window->m_network != nullptr)
	{
		// Cursed copying of a const char* to a std::vector<unsigned char>
		// There is almost definitely a better way of doing this
		unsigned char* buffer = (unsigned char*)window->m_message.value();
		int len = std::strlen(window->m_message.value());
		std::vector<unsigned char> msg{ buffer, buffer + len };

		msg.insert(msg.begin(), NET_TEXT); // Flag message as a text message

		window->m_network->pushData(msg);

		window->m_message.value(""); // Deletes the message buffer once the data has been sent
	}
}

// Used as a call back function to send data from the input buffer to the network send stream (This one is for username changes)
void MainWindow::send_username(Fl_Widget* _widget, void* _userdata)
{
	MainWindow* window = (MainWindow*)_userdata;

	if (window->m_network != nullptr)
	{
		unsigned char* buffer = (unsigned char*)window->m_UsernameInput.value();
		int len = std::strlen(window->m_UsernameInput.value());
		std::vector<unsigned char> msg{ buffer, buffer + len };

		msg.insert(msg.begin(), NET_USER); // Flag message as an edit username message

		window->m_network->pushData(msg);

		window->m_UsernameInput.value(""); // Deletes the message buffer once the data has been sent
	}
}

// See what it's all about
void MainWindow::see_about(Fl_Widget* _widget, void* _userdata)
{
	MainWindow* window = (MainWindow*)_userdata;
	std::shared_ptr<aboutPopup> popUp = window->m_aboutPopup;
	// Finding the position to put the popup in the centre of the main window
	int newX = window->x() + (window->w() / 2) - (popUp->w() / 2);
	int newY = window->y() + (window->h() / 2) - (popUp->h() / 2);

	popUp->position(newX, newY);

	if (window->m_network != nullptr)
	{
		popUp->show(window->m_version.c_str(), window->m_network->getVersion().c_str());
	}
	else
	{
		popUp->show(window->m_version.c_str(), "Not Connected to a server");
	}
}

void MainWindow::xmlToText()
{
	const pugi::xml_document* doc = m_network->getDocument();
	pugi::xml_node messages = doc->child("messages");
	pugi::xml_node users = doc->child("users");

	std::string chat;
	std::string userList;
	// Find all messages in xml and place the respective Username in front of them
	for (pugi::xml_node i = messages.child("message"); i; i = i.next_sibling())
	{
		pugi::xml_node user = users.find_child_by_attribute("user", "id", i.attribute("id").value()); // Finds the corresponding username to the message id

		chat.append(user.attribute("name").value());

		chat.append(" : ");
		chat.append(i.attribute("msg").value() + 1);
		chat.append("\n\n");
	}

	// List all the usernames in the server (Making a note of who's host)
	for (pugi::xml_node u = users.child("user"); u; u = u.next_sibling())
	{
		userList.append(u.attribute("name").value());

		if (u.attribute("id").value()[0] == '0')
		{
			userList.append("(Host)");
		}
		userList.append("\n");
	}

	m_content.text(chat.c_str());
	m_users.text(userList.c_str());
}
