#ifndef PLAYER_SYS_H
#define PLAYER_SYS_H

#include "../Components/PositionComp.h"
#include "../Components/PlayerComp.h"
#include "../Components/RectangleComp.h"
#include "../Components/ColorComp.h"
#include "../ECS/ECS.h"
#include <iostream>

#include "SFML\Graphics.hpp"

class PlayerSys : public System<PositionComp, PlayerComp, RectangleComp, ColorComp>
{
public:
	void init(const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		for (EntityHandle handle : entities)
		{
			ColorComp* color = ecs.getComponent<ColorComp>(handle);
			color->r = color->g = 0.0f;
			color->b = 1.0f;
			std::cout << "PlayerSys INIT" << std::endl;
		}
	}
	void update(float dt, const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		for (EntityHandle handle : entities)
		{
			PositionComp* pos = ecs.getComponent<PositionComp>(handle);
			PlayerComp* player = ecs.getComponent<PlayerComp>(handle);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) pos->x -= player->speed*dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) pos->x += player->speed*dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) pos->y += player->speed*dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) pos->y -= player->speed*dt;

			RectangleComp* rect = ecs.getComponent<RectangleComp>(handle);
			float w2 = rect->width / 2.0f;
			float h2 = rect->height / 2.0f;
			if (pos->x > 1.0f - w2) pos->x = 1.0f - w2;
			if (pos->x < -1.0f + w2) pos->x = -1.0f + w2;
			if (pos->y > 1.0f - h2) pos->y = 1.0f - h2;
			if (pos->y < -1.0f + h2) pos->y = -1.0f + h2;
		}
	}
};

#endif
