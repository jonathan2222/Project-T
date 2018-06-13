#ifndef TIMER_COMP_H
#define TIMER_COMP_H

#include "../ECS/Component.h"

struct TimeComp : public Component<TimeComp>
{
	TimeComp() : time(0.0f) {}
	float time;
};

#endif
