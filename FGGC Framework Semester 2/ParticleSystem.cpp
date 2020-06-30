#include "ParticleSystem.h"
#include <cstdlib>


ParticleSystem::ParticleSystem(vector<GameObject*> gameObjects) : _particles(gameObjects)
{

}

ParticleSystem::ParticleSystem(Appearance* apperance, Vector3D startPos,  float damping, float amountOfObjects, float lifeTime) : _startPos(startPos), _lifeTimeSingleParticle(lifeTime)
{
	for (int i = 0; i < amountOfObjects; i++)
	{
		Transform* transform = new Transform();
		transform->SetScale(0.25, 0.25, 0.25);
		transform->SetPosition(startPos);
	
		_particle = new GameObject("System", apperance, transform);
		_particle->GetParticle()->SetVelocity(Vector3D(std::rand() % 10/100.0f, std::rand() % 10/100.0f, std::rand() % 10/100.0f));
		_particle->GetParticle()->SetAcceleration(Vector3D(std::rand() % 20/100.0f, std::rand() % 20/100.0f, std::rand() % 20/100.0f));
		_particle->GetParticle()->SetMass(20.0f);
		_particle->GetParticle()->SetDampingForce(0.2f);
		_particles.push_back(_particle);

		_lifeTime.push_back(lifeTime);

	}
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::AddGenerator(ParticleForceGenerator* pfg)
{
	for (GameObject* g : _particles)
	{
		g->GetParticle()->AddGenerator(pfg);
	}
}

void ParticleSystem::Update(float deltaTime)
{
	for (GameObject* g : _particles)
	{
		//_lifeTime[g] += deltaTime;
		g->Update(deltaTime);
	}
}

void ParticleSystem::UpdateTest(float deltaTime)
{
	for (int i = 0; i < _particles.size(); i++)
	{
		_lifeTime[i] += deltaTime;
		if (_lifeTime[i] > _lifeTimeSingleParticle)
		{
			_lifeTime[i] = 0.0f;
			_particles[i]->GetTransform()->SetPosition(_startPos);
			_particles[i]->GetParticle()->SetVelocity(Vector3D(std::rand() % 10, std::rand() % 40, std::rand() % 10));
			_particles[i]->GetParticle()->SetAcceleration(Vector3D(std::rand() % 20, std::rand() % 20, std::rand() % 20));
		}
		else
		{
			_particles[i]->Update(deltaTime);
		}

	}
}

void ParticleSystem::Draw(ID3D11DeviceContext* pImmediateContext, ID3D11Buffer* _pConstantBuffer, ConstantBuffer cb)
{
	for (auto gameObject : _particles)
	{
		// Get render material
		Material material = gameObject->GetApperance()->GetMaterial();

		// Copy material to shader
		cb.surface.AmbientMtrl = material.ambient;
		cb.surface.DiffuseMtrl = material.diffuse;
		cb.surface.SpecularMtrl = material.specular;

		// Set world matrix
		cb.World = XMMatrixTranspose(gameObject->GetTransform()->GetWorldMatrix());

		// Set texture
		if (gameObject->GetApperance()->HasTexture())
		{
			ID3D11ShaderResourceView* textureRV = gameObject->GetApperance()->GetTextureRV();
			pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
			cb.HasTexture = 1.0f;
		}
		else
		{
			cb.HasTexture = 0.0f;
		}

		// Update constant buffer
		pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

		// Draw object
		gameObject->Draw(pImmediateContext);

	}

}


