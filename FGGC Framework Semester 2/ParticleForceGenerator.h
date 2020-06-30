#pragma once
class Particle;
#include "Vector3D.h"

class ParticleForceGenerator
{
private:

public:

	virtual void UpdateForce(Particle* particle, float deltaTime) = 0;
};