#ifndef MODEL_COMP_H
#define MODEL_COMP_H

#include "../ECS/Component.h"
#include "../GLAbstractions/VertexArray.h"
#include "../GLAbstractions/IndexBuffer.h"

struct ModelComp : public Component<ModelComp>
{
	VertexArray va;
	IndexBuffer ib;
};

#endif
