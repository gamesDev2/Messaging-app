#include "about.h"

aboutPopup::aboutPopup(int x, int y, int width, int height, const char(*title)):
	Fl_Double_Window(x, y, width, height, title),
	m_maincol(0, 0, w(), h()),
	m_about(0, 0, 0, 0),
	m_spacer(0, 0, 0, 0)
{
	this->add_resizable(m_maincol);

	m_maincol.add_resizable(m_about);
	m_maincol.add_resizable(m_spacer);
	m_maincol.fixed(m_spacer, 1);

	m_spacer.box(FL_FLAT_BOX);
	m_spacer.color(DISCORD_DARK);

	this->end();
	this->hide();
}

void aboutPopup::show(const char* _selfVersion, const char* _serverVersion)
{
	m_about.text("");
	m_about.append("Your version\n");
	m_about.append(_selfVersion);
	m_about.append("\n-----------\nServer version\n");
	m_about.append(_serverVersion);

	Fl_Double_Window::show();

}

void aboutPopup::on_close(Fl_Widget* _widget, void* _userdata)
{
	aboutPopup* window = (aboutPopup*)_userdata;
	window->hide();
}
