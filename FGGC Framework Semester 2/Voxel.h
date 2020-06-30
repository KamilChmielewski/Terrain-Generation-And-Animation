#pragma once
#include "GameObject.h"
#include "ConstantBuffers.h"
#include "SimplexNoise.h"
#include <vector>


class Voxel
{
private:
	ID3D11Buffer* _instanceBuffer;
	InstanceType* _instancePos;
	std::vector<GameObject*> _Voxels;
	Appearance* _apperance;
	ID3D11Device* _pd3dDevice;
	float scalar;
public:
	Voxel(ID3D11Device* device ,Appearance* apperance,int size);
	~Voxel();

	void InstanceBuffer();

	void Update(float deltaTime);

	void Render(ID3D11DeviceContext* pImmediateContext, ID3D11Buffer* _pConstantBuffer, ConstantBuffer cb);

	float SimplexNoise(float x, float y, float z);

    float fractal(size_t octaves, float x, float y, float z) const;
	float fractal(size_t octaves, float x, float y) const;
};