#include "Plane.h"

Plane::Plane()
	: m_IB(6), m_VAO()
{
	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};

	m_VB = VertexBuffer(planeVertices, sizeof(float) * 6 * 8);
	m_VBL.Push<float>(3); // vertex
	m_VBL.Push<float>(3); // normal
	m_VBL.Push<float>(2); // tex coordinate
	
	m_VAO.AddBuffer(m_VB, m_VBL);
}

Plane::~Plane()
{
}

VertexArray Plane::GetVAO() const
{
	return m_VAO;
}

IndexBuffer Plane::GetIB() const
{
	return m_IB;
}
