#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
	: m_VB(&vertices[0], sizeof(Vertex) * vertices.size()), m_VA(), m_IB(&indices[0], indices.size())
{
	m_Vertices = vertices;
	m_Textures = textures;

	/*setup layout*/
	VertexBufferLayout vbl;
	vbl.Push<float>(3); // position
	vbl.Push<float>(3); // normal
	vbl.Push<float>(2); // tex coordinate

	/*vertex array object*/
	m_VA.AddBuffer(m_VB, vbl);
}

void Mesh::Draw(const Shader & shader, const Renderer & renderer) const
{
	/*process texture*/
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		std::string aName = m_Textures[i].GetType();
		std::string number;

		/*--------*/
		if (aName == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (aName == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}
		else if (aName == "texture_normal")
		{
			number = std::to_string(normalNr++);
		}
		else if (aName == "texture_height")
		{
			number = std::to_string(heightNr++);
		}
		/*--------*/
		std::string textureUnitName = aName + number;
		m_Textures[i].Bind(i); 
		shader.SetUniform1i(textureUnitName, i);
	} // end of for

	renderer.Draw(m_VA, m_IB, shader);
}
