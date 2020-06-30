#include "SkeletalAnimation.h"

SkeletalAnimation::SkeletalAnimation(std::wstring filePath, ID3D11Device* pd3dDevice, ID3D11DeviceContext* ImmediateContext) : _pd3dDevice(pd3dDevice), _pImmediateContext(ImmediateContext)
{
	//if(!Load_model(L"boy.md5mesh", New_model, meshSRV, textureNameArray))
	_model = MD5Loader::LoadModel(filePath, _pd3dDevice, _textureArray, _textureArrayNames);

	MD5Loader::LoadMD5Anim(L"Animation\\boy.md5anim", _model);

	_transform = new Transform();
	_transform->SetScale(0.04f, 0.04f, 0.04f);
	_transform->SetPosition(-8.0f, 0.5f, 10.0f);

	_transform->UpdateWorldMatrix();
}

SkeletalAnimation::~SkeletalAnimation()
{

}

void SkeletalAnimation::UpdateAnimation(float deltaTime, int animation)
{
	_model.animations[animation].currAnimTime += deltaTime;			// Update the current animation time

	if (_model.animations[animation].currAnimTime > _model.animations[animation].totalAnimTime)
		_model.animations[animation].currAnimTime = 0.0f;

	// Which frame are we on
	float currentFrame = _model.animations[animation].currAnimTime * _model.animations[animation].frameRate;
	int frame0 = floorf(currentFrame);
	int frame1 = frame0 + 1;

	// Make sure we don't go over the number of frames	
	if (frame0 == _model.animations[animation].numFrames - 1)
		frame1 = 0;

	float interpolation = currentFrame - frame0;	// Get the remainder (in time) between frame0 and frame1 to use as interpolation factor

	std::vector<Joint> interpolatedSkeleton;		// Create a frame skeleton to store the interpolated skeletons in

	// Compute the interpolated skeleton
	for (int i = 0; i < _model.animations[animation].numJoints; i++)
	{
		Joint tempJoint;
		Joint joint0 = _model.animations[animation].frameSkeleton[frame0][i];		// Get the i'th joint of frame0's skeleton
		Joint joint1 = _model.animations[animation].frameSkeleton[frame1][i];		// Get the i'th joint of frame1's skeleton

		tempJoint.parentID = joint0.parentID;											// Set the tempJoints parent id

		// Turn the two quaternions into XMVECTORs for easy computations
		XMVECTOR joint0Orient = XMVectorSet(joint0.orientation.x, joint0.orientation.y, joint0.orientation.z, joint0.orientation.w);
		XMVECTOR joint1Orient = XMVectorSet(joint1.orientation.x, joint1.orientation.y, joint1.orientation.z, joint1.orientation.w);

		// Interpolate positions
		tempJoint.pos.x = joint0.pos.x + (interpolation * (joint1.pos.x - joint0.pos.x));
		tempJoint.pos.y = joint0.pos.y + (interpolation * (joint1.pos.y - joint0.pos.y));
		tempJoint.pos.z = joint0.pos.z + (interpolation * (joint1.pos.z - joint0.pos.z));

		// Interpolate orientations using spherical interpolation (Slerp)
		XMStoreFloat4(&tempJoint.orientation, XMQuaternionSlerp(joint0Orient, joint1Orient, interpolation));

		interpolatedSkeleton.push_back(tempJoint);		// Push the joint back into our interpolated skeleton
	}

	for (int k = 0; k < _model.numSubsets; k++)
	{
		for (int i = 0; i < _model.subsets[k].vertices.size(); ++i)
		{
			ModelVertex tempVert = _model.subsets[k].vertices[i];
			tempVert.pos = XMFLOAT3(0, 0, 0);	// Make sure the vertex's pos is cleared first
			tempVert.normal = XMFLOAT3(0, 0, 0);	// Clear vertices normal

			// Sum up the joints and weights information to get vertex's position and normal
			for (int j = 0; j < tempVert.WeightCount; ++j)
			{
				Weight tempWeight = _model.subsets[k].weights[tempVert.StartWeight + j];
				Joint tempJoint = interpolatedSkeleton[tempWeight.jointID];

				// Convert joint orientation and weight pos to vectors for easier computation
				XMVECTOR tempJointOrientation = XMVectorSet(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
				XMVECTOR tempWeightPos = XMVectorSet(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, 0.0f);

				// We will need to use the conjugate of the joint orientation quaternion
				XMVECTOR tempJointOrientationConjugate = XMQuaternionInverse(tempJointOrientation);

				// Calculate vertex position (in joint space, eg. rotate the point around (0,0,0)) for this weight using the joint orientation quaternion and its conjugate
				// We can rotate a point using a quaternion with the equation "rotatedPoint = quaternion * point * quaternionConjugate"
				XMFLOAT3 rotatedPoint;
				XMStoreFloat3(&rotatedPoint, XMQuaternionMultiply(XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempJointOrientationConjugate));

				// Now move the verices position from joint space (0,0,0) to the joints position in world space, taking the weights bias into account
				tempVert.pos.x += (tempJoint.pos.x + rotatedPoint.x) * tempWeight.bias;
				tempVert.pos.y += (tempJoint.pos.y + rotatedPoint.y) * tempWeight.bias;
				tempVert.pos.z += (tempJoint.pos.z + rotatedPoint.z) * tempWeight.bias;

				// Compute the normals for this frames skeleton using the weight normals from before
				// We can comput the normals the same way we compute the vertices position, only we don't have to translate them (just rotate)
				XMVECTOR tempWeightNormal = XMVectorSet(tempWeight.normal.x, tempWeight.normal.y, tempWeight.normal.z, 0.0f);

				// Rotate the normal
				XMStoreFloat3(&rotatedPoint, XMQuaternionMultiply(XMQuaternionMultiply(tempJointOrientation, tempWeightNormal), tempJointOrientationConjugate));

				// Add to vertices normal and ake weight bias into account
				tempVert.normal.x -= rotatedPoint.x * tempWeight.bias;
				tempVert.normal.y -= rotatedPoint.y * tempWeight.bias;
				tempVert.normal.z -= rotatedPoint.z * tempWeight.bias;
			}

			_model.subsets[k].positions[i] = tempVert.pos;				// Store the vertices position in the position vector instead of straight into the vertex vector
			_model.subsets[k].vertices[i].normal = tempVert.normal;		// Store the vertices normal
			XMStoreFloat3(&_model.subsets[k].vertices[i].normal, XMVector3Normalize(XMLoadFloat3(&_model.subsets[k].vertices[i].normal)));
		}

		// Put the positions into the vertices for this subset
		for (int i = 0; i < _model.subsets[k].vertices.size(); i++)
		{
			_model.subsets[k].vertices[i].pos = _model.subsets[k].positions[i];
		}

		D3D11_MAPPED_SUBRESOURCE mappedVertBuff;
		HRESULT hr = _pImmediateContext->Map(_model.subsets[k].vertBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertBuff);

		// Copy the data into the vertex buffer.
		memcpy(mappedVertBuff.pData, &_model.subsets[k].vertices[0], (sizeof(ModelVertex) * _model.subsets[k].vertices.size()));

		_pImmediateContext->Unmap(_model.subsets[k].vertBuff, 0);

		_transform->UpdateWorldMatrix();

		//_pImmediateContext->UpdateSubresource(_model.subsets[k].vertBuff, 0, NULL, &_model.subsets[k].vertices[0], 0, 0 );
	}
}

void SkeletalAnimation::Render(ConstantBuffer& cb, ID3D11Buffer* constantBuffer)
{
	UINT stride = sizeof(ModelVertex);
	UINT offset = 0;


	for (auto i = 0; i < _model.numSubsets; i++)
	{
		 cb.World = XMMatrixTranspose(_transform->GetWorldMatrix());
		_pImmediateContext->PSSetShaderResources(0, 1, &_textureArray[_model.subsets[i].texArrayIndex]);
		
		_pImmediateContext->IASetIndexBuffer(_model.subsets[i].indexBuff, DXGI_FORMAT_R32_UINT, 0);
		_pImmediateContext->IASetVertexBuffers(0, 1, &_model.subsets[i].vertBuff, &stride, &offset);
		
		_pImmediateContext->UpdateSubresource(constantBuffer, 0, nullptr, &cb, 0, 0);
		
		_pImmediateContext->DrawIndexed(_model.subsets[i].indices.size(), 0, 0);
	}
}