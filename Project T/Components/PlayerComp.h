#ifndef PLAYER_COMP_H
#define PLAYER_COMP_H

#include "../ECS/Component.h"

struct PlayerComp : public Component<PlayerComp>
{
	PlayerComp(float speed = 0.7f) : speed(speed) {}
	float speed;
};

#endif 
