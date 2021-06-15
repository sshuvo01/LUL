#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
	{

	}
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OPENGL ERROR] (" << error <<
			") " << function << " " << file << ", " << line <<
			std::endl;
		return false; // error occurred
	}
	return true;
}

Renderer::Renderer(unsigned int screenW, unsigned int screenH, bool createFramebuffer, bool hdrFramebuffer)
	: m_ScreenWidth(screenW), m_ScreenHeight(screenH), m_HasFrameBuffer(createFramebuffer),
	m_ClearColor(0.2, 0.2, 0.25, 1) 
{
	InitSettings();
	if (!createFramebuffer) return;
	/*a custom frame buffer*/
	CreateFramebuffer(hdrFramebuffer);
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader,
	PrimitiveShape shape) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	GLenum GLShape;

	switch (shape)
	{
	case PrimitiveShape::TRIANGLE:
		GLShape = GL_TRIANGLES;
		break;
	case PrimitiveShape::POINT:
		GLShape = GL_POINTS;
		break;
	default:
		break;
	}
	
	GLCALL(glDrawElements(GLShape, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear(bool color, bool depth, bool stencil, bool accum) const
{
	GLbitfield mask = 0;

	if (color)
	{
		mask = mask | GL_COLOR_BUFFER_BIT;
	}
	if (depth)
	{
		mask = mask | GL_DEPTH_BUFFER_BIT;
	}
	if (stencil)
	{
		mask = mask | GL_STENCIL_BUFFER_BIT;
	}
	if (accum)
	{
		mask = mask | GL_ACCUM_BUFFER_BIT;
	}

	if (mask != 0) GLCALL(glClear(mask));
}

void Renderer::SetClearColor(const glm::vec4 & color)
{
	m_ClearColor = color;
}

void Renderer::EnableDepthTest(bool state) const
{
	if (state)
	{
		GLCALL(glEnable(GL_DEPTH_TEST));
	}
	else
	{
		GLCALL(glDisable(GL_DEPTH_TEST));
	}
}

void Renderer::EnableStencilTest(bool state) const
{
	if (state)
	{
		GLCALL(glEnable(GL_STENCIL_TEST));
	}
	else
	{
		GLCALL(glDisable(GL_STENCIL_TEST));
	}
}

void Renderer::EnableDepthWriting(bool state) const
{
	if (state)
	{
		GLCALL(glDepthMask(GL_TRUE));
	}
	else
	{
		GLCALL(glDepthMask(GL_FALSE));
	}
}

void Renderer::BindFramebuffer() const
{
	if (!m_HasFrameBuffer) return;
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID));
}

void Renderer::UnbindFramebuffer() const
{
	if (!m_HasFrameBuffer) return;
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0)); // default framebuffer
}

void Renderer::BindTexture(unsigned int slot) const
{
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_TextureColorBufferID));
}

void Renderer::ChangeViewport(unsigned int width, unsigned int height, unsigned int x, unsigned int y) const
{
	GLCALL(glViewport(x, y, width, height));
}

void Renderer::ChangeViewport() const
{
	// default
	GLCALL(glViewport(0, 0, m_ScreenWidth, m_ScreenHeight));
}

void Renderer::InitSettings() const
{
	//GLCALL(glClearColor(0.2, 0.2, 0.25, 1));
	GLCALL(glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w));
	//glEnable(GL_BLEND);
	//GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCALL(glEnable(GL_DEPTH_TEST));
	//GLCALL(glDepthFunc(GL_LEQUAL));
	//GLCALL(glEnable(GL_STENCIL_TEST));
}

void Renderer::InitBlending() const
{
	GLCALL( glEnable(GL_BLEND) );
	GLCALL( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
	GLCALL( glBlendEquation(GL_FUNC_ADD) );
}

void Renderer::CreateFramebuffer(bool isHdr)
{
	/*for custom frame buffers*/
	GLCALL(glGenFramebuffers(1, &m_FramebufferID));
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID));
	//color attachment texture
	GLCALL(glGenTextures(1, &m_TextureColorBufferID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_TextureColorBufferID));
	
	if (isHdr)
	{
		GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_ScreenWidth,
			m_ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL));
	}
	else 
	{
		GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_ScreenWidth,
			m_ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	}

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, m_TextureColorBufferID, 0));
	//create renderbuffer
	GLCALL(glGenRenderbuffers(1, &m_RenderbufferObjectID));
	GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderbufferObjectID));
	GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
		m_ScreenWidth, m_ScreenHeight));
	GLCALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER, m_RenderbufferObjectID));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR:: framebuffer not complete" << std::endl;
		m_HasFrameBuffer = false;
	}
	/*unbind the created framebuffer*/
	UnbindFramebuffer();
}
