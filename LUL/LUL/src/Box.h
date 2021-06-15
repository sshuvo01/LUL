#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

class Box
{
private:
	VertexBuffer m_VB;
	VertexArray m_VAO;
	VertexBufferLayout m_VBL;
	IndexBuffer m_IB;
public:
	Box();
	~Box();

	////////
	VertexArray GetVAO() const;
	IndexBuffer GetIB() const;
};
