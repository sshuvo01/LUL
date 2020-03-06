#include "Model.h"

void Model::LoadModel(const std::string & path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR:ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// retrieve the directory path of the filepath
	m_Directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[ node->mMeshes[i] ];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; 
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;

			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoord = vec;
		}
		else
		{
			vertex.texCoord = glm::vec2(0.0f, 0.0f);
		}
		/*
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.tangent = vector;
		
		//bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.bitangent = vector;
		*/
		vertices.push_back(vertex);
	} // end of for

	for (unsigned int k = 0; k < mesh->mNumFaces; k++)
	{
		aiFace face = mesh->mFaces[k];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	} // end of for

	//IndexBuffer ib(&indices[0], indices.size());
	/*load some materials yeah*/
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	
	// diffuse maps
	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material,
		aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// specular maps
	std::vector<Texture> specularMaps = LoadMaterialTextures(material,
		aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// normal maps
	std::vector<Texture> normalMaps = LoadMaterialTextures(material,
		aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// height maps 
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, 
		aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	/*
	std::cout << vertices.size() << std::endl;
	std::cout << indices.size() << std::endl;
	std::cout << textures.size() << std::endl;
	*/
	//std::cin.get();

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial * mat, 
	aiTextureType type, const std::string & typeName)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		
		for (unsigned int j = 0; j < m_Textures.size(); j++)
		{
			std::string textureFileName = m_Textures[j].GetFileName();
			if (std::strcmp(textureFileName.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_Textures[j]);
				skip = true;
				//std::cout << "skipping texture loading, " << textureFileName << " already loaded"
					//<< std::endl;
				// texture already loaded
				break;
			} // end of if
		} // end of for
		
		if (!skip)
		{
			// texture not loaded
			std::string fName = std::string(str.C_Str());
			std::string fPath = this->m_Directory + "/" + fName;
			//std::cout << fPath << std::endl;
			Texture aTexture(fPath); // create the texture 
			aTexture.SetFileName(fName);
			aTexture.SetType(typeName);
			//for the mesh
			textures.push_back(aTexture);
			// for the whole model
			m_Textures.push_back(aTexture);
		} // end of if
	} // end of for

	return textures;
} // end of LoadMaterialTextures

Model::Model(const std::string & path, bool gamma)
	: m_GammaCorrection(gamma)
{
	LoadModel(path);
}

void Model::Draw(const Shader & shader, const Renderer & renderer) const
{
	for (unsigned int i = 0; i < m_Meshes.size(); i++)
	{
		m_Meshes[i].Draw(shader, renderer);
	} // end of forrrrr
}
