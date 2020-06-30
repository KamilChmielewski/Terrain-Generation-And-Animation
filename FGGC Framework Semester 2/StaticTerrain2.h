#pragma once
#include "Appearance.h"
#include "Transform.h"
#include "DDSTextureLoader.h"
#include "DiamondSquare.h"

#include <d3d11.h>

using namespace DirectX;

struct TerrainVertex2
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
};

struct Vertex2
{
	float x, y, z;
};

class StaticTerrain2
{
private:
	Appearance* _appearance;
	Transform* _transform;

	TerrainVertex2* _terrainVertex;
	Vertex2*		  _vertex;

	ID3D11ShaderResourceView* _textureRV;
	ID3D11Device* _pd3dDevice;
	ID3D11DeviceContext* _pImmediateContext;
	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;

	Geometry _planeGeometry;

	Material _planeMaterial;
	DiamondSquare* _diamondSquare;

public:
	StaticTerrain2(ID3D11Device* device, ID3D11DeviceContext* ImmediateContext, char* heightMap, wchar_t* TexturePath);
	~StaticTerrain2();

	Appearance* const GetApperance() const { return _appearance; }
	Transform* const GetTransform() const { return _transform; }

	void SetGridUp();
	void SetupBuffers();
	void Update();
	void Render();

};