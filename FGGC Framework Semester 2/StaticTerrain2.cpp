#include "StaticTerrain2.h"
#include <vector>


StaticTerrain2::StaticTerrain2(ID3D11Device* device, ID3D11DeviceContext* ImmediateContext, char* heightMap, wchar_t* TexturePath) : _pd3dDevice(device), _pImmediateContext(ImmediateContext)
{
	_transform = new Transform();
	CreateDDSTextureFromFile(_pd3dDevice, TexturePath, nullptr, &_textureRV);

	SetGridUp();
}

StaticTerrain2::~StaticTerrain2()
{

}

void StaticTerrain2::SetGridUp()
{
	HRESULT result;
	int rows = 128;
	int cols = 256;
	int Width = 2;
	int dx = 2;
	int dz = 2;
	int d = 2;
	int index = 0;

	_diamondSquare = new DiamondSquare(rows + 1, cols + 1, 180);

	int** map = _diamondSquare->GetMap();

	int vertexCount = rows * cols;
	int NumFaces = (rows - 1) * (cols - 1) * 6;
	
	_terrainVertex = new TerrainVertex2[vertexCount];

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			_terrainVertex[index].Pos.x = j * dx + (-Width * 0.5);
			_terrainVertex[index].Pos.y = map[i][j];
			_terrainVertex[index].Pos.z = -(i * dz) + (d * 0.5);

			_terrainVertex[index].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			index++;
		}
	}

	std::vector<DWORD> indices(NumFaces * 3);

	int k = 0;
	int texUIndex = 0;
	int texVIndex = 0;

	/*
		new order
		Top Right
		Top left
		Bottom right

		Top Right
		Bottom left
		Bottom Right
	*/
	for (DWORD i = 0; i < rows - 1; i++)
	{
		for (DWORD j = 0; j < cols - 1; j++)
		{
			indices[k] = i * cols + j;        // Bottom left of quad
			_terrainVertex[i * cols + j].Tex = XMFLOAT2(texUIndex + 0.0f, texVIndex + 1.0f);

			indices[k + 1] = i * cols + j + 1;        // Bottom right of quad
			_terrainVertex[i * cols + j + 1].Tex = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

			indices[k + 2] = (i + 1) * cols + j;    // Top left of quad
			_terrainVertex[(i + 1) * cols + j].Tex = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);

			indices[k + 3] = (i + 1) * cols + j;    // Top left of quad
			_terrainVertex[(i + 1) * cols + j].Tex = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);

			indices[k + 4] = i * cols + j + 1;        // Bottom right of quad
			_terrainVertex[i * cols + j + 1].Tex = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

			indices[k + 5] = (i + 1) * cols + j + 1;    // Top right of quad
			_terrainVertex[(i + 1) * cols + j + 1].Tex = XMFLOAT2(texUIndex + 1.0f, texVIndex + 0.0f);

			k += 6; // next quad

			texUIndex++;
		}
		texUIndex = 0;
		texVIndex++;
	}

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(TerrainVertex2) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = &_terrainVertex[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = _pd3dDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result))
	{
		std::cout << "Failed to Create Terrain Vertex Buffer" << std::endl;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = _pd3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result))
	{
		std::cout << "Failed to Create Terrain Index Buffer" << std::endl;
	}

	_planeGeometry.vertexBuffer = _vertexBuffer;
	_planeGeometry.indexBuffer = _indexBuffer;
	_planeGeometry.numberOfIndices = NumFaces;
	_planeGeometry.vertexBufferOffset = 0;
	_planeGeometry.vertexBufferStride = sizeof(TerrainVertex2);

	Material noSpecMaterial;
	noSpecMaterial.ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	noSpecMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	noSpecMaterial.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	noSpecMaterial.specularPower = 0.0f;

	_appearance = new Appearance(_planeGeometry, noSpecMaterial, _pImmediateContext);
	_appearance->SetTextureRV(_textureRV);

}

void StaticTerrain2::SetupBuffers()
{

}

void StaticTerrain2::Update()
{
	_transform->UpdateWorldMatrix();
}

void StaticTerrain2::Render()
{
	_appearance->DrawTerrain();
}