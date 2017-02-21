#pragma once
#include <glm/glm.hpp>

class Spotlight
{
public:
	// Camera Attributes
	glm::vec3 aColor;

	glm::vec3 aPosition;
	glm::vec3 aTarget;
	glm::vec3 aDirection;

	float aCutoff;
	float aOuterCutoff;
	float aConstant;
	float aLinear;
	float aQuadratic;

	glm::vec3 aAmbient;
	glm::vec3 aDiffuse;
	glm::vec3 aSpecular;

	~Spotlight() {}

	// Constructor with vectors
	Spotlight(glm::vec3 pPosition, glm::vec3 pTarget, float pCutoff, float pOuterCutoff, float pConstant, float pLinear, float pQuadratic)
	{
		this->aPosition = pPosition;
		this->aTarget = pTarget;

		this->aDirection = pTarget - pPosition;

		this->aCutoff = pCutoff;
		this->aOuterCutoff = pOuterCutoff;
		this->aConstant = pConstant;
		this->aLinear = pLinear;
		this->aQuadratic = pQuadratic;
	}

	void mpSetColor(float pR, float pG, float pB)
	{
		this->aColor.r = pR;
		this->aColor.g = pG;
		this->aColor.b = pB;

		this->aDiffuse = this->aColor * glm::vec3(0.8f); // Decrease the influence
		this->aAmbient = this->aDiffuse * glm::vec3(0.2f); // Low influence
		this->aSpecular = glm::vec3(1.0f);
	}
};