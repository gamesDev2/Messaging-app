#pragma once
#include <FL/Fl_Input.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Box.H>

#include "GUIStyles.h"

// Reduces the amount of work needed to add a row that is structured like this: Label : {Input Field}
class fieldRow : public Fl_Flex
{
public:
	fieldRow(int x, int y, int width, int height, const char(*title));

	const char* value();
	void value(const char* _newValue);

private:
	Fl_Box m_label;
	Fl_Input m_inputField;
};