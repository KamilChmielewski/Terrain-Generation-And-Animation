#pragma once
#include "ParticleForceGenerator.h"
#include "GravityGenerator.h"
#include "LaminarGenerator.h"
#include "TurbulentGenerator.h"
#include "Appearance.h"
#include <vector>
#include "GameObject.h"
#include "ConstantBuffers.h"

class ParticleSystem
{
private:
	std::vector<GameObject*> _particles;
	std::vector<float> _lifeTime;
	float _lifeTimeSingleParticle;
	Vector3D _startPos;
	GameObject* _particle;


public:
	ParticleSystem(vector<GameObject*> gameObjects);
	ParticleSystem(Appearance* apperance, Vector3D startPos, float damping, float amountOfObjects, float lifeTime);
	~ParticleSystem();
	void AddGenerator(ParticleForceGenerator* pfg);
	void Update(float deltaTime);
	void UpdateTest(float deltaTime);
	void Draw(ID3D11DeviceContext* pImmediateContext, ID3D11Buffer* _pConstantBuffer, ConstantBuffer cb);


};