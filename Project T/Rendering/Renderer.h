#ifndef RENDERER_H
#define RENDERER_H

#include "..\GLAbstractions\VertexArray.h"
#include "..\GLAbstractions\IndexBuffer.h"
#include "..\Shaders\Shader.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader* shader = nullptr) const;

private:
	Shader * defaultShader;
};

#endif
