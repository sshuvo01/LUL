#pragma once
#include <iostream>
#include <vector>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	//glm::vec3 tangent;
	//glm::vec3 bitangent;
};

class Mesh
{
private:
	// Mesh data
	std::vector<Vertex> m_Vertices;
	std::vector<Texture> m_Textures;
	
	// Render data
	VertexBuffer m_VB;
	VertexArray m_VA;
	IndexBuffer m_IB;

	// private functions
	void SetupMesh();
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
		const std::vector<Texture>& textures);
	void Draw(const Shader& shader, const Renderer& renderer) const;

};