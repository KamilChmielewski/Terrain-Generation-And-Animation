#pragma once
#include "ParticleForceGenerator.h"

class GravityGenerator : public ParticleForceGenerator
{
private:
	Vector3D _gravity = Vector3D(0.0f, -9.8f, 0.0f);

public:
	bool gravityIsOn = true;
	GravityGenerator();
	~GravityGenerator();

	void UpdateForce(Particle* particle, float deltaTime);
};