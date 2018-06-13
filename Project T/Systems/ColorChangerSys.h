#ifndef COLOR_CHANGER_SYS_H
#define COLOR_CHANGER_SYS_H

#include "../Components/ColorComp.h"
#include "../Components/CollisionComp.h"

#include "../ECS/ECS.h"
#include <iostream>

#include "../Components/PlayerComp.h"

class ColorChangerSys : public System<ColorComp, CollisionComp>
{
public:
	void init(const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		for (EntityHandle handle : entities)
		{
			ColorComp* color = ecs.getComponent<ColorComp>(handle);
			color->r = color->g = color->b = 0.0f;
			std::cout << "ColorChangerSys INIT" << std::endl;
		}
	}
	void update(float dt, const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		unsigned int numEntities = entities.size();
		for (unsigned int i = 0; i < numEntities; i++)
		{
			EntityHandle handle = entities[i];
			ColorComp* color = ecs.getComponent<ColorComp>(handle);
			CollisionComp* coll = ecs.getComponent<CollisionComp>(handle);
			if (Utils::isBitSet<Bitmask>(coll->collidedWith, getComponentTypeID<PlayerComp>()))
				color->r = color->g = color->b = 0.25f;
		}
	}
};

#endif
