#pragma once
#include "Appearance.h"
#include "Transform.h"
#include "DDSTextureLoader.h"
#include <d3d11.h>

using namespace DirectX;


struct Vertex
{
	float x, y, z;
};

struct HeightMapType
{
	float x, y, z;
	float nx, ny, nz;
	float tu, tv;
};

class StaticTerrain
{
private:
	void Initialise(char* heightMap, wchar_t* TexturePath);
	void InitialiseBuffers();

public:
	StaticTerrain(ID3D11Device* device, ID3D11DeviceContext* ImmediateContext, char* heightMap, wchar_t* TexturePath);
	~StaticTerrain();
	
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void CalculateTextureCoordinates();

	Appearance* const GetApperance() const { return _appearance; }
	Transform*  const GetTransform() const { return _transform;  }
	void UpdateWorldMatrix();
	void Render();

	void SetTextureRV(ID3D11ShaderResourceView* textureRV) { _textureRV = textureRV; }
	ID3D11ShaderResourceView* GetTextureRV() const { return _textureRV; }

	//private variables 
private:
	int _terrainWidth, _terrainHeight;
	int _vertexCount, _indexCount;
	int _textureRepeat = 32;
	HeightMapType* _heightMap;

	Appearance* _appearance;
	Transform* _transform;

	ID3D11ShaderResourceView* _textureRV;
	ID3D11Device* _pd3dDevice;
	ID3D11DeviceContext* _pImmediateContext;
	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
	
	Geometry _planeGeometry;

	Material _planeMaterial;
};