#ifndef SPAWNSER_COMP_H
#define SPAWNSER_COMP_H

#include "../ECS/Component.h"
#include "../Maths/Vectors/Vec2.h"

struct SpawnerComp : public Component<SpawnerComp>
{
	SpawnerComp() : hasSpawnd(false), pos(0.0f, 0.0f) {}
	Vec2 pos;
	bool hasSpawnd;
};

#endif