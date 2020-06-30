#include "RigidBody.h"

RigidBody::RigidBody(Transform* transform) : _transform(transform)
{
	_mass = 200;
	//_world = transform->GetRigidWorld();
	XMStoreFloat4x4(&_world, _transform->GetRigidWorld());
	//_transform->worldMatrix() = XMMatrixIdentity();
	XMStoreFloat3x3(&inertiaTensor, XMMatrixScaling(1.0f / 12.0f * _mass*(pow(_transform->GetScale().z, 2) + pow(_transform->GetScale().y, 2.0f)),
													1.0f / 12.0f * _mass*(pow(_transform->GetScale().x, 2) + pow(_transform->GetScale().z, 2.0f)),
													1.0f / 12.0f * _mass*(pow(_transform->GetScale().x, 2) + pow(_transform->GetScale().y, 2.0f))));

	XMLoadFloat3x3(&inertiaTensor);
	_position = _transform->GetPosition();

	XMStoreFloat4x4(&_world, XMMatrixTranslation(_position.x, _position.y, _position.z));

	XMStoreFloat3x3(&_it, XMLoadFloat3x3(&inertiaTensor));

	XMStoreFloat3x3(&_inverseWorld, XMMatrixInverse(nullptr, XMLoadFloat4x4(&_world)));

	XMStoreFloat3x3(&_inertiaTensorWorld, XMMatrixTranspose(XMLoadFloat3x3(&_inverseWorld)) * XMLoadFloat3x3(&_it) * XMLoadFloat3x3(&_inverseWorld));

	_torqueAccum = Vector3D(0.0f, 0.0f, 0.0f);
	
}

RigidBody::~RigidBody()
{
}

void RigidBody::Update(float t)
{
	//_lastFrameAcceleration = _acceleration;
	_lastFrameAcceleration = _forceAccum / _mass;
	//_torqueAccum = Vector3D(0, 15, 0);
	Vector3D _angularAcceleration = transform(_torqueAccum, XMLoadFloat3x3(&inertiaTensor));

	_velocity.AddScaledVector(_lastFrameAcceleration, t);

	//_rotation *= 0.95f;
	_rotation.AddScaledVector(_angularAcceleration, t);
	

	_velocity *= pow(0.4, t);
	_rotation *= pow(0.1, t);

	_position += _velocity * t + _acceleration * t * t * 0.5;

	if (_position.z < -20)
	{
		_position.z = 20;
	}
	if (_position.z > 20)
	{
		_position.z = -20;
	}

	_orientation.addScaledVector(_rotation, t);

	CalculateDerivedData();

	ClearAccumulators();
}

void RigidBody::CalculateDerivedData()
{
	_orientation.Normalise();

	Quaternion q;
	
	q.i = 0;
	q.j = 0;
	q.k = 0;
	q.r = 0;
	XMMATRIX test = CalculateTransformMatrix(XMLoadFloat4x4(&_world), _position, _orientation);

	_transform->SetRigidMatrix(test);
	ItToWorld();

	ClearAccumulators();
}

void RigidBody::ClearAccumulators()
{
	_forceAccum.Zero();
	_torqueAccum.Zero();
}

void RigidBody::AddForce(const Vector3D& force)
{
	_forceAccum += force;
}

void RigidBody::AddForceAtPoint(const Vector3D& force, const Vector3D& point)
{
	Vector3D pt = point;
	//pt -= _position;

	//_forceAccum += force;
	_torqueAccum += point;//pt % force;
}

void RigidBody::AddForceAtBodyPoint(const Vector3D & force, const Vector3D & point)
{

}

void RigidBody::AddTorque(const Vector3D& torque)
{
	_torqueAccum += torque;
}

void RigidBody::SetAcceleration(const Vector3D & acceleration)
{
}

void RigidBody::ItToWorld()
{

	XMMATRIX it = XMLoadFloat3x3(&inertiaTensor);
	XMMATRIX inverseWorld = XMMatrixInverse(nullptr, XMLoadFloat4x4(&_world));
	XMMATRIX ITW = XMMatrixTranspose(inverseWorld) * it * inverseWorld;

	XMStoreFloat3x3(&_inertiaTensorWorld, ITW);
	XMStoreFloat3x3(&inertiaTensor, XMMatrixInverse(nullptr, ITW));

}

Vector3D RigidBody::transform(Vector3D & torque, XMMATRIX worldInertia)
{
	XMFLOAT3 value;
	value.x = torque.x;
	value.y = torque.y;
	value.z = torque.z;

	XMVECTOR valueVector = XMLoadFloat3(&value);
	valueVector = XMVector3Transform(valueVector, worldInertia);

	XMFLOAT3 returnedVector;

	XMStoreFloat3(&returnedVector, valueVector);

	return Vector3D(returnedVector.x, returnedVector.y, returnedVector.z);
}

