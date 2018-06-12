#include <iostream>

#include "GUI\Display.h"
#include "Rendering\Renderer.h"

#include "Maths\Vectors\Vec2.h"
#include "Maths\Vectors\Vec3.h"

#include "ECS\ECS.h"

#include <crtdbg.h>

struct PositionComp : public Component<PositionComp>
{
	PositionComp() : x(0.0f), y(0.0f), z(0.0f) {}
	PositionComp(float x, float y, float z) : x(x), y(y), z(z) {}
	float x;
	float y;
	float z;
};

struct HealthComp : public Component<HealthComp>
{
	HealthComp(float health = 100.0f) : health(health) {}
	float health;
};

struct RectangleComp : public Component<RectangleComp>
{
	RectangleComp(float width = 100.0f, float height = 100.0f) : width(width), height(height) {}
	float width;
	float height;
};

struct ModelComp : public Component<ModelComp>
{
	VertexArray va;
	IndexBuffer ib;
};

class HealthSys : public System<HealthComp, PositionComp>
{
public:
	void init()
	{
		std::cout << "Hello HealthSystem!" << std::endl;
	}
	void update(float dt, const std::vector<IComponent*>& components, Renderer* renderer, ECS& ecs)
	{
		std::cout << "Update HealthSystem!" << std::endl;
	}
};

class PositionSys : public System<PositionComp>
{
public:
	void init()
	{
		std::cout << "Hello PositionSystem!" << std::endl;
	}
	void update(float dt, const std::vector<EntityHandle>& entities, Renderer* renderer, ECS& ecs)
	{
		std::cout << "Update PositionSystem!" << std::endl;
	}
};

class RenderSys : public System<PositionComp, ModelComp>
{
public:
	void init()
	{
		std::cout << "Hello RenderSys!" << std::endl;
	}
	void update(float dt, const std::vector<EntityHandle>& entities, Renderer* renderer, ECS& ecs)
	{
		std::cout << "Update RenderSys!" << std::endl;
		ModelComp* mc;
		for (EntityHandle handle : entities)
		{
			mc = ecs.getComponent<ModelComp>(handle);
			renderer->draw(mc->va, mc->ib);
		}
	}
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Display display("Project T", 600, 600);
	Renderer renderer;

	PositionComp a, b;
	HealthComp c, d;
	std::cout << "[Position] a: id = " << a.ID << ", size = " << a.SIZE << " | b: id = " << b.ID << ", size = " << b.SIZE << std::endl;
	std::cout << "[Health] c: id = " << c.ID << ", size = " << c.SIZE << " | d: id = " << d.ID << ", size = " << d.SIZE << std::endl;

	ModelComp* mc = new ModelComp();
	struct Vertex
	{
		Vec2 pos;
		Vec3 color;
	};
	Vertex data[4] = {
		{ Vec2(-0.5f, 0.5f), Vec3(1.0f, 0.0f, 0.0f) },
		{ Vec2(0.5f, 0.5f), Vec3(0.0f, 1.0f, 0.0f) },
		{ Vec2(0.5f, -0.5f), Vec3(0.0f, 0.0f, 1.0f) },
		{ Vec2(-0.5f, -0.5f), Vec3(1.0f, 1.0f, 1.0f) }
	};
	VertexBuffer vb(data, sizeof(Vertex) * 4);
	VertexBufferLayout layout;
	layout.push<float>(2); // Pos
	layout.push<float>(3); // Color
	mc->va.addBuffer(vb, layout);
	unsigned int indices[] = { 0, 2, 1, 0, 3, 2 };
	mc->ib.make(indices, 6);


	ECS ecs;
	EntityHandle handle1 = ecs.addEntity<PositionComp, RectangleComp, ModelComp>({ new PositionComp(), new RectangleComp(), mc});
	EntityHandle handle2 = ecs.addEntity<HealthComp, PositionComp, RectangleComp>();
	//EntityHandle handle3 = ecs.addEntity<PositionComp, RectangleComp, ModelComp>();

	ecs.addSystem(new RenderSys());

	/*
	VertexArray va;
	struct Vertex
	{
		Vec2 pos;
		Vec3 color;
	};
	Vertex data[4] = {
		{ Vec2(-0.5f, 0.5f), Vec3(1.0f, 0.0f, 0.0f) },
		{ Vec2(0.5f, 0.5f), Vec3(0.0f, 1.0f, 0.0f) },
		{ Vec2(0.5f, -0.5f), Vec3(0.0f, 0.0f, 1.0f) },
		{ Vec2(-0.5f, -0.5f), Vec3(1.0f, 1.0f, 1.0f) }
	};
	VertexBuffer vb(data, sizeof(Vertex)*4);
	VertexBufferLayout layout;
	layout.push<float>(2); // Pos
	layout.push<float>(3); // Color
	va.addBuffer(vb, layout);
	unsigned int indices[] = {0, 2, 1, 0, 3, 2};
	IndexBuffer ib(indices, 6);
	*/
	Shader shader("./Resources/Shaders/test.fs", "./Resources/Shaders/test.vs");

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	while (display.isOpen())
	{
		display.processEvents();

		display.getWindowPtr()->setActive();

		ecs.updateSystems(0.1f, &renderer);
		/*
		// Opengl rendering
		shader.bind();
		va.bind();
		ib.bind();
		glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0);
		*/
		display.getWindowPtr()->display();
	}

	return 0;
}