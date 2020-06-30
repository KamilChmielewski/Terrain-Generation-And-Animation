#pragma once
#include <xstring>
#include <DirectXMath.h>
#include <vector>
//#include <minwindef.h>
#include <Windows.h>
#include <d3d11.h>
#include "ConstantBuffers.h"

using namespace DirectX;

struct ModelVertex	//Overloaded Vertex Structure
{
	ModelVertex() {}
	ModelVertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz,
		float tx, float ty, float tz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
	//XMFLOAT3 tangent;
	//XMFLOAT3 biTangent;

	///////////////**************new**************////////////////////
		// Will not be sent to shader
	int StartWeight;
	int WeightCount;
	///////////////**************new**************////////////////////
};

struct Joint
{
	std::wstring name;
	int parentID;

	XMFLOAT3 pos;
	XMFLOAT4 orientation;
};

struct BoundingBox
{
	XMFLOAT3 min, max;
};

struct FrameData
{
	int frameID;
	std::vector<float> frameData;
};

struct AnimJointInfo
{
	std::wstring name;
	int parentID;
	int flags;
	int startIndex;
};

struct ModelAnimation
{
	int numFrames;
	int numJoints;
	int frameRate;
	int numAnimatedComponents;

	float frameTime;
	float totalAnimTime;
	float currAnimTime;

	std::vector<AnimJointInfo> jointInfo;
	std::vector<BoundingBox> frameBounds;
	std::vector<Joint>	baseFrameJoints;
	std::vector<FrameData>	frameData;
	std::vector<std::vector<Joint>> frameSkeleton;
};

struct Weight
{
	int jointID;
	float bias;
	XMFLOAT3 pos;
	XMFLOAT3 normal;
};

struct ModelSubset
{
	int texArrayIndex;
	int numTriangles;

	std::vector<ModelVertex> vertices;
	std::vector<DWORD> indices;
	std::vector<Weight> weights;

	std::vector<XMFLOAT3> positions;

	ID3D11Buffer* vertBuff;
	ID3D11Buffer* indexBuff;

};

struct Model3D
{
	int numSubsets;
	int numJoints;

	std::vector<Joint> joints;
	std::vector<ModelSubset> subsets;

	std::vector<ModelAnimation> animations;
};