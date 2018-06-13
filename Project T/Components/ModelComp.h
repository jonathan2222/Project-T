#ifndef MODEL_COMP_H
#define MODEL_COMP_H

#include "../ECS/Component.h"

struct ModelComp : public Component<ModelComp>
{
	ModelComp(unsigned int modelIndex = 0) : modelIndex(modelIndex) {}
	unsigned int modelIndex;
};

#endif
