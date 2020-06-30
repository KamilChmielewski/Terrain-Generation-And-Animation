#include "GravityGenerator.h"
#include "Particle.h"

GravityGenerator::GravityGenerator()
{
}

GravityGenerator::~GravityGenerator()
{
}

void GravityGenerator::UpdateForce(Particle* particle, float deltaTime)
{
	if (particle->_isGravityOn)
	{
		particle->AddForce(_gravity * particle->GetMass());
	}
	
}
