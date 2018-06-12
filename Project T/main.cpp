#include <iostream>

#include "GUI\Display.h"
#include "Rendering\Renderer.h"

#include "Maths\Vectors\Vec2.h"
#include "Maths\Vectors\Vec3.h"

#include "ECS\ECS.h"

#include <random>
#include "Utils\Timer.h"
#include <crtdbg.h>

// ---------------------------------------- COMPONENTS ----------------------------------------

struct TimeComp : public Component<TimeComp>
{
	TimeComp() : time(0.0f) {}
	float time;
};

struct PositionComp : public Component<PositionComp>
{
	PositionComp() : x(0.0f), y(0.0f) {}
	PositionComp(float x, float y) : x(x), y(y) {}
	float x, y;
};

struct CollisionComp : public Component<CollisionComp>
{
	CollisionComp() { hasCollided = false; }
	bool hasCollided;
};

struct ColorComp : public Component<ColorComp>
{
	ColorComp(float r, float g, float b) : r(r), g(g), b(b) {}
	float r, g, b;
};

struct EnemyComp : public Component<EnemyComp>
{
	EnemyComp(float speed = 0.025f) : speed(speed) { isDead = false; this->dir = std::rand() % 4; }
	bool isDead;
	float speed;
	int dir;
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

struct PlayerComp : public Component<PlayerComp>
{
	PlayerComp(float speed = 0.025f) : speed(speed) {}
	float speed;
};

// ---------------------------------------- SYSTEMS ----------------------------------------

class ColorChangerSys : public System<ColorComp, CollisionComp>
{
public:
	void init() {}
	void update(float dt, const std::vector<EntityHandle>& entities, Renderer* renderer, ECS& ecs)
	{
		for (EntityHandle handle : entities)
		{
			ColorComp* color = ecs.getComponent<ColorComp>(handle);
			CollisionComp* coll = ecs.getComponent<CollisionComp>(handle);
			if (coll->hasCollided)
				color->r = color->g = color->b = 0.25f;
		}
	}
};

class CollisionSys : public System<PositionComp, RectangleComp, CollisionComp>
{
public:
	void init() {}
	void update(float dt, const std::vector<EntityHandle>& entities, Renderer* renderer, ECS& ecs)
	{
		for (EntityHandle handle : entities)
		{
			PositionComp* pos = ecs.getComponent<PositionComp>(handle);
			RectangleComp* rect = ecs.getComponent<RectangleComp>(handle);
			CollisionComp* coll = ecs.getComponent<CollisionComp>(handle);

			bool collided = false;
			for (EntityHandle handle2 : entities)
			{
				if (handle2 != handle)
				{
					PositionComp* pos2 = ecs.getComponent<PositionComp>(handle2);
					RectangleComp* rect2 = ecs.getComponent<RectangleComp>(handle2);

					if (std::max(pos->x - rect->width / 2.0f, pos2->x - rect2->width / 2.0f) < 
						std::min(pos->x + rect->width / 2.0f, pos2->x + rect2->width / 2.0f))
						if (std::max(pos->y - rect->height / 2.0f, pos2->y - rect2->height / 2.0f) < 
							std::min(pos->y + rect->height / 2.0f, pos2->y + rect2->height / 2.0f))
						{
							// Collision!
							collided = true;							
						}
				}
			}
			coll->hasCollided = collided;
		}
	}
private:
};

class EnemySys : public System<EnemyComp, PositionComp, RectangleComp, TimeComp>
{
public:
	void init(){}
	void update(float dt, const std::vector<EntityHandle>& entities, Renderer* renderer, ECS& ecs)
	{
		for (EntityHandle handle : entities)
		{
			PositionComp* pos = ecs.getComponent<PositionComp>(handle);
			EnemyComp* enemy = ecs.getComponent<EnemyComp>(handle);
			TimeComp* time = ecs.getComponent<TimeComp>(handle);
			time->time += dt;

			if (!enemy->isDead)
			{
				if (time->time > 2.5f)
				{
					enemy->dir = std::rand() % 4;
					time->time = 0.0f;
				}
				switch (enemy->dir)
				{
				case 0: pos->y += enemy->speed*dt; break;
				case 1:	pos->y -= enemy->speed*dt; break;
				case 2:	pos->x += enemy->speed*dt; break;
				case 3:	pos->x -= enemy->speed*dt; break;
				}

				RectangleComp* rect = ecs.getComponent<RectangleComp>(handle);
				float w2 = rect->width / 2.0f;
				float h2 = rect->height / 2.0f;
				if (pos->x > 1.0f - w2) pos->x = 1.0f - w2;
				if (pos->x < -1.0f + w2) pos->x = -1.0f + w2;
				if (pos->y > 1.0f - h2) pos->y = 1.0f - h2;
				if (pos->y < -1.0f + h2) pos->y = -1.0f + h2;
			}
		}
	}
private:
};

class PlayerSys : public System<PositionComp, PlayerComp, RectangleComp>
{
public:
	void init(){}
	void update(float dt, const std::vector<EntityHandle>& entities, Renderer* renderer, ECS& ecs)
	{
		for (EntityHandle handle : entities)
		{
			PositionComp* pos = ecs.getComponent<PositionComp>(handle);
			PlayerComp* player = ecs.getComponent<PlayerComp>(handle);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) pos->x -= player->speed*dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) pos->x += player->speed*dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) pos->y += player->speed*dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) pos->y -= player->speed*dt;

			RectangleComp* rect = ecs.getComponent<RectangleComp>(handle);
			float w2 = rect->width/2.0f;
			float h2 = rect->height / 2.0f;
			if (pos->x > 1.0f - w2) pos->x = 1.0f - w2;
			if (pos->x < -1.0f + w2) pos->x = -1.0f + w2;
			if (pos->y > 1.0f - h2) pos->y = 1.0f - h2;
			if (pos->y < -1.0f + h2) pos->y = -1.0f + h2;
		}
	}
};

