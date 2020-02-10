#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCALL( glGenVertexArrays(1, &m_RendererID) );
	GLCALL( glBindVertexArray(m_RendererID) );
}

VertexArray::~VertexArray()
{
	GLCALL( glDeleteVertexArrays(1, &m_RendererID) ); 
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl)
{
	vb.Bind();
	auto& elements = vbl.GetElements();
	
	for (int i = 0; i < elements.size(); i++)
	{
		auto& elm = elements[i];
		
		GLCALL( glEnableVertexAttribArray(i) );
		GLCALL( glVertexAttribPointer(i, elm.count, elm.type, elm.normalized, 
			vbl.GetStride(), (const void*) elm.offset) );
	}
}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCALL(glBindVertexArray(0));
}
