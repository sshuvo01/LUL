#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

class Plane
{
private:
	VertexBuffer			m_VB;
	VertexArray				m_VAO;
	VertexBufferLayout		m_VBL;
	IndexBuffer				m_IB;
public:
	Plane();
	~Plane();

	////////
	VertexArray GetVAO() const;
	IndexBuffer GetIB() const;
};