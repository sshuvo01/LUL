#pragma once
#include "Renderer.h"
#include "stb/stb_image.h"

class Texture 
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::string m_FileName;
	std::string m_Type;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& path, bool flipUV = false);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;


	/* setters and getters */
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline std::string GetType() const { return m_Type; }
	inline void SetType(const std::string& type) { m_Type = type; }
	inline std::string GetFilePath() const { return m_FilePath; }
	inline std::string GetFileName() const { return m_FileName; }
	inline void SetFileName(const std::string& fileName) { m_FileName = fileName; }
};
