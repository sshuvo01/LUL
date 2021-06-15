#pragma once
#include <glm/vec3.hpp>

struct LightProperty
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

typedef const glm::vec3& cVec3r;

class Light
{
private:
	LightProperty m_Property;
public:
	Light(cVec3r ambient, cVec3r diffuse, cVec3r specular);
	Light(const LightProperty& property);
	Light() {} // empty constructor
	
	// setters
	inline void SetAmbient(cVec3r ambient) { m_Property.ambient = ambient; }
	inline void SetDiffuse(cVec3r diffuse) { m_Property.diffuse = diffuse; }
	inline void SetSpecular(cVec3r specular) { m_Property.specular = specular; }
	inline void SetProperty(const LightProperty& lp) { m_Property = lp; }

	// getters
	inline glm::vec3 GetAmbient() const { return m_Property.ambient; }
	inline glm::vec3 GetDiffuse() const { return m_Property.diffuse; }
	inline glm::vec3 GetSpecular() const { return m_Property.specular; }
	inline LightProperty GetProperty() const { return m_Property; }
};