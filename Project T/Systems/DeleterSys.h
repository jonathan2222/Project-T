#ifndef DELETER_SYS_H
#define DELETER_SYS_H

#include "../Components/EnemyComp.h"
#include "../Components/PositionComp.h"
#include "../Components/RectangleComp.h"
#include "../ECS/ECS.h"
#include <iostream>

#include "SFML\Graphics.hpp"

#include "../World.h"

class DeleterSys : public System<EnemyComp, PositionComp, RectangleComp>
{
public:
	void init(const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		std::cout << "DeleterSys INIT" << std::endl;
	}
	void update(float dt, const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		sf::Vector2u windowSize = container->getDisplay()->getWindowPtr()->getSize();
		sf::Vector2i mousePos = sf::Mouse::getPosition(*container->getDisplay()->getWindowPtr());
		sf::Vector2f mPos((float)mousePos.x/ (float)windowSize.x * 2.0f - 1.0f, -((float)mousePos.y / (float)windowSize.y * 2.0f - 1.0f));
		unsigned int numEntities = entities.size();
		std::vector<EntityHandle> shouldBeDeleted;
		for (unsigned int i = 0; i < numEntities; i++)
		{
			EntityHandle handle = entities[i];
			PositionComp* pos = ecs.getComponent<PositionComp>(handle);
			RectangleComp* rect = ecs.getComponent<RectangleComp>(handle);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				if (mPos.x > pos->x - rect->width / 2.0f && mPos.x < pos->x + rect->width / 2.0f &&
					mPos.y > pos->y - rect->height / 2.0f && mPos.y < pos->y + rect->height / 2.0f)
				{
					shouldBeDeleted.push_back(handle);
				}
			}
		}
		for(EntityHandle handle : shouldBeDeleted)
			ecs.deleteEntity(handle);
	}
private:
};

#endif
