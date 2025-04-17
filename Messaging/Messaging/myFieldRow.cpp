#include "myFieldRow.h"

fieldRow::fieldRow(int x, int y, int width, int height, const char(*title)):
	Fl_Flex(0, 0, 0, 0),
	m_label(0, 0, 0, 0, title),
	m_inputField(0, 0, 0, 0)
{
	type(Fl_Flex::ROW);
	margin(5);
	add_resizable(m_label);
	add_resizable(m_inputField);

	fixed(m_label, 120);

	m_label.labelfont(FL_COURIER);
	m_inputField.textfont(FL_COURIER);
	
	m_label.box(FL_FLAT_BOX);
	m_label.color(DISCORD_DARK);
	m_label.labelcolor(DIRTY_WHITE);

	m_inputField.box(FL_FLAT_BOX);
	m_inputField.color(DISCORD_LIGHT);
	m_inputField.textcolor(DIRTY_WHITE);
	m_inputField.cursor_color(BRIGHT_WHITE);
}

const char* fieldRow::value()
{
	return m_inputField.value();
}

void fieldRow::value(const char* _newValue)
{
	m_inputField.value(_newValue);
}
