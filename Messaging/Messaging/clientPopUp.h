#pragma once

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Box.H>
#include <string>
#include <algorithm>
#include <iostream>

#include "GUIStyles.h"
#include "myButton.h"
#include "myFieldRow.h"

// I know that this is nearly identical to createServerPopUp but I want the two popups to be unique. That and it's difficult not to recode the whole thing anyway since we have to insert the extra widgets in the middle of the constructor.
class joinServerPopUp : public Fl_Double_Window
{
public:
	joinServerPopUp(int x, int y, int width, int height, const char(*title));
	int port();
	std::string address();
	const char* userName();
	bool dataSubmitted();

private:
	static void on_close(Fl_Widget* _widget, void* _userdata);
	static void on_accept(Fl_Widget* _widget, void* _userdata);
	static void on_cancel(Fl_Widget* _widget, void* _userdata);
	bool checkValid(std::string _port);


	bool m_accepted;


	Fl_Flex m_maincol;
	Fl_Flex m_bottomRow;
	
	fieldRow m_username;
	fieldRow m_address;
	fieldRow m_port;

	blueButton m_cancel;
	blueButton m_accept;
	Fl_Box m_leftSpacer;
	Fl_Box m_centreSpacer;
	Fl_Box m_rightSpacer;
};