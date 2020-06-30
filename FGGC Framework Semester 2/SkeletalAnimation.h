#pragma once
#include "MD5Loader.h"
#include "Transform.h"
class SkeletalAnimation
{
private:
	std::vector<ID3D11ShaderResourceView*> _textureArray;
	std::vector<std::wstring> _textureArrayNames;
	ID3D11Device* _pd3dDevice;
	ID3D11DeviceContext* _pImmediateContext;
	Model3D _model;
	Transform* _transform;

public:
	SkeletalAnimation(std::wstring filePath, ID3D11Device* _pd3dDevice, ID3D11DeviceContext* immediateContext);
	~SkeletalAnimation();
	
	Transform* GetTransform() { return _transform; }
	void UpdateAnimation(float deltaTime, int animation);
	void Render(ConstantBuffer& cb, ID3D11Buffer* constantBuffer);
};