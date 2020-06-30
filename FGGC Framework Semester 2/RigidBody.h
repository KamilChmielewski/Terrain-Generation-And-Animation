#pragma once
#include "Vector3D.h"
#include "Transform.h"

using namespace DirectX;

class RigidBody
{
private:
	float _mass;

	float _linearDamping;

	float _angularDamping;

	float _motion;

	bool isAwake;
	
	bool canSleep;

	Vector3D _forceAccum;

	Vector3D _torqueAccum;

	Vector3D _acceleration;

	Vector3D _velocity;

	Vector3D _lastFrameAcceleration;

	Vector3D _position;

	Vector3D _rotation;

	Quaternion _orientation;

	Transform* _transform;

	XMFLOAT3X3 _it;
	XMFLOAT3X3 _itw;
	XMFLOAT3X3 _inertiaToWorld;
	XMFLOAT3X3 _inverseWorld;
	XMFLOAT3X3 _inertiaTensorWorld;
	XMFLOAT4X4 _world;

	XMFLOAT3X3 inertiaTensor;

public:
	RigidBody(Transform* transform);
	~RigidBody();

	XMMATRIX GetWorld() { return XMLoadFloat4x4(&_world); }

	void SetMass(float mass) { _mass = mass; }
	void SetAngularDamping(float const angleDamping) { _angularDamping = angleDamping; }
	void SetLinearDamping(float damping) { _linearDamping = damping; }
	void CalculateDerivedData();
	void ItToWorld();

	void Update(float t);

	void ClearAccumulators();

	void AddForce(const Vector3D &force);

	void AddForceAtPoint(const Vector3D &force, const Vector3D &point);

	void AddForceAtBodyPoint(const Vector3D &force, const Vector3D &point);

	void AddTorque(const Vector3D &torque);

	void SetAcceleration(const Vector3D &acceleration);

	Vector3D transform(Vector3D& torque, XMMATRIX worldInertia);

	//void GetAcceleration(Vector3D *acceleration) const;

};