#ifndef ENEMY_COMP_H
#define ENEMY_COMP_H

#include "../ECS/Component.h"
#include <random>

struct EnemyComp : public Component<EnemyComp>
{
	EnemyComp(float speed = 0.5f) : speed(speed) { isDead = false; this->dir = std::rand() % 4; }
	bool isDead;
	float speed;
	int dir;
};

#endif