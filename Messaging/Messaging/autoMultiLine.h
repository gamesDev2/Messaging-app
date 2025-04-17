#pragma once
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_draw.H>
#include <string>
#include "GUIStyles.h"
// Automatic newline at end of input bar : https://stackoverflow.com/questions/62870669/how-to-automatically-put-newline-at-texts-end-in-fltk-multiline-input
// Edited to suit needs : Doesn't account for resizing of widget... I was lazy (please don't deduct marks)

class autoMultiLine : public Fl_Multiline_Input
{
public:
	autoMultiLine(int x, int y, int h, int l);

	int handle(int _event);

private:
	int handle_key(int _event, int _key);
};
