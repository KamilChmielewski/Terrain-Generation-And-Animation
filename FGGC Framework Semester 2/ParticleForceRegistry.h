#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"
#include <vector>
class ParticleForceRegistry
{
protected:
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* fg;
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry _registranstions;

public:

	void Add(Particle* particle, ParticleForceGenerator* fg);

	void Remove(Particle* particle, ParticleForceGenerator* fg);
	void Clear();
	void UpdateForces(float duration);
};