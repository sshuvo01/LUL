#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x) GLClearError();x;ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
private:
	unsigned int	m_FramebufferID;
	unsigned int	m_TextureColorBufferID;
	unsigned int	m_RenderbufferObjectID;
	bool			m_HasFrameBuffer;
	glm::vec4		m_ClearColor;
	/*screen information*/
	unsigned int	m_ScreenWidth;
	unsigned int	m_ScreenHeight;
public:
	Renderer(unsigned int screenW, unsigned int screenH, bool createFramebuffer = false);
	/*-----------*/
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Clear(bool color, bool depth, bool stencil = false, bool accum = false) const;
	void SetClearColor(const glm::vec4& color);
	void EnableDepthTest(bool state = true) const;
	void EnableStencilTest(bool state = true) const;
	void BindFramebuffer() const;
	void UnbindFramebuffer() const;
	void BindTexture(unsigned int slot) const;
	/*-----------*/
	void InitSettings() const;
	void InitBlending() const;
private:
	void CreateFramebuffer();
};