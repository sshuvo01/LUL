#pragma once
#include "Renderer.h"

class DepthCubemap
{
//private:
public:
	unsigned int		m_FramebufferID;
	unsigned int		m_DepthCubemapID;
	unsigned int		m_Width;
	unsigned int		m_Height;

	DepthCubemap(unsigned int width = 1024, unsigned int height = 1024);
	void BindFramebuffer() const;
	void BindCubemap(unsigned int slot = 0) const;
	void UnbindFramebuffer() const;

	// getters
	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }
};