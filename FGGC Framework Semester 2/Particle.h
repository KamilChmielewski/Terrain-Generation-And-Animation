#pragma once
#include "Vector3D.h"
#include "Transform.h"
//#include "GameObject.h"
#include <vector>
#include "ParticleForceGenerator.h"

class Particle
{
private:
//	vector<GameObject*> _gameObjects;
//	GameObject* _gameObject;
	Vector3D _velocity;
	Vector3D _position;
	Vector3D _acceleration;
	Vector3D _forceAccum;
	Transform* _transform;
	std::vector<ParticleForceGenerator*> _forceGenerators;
	float _dampingForce;
	float _mass;
	
public:
	bool _isGravityOn = true;
	Particle(Transform* transform);
//	Particle(vector<GameObject*> objects);
//	Particle(GameObject* gameObject);
	~Particle();
	void Update(float t);
	void MoveConstVelocity(float tstep);
	void SetDampingForce(float damping);
	void SetMass(float mass);
	void AddForce(const Vector3D& force);
	void AddGenerator(ParticleForceGenerator* pfg);
	float GetDampingForce() { return _dampingForce; }
	void SetAcceleration(Vector3D acceleration) { _acceleration = acceleration; }
	Vector3D GetAcceleration() { return _acceleration; }
	Vector3D GetVelocity() { return _velocity; }
	float GetMass() const { return _mass; }
	void SetVelocity(Vector3D vel) { _velocity = vel; }

	Vector3D const GetPosition() const { return _position; }
};