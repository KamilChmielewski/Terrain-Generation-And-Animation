#include <iostream>
#include "StaticTerrain.h"

StaticTerrain::StaticTerrain(ID3D11Device* device, ID3D11DeviceContext* ImmediateContext, char* HeightPath, wchar_t* TexturePath) : _pd3dDevice(device), _pImmediateContext(ImmediateContext)
{
	_transform = new Transform();
	Initialise(HeightPath, TexturePath);
}

StaticTerrain::~StaticTerrain()
{

}

void StaticTerrain::Initialise(char* HeightMap, wchar_t* TexturePath)
{
	bool result;

	// Load in the height map for the terrain.
	result = LoadHeightMap(HeightMap);
	if (!result)
	{
		std::cout << "Failed to load hieght map" << std::endl;
	}

	//Normalize height map
	NormalizeHeightMap();

	//calculate normals of terrain data
	result = CalculateNormals();
	if (!result)
	{
		std::cout << "Failed to calculate Normals" << std::endl;
	}

	//Calculate texture coordinates
	CalculateTextureCoordinates();

	//Texture loading....
	CreateDDSTextureFromFile(_pd3dDevice, TexturePath, nullptr, &_textureRV);

	InitialiseBuffers();
}

void StaticTerrain::InitialiseBuffers()
{
	TerrainVertex* vertices;
	unsigned long* indices;
	int index, i, j;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;
	float tu, tv;

	// Calculate the number of vertices in the terrain mesh.
	_vertexCount = (_terrainWidth - 1) * (_terrainHeight - 1) * 12;
	vertices = new TerrainVertex[_vertexCount];

	// Set the index count to the same as the vertex count.
	_indexCount = _vertexCount;

	indices = new unsigned long[_indexCount];

	if (!indices)
		std::cout << "failed to setup indicies" << std::endl;

	// Initialize the index to the vertex buffer.
	index = 0;

	// Load the vertex and index array with the terrain data.
	for (j = 0; j < (_terrainHeight - 1); j++)
	{
		for (i = 0; i < (_terrainWidth - 1); i++)
		{
			index1 = (_terrainHeight * j) + i;          // Bottom left.
			index2 = (_terrainHeight * j) + (i + 1);      // Bottom right.
			index3 = (_terrainHeight * (j + 1)) + i;      // Upper left.
			index4 = (_terrainHeight * (j + 1)) + (i + 1);  // Upper right.

			// Upper left.
			tv = _heightMap[index3].tv;

			// Modify the texture coordinates to cover the top edge.
			if (tv == 1.0f) { tv = 0.0f; }

			vertices[index].Pos.x = _heightMap[index3].x;
			vertices[index].Pos.y = _heightMap[index3].y;
			vertices[index].Pos.z = _heightMap[index3].z;
			
			vertices[index].Tex.x = _heightMap[index3].tu;
			vertices[index].Tex.y = _heightMap[index3].tv;
			
			vertices[index].Normal.x = _heightMap[index3].nx;
			vertices[index].Normal.y = _heightMap[index3].ny;
			vertices[index].Normal.z = _heightMap[index3].nz;
			
			indices[index] = index;
			index++;

			// Upper right.
			tu = _heightMap[index4].tu;
			tv = _heightMap[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) { tu = 1.0f; }
			if (tv == 1.0f) { tv = 0.0f; }

			vertices[index].Pos.x = _heightMap[index4].x;
			vertices[index].Pos.y = _heightMap[index4].y;
			vertices[index].Pos.z = _heightMap[index4].z;

			vertices[index].Tex.x = tu;
			vertices[index].Tex.y = tv;

			vertices[index].Normal.x = _heightMap[index4].nx;
			vertices[index].Normal.y = _heightMap[index4].ny;
			vertices[index].Normal.z = _heightMap[index4].nz;
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].Pos.x = _heightMap[index1].x;
			vertices[index].Pos.y = _heightMap[index1].y;
			vertices[index].Pos.z = _heightMap[index1].z;

			vertices[index].Tex.x = _heightMap[index1].tu;
			vertices[index].Tex.y = _heightMap[index1].tv;

			vertices[index].Normal.x = _heightMap[index1].nx;
			vertices[index].Normal.y = _heightMap[index1].ny;
			vertices[index].Normal.z = _heightMap[index1].nz;
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].Pos.x = _heightMap[index1].x;
			vertices[index].Pos.y = _heightMap[index1].y;
			vertices[index].Pos.z = _heightMap[index1].z;

			vertices[index].Tex.x = _heightMap[index1].tu;
			vertices[index].Tex.y = _heightMap[index1].tv;

			vertices[index].Normal.x = _heightMap[index1].nx;
			vertices[index].Normal.y = _heightMap[index1].ny;
			vertices[index].Normal.z = _heightMap[index1].nz;
			indices[index] = index;
			index++;

			// Upper right.
			tu = _heightMap[index4].tu;
			tv = _heightMap[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) { tu = 1.0f; }
			if (tv == 1.0f) { tv = 0.0f; }

			vertices[index].Pos.x = _heightMap[index4].x;
			vertices[index].Pos.y = _heightMap[index4].y;
			vertices[index].Pos.z = _heightMap[index4].z;

			vertices[index].Tex.x = tu;
			vertices[index].Tex.y = tv;

			vertices[index].Normal.x = _heightMap[index4].nx;
			vertices[index].Normal.y = _heightMap[index4].ny;
			vertices[index].Normal.z = _heightMap[index4].nz;
			indices[index] = index;
			index++;

			// Bottom right.
			tu = _heightMap[index2].tu;

			// Modify the texture coordinates to cover the right edge.
			if (tu == 0.0f) { tu = 1.0f; }

			vertices[index].Pos.x = _heightMap[index2].x;
			vertices[index].Pos.y = _heightMap[index2].y;
			vertices[index].Pos.z = _heightMap[index2].z;

			vertices[index].Tex.x = tu;
			vertices[index].Tex.y = _heightMap[index2].tv;

			vertices[index].Normal.x = _heightMap[index2].nx;
			vertices[index].Normal.y = _heightMap[index2].ny;
			vertices[index].Normal.z = _heightMap[index2].nz;
			indices[index] = index;
			index++;
		}
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(TerrainVertex) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = _pd3dDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result))
	{
		std::cout << "Failed to Create Terrain Vertex Buffer" << std::endl;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
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
	_planeGeometry.numberOfIndices = _vertexCount;
	_planeGeometry.vertexBufferOffset = 0;
	_planeGeometry.vertexBufferStride = sizeof(TerrainVertex);

	Material noSpecMaterial;
	noSpecMaterial.ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	noSpecMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	noSpecMaterial.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	noSpecMaterial.specularPower = 0.0f;

	_appearance = new Appearance(_planeGeometry, noSpecMaterial, _pImmediateContext);
	_appearance->SetTextureRV(_textureRV);

}

