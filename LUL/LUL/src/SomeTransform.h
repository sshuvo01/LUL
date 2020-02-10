#pragma once
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
	//these functions change the input matrix
	static void Translate(glm::mat4& matrix, const glm::vec3& direction);
	static void Rotate(glm::mat4& matrix, const float& angleInDegree, const glm::vec3& axis);
};