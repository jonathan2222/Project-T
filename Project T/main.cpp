#include <iostream>
#include "SFML\Graphics.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "Project T");

	sf::RectangleShape rect(sf::Vector2f(200, 200));
	rect.setPosition(200, 200);

	while (window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		window.draw(rect);
		window.display();
	}

	return 0;
}