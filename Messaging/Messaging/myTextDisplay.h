#pragma once
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include "GUIStyles.h"

class textDisplay : public Fl_Text_Display
{
public:
	textDisplay(int x, int y, int width, int height, const char(*title) = "");
	~textDisplay();

	void text(const char* _text);

	void append(const char* _text);
private:
	Fl_Text_Buffer m_buffer;
};