#ifndef RENDERING_SYS_H
#define RENDERING_SYS_H

#include "../Components/PositionComp.h"
#include "../Components/ModelComp.h"
#include "../Components/RectangleComp.h"
#include "../Components/ColorComp.h"
#include "../ECS/ECS.h"
#include <iostream>

class RenderingSys : public System<PositionComp, ModelComp, RectangleComp, ColorComp>
{
public:
	void init(const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		for (EntityHandle handle : entities)
		{
			std::cout << "RenderSys INIT" << std::endl;
		}
	}
	void update(float dt, const std::vector<EntityHandle>& entities, ECS& ecs, Container* container)
	{
		//std::cout << "Update RenderSys!" << std::endl;
		ModelComp* mc;
		PositionComp* pos;
		RectangleComp* rect;
		ColorComp* color;
		unsigned int numEntities = entities.size();
		for (unsigned int i = 0; i < numEntities; i++)
		{
			EntityHandle handle = entities[i];
			mc = ecs.getComponent<ModelComp>(handle);
			pos = ecs.getComponent<PositionComp>(handle);
			rect = ecs.getComponent<RectangleComp>(handle);
			color = ecs.getComponent<ColorComp>(handle);
			container->getRenderer()->bindShader();
			container->getRenderer()->getDefaultShader()->setUniform2f("offset", pos->x, pos->y);
			container->getRenderer()->getDefaultShader()->setUniform2f("scale", rect->width / 2.0f, rect->height / 2.0f);
			container->getRenderer()->getDefaultShader()->setUniform3f("tint", color->r, color->g, color->b);

			Model* m = container->getModel(mc->modelIndex);
			container->getRenderer()->draw(m->va, m->ib);
		}
	}
};

#endif