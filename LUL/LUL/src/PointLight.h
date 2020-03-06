#pragma once
#include "Light.h"
#include "Shader.h"

struct LightAttenuation
{
	float constant;
	float linear;
	float quadratic;
};

class PointLight : public Light
{
private:
	glm::vec3 m_Position;
	LightAttenuation m_Attenuation;
public:
	PointLight(cVec3r position, cVec3r ambient, cVec3r diffuse, cVec3r specular,
		float attConstant, float attLinear, float attQuadratic);
	PointLight() {} // 

	// setters
	inline void SetPosition(cVec3r position) { m_Position = position; }
	inline void SetAttenuation(const LightAttenuation& la) { m_Attenuation = la; }
	// getters
	inline glm::vec3 GetPosition() const { return m_Position; }
	inline LightAttenuation GetAttenuation() const { return m_Attenuation; }

	// other functions
	void SetShaderValues(const Shader& shader) const;
};