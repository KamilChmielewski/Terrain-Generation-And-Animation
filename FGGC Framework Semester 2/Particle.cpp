#include "Particle.h"

Particle::Particle(Transform* transform)
{
	_transform = transform; 
	_position =  _transform->GetPosition();
	_mass = 0;
	_isGravityOn = true;
	//std::cout << "X: " << _position.x << " Y: " << _position.y << " Z: " << _position.z << std::endl;

	//_velocity = Vector3D(0.0f, 0.0f, -20.0f);
	//_acceleration = Vector3D(0.0f, 200.0f, 0.0f);
	_forceAccum = Vector3D();
}


Particle::~Particle()
{

}

void Particle::Update(float tstep)
{
	if (_mass <= 0)
		return;

	for (ParticleForceGenerator* fg : _forceGenerators)
	{
		fg->UpdateForce(this, tstep);
	}
	
	_acceleration = _forceAccum / _mass;
			
	 _position = _transform->GetPosition() +(_velocity * tstep + _acceleration * tstep * tstep * 0.5);

	_velocity += _acceleration * tstep;

	_velocity *= 0.98;

	if (_position.z < -20)
	{
		_position.z = 20;
	}
	if (_position.z > 20)
	{
		_position.z = -20;
	}

	_transform->SetPosition(_position);

	_forceAccum.Zero();
	
}

void Particle::MoveConstVelocity(float tstep)
{
	_position.x += _velocity.x * tstep;
	_position.y += _velocity.y * tstep;
	_position.z += _velocity.z * tstep;

	_transform->SetPosition(_position);
}

void Particle::SetDampingForce(float damping)
{
	_dampingForce = damping;
}

void Particle::SetMass(float mass)
{
	_mass = mass;
}

void Particle::AddForce(const Vector3D& force)
{
	_forceAccum += force;
}

void Particle::AddGenerator(ParticleForceGenerator * pfg)
{
	_forceGenerators.push_back(pfg);
}
