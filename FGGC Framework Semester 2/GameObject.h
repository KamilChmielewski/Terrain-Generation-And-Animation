#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>
#include "Appearance.h"
#include "Particle.h"
#include "Transform.h"
#include "RigidBody.h"

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject(string type, Appearance* appearance);
	GameObject(string type, Appearance* appearance, Transform* transform);
	GameObject(Appearance* appearance, Transform* transform);

	~GameObject();
	
	string GetType() const { return _type; }

	Geometry GetGeometryData() const { return _geometry; }

	Material GetMaterial() const { return _material; }

	Transform* const GetTransform() const { return _transform; }

	Appearance* const GetApperance() const { return _apperance; }

	Particle* const GetParticle() const { return _particle; }

	RigidBody* const GetRigidBody() const { return _rigidBody; }

	CollisionPrimitive* const GetCollisionPrimitive() const { return _collision; }
	Plane const GetPlane() const { return _plane; }
	Sphere const GetSphere() const { return _sphere; }


	void SetCollisionPrimitive(CollisionPrimitive collision);
	void SetPlane(Vector3D planeDirection) { _plane._type = "Plane"; _plane._direction = planeDirection; _plane._offset = 0.0f; }
	void SetSphere(float radius) { _sphere._radius = radius;  _sphere._type = "Sphere";  }

	void SetCoefficient(float data) { _coefficientRestitution = data; }
	float GetCoefficient() const { return _coefficientRestitution; }
	
	void SetTextureRV(ID3D11ShaderResourceView * textureRV) { _textureRV = textureRV; }
	ID3D11ShaderResourceView* GetTextureRV() const { return _textureRV; }
	bool HasTexture() const { return _textureRV ? true : false; }

	void SetDrawable(bool state) { _drawable = state; }
	bool IsDrawable() const { return _drawable ? true : false; }

	void Update(float t);
	void UpdateRigidBody(float t);
	void Draw(ID3D11DeviceContext * pImmediateContext);

private:
	Transform* _transform;
	Appearance* _apperance;
	Particle* _particle;
	RigidBody* _rigidBody;
	CollisionPrimitive* _collision;
	
	Plane _plane;
	Sphere _sphere;
	string _type;

	Geometry _geometry;
	Material _material;

	float _coefficientRestitution;
	bool _drawable = false;

	ID3D11ShaderResourceView* _textureRV;
};

