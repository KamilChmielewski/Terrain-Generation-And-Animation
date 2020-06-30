#include "ParticleForceRegistry.h"

void ParticleForceRegistry::Add(Particle* particle, ParticleForceGenerator* fg)
{
	ParticleForceRegistry::ParticleForceRegistration registration;
	registration.particle = particle;
	registration.fg = fg;
	_registranstions.push_back(registration);
}

void ParticleForceRegistry::Remove(Particle* particle, ParticleForceGenerator * fg)
{
}

void ParticleForceRegistry::Clear()
{
}

void ParticleForceRegistry::UpdateForces(float duration)
{
}
