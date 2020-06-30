#pragma once
#include "Structures.h"
#include <xstring>

namespace MD5Loader
{
	Model3D LoadModel(std::wstring filepath, ID3D11Device* _pd3dDevice, std::vector<ID3D11ShaderResourceView*>& shaderResourceArray, std::vector<std::wstring>textureNameArray);

	bool LoadMD5Anim(std::wstring filepath, Model3D& _model);
	
}