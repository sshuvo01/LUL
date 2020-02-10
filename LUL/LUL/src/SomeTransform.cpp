#include "SomeTransform.h"

void Transform::Translate(glm::mat4 & matrix, const glm::vec3 & direction)
{
	matrix = glm::translate(matrix, direction);
}

void Transform::Rotate(glm::mat4 & matrix, const float & angleInDegree, const glm::vec3 & axis)
{
	matrix =  glm::rotate(matrix, glm::radians(angleInDegree), axis);
}
