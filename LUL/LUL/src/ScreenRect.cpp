#include "ScreenRect.h"

ScreenRect::ScreenRect()
	: m_VAO()
{
	float quadVertices[] = 
	{
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	unsigned int indices[] = { 0, 1, 2, 2, 1, 3 };
	m_IB = IndexBuffer(indices, 6);

	m_VB = VertexBuffer(quadVertices, sizeof(float) * 20);
	m_VBL.Push<float>(3); // positions
	m_VBL.Push<float>(2); // texture Coords
	m_VAO.AddBuffer(m_VB, m_VBL);
}

ScreenRect::~ScreenRect()
{
}

VertexArray ScreenRect::GetVAO() const
{
	return m_VAO;
}

IndexBuffer ScreenRect::GetIB() const
{
	return m_IB;
}
