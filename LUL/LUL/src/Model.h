#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

#include "Mesh.h"
#include "Texture.h"
#include "IndexBuffer.h"

class Model
{
private:
	std::vector<Texture> m_Textures;
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;
	bool m_GammaCorrection;

	/* functions */
	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, 
		aiTextureType type, const std::string& typeName);
public:
	Model(const std::string& path, bool gamma = false);
	void Draw(const Shader& shader, const Renderer& renderer) const;
};
