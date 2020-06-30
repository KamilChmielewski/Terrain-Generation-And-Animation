#include "Appearance.h"

Appearance::Appearance(Geometry geometry, Material material, ID3D11DeviceContext * pImmediateContext) : _geometry(geometry), _material(material), _pImmediateContext(pImmediateContext)
{
}

Appearance::~Appearance()
{
}

void Appearance::Draw()
{
	_pImmediateContext->IASetVertexBuffers(0, 1, &_geometry.vertexBuffer, &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	_pImmediateContext->IASetIndexBuffer(_geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	_pImmediateContext->DrawIndexed(_geometry.numberOfIndices, 0, 0);
}

void Appearance::DrawInstance()
{
	unsigned int strides[2];
	unsigned int offsets[2];

	offsets[0] = 0;
	offsets[1] = 0;

	strides[0] = _geometry.vertexBufferStride;
	strides[1] = sizeof(InstanceType);

	ID3D11Buffer* bufferPointers[2];
	bufferPointers[0] = _geometry.vertexBuffer;
	bufferPointers[1] = _geometry.instanceBuffer;

	_pImmediateContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	_pImmediateContext->IASetIndexBuffer(_geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//_pImmediateContext->DrawInstanced(_geometry.numberOfIndices * _geometry.instanceCount, _geometry.instanceCount, 0, 0);
	_pImmediateContext->DrawIndexedInstanced(_geometry.numberOfIndices, _geometry.instanceCount, 0, 0, 0);
}

void Appearance::DrawTerrain()
{
	_pImmediateContext->IASetVertexBuffers(0, 1, &_geometry.vertexBuffer, &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	_pImmediateContext->IASetIndexBuffer(_geometry.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	_pImmediateContext->DrawIndexed(_geometry.numberOfIndices, 0, 0);
}