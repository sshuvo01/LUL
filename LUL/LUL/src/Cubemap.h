#pragma once
#include "Renderer.h"
#include "stb/stb_image.h"

class Cubemap
{
private:
	unsigned int		m_RendererID;
	std::string			m_Directory;			/*parent directory*/
	std::string			m_FacesName[6];			/*6 faces*/
	unsigned char*		m_LocalBuffer[6];
	int					m_Width[6], m_Height[6], m_BPP[6];
public:
	Cubemap(const std::string& dir, const std::string* face);
	~Cubemap();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
};