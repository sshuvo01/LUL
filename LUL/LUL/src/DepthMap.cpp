#include <string>
#include <iostream>
#include "DepthMap.h"

DepthMap::DepthMap(unsigned int width, unsigned int height)
	: m_Width(width), m_Height(height)
{
	GLCALL(glGenFramebuffers(1, &m_FramebufferID));
	GLCALL(glGenTextures(1, &m_DepthMapID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_DepthMapID));
	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLCALL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

	/*bind the frame buffer*/
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID));
	GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
		GL_TEXTURE_2D, m_DepthMapID, 0));
	GLCALL(glDrawBuffer(GL_NONE));
	GLCALL(glReadBuffer(GL_NONE));
	UnbindFramebuffer();
}

DepthMap::~DepthMap()
{
}

void DepthMap::BindTexture(unsigned int slot) const
{
	GLCALL(glActiveTexture(GL_TEXTURE0+slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_DepthMapID));
}

void DepthMap::BindFramebuffer() const
{
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID));
}

void DepthMap::UnbindFramebuffer() const
{
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
