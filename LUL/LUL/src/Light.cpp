#include "Light.h"

Light::Light(cVec3r ambient, cVec3r diffuse, cVec3r specular)
	: m_Property{ ambient, diffuse, specular }
{

}

Light::Light(const LightProperty & property)
	: m_Property(property)
{

}
