#ifndef COLLISION_COMP_H
#define COLLISION_COMP_H

#include "../ECS/Component.h"

struct CollisionComp : public Component<CollisionComp>
{
	CollisionComp() { hasCollided = false; }
	bool hasCollided;
};


#endif
