#include <FL/Fl_Generic_Image.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <memory>

#include "Timer.h"
#include "autoMultiLine.h"
#include "netInt.h"
#include "wsa.h"
#include "GUIStyles.h"
#include "myButton.h"
#include "myFieldRow.h"
#include "myTextDisplay.h"
#include "serverPopUp.h"
#include "clientPopUp.h"
#include "about.h"

// We use double window instead of regular window to prevent flickering.
// (Apparently this system doesn't natively support something called "double-buffering". Whatever that means)
class MainWindow : Fl_Double_Window, Timer, Wsa
{
public:
	MainWindow(int x, int y, int width, int height, const char(*title), const char(*version));

private:
	void tick();

	// Callback funcs
	static void on_close(Fl_Widget* _widget, void* _userdata);
	static void on_create(Fl_Widget* _widget, void* _userdata);
	static void on_join(Fl_Widget* _widget, void* _userdata);
	static void on_disconnect(Fl_Widget* _widget, void* _userdata);
	static void send_data(Fl_Widget* _widget, void* _userdata);
	static void send_username(Fl_Widget* _widget, void* _userdata);
	static void see_about(Fl_Widget* _widget, void* _userdata);

	void xmlToText();
	
	// Some of these flex's got some pretty odd names towards the end of development...
	Fl_Flex m_mainrow;
	Fl_Flex m_maincol;
	Fl_Flex m_secondarycol;
	Fl_Flex m_logcol;
	Fl_Flex m_secondaryBottomrow;
	Fl_Flex m_bottomrow;
	Fl_Flex m_bottomcolumns;
	Fl_Menu_Bar m_menubar;

	textDisplay m_content;

	textDisplay m_users;

	textDisplay m_log;

	fieldRow m_UsernameInput;
	blueButton m_changeUsername;

	autoMultiLine m_message;

	Fl_Box m_spacer;
	Fl_Box m_sendSpacer1;
	blueButton m_send;
	Fl_Box m_sendSpacer2;

	std::shared_ptr<netInterface> m_network;
	std::shared_ptr<createServerPopUp> m_serverPopup;
	std::shared_ptr<joinServerPopUp> m_clientPopup;
	std::shared_ptr<aboutPopup> m_aboutPopup;

	std::string m_version;
};