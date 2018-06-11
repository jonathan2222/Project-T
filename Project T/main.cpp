#include <iostream>

#include "GUI\Display.h"
#include "GLAbstractions\VertexArray.h"
#include "GLAbstractions\IndexBuffer.h"
#include "Shaders\Shader.h"

#include "Maths\Vectors\Vec2.h"
#include "Maths\Vectors\Vec3.h"

#include "ECS\ECS.h"
//#include "ECS\Entity.h"
//#include "ECS\SystemManager.h"

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
/*
struct Player : public Entity
{
	Player() : Entity()
	{
		addComponent<HealthComp>(50.0f);
		addComponent<PositionComp>(20.0f, 40.0f, 1.0f);
	};
};*/

class HealthSys : public System<HealthComp, PositionComp>
{
public:
	void init()
	{

	}
	void update(float dt)
	{

	}
	void draw()
	{

	}
};

class PositionSys : public System<PositionComp>
{
public:
	void init(){}
	void update(float dt){}
	void draw(){}
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	PositionComp a, b;
	HealthComp c, d;
	std::cout << "[Position] a: id = " << a.ID << ", size = " << a.SIZE << " | b: id = " << b.ID << ", size = " << b.SIZE << std::endl;
	std::cout << "[Health] c: id = " << c.ID << ", size = " << c.SIZE << " | d: id = " << d.ID << ", size = " << d.SIZE << std::endl;

	ECS ecs;
	EntityHandle handle = ecs.addEntity<HealthComp, PositionComp>();
	ecs.removeComponent<HealthComp>(handle);

	/*Player p;

	p.addComponent<HealthComp>(50.0f);
	p.addComponent<PositionComp>(20.0f, 40.0f, 1.0f);
	if (p.hasComponent<HealthComp>())
	{
		HealthComp& hc = p.getComponent<HealthComp>();
		std::cout << "First health: " << hc.health << std::endl;
		hc.health = 10.0f;
	}

	if (p.hasComponent<HealthComp>())
	{
		HealthComp& hc = p.getComponent<HealthComp>();
		std::cout << "New health: " << hc.health << std::endl;
	}

	if (p.hasComponent<PositionComp>())
	{
		PositionComp& pc = p.getComponent<PositionComp>();
		std::cout << "Position: " << pc.x << ", " << pc.y << ", " << pc.z << std::endl;
	}

	SystemManager sysManager;
	sysManager.addSystem<PositionSys>();
	sysManager.addSystem<HealthSys>();
	*/

	Display display("Project T", 600, 600);

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

	Shader shader("./Resources/Shaders/test.fs", "./Resources/Shaders/test.vs");

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	while (display.isOpen())
	{
		display.processEvents();

		display.getWindowPtr()->setActive();

		// Opengl rendering
		shader.bind();
		va.bind();
		ib.bind();
		glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0);
		
		display.getWindowPtr()->display();
	}

	return 0;
}