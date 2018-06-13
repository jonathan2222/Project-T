#ifndef COLLISION_SYS_H
#define COLLISION_SYS_H

#include "../Components/PositionComp.h"
#include "../Components/RectangleComp.h"
#include "../Components/CollisionComp.h"
#include "../ECS/ECS.h"

#include <iostream>
#include <algorithm>

class CollisionSys : public System<PositionComp, RectangleComp, CollisionComp>
{
public:
	void init(const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		for (EntityHandle handle : entities)
			std::cout << "CollisionSys INIT" << std::endl;
	}
	void update(float dt, const std::vector<EntityHandle>& entities, Renderer* renderer, ECS& ecs, Container* container)
	{
		for (EntityHandle handle : entities)
		{
			PositionComp* pos = ecs.getComponent<PositionComp>(handle);
			RectangleComp* rect = ecs.getComponent<RectangleComp>(handle);
			CollisionComp* coll = ecs.getComponent<CollisionComp>(handle);

			bool collided = false;
			for (EntityHandle handle2 : entities)
			{
				if (handle2 != handle)
				{
					PositionComp* pos2 = ecs.getComponent<PositionComp>(handle2);
					RectangleComp* rect2 = ecs.getComponent<RectangleComp>(handle2);

					if (std::max(pos->x - rect->width / 2.0f, pos2->x - rect2->width / 2.0f) <
						std::min(pos->x + rect->width / 2.0f, pos2->x + rect2->width / 2.0f))
						if (std::max(pos->y - rect->height / 2.0f, pos2->y - rect2->height / 2.0f) <
							std::min(pos->y + rect->height / 2.0f, pos2->y + rect2->height / 2.0f))
						{
							// Collision!
							collided = true;
						}
				}
			}
			coll->hasCollided = collided;
		}
	}
private:
};

#endif
