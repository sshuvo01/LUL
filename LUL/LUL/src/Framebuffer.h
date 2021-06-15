#pragma once
#include "Renderer.h"

struct FramebufferSettings
{
	TextureRepeatMode texRepMode = TextureRepeatMode::REPEAT;
	TextureFilteringMode texFiltMode = TextureFilteringMode::LINEAR;
};

class Framebuffer
{
private:
	unsigned int			m_Width;
	unsigned int			m_Height;
	bool					m_IsHDR;
	FramebufferSettings		m_Settings;

	unsigned int			m_FramebufferID;
	unsigned int			m_RenderbufferID;
	unsigned int*			m_ColorbuffersID;
	unsigned int			m_ColorbuffesCount;
public:
	Framebuffer
	(
		unsigned int width, unsigned int height, 
		FramebufferSettings settings = FramebufferSettings(), 
		unsigned int colorbufferCountbool = 1, 
		bool isHDR = true
	);
	~Framebuffer();
	//Framebuffer(const Framebuffer&) = delete;
	//Framebuffer(const Framebuffer&&) = delete;
	//Framebuffer& operator=(const Framebuffer&) = delete;
	//Framebuffer& operator=(const Framebuffer&&) = delete;
	
	void BindFramebuffer() const;
	void UnbindFramebuffer() const;
	void BindTexture(unsigned int slot = 0, unsigned int colorbufferIndex = 0) const;
};


