#include "Camera.h"
#include <iostream>

Camera::Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
	: _eye(position), _at(at), _up(up), _windowWidth(windowWidth), _windowHeight(windowHeight), _nearDepth(nearDepth), _farDepth(farDepth)
{
	XMStoreFloat4x4(&_view, XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_at), XMLoadFloat3(&_up)));
	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(0.25f * XM_PI, _windowWidth / _windowHeight, _nearDepth, _farDepth));
	horizontalAngle = 0.0f;
	verticalAngle = 0.0f;

	vEye = XMLoadFloat3(&_eye);
}

Camera::~Camera()
{
}

void Camera::Update()
{

	static bool initCursor = false;

	if ((GetKeyState(VK_SHIFT) & 0x8000))
	{
		if (!initCursor)
		{
			SetCursorPos(_windowWidth / 2, _windowHeight / 2);
			initCursor = true;
		}

		POINT _mousePos;

		GetCursorPos(&_mousePos);

		SetCursorPos(_windowWidth / 2, _windowHeight / 2);

		horizontalAngle -= mouseSpeed * float(_windowWidth / 2 - _mousePos.x);
		verticalAngle += mouseSpeed * float(_windowHeight / 2 - _mousePos.y);

		XMFLOAT3 direction = XMFLOAT3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
		XMFLOAT3 right = XMFLOAT3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));
		
		XMVECTOR vDirection = XMLoadFloat3(&direction);
		XMVECTOR vRight = XMLoadFloat3(&right);
		XMVECTOR vUp = XMVector3Cross(vRight, vDirection);
				
		//XMVECTOR vEye = XMLoadFloat3(&_eye);

		if ((GetAsyncKeyState('W') & 0x8000))
		{
			vEye += (vDirection * 0.05 * speed);
		}
		if ((GetAsyncKeyState('S') & 0x8000))
		{
			vEye -= (vDirection * 0.05 * speed);
		}
		if ((GetAsyncKeyState('D') & 0x8000))
		{
			vEye -= (vRight * 0.05 * speed);
		}
		if ((GetAsyncKeyState('A') & 0x8000))
		{
			vEye += (vRight * 0.05 * speed);
		}
		
		XMStoreFloat4x4(&_view, XMMatrixLookAtLH(vEye, vEye + vDirection, vUp));
		
	}
	else if (!(GetKeyState(VK_SHIFT) & 0x8000))
	{
		initCursor = false;
	}

}

void Camera::Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_nearDepth = nearDepth;
	_farDepth = farDepth;
}

XMFLOAT4X4 Camera::GetViewProjection() const 
{ 
	XMMATRIX view = XMLoadFloat4x4(&_view);
	XMMATRIX projection = XMLoadFloat4x4(&_projection);

	XMFLOAT4X4 viewProj;

	XMStoreFloat4x4(&viewProj, view * projection);

	return viewProj;
}