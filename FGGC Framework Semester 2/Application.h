#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include "Camera.h"

#include <vector>
#include "GameObject.h"
#include "Appearance.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "CollisionDetection.h"
#include "CollisionResolution.h"
#include "ConstantBuffers.h"
#include "StaticTerrain.h"
#include "StaticTerrain2.h"
#include "Voxel.h"
#include "SkeletalAnimation.h"

using namespace DirectX;


struct SimpleVertex
{
    XMFLOAT3 PosL;
	XMFLOAT3 NormL;
	XMFLOAT2 Tex;
};


class Application
{
private:
	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*           _pd3dDevice;
	ID3D11DeviceContext*    _pImmediateContext;
	IDXGISwapChain*         _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader*     _pVertexShader;
	ID3D11PixelShader*      _pPixelShader;
	
	ID3D11VertexShader* _pVertexInstanceShader;
	ID3D11PixelShader* _pPixelInstanceShader;
	ID3D11InputLayout* _pInstanceLayout;
	ID3D11InputLayout*      _pVertexLayout;


	ID3D11Buffer*           _pVertexBuffer;
	ID3D11Buffer*           _pIndexBuffer;

	ID3D11Buffer*           _pPlaneVertexBuffer;
	ID3D11Buffer*           _pPlaneIndexBuffer;

	ID3D11Buffer*           _pConstantBuffer;

	ID3D11DepthStencilView* _depthStencilView = nullptr;
	ID3D11Texture2D* _depthStencilBuffer = nullptr;

	ID3D11ShaderResourceView * _pTextureRV = nullptr;

	ID3D11ShaderResourceView * _pGroundTextureRV = nullptr;

	ID3D11SamplerState * _pSamplerLinear = nullptr;

	Light basicLight;

	vector<GameObject*> _gameObjects;

	GameObject* testObj;
	StaticTerrain* _staticTerrain;
	StaticTerrain2* _staticTerrain2;
	Voxel* _voxel;

	SkeletalAnimation* _skeleton;

	Appearance* _cubeAppearance;
	Appearance* _planeAppearance;

	Transform* _cubeTransform;
	Transform* _planeTransform;
	Particle* _paricle;
	ParticleSystem* _particleSystem;
	ParticleSystem* _testParticleSystem;
	GravityGenerator* _gravityGenerator;
	LaminarGenerator* _laminarGenerator;
	TurbulentGenerator* _turbulentGenerator;


	int _currentObject = 1;


	Camera* _camera;
	float _cameraOrbitRadius = 7.0f;
	float _cameraOrbitRadiusMin = 2.0f;
	float _cameraOrbitRadiusMax = 50.0f;
	float _cameraOrbitAngleXZ = -90.0f;
	float _cameraSpeed = 2.0f;

	UINT _WindowHeight;
	UINT _WindowWidth;

	// Render dimensions - Change here to alter screen resolution
	UINT _renderHeight = 1080;
	UINT _renderWidth = 1920;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;

private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();

	void moveForward(int objectNumber);

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	bool HandleKeyboard(MSG msg);

	void Update(float const deltaTime);
	void Draw();

	HWND GetHWND() { return _hWnd; };
};

