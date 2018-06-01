#include "Display.h"

Display::Display(const std::string & title, unsigned int width, unsigned int height)
{
	this->window = new sf::RenderWindow(sf::VideoMode(width, height), title);
}

Display::~Display()
{
	delete this->window;
}

void Display::processEvents()
{
	sf::Event evnt;
	while (this->window->pollEvent(evnt))
	{
		if (evnt.type == sf::Event::Closed)
		{
			this->window->close();
		}
	}
}

sf::RenderWindow * Display::getWindowPtr()
{
	return this->window;
}

bool Display::isOpen()
{
	return this->window->isOpen();
}
