#include <string>
#include <iostream>
#include "Texture.h"
#include "stb/stb_image.h"

Texture::Texture(const std::string & path, bool gamma,  GLenum repeatMode, bool flipUV)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
	  m_Width(0), m_Height(0), m_BPP(0)
{
	if(flipUV) stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0);

	if (m_LocalBuffer)
	{

	}
	else
	{
		std::cout << "STBI_LOAD FAILED:: " << path << std::endl;
	}

	GLenum format;
	GLenum internalFormat;

	if (m_BPP == 1)
	{
		format = GL_RED;
		internalFormat = GL_RED;
	}
	else if (m_BPP == 3)
	{
		format = GL_RGB;
		internalFormat = gamma ? GL_SRGB : GL_RGB;
	}
	else if (m_BPP == 4)
	{
		format = GL_RGBA;
		internalFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
	}
	
	{
		std::cout << "number of channel: " << m_BPP << std::endl;
		std::cout << m_FilePath << std::endl;
	}


	GLCALL(glGenTextures(1, &m_RendererID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer));

	GLCALL( glGenerateMipmap(GL_TEXTURE_2D) );

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatMode));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatMode));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	//GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
	//Bind(0);
}

Texture::~Texture()
{
	//GLCALL(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	//GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}
