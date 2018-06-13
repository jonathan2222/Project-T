#ifndef ENEMY_SYS_H
#define ENEMY_SYS_H

#include "../Components/EnemyComp.h"
#include "../Components/PositionComp.h"
#include "../Components/RectangleComp.h"
#include "../Components/TimerComp.h"
#include "../Components/ColorComp.h"
#include "../ECS/ECS.h"
#include <iostream>

class EnemySys : public System<EnemyComp, PositionComp, RectangleComp, TimeComp, ColorComp>
{
public:
	void init(const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		for (EntityHandle handle : entities)
		{
			ColorComp* color = ecs.getComponent<ColorComp>(handle);
			color->r = 1.0f;
			color->g = color->b = 0.0f;
			std::cout << "EnemySys INIT" << std::endl;
		}
	}
	void update(float dt, const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		unsigned int numEntities = entities.size();
		for (unsigned int i = 0; i < numEntities; i++)
		{
			EntityHandle handle = entities[i];
			PositionComp* pos = ecs.getComponent<PositionComp>(handle);
			EnemyComp* enemy = ecs.getComponent<EnemyComp>(handle);
			TimeComp* time = ecs.getComponent<TimeComp>(handle);
			time->time += dt;

			if (!enemy->isDead)
			{
				if (time->time > 10.0f)
				{
					enemy->dir = std::rand() % 4;
					time->time = 0.0f;
				}
				switch (enemy->dir)
				{
				case 0: pos->y += enemy->speed*dt; break;
				case 1:	pos->y -= enemy->speed*dt; break;
				case 2:	pos->x += enemy->speed*dt; break;
				case 3:	pos->x -= enemy->speed*dt; break;
				}

				RectangleComp* rect = ecs.getComponent<RectangleComp>(handle);
				float w2 = rect->width / 2.0f;
				float h2 = rect->height / 2.0f;
				if (pos->x > 1.0f - w2) pos->x = 1.0f - w2;
				if (pos->x < -1.0f + w2) pos->x = -1.0f + w2;
				if (pos->y > 1.0f - h2) pos->y = 1.0f - h2;
				if (pos->y < -1.0f + h2) pos->y = -1.0f + h2;
			}
		}
	}
private:
};

#endif
