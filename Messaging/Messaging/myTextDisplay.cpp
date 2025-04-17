#include "myTextDisplay.h"

textDisplay::textDisplay(int x, int y, int width, int height, const char(*title)):
	Fl_Text_Display(x, y, width, height, title),
	m_buffer(0)
{
	buffer(m_buffer);

	color(DISCORD_DARK);
	box(FL_FLAT_BOX);
	textfont(FL_COURIER);
	textcolor(DIRTY_WHITE);
}

textDisplay::~textDisplay()
{
	buffer(0);
}

void textDisplay::text(const char* _text)
{
	m_buffer.text(_text);
}

void textDisplay::append(const char* _text)
{
	m_buffer.append(_text);
}
