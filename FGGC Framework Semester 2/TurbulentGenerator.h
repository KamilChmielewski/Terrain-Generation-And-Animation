#pragma once
#include "ParticleForceGenerator.h"

class TurbulentGenerator : public ParticleForceGenerator
{
private:
	float _dragCoefficient = 0.70f;

public:
	TurbulentGenerator();
	~TurbulentGenerator();

	void UpdateForce(Particle* particle, float deltaTime);
};