#pragma once
#include "ParticleForceGenerator.h"

class LaminarGenerator : public ParticleForceGenerator
{
private:
	float _dragCoefficient;

public:
	LaminarGenerator();
	~LaminarGenerator();
	void SetDragCoefficient(float drag) { _dragCoefficient = drag; }
	float GetDragCoefficient() const { return _dragCoefficient; }

	void UpdateForce(Particle* particle, float deltaTime);
};