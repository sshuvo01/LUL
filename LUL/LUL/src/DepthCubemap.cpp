#include "DepthCubemap.h"
#include <iostream>

DepthCubemap::DepthCubemap(unsigned int width, unsigned int height)
	:m_Width(width), m_Height(height)
{
	GLCALL( glGenFramebuffers(1, &m_FramebufferID) );
	GLCALL(glGenTextures(1, &m_DepthCubemapID) );
	GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthCubemapID));

	for (unsigned int i = 0; i < 6; i++)
	{
		GLCALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_DEPTH_COMPONENT, 
			m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	} // end of for
	
	GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	// attach depth texture as FBO's depth buffer
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID));
	GLCALL(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
		m_DepthCubemapID, 0));
	GLCALL(glDrawBuffer(GL_NONE));
	GLCALL(glReadBuffer(GL_NONE));

	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		std::cout << "framebuffer fucked up" << std::endl;
	}

	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void DepthCubemap::BindFramebuffer() const
{
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID));
}

void DepthCubemap::BindCubemap(unsigned int slot) const
{
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthCubemapID));
}

void DepthCubemap::UnbindFramebuffer() const
{
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
