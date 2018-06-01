#ifndef DISPLAY_H
#define DISPLAY_H

#include "SFML\Graphics.hpp"

#include <string>

class Display
{
public:
	Display(const std::string& title, unsigned int width, unsigned int height);
	virtual ~Display();

	void processEvents();

	sf::RenderWindow* getWindowPtr();

	bool isOpen();

private:
	sf::RenderWindow* window;
};

#endif
