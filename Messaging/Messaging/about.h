#pragma once
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Box.H>
#include <iostream>
#include "myTextDisplay.h"
#include "GUIStyles.h"

class aboutPopup : public Fl_Double_Window
{
public:
	aboutPopup(int x, int y, int width, int height, const char(*title));

	void show(const char* _selfVersion, const char* _serverVersion);

private:
	static void on_close(Fl_Widget* _widget, void* _userdata);

	Fl_Flex m_maincol;
	textDisplay m_about;
	Fl_Box m_spacer;

};