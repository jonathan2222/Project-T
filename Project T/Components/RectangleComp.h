#ifndef RECTANGLE_COMP_H
#define RECTANGLE_COMP_H

#include "../ECS/Component.h"

struct RectangleComp : public Component<RectangleComp>
{
	RectangleComp(float width = 0.5f, float height = 0.5f) : width(width), height(height) {}
	float width;
	float height;
};

#endif 
