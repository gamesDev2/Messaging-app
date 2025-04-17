#include "serverPopUp.h"

createServerPopUp::createServerPopUp(int x, int y, int width, int height, const char(*title))
	:Fl_Double_Window(x, y, width, height, title),
	m_maincol(0, 0, w(), h()),
	m_username(0, 0, 0, 0, "Username: "),
	m_port(0, 0, 0, 0, "Port: "),
	m_bottomRow(0, 0, 0, 0),
	m_cancel(0, 0, 0, 0, "Cancel"),
	m_accept(0, 0, 0, 0, "Accept"),
	m_leftSpacer(0, 0, 0, 0),
	m_centreSpacer(0, 0, 0, 0),
	m_rightSpacer(0, 0, 0, 0),
	m_accepted(false)
{
	this->add_resizable(m_maincol);

	// Adding child flex's to the main flex column
	m_maincol.add_resizable(m_username);
	m_maincol.add_resizable(m_port);
	m_maincol.add_resizable(m_bottomRow);

	// Setting the heights of each row
	m_maincol.fixed(m_username, 30);
	m_maincol.fixed(m_port, 30);
	m_maincol.fixed(m_bottomRow, 30);

	// Setting up the accept and cancel buttons' row
	m_bottomRow.type(Fl_Flex::ROW);
	m_bottomRow.margin(5);
	m_bottomRow.add_resizable(m_leftSpacer);
	m_bottomRow.add_resizable(m_cancel);
	m_bottomRow.add_resizable(m_centreSpacer);
	m_bottomRow.add_resizable(m_accept);
	m_bottomRow.add_resizable(m_rightSpacer);

	m_bottomRow.fixed(m_cancel, 120);
	m_bottomRow.fixed(m_accept, 120);

	this->color(DISCORD_DARK);

	m_accept.callback(on_accept, this);
	m_cancel.callback(on_cancel, this);

	this->end();
	this->hide();
}

int createServerPopUp::port()
{
	std::string portStr = m_port.value();
	if (checkValid(portStr))
	{
		return std::stoi(portStr);
	}
	else
	{
		throw std::runtime_error("Invalid port");
	}
}

const char* createServerPopUp::userName()
{
	return m_username.value();
}

bool createServerPopUp::dataSubmitted()
{
	return m_accepted;
}



void createServerPopUp::on_close(Fl_Widget* _widget, void* _userdata)
{
	createServerPopUp* window = (createServerPopUp*)_userdata;
	window->m_accepted = false;
	window->hide();
}

void createServerPopUp::on_accept(Fl_Widget* _widget, void* _userdata)
{
	createServerPopUp* window = (createServerPopUp*)_userdata;

	window->m_accepted = true;
	window->hide();
}

void createServerPopUp::on_cancel(Fl_Widget* _widget, void* _userdata)
{
	createServerPopUp* window = (createServerPopUp*)_userdata;

	window->m_accepted = false;
	window->hide();
}

bool createServerPopUp::checkValid(std::string _port)
{

	if (!std::all_of(_port.begin(), _port.end(), // check if string contains only numbers: https://www.delftstack.com/howto/cpp/how-to-determine-if-a-string-is-number-cpp/
		[](unsigned char c) { return std::isdigit(c); }))
	{
		return false;
	}

	if (_port.size() > 5) // 65535 has less characters than this anyway, we do this to prevent us from trying to convert numbers too large for an int
	{
		return false;
	}

	int numPort = std::stoi(_port);

	if (numPort > 65535 || numPort < 1) // Making sure port is in valid range
	{
		return false;
	}

}
