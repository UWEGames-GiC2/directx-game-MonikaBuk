#include "pch.h"
#include "FPSCamera.h"
#include "GameData.h"
#include <dinput.h>
#include "GameData.h"



FPSCamera::FPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _targetPos,  Vector3 _up, Vector3 _dpos, float _width, float _height)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	m_targetPosObject = _targetPos;
	m_dpos = _dpos;
	m_width = _width;
	m_height = _height;
}

FPSCamera::~FPSCamera()
{
	 
}

void FPSCamera::Tick(GameData* _GD)
{
	float rotSpeed = _GD->m_dt;
	auto mouse = _GD->m_MS;
	float offsetX = XMConvertToRadians(35.5);
	float offsetY = XMConvertToRadians(40);

	m_forwardVector = Vector3::Forward;
	if (mouse.positionMode == Mouse::MODE_RELATIVE)
	{
		Vector3 delta = Vector3(mouse.x, mouse.y, 0.f)
			* rotSpeed;
		newPitch -= delta.y;
		if (m_pitch > maxPitch) newPitch = maxPitch;
		if (m_pitch < minPitch) newPitch = minPitch;
	}
	newYawn = m_targetPosObject->GetYaw();
	rotCam = Matrix::CreateFromYawPitchRoll(newYawn - offsetX, newPitch + offsetY, 0.0f);
	m_target = m_targetPosObject->GetPos();
	m_pos = m_target + Vector3::Transform(m_dpos, rotCam);
	


	Camera::Tick(_GD);
}

Vector3 FPSCamera::GetCenterOfScreen(GameData* _GD)
{
	POINT p;

		if (GetCursorPos(&p))
		{
			//cursor position now in p.x and p.y
		}

	DirectX::XMVECTOR mouseNear = DirectX::XMVectorSet((float)m_width/2, m_height/2, 0.0f, 0.0f);
	DirectX::XMVECTOR mouseFar = DirectX::XMVectorSet((float)m_width/2, m_height / 2, 1.0f, 0.0f);
	DirectX::XMVECTOR unprojectedNear = DirectX::XMVector3Unproject(mouseNear, 0, 0, m_width, m_height, m_nearPlaneDistance, m_farPlaneDistance,
		GetProj(), GetView(), DirectX::XMMatrixIdentity());
	DirectX::XMVECTOR unprojectedFar = DirectX::XMVector3Unproject(mouseFar, 0, 0, m_width, m_height, m_nearPlaneDistance, m_farPlaneDistance,
		GetProj(), GetView(), DirectX::XMMatrixIdentity());
	DirectX::XMVECTOR result = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(unprojectedFar, unprojectedNear));
	DirectX::XMFLOAT3 direction;
	DirectX::XMStoreFloat3(&direction, result);
	return direction;

	//Get the distance to the ground.
	//DirectX::XMFLOAT3 cameraPosition = GetPos();

	//Get the point on the ground.
	//cameraPosition.x += direction.x * (cameraPosition.y / -direction.y);
	//cameraPosition.z += direction.z * (cameraPosition.y / -direction.y);

	
}

