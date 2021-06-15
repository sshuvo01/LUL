#pragma once
#include "VertexBuffer.h"

enum class DataType
{
	MAT3 = 9, MAT4 = 16, VEC2 = 2, VEC3 = 3, VEC4 = 4
};

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int	m_stride;
	unsigned int	m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl);
	void AddInstanceBuffer(const VertexBuffer& vb, DataType type, unsigned int index,
		unsigned int divisor);
	void Bind() const;
	void Unbind() const;
};