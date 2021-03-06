#include <string>
#include <iostream>
#include "Cubemap.h"
#include "stb/stb_image.h"

Cubemap::Cubemap(const std::string & dir, const std::string * face)
{
	// loads a cubemap texture from 6 individual texture faces
	// order:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// +Z (front) 
	// -Z (back)
	// -------------------------------------------------------
	GLCALL(glGenTextures(1, &m_RendererID));
	GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

	/*load them*/
	int numOfFaces = 6;
	std::string tempName;

	for (int i = 0; i < numOfFaces; i++)
	{
		tempName = dir + '/' + face[i];
		m_LocalBuffer[i] = stbi_load(tempName.c_str(),
			&m_Width[i], &m_Height[i], &m_BPP[i], 0);

		if (m_LocalBuffer[i])
		{

		}
		else
		{
			std::cout << "STBI_LOAD FAILED:: " << tempName << std::endl;
		}

		GLCALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, m_Width[i], m_Height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer[i]));
	} // end of for

	GLCALL( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
	GLCALL( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );

	GLCALL( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
	GLCALL( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
	GLCALL( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE) );
}

Cubemap::~Cubemap()
{
}

void Cubemap::Bind(unsigned int slot) const
{
	GLCALL( glActiveTexture(GL_TEXTURE0 + slot) );
	GLCALL( glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID) );
}

void Cubemap::Unbind() const
{
}
