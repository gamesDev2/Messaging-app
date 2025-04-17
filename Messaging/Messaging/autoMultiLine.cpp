#include "autoMultiLine.h"


autoMultiLine::autoMultiLine(int x, int y, int h, int l):
	Fl_Multiline_Input(x, y, h, l)
{
    Fl_Multiline_Input::when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED); // Will generate a callback when the Enter key is pressed : https://www.fltk.org/doc-1.3/classFl__Widget.html#ad4ef380ffb50f4d00fe93f7b6677e9e1

    box(FL_FLAT_BOX);
    color(DISCORD_LIGHT);
    textcolor(DIRTY_WHITE);
    cursor_color(BRIGHT_WHITE);

    //maximum_size(252); // Our cobs algorithm can only handle messages of length 253 or lower add in the network flag at the start of the message and we can only handle a message of 252
}

int autoMultiLine::handle(int _event)
{
    switch (_event) {
        // When a key is pressed and the widget is waiting for input
    case FL_KEYBOARD:
        return handle_key(_event, Fl::event_key());
    default:
        return Fl_Multiline_Input::handle(_event);
    };
}

int autoMultiLine::handle_key(int _event, int _key)
{
    switch (_key)
    {
    case (FL_BackSpace):
        // Allowing Fl_Multiline_Input to handle the delection of characters
        return Fl_Multiline_Input::handle(_event);

    default:
        // Converting to std::string just because manipulation is easer
        std::string s(this->value());
        std::string s2;
        /*
           Search for the last newline char:
           the subsequent substring must be shorter
           than the width of the input field
        */
        std::size_t found = s.find_last_of("\n");
        if (found < std::string::npos)
            s2 = s.substr(found);
        else
            s2 = s;

        
        // Length of a char under the current fl_font.
        double lc = fl_width("a");
        // Maximum length for the substring
        int string_length = this->w() / lc - 1;

        std::size_t lastWord = s.find_last_of(' ');

        bool changeFlag = false;
        // if the substring is longer than the max allowed length, then we place a new line at the start of the last word
        if (s2.length() > string_length and lastWord < std::string::npos)
        {
            s[lastWord] = '\n';
            changeFlag = true;
        }
        else if (s2.length() > string_length)
        {
            s += "\n";
            changeFlag = true;
        }

        // Update the input field: If I don't use the flag then the thing breaks slightly and I have no clue why
        if (changeFlag)
        {
            this->value(s.c_str());
        }

        return Fl_Multiline_Input::handle(_event);
    }
}