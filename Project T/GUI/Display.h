#ifndef DISPLAY_H
#define DISPLAY_H

#include "GL\glew.h"
#include "SFML\Window.hpp"

#include <string>

class Display
{
public:
	Display();
	Display(const std::string& title, unsigned int width, unsigned int height);
	virtual ~Display();

	void processEvents();

	sf::Window* getWindowPtr();

	bool isOpen();

private:
	void init();

	sf::Window* window;
	unsigned int width;
	unsigned int height;
	std::string title;
};

#endif
