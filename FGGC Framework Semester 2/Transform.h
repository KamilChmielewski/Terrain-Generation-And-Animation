#pragma once
#include <DirectXMath.h>
#include "Vector3D.h"

using namespace DirectX;

class Transform
{
private:
	Vector3D _position;
	Vector3D _scale;
	Vector3D _rotation;

	XMFLOAT4X4 _world;
	XMMATRIX _quaternionMatrix;
	//XMMATRIX scale;
	//XMMATRIX translation;

	XMMATRIX world;

	XMFLOAT4X4 _rigidWorld;

	//translation * _position;

	Transform* _parent;

public:
	Transform();

	~Transform();

	void SetPosition(Vector3D position);
	void SetPosition(float x, float y, float z);

	void SetScale(Vector3D scale);
	void SetScale(float x, float y, float z);

	void SetRotation(Vector3D rotation);
	void SetRotation(float x, float y, float z);
	
	void AddRotation(Vector3D rotation);
	void AddRotation(float x, float y, float z);

	void SetTranslation(Vector3D translation);
	void SetTranslation(float x, float y, float z);

	Vector3D GetPosition() const { return _position; }
	Vector3D GetScale() const { return _scale; }
	Vector3D GetRotation() const { return _rotation; }
	XMMATRIX GetQuaternionMatrix() { return _quaternionMatrix; }

	void UpdateWorldMatrix();
	

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	XMMATRIX worldMatrix() { return world; }

	XMMATRIX GetRigidWorld() { return XMLoadFloat4x4(&_rigidWorld); }
	
	void SetRigidMatrix(XMMATRIX matrix) { XMStoreFloat4x4(&_rigidWorld, matrix); };

	void SetParent(Transform* parent) { _parent = parent; }
};