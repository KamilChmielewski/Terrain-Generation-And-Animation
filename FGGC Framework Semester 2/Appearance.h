#pragma once
#include <DirectXMath.h>
#include "Vector3D.h"
#include <d3d11_1.h>


using namespace DirectX;

struct TerrainVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
};

struct Geometry
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* instanceBuffer = nullptr;
	int			  instanceCount;
	int numberOfIndices;

	UINT vertexBufferStride;
	UINT vertexBufferOffset;
};

struct InstanceType
{
	Vector3D pos;
};

struct Material
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
	float specularPower;
};

class Appearance
{
private:
	Geometry _geometry;
	Material _material;
	ID3D11ShaderResourceView* _textureRV;
	ID3D11DeviceContext* _pImmediateContext;

public:
	Appearance(Geometry geometry, Material material, ID3D11DeviceContext* pImmediateContext);
	~Appearance();
	void SetTextureRV(ID3D11ShaderResourceView* textureRV) { _textureRV = textureRV; }
	bool HasTexture() const { return _textureRV ? true : false; }
	
	void	SetInstanceBuffer(ID3D11Buffer* buffer) { _geometry.instanceBuffer = buffer; }
	void	SetInstanceCount(int count) { _geometry.instanceCount = count; }
	Geometry GetGeometryData() const { return _geometry; }
	Material GetMaterial() const { return _material; }
	
	ID3D11ShaderResourceView* GetTextureRV() const { return _textureRV; }
	void Draw();
	void DrawTerrain();
	void DrawInstance();
};