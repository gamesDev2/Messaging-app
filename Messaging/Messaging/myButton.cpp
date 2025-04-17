#include "myButton.h"
#include "GUIStyles.h"

blueButton::blueButton(int x, int y, int width, int height, const char(*title)) :
	Fl_Button(x, y, width, height, title)
{
	box(FL_RFLAT_BOX);
	color(DARKISH_BLUE);
	color2(NAVY_BLUE);
	labelcolor(DIRTY_WHITE);
	labelfont(FL_COURIER);
}
