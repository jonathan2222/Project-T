#ifndef SPAWNER_SYS_H
#define SPAWNER_SYS_H

#include "../Components/SpawnerComp.h"
#include "../Components/CollisionComp.h"
#include "../ECS/ECS.h"
#include <iostream>

#include "../Components/EnemyComp.h"
#include "../Components/PositionComp.h"
#include "../Components/RectangleComp.h"
#include "../Components/ColorComp.h"
#include "../Components/ModelComp.h"
#include "../Components/TimerComp.h"

#include "../World.h"

class SpawnerSys : public System<SpawnerComp, CollisionComp, ColorComp>
{
public:
	void init(const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		for (EntityHandle handle : entities)
		{
			SpawnerComp* spawner = ecs.getComponent<SpawnerComp>(handle);
			spawner->pos.x = ((std::rand() % 100) / 100.0f)*2.0f - 1.0f;
			spawner->pos.y = ((std::rand() % 100) / 100.0f)*2.0f - 1.0f;
			std::cout << "SpawnerSys INIT" << std::endl;
		}
	}
	void update(float dt, const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		unsigned int numEntities = entities.size();
		for (unsigned int i = 0; i < numEntities; i++)
		{
			EntityHandle handle = entities[i];
			SpawnerComp* spawner = ecs.getComponent<SpawnerComp>(handle);
			CollisionComp* coll = ecs.getComponent<CollisionComp>(handle);
			if ((Utils::isBitSet<Bitmask>(coll->collidedWith, getComponentTypeID<PlayerComp>()) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) && spawner->hasSpawnd == false)
			{
				//Spawn
				spawner->pos.x = ((std::rand() % 100) / 100.0f)*2.0f - 1.0f;
				spawner->pos.y = ((std::rand() % 100) / 100.0f)*2.0f - 1.0f;

				ecs.addEntity<PositionComp, RectangleComp, ModelComp, EnemyComp, ColorComp, CollisionComp, TimeComp, SpawnerComp>(
					{ new PositionComp(spawner->pos.x, spawner->pos.y), new RectangleComp(0.05f, 0.05f), new ModelComp(container->getNumModels() - 1), new EnemyComp(), new ColorComp(), new CollisionComp(), new TimeComp(), new SpawnerComp() }
				);
				ColorComp* color = ecs.getComponent<ColorComp>(handle);
				color->r *= 0.5f;
				color->g *= 0.5f;
				color->b *= 0.5f;
				spawner->hasSpawnd = true;
			}
		}
	}
};

#endif
