#include "MD5Loader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <WICTextureLoader.h>

namespace MD5Loader
{
	Model3D MD5Loader::LoadModel(std::wstring filePath, ID3D11Device* _pd3dDevice, std::vector<ID3D11ShaderResourceView*>& shaderResourceArray, std::vector<std::wstring>textureNameArray)
	{
		HRESULT hr;
		Model3D model;

		std::wifstream file;
		file.open(filePath.c_str());

		std::wstring buffer;

		if (!file.good())
		{
			std::cout << "Can't open MD5 file" << filePath.c_str() << std::endl;
			return model;
		}

		while (!file.eof())
		{
			file >> buffer;
			//std::getline(file, buffer);
			if (buffer == L"commandline")
			{
				std::getline(file, buffer);
			}
			else if (buffer == L"numJoints")
			{
				file >> model.numJoints;
			}
			else if (buffer == L"numMeshes")
			{
				file >> model.numSubsets;
			}
			else if(buffer == L"joints")
			{
				Joint tempJoint;

				file >> buffer; // Skip '{'
				for (auto i = 0; i < model.numJoints; i++)
				{
					file >> tempJoint.name;

					if (tempJoint.name[tempJoint.name.size() - 1] != '"')
					{
						wchar_t checkChar;
						bool jointNameFound = false;
						while (!jointNameFound)
						{
							checkChar = file.get();
							if (checkChar == '"')
								jointNameFound = true;
							tempJoint.name += checkChar;
						}
					}
					file >> tempJoint.parentID;	// Store Parent joint's ID

					file >> buffer;			// Skip the "("

					// Store position of this joint (swap y and z axis if model was made in RH Coord Sys)
					file >> tempJoint.pos.x >> tempJoint.pos.z >> tempJoint.pos.y;

					file >> buffer >> buffer;	// Skip the ")" and "("

					// Store orientation of this joint
					file >> tempJoint.orientation.x >> tempJoint.orientation.z >> tempJoint.orientation.y;

					// Remove the quotation marks from joints name
					tempJoint.name.erase(0, 1);
					tempJoint.name.erase(tempJoint.name.size() - 1, 1);

					// Compute the w axis of the quaternion (The MD5 model uses a 3D vector to describe the
					// direction the bone is facing. However, we need to turn this into a quaternion, and the way
					// quaternions work, is the xyz values describe the axis of rotation, while the w is a value
					// between 0 and 1 which describes the angle of rotation)
					float t = 1.0f - (tempJoint.orientation.x * tempJoint.orientation.x)
						- (tempJoint.orientation.y * tempJoint.orientation.y)
						- (tempJoint.orientation.z * tempJoint.orientation.z);
					if (t < 0.0f)
					{
						tempJoint.orientation.w = 0.0f;
					}
					else
					{
						tempJoint.orientation.w = -sqrtf(t);
					}

					std::getline(file, buffer);		// Skip rest of this line

					model.joints.push_back(tempJoint);
				}

				file >> buffer;
			}
			else if (buffer == L"mesh")
			{
				ModelSubset subset;
				int numVerts, numTris, numWeights;
				file >> buffer; //skips the {
				
				file >> buffer;

				while (buffer != L"}")			// Read until '}'
				{
					// In this lesson, for the sake of simplicity, we will assume a textures filename is givin here.
					// Usually though, the name of a material (stored in a material library. Think back to the lesson on
					// loading .obj files, where the material library was contained in the file .mtl) is givin. Let this
					// be an exercise to load the material from a material library such as obj's .mtl file, instead of
					// just the texture like we will do here.
					if (buffer == L"shader")		// Load the texture or material
					{
						std::wstring fileNamePath;
						file >> fileNamePath;			// Get texture's filename

						// Take spaces into account if filename or material name has a space in it
						if (fileNamePath[fileNamePath.size() - 1] != '"')
						{
							wchar_t checkChar;
							bool fileNameFound = false;
							while (!fileNameFound)
							{
								checkChar = file.get();

								if (checkChar == '"')
									fileNameFound = true;

								fileNamePath += checkChar;
							}
						}

						// Remove the quotation marks from texture path
						fileNamePath.erase(0, 1);
						fileNamePath.erase(fileNamePath.size() - 1, 1);

						//check if this texture has already been loaded
						bool alreadyLoaded = false;
						for (int i = 0; i < textureNameArray.size(); ++i)
						{
							if (fileNamePath == textureNameArray[i])
							{
								alreadyLoaded = true;
								subset.texArrayIndex = i;
							}
						}

						//if the texture is not already loaded, load it now
						if (!alreadyLoaded)
						{
							ID3D11ShaderResourceView* tempMeshSRV;

							std::wstring prefix = std::wstring(L"Animation\\");
							std::wstring TextureFilePath = prefix + fileNamePath;
							hr = CreateWICTextureFromFile(_pd3dDevice, TextureFilePath.c_str(), nullptr, &tempMeshSRV, 0);
							if (SUCCEEDED(hr))
							{
								textureNameArray.push_back(fileNamePath.c_str());
								subset.texArrayIndex = shaderResourceArray.size();
								shaderResourceArray.push_back(tempMeshSRV);
							}
							else
							{
								MessageBox(0, fileNamePath.c_str(),		//display message
									L"Could Not Open:", MB_OK);
								return model;
							}
						}

						std::getline(file, buffer);				// Skip rest of this line
					}
					else if (buffer == L"numverts")
					{
						file >> numVerts;								// Store number of vertices

						std::getline(file, buffer);				// Skip rest of this line

						for (int i = 0; i < numVerts; i++)
						{
							ModelVertex tempVert;

							file >> buffer						// Skip "vert # ("
								>> buffer
								>> buffer;

							file >> tempVert.texCoord.x				// Store tex coords
								>> tempVert.texCoord.y;

							file >> buffer;						// Skip ")"

							file >> tempVert.StartWeight;				// Index of first weight this vert will be weighted to

							file >> tempVert.WeightCount;				// Number of weights for this vertex

							std::getline(file, buffer);			// Skip rest of this line

							subset.vertices.push_back(tempVert);		// Push back this vertex into subsets vertex vector
						}
					}
					else if (buffer == L"numtris")
					{
						file >> numTris;
						subset.numTriangles = numTris;

						std::getline(file, buffer);				// Skip rest of this line

						for (int i = 0; i < numTris; i++)				// Loop through each triangle
						{
							DWORD tempIndex;
							file >> buffer;						// Skip "tri"
							file >> buffer;						// Skip tri counter

							for (int k = 0; k < 3; k++)					// Store the 3 indices
							{
								file >> tempIndex;
								subset.indices.push_back(tempIndex);
							}

							std::getline(file, buffer);			// Skip rest of this line
						}
					}
					else if (buffer == L"numweights")
					{
						file >> numWeights;

						std::getline(file, buffer);				// Skip rest of this line

						for (int i = 0; i < numWeights; i++)
						{
							Weight tempWeight;
							file >> buffer >> buffer;		// Skip "weight #"

							file >> tempWeight.jointID;				// Store weight's joint ID

							file >> tempWeight.bias;					// Store weight's influence over a vertex

							file >> buffer;						// Skip "("

							file >> tempWeight.pos.x					// Store weight's pos in joint's local space
								>> tempWeight.pos.z
								>> tempWeight.pos.y;

							std::getline(file, buffer);			// Skip rest of this line

							subset.weights.push_back(tempWeight);		// Push back tempWeight into subsets Weight array
						}

					}
					else
						std::getline(file, buffer);				// Skip anything else

					file >> buffer;								// Skip "}"
				}

				for (auto i = 0; i < subset.vertices.size(); i++)
				{
					ModelVertex tempVert = subset.vertices[i];
					tempVert.pos = XMFLOAT3(0, 0, 0);	// Make sure the vertex's pos is cleared first

					// Sum up the joints and weights information to get vertex's position
					for (int j = 0; j < tempVert.WeightCount; ++j)
					{
						Weight tempWeight = subset.weights[tempVert.StartWeight + j];
						Joint tempJoint = model.joints[tempWeight.jointID];

						// Convert joint orientation and weight pos to vectors for easier computation
						// When converting a 3d vector to a quaternion, you should put 0 for "w", and
						// When converting a quaternion to a 3d vector, you can just ignore the "w"
						XMVECTOR tempJointOrientation = XMVectorSet(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
						XMVECTOR tempWeightPos = XMVectorSet(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, 0.0f);

						// We will need to use the conjugate of the joint orientation quaternion
						// To get the conjugate of a quaternion, all you have to do is inverse the x, y, and z
						XMVECTOR tempJointOrientationConjugate = XMVectorSet(-tempJoint.orientation.x, -tempJoint.orientation.y, -tempJoint.orientation.z, tempJoint.orientation.w);

						// Calculate vertex position (in joint space, eg. rotate the point around (0,0,0)) for this weight using the joint orientation quaternion and its conjugate
						// We can rotate a point using a quaternion with the equation "rotatedPoint = quaternion * point * quaternionConjugate"
						XMFLOAT3 rotatedPoint;
						XMStoreFloat3(&rotatedPoint, XMQuaternionMultiply(XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempJointOrientationConjugate));

						// Now move the verices position from joint space (0,0,0) to the joints position in world space, taking the weights bias into account
						// The weight bias is used because multiple weights might have an effect on the vertices final position. Each weight is attached to one joint.
						tempVert.pos.x += (tempJoint.pos.x + rotatedPoint.x) * tempWeight.bias;
						tempVert.pos.y += (tempJoint.pos.y + rotatedPoint.y) * tempWeight.bias;
						tempVert.pos.z += (tempJoint.pos.z + rotatedPoint.z) * tempWeight.bias;

						// Basically what has happened above, is we have taken the weights position relative to the joints position
						// we then rotate the weights position (so that the weight is actually being rotated around (0, 0, 0) in world space) using
						// the quaternion describing the joints rotation. We have stored this rotated point in rotatedPoint, which we then add to
						// the joints position (because we rotated the weight's position around (0,0,0) in world space, and now need to translate it
						// so that it appears to have been rotated around the joints position). Finally we multiply the answer with the weights bias,
						// or how much control the weight has over the final vertices position. All weight's bias effecting a single vertex's position
						// must add up to 1.
					}

					subset.positions.push_back(tempVert.pos);			// Store the vertices position in the position vector instead of straight into the vertex vector
					// since we can use the positions vector for certain things like collision detection or picking
					// without having to work with the entire vertex structure.
				}

				for (auto i = 0; i < subset.vertices.size(); i++)
				{
					subset.vertices[i].pos = subset.positions[i];
				}

				std::vector<XMFLOAT3> tempNormal;

				XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

				float vecX, vecY, vecZ;

				//Two edges of the triangle
				XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

				//Compute face normals
				for (int i = 0; i < subset.numTriangles; ++i)
				{
					//Get the vector describing one edge of our triangle (edge 0,2)
					vecX = subset.vertices[subset.indices[(i * 3)]].pos.x - subset.vertices[subset.indices[(i * 3) + 2]].pos.x;
					vecY = subset.vertices[subset.indices[(i * 3)]].pos.y - subset.vertices[subset.indices[(i * 3) + 2]].pos.y;
					vecZ = subset.vertices[subset.indices[(i * 3)]].pos.z - subset.vertices[subset.indices[(i * 3) + 2]].pos.z;
					edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our first edge

					//Get the vector describing another edge of our triangle (edge 2,1)
					vecX = subset.vertices[subset.indices[(i * 3) + 2]].pos.x - subset.vertices[subset.indices[(i * 3) + 1]].pos.x;
					vecY = subset.vertices[subset.indices[(i * 3) + 2]].pos.y - subset.vertices[subset.indices[(i * 3) + 1]].pos.y;
					vecZ = subset.vertices[subset.indices[(i * 3) + 2]].pos.z - subset.vertices[subset.indices[(i * 3) + 1]].pos.z;
					edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our second edge

					//Cross multiply the two edge vectors to get the un-normalized face normal
					XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));

					tempNormal.push_back(unnormalized);
				}
				//Compute vertex normals (normal Averaging)
				XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				int facesUsing = 0;
				float tX, tY, tZ;	//temp axis variables

				//Go through each vertex
				for (int i = 0; i < subset.vertices.size(); ++i)
				{
					//Check which triangles use this vertex
					for (int j = 0; j < subset.numTriangles; ++j)
					{
						if (subset.indices[j * 3] == i ||
							subset.indices[(j * 3) + 1] == i ||
							subset.indices[(j * 3) + 2] == i)
						{
							tX = XMVectorGetX(normalSum) + tempNormal[j].x;
							tY = XMVectorGetY(normalSum) + tempNormal[j].y;
							tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

							normalSum = XMVectorSet(tX, tY, tZ, 0.0f);	//If a face is using the vertex, add the unormalized face normal to the normalSum

							facesUsing++;
						}
					}

					//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
					normalSum = normalSum / facesUsing;

					//Normalize the normalSum vector
					normalSum = XMVector3Normalize(normalSum);

					//Store the normal and tangent in our current vertex
					subset.vertices[i].normal.x = -XMVectorGetX(normalSum);
					subset.vertices[i].normal.y = -XMVectorGetY(normalSum);
					subset.vertices[i].normal.z = -XMVectorGetZ(normalSum);

					//Clear normalSum, facesUsing for next vertex
					normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
					facesUsing = 0;
				}

				// Create index buffer
				D3D11_BUFFER_DESC indexBufferDesc;
				ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

				indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				indexBufferDesc.ByteWidth = sizeof(DWORD) * subset.numTriangles * 3;
				indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				indexBufferDesc.CPUAccessFlags = 0;
				indexBufferDesc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA iinitData;

				iinitData.pSysMem = &subset.indices[0];
				_pd3dDevice->CreateBuffer(&indexBufferDesc, &iinitData, &subset.indexBuff);

				//Create Vertex Buffer
				D3D11_BUFFER_DESC vertexBufferDesc;
				ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

				vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;							// We will be updating this buffer, so we must set as dynamic
				vertexBufferDesc.ByteWidth = sizeof(ModelVertex) * subset.vertices.size();
				vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				// Give CPU power to write to buffer
				vertexBufferDesc.MiscFlags = 0;

				D3D11_SUBRESOURCE_DATA vertexBufferData;

				ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
				vertexBufferData.pSysMem = &subset.vertices[0];
				hr = _pd3dDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &subset.vertBuff);

				// Push back the temp subset into the models subset vector
				model.subsets.push_back(subset);
			}
		}

		return model;
	}

	bool MD5Loader::LoadMD5Anim(std::wstring filePath, Model3D& _model)
	{
		ModelAnimation tempAnim;						// Temp animation to later store in our model's animation array

		std::wifstream fileIn(filePath.c_str());		// Open file

		std::wstring checkString;						// Stores the next string from our file

		if (fileIn)										// Check if the file was opened
		{
			while (fileIn)								// Loop until the end of the file is reached
			{
				fileIn >> checkString;					// Get next string from file

				if (checkString == L"MD5Version")		// Get MD5 version (this function supports version 10)
				{
					fileIn >> checkString;
					/*MessageBox(0, checkString.c_str(),	//display message
					L"MD5Version", MB_OK);*/
				}
				else if (checkString == L"commandline")
				{
					std::getline(fileIn, checkString);	// Ignore the rest of this line
				}
				else if (checkString == L"numFrames")
				{
					fileIn >> tempAnim.numFrames;				// Store number of frames in this animation
				}
				else if (checkString == L"numJoints")
				{
					fileIn >> tempAnim.numJoints;				// Store number of joints (must match .md5mesh)
				}
				else if (checkString == L"frameRate")
				{
					fileIn >> tempAnim.frameRate;				// Store animation's frame rate (frames per second)
				}
				else if (checkString == L"numAnimatedComponents")
				{
					fileIn >> tempAnim.numAnimatedComponents;	// Number of components in each frame section
				}
				else if (checkString == L"hierarchy")
				{
					fileIn >> checkString;				// Skip opening bracket "{"

					for (int i = 0; i < tempAnim.numJoints; i++)	// Load in each joint
					{
						AnimJointInfo tempJoint;

						fileIn >> tempJoint.name;		// Get joints name
						// Sometimes the names might contain spaces. If that is the case, we need to continue
						// to read the name until we get to the closing " (quotation marks)
						if (tempJoint.name[tempJoint.name.size() - 1] != '"')
						{
							wchar_t checkChar;
							bool jointNameFound = false;
							while (!jointNameFound)
							{
								checkChar = fileIn.get();

								if (checkChar == '"')
									jointNameFound = true;

								tempJoint.name += checkChar;
							}
						}

						// Remove the quotation marks from joints name
						tempJoint.name.erase(0, 1);
						tempJoint.name.erase(tempJoint.name.size() - 1, 1);

						fileIn >> tempJoint.parentID;			// Get joints parent ID
						fileIn >> tempJoint.flags;				// Get flags
						fileIn >> tempJoint.startIndex;			// Get joints start index

						// Make sure the joint exists in the model, and the parent ID's match up
						// because the bind pose (md5mesh) joint hierarchy and the animations (md5anim)
						// joint hierarchy must match up
						bool jointMatchFound = false;
						for (int k = 0; k < _model.numJoints; k++)
						{
							if (_model.joints[k].name == tempJoint.name)
							{
								if (_model.joints[k].parentID == tempJoint.parentID)
								{
									jointMatchFound = true;
									tempAnim.jointInfo.push_back(tempJoint);
								}
							}
						}
						if (!jointMatchFound)					// If the skeleton system does not match up, return false
							return false;						// You might want to add an error message here

						std::getline(fileIn, checkString);		// Skip rest of this line
					}
				}
				else if (checkString == L"bounds")			// Load in the AABB for each animation
				{
					fileIn >> checkString;						// Skip opening bracket "{"

					for (int i = 0; i < tempAnim.numFrames; i++)
					{
						BoundingBox tempBB;

						fileIn >> checkString;					// Skip "("
						fileIn >> tempBB.min.x >> tempBB.min.z >> tempBB.min.y;
						fileIn >> checkString >> checkString;	// Skip ") ("
						fileIn >> tempBB.max.x >> tempBB.max.z >> tempBB.max.y;
						fileIn >> checkString;					// Skip ")"

						tempAnim.frameBounds.push_back(tempBB);
					}
				}
				else if (checkString == L"baseframe")			// This is the default position for the animation
				{												// All frames will build their skeletons off this
					fileIn >> checkString;						// Skip opening bracket "{"

					for (int i = 0; i < tempAnim.numJoints; i++)
					{
						Joint tempBFJ;

						fileIn >> checkString;						// Skip "("
						fileIn >> tempBFJ.pos.x >> tempBFJ.pos.z >> tempBFJ.pos.y;
						fileIn >> checkString >> checkString;		// Skip ") ("
						fileIn >> tempBFJ.orientation.x >> tempBFJ.orientation.z >> tempBFJ.orientation.y;
						fileIn >> checkString;						// Skip ")"

						tempAnim.baseFrameJoints.push_back(tempBFJ);
					}
				}
				else if (checkString == L"frame")		// Load in each frames skeleton (the parts of each joint that changed from the base frame)
				{
					FrameData tempFrame;

					fileIn >> tempFrame.frameID;		// Get the frame ID

					fileIn >> checkString;				// Skip opening bracket "{"

					for (int i = 0; i < tempAnim.numAnimatedComponents; i++)
					{
						float tempData;
						fileIn >> tempData;				// Get the data

						tempFrame.frameData.push_back(tempData);
					}

					tempAnim.frameData.push_back(tempFrame);

					///*** build the frame skeleton ***///
					std::vector<Joint> tempSkeleton;

					for (int i = 0; i < tempAnim.jointInfo.size(); i++)
					{
						int k = 0;						// Keep track of position in frameData array

						// Start the frames joint with the base frame's joint
						Joint tempFrameJoint = tempAnim.baseFrameJoints[i];

						tempFrameJoint.parentID = tempAnim.jointInfo[i].parentID;

						// Notice how I have been flipping y and z. this is because some modeling programs such as
						// 3ds max (which is what I use) use a right handed coordinate system. Because of this, we
						// need to flip the y and z axes. If your having problems loading some models, it's possible
						// the model was created in a left hand coordinate system. in that case, just reflip all the
						// y and z axes in our md5 mesh and anim loader.
						if (tempAnim.jointInfo[i].flags & 1)		// pos.x	( 000001 )
							tempFrameJoint.pos.x = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

						if (tempAnim.jointInfo[i].flags & 2)		// pos.y	( 000010 )
							tempFrameJoint.pos.z = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

						if (tempAnim.jointInfo[i].flags & 4)		// pos.z	( 000100 )
							tempFrameJoint.pos.y = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

						if (tempAnim.jointInfo[i].flags & 8)		// orientation.x	( 001000 )
							tempFrameJoint.orientation.x = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

						if (tempAnim.jointInfo[i].flags & 16)	// orientation.y	( 010000 )
							tempFrameJoint.orientation.z = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

						if (tempAnim.jointInfo[i].flags & 32)	// orientation.z	( 100000 )
							tempFrameJoint.orientation.y = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];


						// Compute the quaternions w
						float t = 1.0f - (tempFrameJoint.orientation.x * tempFrameJoint.orientation.x)
							- (tempFrameJoint.orientation.y * tempFrameJoint.orientation.y)
							- (tempFrameJoint.orientation.z * tempFrameJoint.orientation.z);
						if (t < 0.0f)
						{
							tempFrameJoint.orientation.w = 0.0f;
						}
						else
						{
							tempFrameJoint.orientation.w = -sqrtf(t);
						}

						// Now, if the upper arm of your skeleton moves, you need to also move the lower part of your arm, and then the hands, and then finally the fingers (possibly weapon or tool too)
						// This is where joint hierarchy comes in. We start at the top of the hierarchy, and move down to each joints child, rotating and translating them based on their parents rotation
						// and translation. We can assume that by the time we get to the child, the parent has already been rotated and transformed based of it's parent. We can assume this because
						// the child should never come before the parent in the files we loaded in.
						if (tempFrameJoint.parentID >= 0)
						{
							Joint parentJoint = tempSkeleton[tempFrameJoint.parentID];

							// Turn the XMFLOAT3 and 4's into vectors for easier computation
							XMVECTOR parentJointOrientation = XMVectorSet(parentJoint.orientation.x, parentJoint.orientation.y, parentJoint.orientation.z, parentJoint.orientation.w);
							XMVECTOR tempJointPos = XMVectorSet(tempFrameJoint.pos.x, tempFrameJoint.pos.y, tempFrameJoint.pos.z, 0.0f);
							XMVECTOR parentOrientationConjugate = XMVectorSet(-parentJoint.orientation.x, -parentJoint.orientation.y, -parentJoint.orientation.z, parentJoint.orientation.w);

							// Calculate current joints position relative to its parents position
							XMFLOAT3 rotatedPos;
							XMStoreFloat3(&rotatedPos, XMQuaternionMultiply(XMQuaternionMultiply(parentJointOrientation, tempJointPos), parentOrientationConjugate));

							// Translate the joint to model space by adding the parent joint's pos to it
							tempFrameJoint.pos.x = rotatedPos.x + parentJoint.pos.x;
							tempFrameJoint.pos.y = rotatedPos.y + parentJoint.pos.y;
							tempFrameJoint.pos.z = rotatedPos.z + parentJoint.pos.z;

							// Currently the joint is oriented in its parent joints space, we now need to orient it in
							// model space by multiplying the two orientations together (parentOrientation * childOrientation) <- In that order
							XMVECTOR tempJointOrient = XMVectorSet(tempFrameJoint.orientation.x, tempFrameJoint.orientation.y, tempFrameJoint.orientation.z, tempFrameJoint.orientation.w);
							tempJointOrient = XMQuaternionMultiply(parentJointOrientation, tempJointOrient);

							// Normalize the orienation quaternion
							tempJointOrient = XMQuaternionNormalize(tempJointOrient);

							XMStoreFloat4(&tempFrameJoint.orientation, tempJointOrient);
						}

						// Store the joint into our temporary frame skeleton
						tempSkeleton.push_back(tempFrameJoint);
					}

					// Push back our newly created frame skeleton into the animation's frameSkeleton array
					tempAnim.frameSkeleton.push_back(tempSkeleton);

					fileIn >> checkString;				// Skip closing bracket "}"
				}
			}

			// Calculate and store some usefull animation data
			tempAnim.frameTime = 1.0f / tempAnim.frameRate;						// Set the time per frame
			tempAnim.totalAnimTime = tempAnim.numFrames * tempAnim.frameTime;	// Set the total time the animation takes
			tempAnim.currAnimTime = 0.0f;										// Set the current time to zero

			_model.animations.push_back(tempAnim);							// Push back the animation into our model object
		}
		else	// If the file was not loaded
		{
			// create message
			std::wstring message = L"Could not open: ";
			message += filePath;

			MessageBox(0, message.c_str(),				// display message
				L"Error", MB_OK);

			return false;
		}
		return true;
	}
}