#ifndef POSITION_COMP_H
#define POSITION_COMP_H

#include "../ECS/Component.h"

struct PositionComp : public Component<PositionComp>
{
	PositionComp() : x(0.0f), y(0.0f) {}
	PositionComp(float x, float y) : x(x), y(y) {}
	float x, y;
};

#endif
