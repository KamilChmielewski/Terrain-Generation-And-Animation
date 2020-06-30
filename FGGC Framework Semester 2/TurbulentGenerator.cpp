#include "TurbulentGenerator.h"
#include "Particle.h"

TurbulentGenerator::TurbulentGenerator()
{
}

TurbulentGenerator::~TurbulentGenerator()
{
}

void TurbulentGenerator::UpdateForce(Particle* particle, float deltaTime)
{
	Vector3D turbulent = Vector3D(2.0f, 0.0f, 0.0f);
	float velocityMagnitude = turbulent.Magnitude(); //particle->GetVelocity().Magnitude();

	//particle->GetVelocity().Normalise();
	turbulent.Normalise();

	Vector3D unitVelocity = turbulent;

	float dragMagnitude = _dragCoefficient * velocityMagnitude * velocityMagnitude;

	Vector3D _dragForce = unitVelocity * -dragMagnitude;

	particle->AddForce(_dragForce);
}
