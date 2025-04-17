#include "MainWindow.h"
#include <iostream>

int main(int argc, char *argv[])
{
	MainWindow win(0, 0, 1280, 447, "Harmony", "1.0.0");

	return Fl::run();
}
