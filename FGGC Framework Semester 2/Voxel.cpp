#include "Voxel.h"

Voxel::Voxel(ID3D11Device* device, Appearance* apperance, int size) : _apperance(apperance), _pd3dDevice(device)
{
	float lessZero = 4.0f;
	int overZero = 0;
	float evalvation;
	scalar = 0.9;

	int index = 0;
	_Voxels.clear();
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			for (int z = 0; z < size; z++)
			{
				//std::cout << "NOISE: " << SimplexNoise(x * scalar, y * scalar, z * scalar) << std::endl;
				if (SimplexNoise(x, y, z) < 0.2f)
				{
					auto trans = new Transform();
					trans->SetScale(0.5f, 0.5f, 0.5f);
					trans->SetPosition(x + 4, y + 4, z + 4);
					GameObject* voxel = new GameObject("VOxel", apperance, trans);
					//if (SimplexNoise::noise(x, y, z) < 0.5f)
					voxel->SetDrawable(true);

					voxel->GetTransform()->UpdateWorldMatrix();

					_Voxels.push_back(voxel);
					index++;
				}
			}
		}
	}

	InstanceBuffer();
	_apperance->SetInstanceBuffer(_instanceBuffer);
	_apperance->SetInstanceCount(_Voxels.size());

}

Voxel::~Voxel()
{

}

void Voxel::Update(float deltaTime)
{
	for (GameObject* g : _Voxels)
	{
	//	//_lifeTime[g] += deltaTime;
		g->Update(deltaTime);
	}
}

void Voxel::Render(ID3D11DeviceContext* pImmediateContext, ID3D11Buffer* _pConstantBuffer, ConstantBuffer cb)
{
	ID3D11ShaderResourceView* textureRV = _apperance->GetTextureRV();
	pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
	cb.HasTexture = 1.0f;

	Material material = _apperance->GetMaterial();

	// Copy material to shader
	cb.surface.AmbientMtrl = material.ambient;
	cb.surface.DiffuseMtrl = material.diffuse;
	cb.surface.SpecularMtrl = material.specular;
	XMMATRIX identiy = XMMatrixIdentity();
	cb.World = XMMatrixTranspose(identiy);

	pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	_apperance->DrawInstance();



	//for (GameObject* gameObject : _Voxels)
	//{
	//		Material material = gameObject->GetApperance()->GetMaterial();

	//		// Copy material to shader
	//		cb.surface.AmbientMtrl = material.ambient;
	//		cb.surface.DiffuseMtrl = material.diffuse;
	//		cb.surface.SpecularMtrl = material.specular;

	//		// Set world matrix
	//		cb.World = XMMatrixTranspose(gameObject->GetTransform()->GetWorldMatrix());

	//		// Set texture
	//		/*if (gameObject->GetApperance()->HasTexture())
	//		{
	//			ID3D11ShaderResourceView* textureRV = gameObject->GetApperance()->GetTextureRV();
	//			pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
	//			cb.HasTexture = 1.0f;
	//		}
	//		else
	//		{
	//			cb.HasTexture = 0.0f;
	//		}*/

	//		// Update constant buffer
	//		pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	//		// Draw object
	//		_apperance->Draw();
	//		//gameObject->Draw(pImmediateContext);
	//}
}


float Voxel::fractal(size_t octaves, float x, float y, float z) const
{
	float mFrequency = 0.01;
	float mAmplitude = 1.0f;
	float mPersistence = 0.25;
	float mLacunarity = 2.0f;

	float output = 0.f;
	float denom = 0.f;
	float frequency = mFrequency; //startFrequency
	float amplitude = mAmplitude;

	for (size_t i = 0; i < octaves; i++) {
		output += (amplitude * SimplexNoise::noise(x * frequency, y * frequency, z * frequency));
		denom += amplitude;

		frequency *= mLacunarity;
		amplitude *= mPersistence;
	}

	return (output / denom);
}

float Voxel::fractal(size_t octaves, float x, float y) const
{
	float output = 0.f;
	float denom = 0.f;
	float frequency = 4.0;
	float amplitude = 128.0;

	for (size_t i = 0; i < octaves; i++) {
		output += (amplitude * SimplexNoise::noise(x * frequency, y * frequency));
		denom += amplitude;

		frequency *= 2.0f;
		amplitude *= 0.5f;
	}

	return (output / denom);
}


float Voxel::SimplexNoise(float x, float y, float z)
{
	size_t octave = 2.0f;

	float ab = fractal(octave, x, y, z);
	float bc = fractal(octave, y, z, x);
	float ac = fractal(octave, x, z, y);

	float ba = fractal(octave,y, x, z);
	float cb = fractal(octave,z, y, x);
	float ca = fractal(octave, z, x, y);

	float abc = ab + bc + ac + ba + cb + ca;

	//return abc / 6.0f;
	return ab;
}

void Voxel::InstanceBuffer()
{
	HRESULT result;

	D3D11_SUBRESOURCE_DATA instanceData;
	D3D11_BUFFER_DESC instanceBufferDesc;

	_instancePos = new InstanceType[_Voxels.size()];
	
	for (int i = 0; i < _Voxels.size(); i++)
	{
		_instancePos[i].pos.x = _Voxels[i]->GetTransform()->GetPosition().x;
		_instancePos[i].pos.y = _Voxels[i]->GetTransform()->GetPosition().y;
		_instancePos[i].pos.z = _Voxels[i]->GetTransform()->GetPosition().z;
	}
	ZeroMemory(&instanceBufferDesc, sizeof(instanceBufferDesc));
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * _Voxels.size();
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	//instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	ZeroMemory(&instanceData, sizeof(instanceData));
	instanceData.pSysMem = _instancePos;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = _pd3dDevice->CreateBuffer(&instanceBufferDesc, &instanceData, &_instanceBuffer);
	if (FAILED(result))
	{
		std::cout << "Failed to create instance buffer" << std::endl;
	}
}