bool StaticTerrain::LoadHeightMap(char* filePath)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;


	// Open the height map file in binary.
	error = fopen_s(&filePtr, filePath, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	_terrainWidth = bitmapInfoHeader.biWidth;
	_terrainHeight = bitmapInfoHeader.biHeight;
	
	// Calculate the size of the bitmap image data.
	imageSize = _terrainWidth * _terrainHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Create the structure to hold the height map data.
	_heightMap = new HeightMapType[_terrainWidth * _terrainHeight];
	if (!_heightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j < _terrainHeight; j++)
	{
		for (i = 0; i < _terrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (_terrainHeight * j) + i;

			_heightMap[index].x = (float)i;
			_heightMap[index].y = (float)height;
			_heightMap[index].z = (float)j;

			k += 3;
		}
	}

	// Release the bitmap image data.
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}

void StaticTerrain::NormalizeHeightMap()
{
	for (int j = 0; j < _terrainHeight; j++)
	{
		for (int i = 0; i < _terrainWidth; i++)
		{
			_heightMap[(_terrainHeight * j) + i].y /= 15.0f;
		}
	}
}

bool StaticTerrain::CalculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	Vertex* normals;

	normals = new Vertex[(_terrainHeight - 1) * (_terrainWidth - 1)];
	if (!normals)
	{
		return false;
	}

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j < (_terrainHeight - 1); j++)
	{
		for (i = 0; i < (_terrainWidth - 1); i++)
		{
			index1 = (j * _terrainHeight) + i;
			index2 = (j * _terrainHeight) + (i + 1);
			index3 = ((j + 1) * _terrainHeight) + i;

			// Get three vertices from the face.
			vertex1[0] = _heightMap[index1].x;
			vertex1[1] = _heightMap[index1].y;
			vertex1[2] = _heightMap[index1].z;

			vertex2[0] = _heightMap[index2].x;
			vertex2[1] = _heightMap[index2].y;
			vertex2[2] = _heightMap[index2].z;

			vertex3[0] = _heightMap[index3].x;
			vertex3[1] = _heightMap[index3].y;
			vertex3[2] = _heightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (_terrainHeight - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for (j = 0; j < _terrainHeight; j++)
	{
		for (i = 0; i < _terrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (_terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (_terrainHeight - 1)))
			{
				index = (j * (_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (_terrainWidth - 1)) && (j < (_terrainHeight - 1)))
			{
				index = (j * (_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * _terrainHeight) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			_heightMap[index].nx = (sum[0] / length);
			_heightMap[index].ny = (sum[1] / length);
			_heightMap[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete[] normals;
	normals = 0;

	return true;
}

void StaticTerrain::CalculateTextureCoordinates()
{
	int incrementCount, i, j, tuCount, tvCount;
	float incrementValue, tuCoordinate, tvCoordinate;


	// Calculate how much to increment the texture coordinates by.
	incrementValue = (float)_textureRepeat / (float)_terrainWidth;

	// Calculate how many times to repeat the texture.
	incrementCount = _terrainWidth / _textureRepeat;

	// Initialize the tu and tv coordinate values.
	tuCoordinate = 0.0f;
	tvCoordinate = 1.0f;

	// Initialize the tu and tv coordinate indexes.
	tuCount = 0;
	tvCount = 0;

	// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	for (j = 0; j < _terrainHeight; j++)
	{
		for (i = 0; i < _terrainWidth; i++)
		{
			// Store the texture coordinate in the height map.
			_heightMap[(_terrainHeight * j) + i].tu = tuCoordinate;
			_heightMap[(_terrainHeight * j) + i].tv = tvCoordinate;

			// Increment the tu texture coordinate by the increment value and increment the index by one.
			tuCoordinate += incrementValue;
			tuCount++;

			// Check if at the far right end of the texture and if so then start at the beginning again.
			if (tuCount == incrementCount)
			{
				tuCoordinate = 0.0f;
				tuCount = 0;
			}
		}

		// Increment the tv texture coordinate by the increment value and increment the index by one.
		tvCoordinate -= incrementValue;
		tvCount++;

		// Check if at the top of the texture and if so then start at the bottom again.
		if (tvCount == incrementCount)
		{
			tvCoordinate = 1.0f;
			tvCount = 0;
		}
	}

}

void StaticTerrain::UpdateWorldMatrix()
{
	_transform->UpdateWorldMatrix();
}

void StaticTerrain::Render()
{
	_appearance->DrawTerrain();
}
