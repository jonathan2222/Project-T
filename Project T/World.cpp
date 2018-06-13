#include "World.h"

#include "Maths\Vectors\Vec2.h"
#include "Maths\Vectors\Vec3.h"

World::World()
{
	this->mc = createModelComp();
}

World::~World()
{
	delete this->mc;
}

ModelComp * World::getModelComp() const
{
	return this->mc;
}

ModelComp * World::createModelComp() const
{
	ModelComp* m = new ModelComp();
	struct Vertex
	{
		Vec2 pos;
		Vec3 color;
	};
	Vertex data[4] = {
		{ Vec2(-1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f) },
	{ Vec2(1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f) },
	{ Vec2(1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f) },
	{ Vec2(-1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f) }
	};
	VertexBuffer vb(data, sizeof(Vertex) * 4);
	VertexBufferLayout layout;
	layout.push<float>(2); // Pos
	layout.push<float>(3); // Color
	m->va.addBuffer(vb, layout);
	unsigned int indices[] = { 0, 2, 1, 0, 3, 2 };
	m->ib.make(indices, 6);
	return m;
}
