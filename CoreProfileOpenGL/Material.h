#pragma once
#include <glm/glm.hpp>

class Material
{
public:
	glm::vec3 aDiffuse;
	glm::vec3 aSpecular;
	float aShininess;

	~Material() {}

	// Constructor with vectors
	Material(glm::vec3 pColor, float pShininess)
	{
		this->aDiffuse = pColor;
		this->aSpecular = glm::vec3(0.3f);
		this->aShininess = pShininess;
	}
};