class RenderSys : public System<PositionComp, ModelComp, RectangleComp, ColorComp>
{
public:
	void init(){}
	void update(float dt, const std::vector<EntityHandle>& entities, Renderer* renderer, ECS& ecs)
	{
		//std::cout << "Update RenderSys!" << std::endl;
		ModelComp* mc;
		PositionComp* pos;
		RectangleComp* rect;
		ColorComp* color;
		for (EntityHandle handle : entities)
		{
			mc = ecs.getComponent<ModelComp>(handle);
			pos = ecs.getComponent<PositionComp>(handle);
			rect = ecs.getComponent<RectangleComp>(handle);
			color = ecs.getComponent<ColorComp>(handle);
			renderer->bindShader();
			renderer->getDefaultShader()->setUniform2fv("offset", 1, &Vec2(pos->x, pos->y)[0]);
			renderer->getDefaultShader()->setUniform2f("scale", rect->width/2.0f, rect->height/2.0f);
			renderer->getDefaultShader()->setUniform3f("tint", color->r, color->g, color->b);
			renderer->draw(mc->va, mc->ib);
		}
	}
};

// ---------------------------------------- MAIN ----------------------------------------

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::srand(std::time(nullptr));

	Display display("Project T", 600, 600);
	Renderer renderer;

	ModelComp* mc = new ModelComp();
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
	mc->va.addBuffer(vb, layout);
	unsigned int indices[] = { 0, 2, 1, 0, 3, 2 };
	mc->ib.make(indices, 6);


	ECS ecs;
	/*
	
	TODO: Make it so the user can use same Component on many entities!
	
	*/
	EntityHandle handle1 = ecs.addEntity<PositionComp, RectangleComp, ModelComp, PlayerComp, ColorComp, CollisionComp>(
		{ new PositionComp(0.0f, 0.0f), new RectangleComp(0.1f, 0.1f), mc, new PlayerComp(), new ColorComp(0.0f, 0.0f, 1.0f), new CollisionComp() }
	);
	EntityHandle handle2 = ecs.addEntity<PositionComp, RectangleComp, ModelComp, EnemyComp, ColorComp, CollisionComp, TimeComp>(
		{ new PositionComp(0.5f, 0.2f), new RectangleComp(0.05f, 0.05f), mc, new EnemyComp(), new ColorComp(1.0f, 0.0f, 0.0f), new CollisionComp(), new TimeComp() }
	);
	EntityHandle handle3 = ecs.addEntity<PositionComp, RectangleComp, ModelComp, EnemyComp, ColorComp, CollisionComp, TimeComp>(
		{ new PositionComp(-0.5f, -0.2f), new RectangleComp(0.2f, 0.2f), mc, new EnemyComp(), new ColorComp(1.0f, 0.0f, 0.0f), new CollisionComp(), new TimeComp() }
	);

	ecs.addSystem(new ColorChangerSys());
	ecs.addSystem(new CollisionSys());
	ecs.addSystem(new EnemySys());
	ecs.addSystem(new PlayerSys());
	ecs.addSystem(new RenderSys());


	Shader shader("./Resources/Shaders/test.fs", "./Resources/Shaders/test.vs");

	float dt = 0.0f;
	Timer timer;
	

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	while (display.isOpen())
	{
		display.processEvents();

		display.getWindowPtr()->setActive();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ecs.updateSystems(dt, &renderer);

		display.setTitleSufix(" dt: " + std::to_string(dt));

		timer.update();
		if (timer.getDeltaTime() > 1.0f / 60.0f)
		{
			dt = timer.getDeltaTime();
			timer.restart();
		}

		display.getWindowPtr()->display();
	}

	return 0;
}