#include <iostream>

#include "GUI\Display.h"

int main()
{
	Display display("Project T", 600, 600);

	while (display.isOpen())
	{
		display.processEvents();
	}

	return 0;
}