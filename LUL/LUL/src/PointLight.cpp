#include "PointLight.h"

PointLight::PointLight(cVec3r position, cVec3r ambient, cVec3r diffuse, cVec3r specular, 
	float attConstant, float attLinear, float attQuadratic)
	: m_Position(position), Light(ambient, diffuse, specular), 
	m_Attenuation{attConstant, attLinear, attQuadratic}
{

}

void PointLight::SetShaderValues(const Shader & shader) const
{

}
