#ifndef COLOR_COMP_H
#define COLOR_COMP_H

#include "../ECS/Component.h"

struct ColorComp : public Component<ColorComp>
{
	ColorComp(float r = 0.0f, float g = 0.0f, float b = 0.0f) : r(r), g(g), b(b) {}
	float r, g, b;
};


#endif
