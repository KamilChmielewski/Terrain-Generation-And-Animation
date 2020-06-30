
#include "Transform.h"
#include <math.h>

Transform::Transform()
{
	_parent = nullptr;
	//_rigidWorld = XMMatrixIdentity();
	XMStoreFloat4x4(&_rigidWorld, XMMatrixIdentity());
}

Transform::~Transform()
{
}

void Transform::SetPosition(Vector3D pos)
{
	_position = pos;
}

void Transform::SetPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
}

void Transform::SetScale(Vector3D scale)
{
	_scale = scale;
}

void Transform::SetScale(float x, float y, float z)
{
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;
}

void Transform::SetRotation(Vector3D rotation)
{
	_rotation = rotation;
}

void Transform::SetRotation(float x, float y, float z)
{
	_rotation.x = x;
	_rotation.y = y;
	_rotation.z = z;
}

void Transform::AddRotation(Vector3D rotation)
{
	_rotation += rotation;
}

void Transform::AddRotation(float x, float y, float z)
{
	x = x * (XM_PI / 180);
	y = y * (XM_PI / 180);
	z = z * (XM_PI / 180);
	_rotation.x += x;
	_rotation.y += y;
	_rotation.z += z;
}

void Transform::SetTranslation(Vector3D translation)
{
	_position = translation;
}

void Transform::SetTranslation(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
}

void Transform::UpdateWorldMatrix()
{
	XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	XMMATRIX rotation = XMMatrixRotationX(_rotation.x) * XMMatrixRotationY(_rotation.y) * XMMatrixRotationZ(_rotation.z);
	XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);

	XMLoadFloat4x4(&_rigidWorld);
	//XMStoreFloat4x4(&_rigidWorld, scale * translation);
	//world = scale * rotation * translation;
	//XMMatrixDecompose()

	XMStoreFloat4x4(&_world, scale * rotation * translation);

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _parent->GetWorldMatrix());
	}
}
