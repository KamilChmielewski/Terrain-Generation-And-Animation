#include "LaminarGenerator.h"
#include "Particle.h"

LaminarGenerator::LaminarGenerator()
{
	_dragCoefficient = 0.4;
}

LaminarGenerator::~LaminarGenerator()
{
}

void LaminarGenerator::UpdateForce(Particle* particle, float deltaTime)
{
	Vector3D _dragForce = particle->GetVelocity() * -_dragCoefficient;
	particle->AddForce(_dragForce);
}
