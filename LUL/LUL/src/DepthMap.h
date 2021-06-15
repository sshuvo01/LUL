#pragma once
#include "Renderer.h"

class DepthMap
{
private:
	unsigned int		m_FramebufferID;
	unsigned int		m_DepthMapID;
	unsigned int		m_Width;
	unsigned int		m_Height;
public:
	DepthMap(unsigned int width = 1024, unsigned int height = 1024);
	~DepthMap();

	void BindFramebuffer() const;
	void BindTexture(unsigned int slot = 0) const;
	void UnbindFramebuffer() const;
	/*getters*/
	inline unsigned int GetWidht() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }
};