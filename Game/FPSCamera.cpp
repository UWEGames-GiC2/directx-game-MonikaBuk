#include "pch.h"
#include "FPSCamera.h"
#include "GameData.h"
#include <dinput.h>
#include "GameData.h"



FPSCamera::FPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, std::shared_ptr<GameObject> _targetPos,  Vector3 _up, Vector3 _dpos, float _width, float _height)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	m_targetPosObject = _targetPos;
	m_dpos = _dpos;
	m_width = _width;
	m_height = _height;
	m_newPitch = _targetPos->GetPitch();
}

FPSCamera::~FPSCamera()
{
	 
}

void FPSCamera::Tick(GameData* _GameData)
{
	float rotSpeed = _GameData->m_DeltaTime /2;
	auto mouse = _GameData->m_Mouse;


	if (!_GameData->isPaused) {
		m_forwardVector = Vector3::Forward;
		if (mouse.positionMode == Mouse::MODE_RELATIVE)
		{
			m_newPitch -= (mouse.y * rotSpeed);
			if (m_newPitch > m_maxPitch)
			{
				m_newPitch = m_maxPitch;
			}
			if (m_newPitch < m_minPitch)
			{
				m_newPitch = m_minPitch;
			}
		}
	}
	m_newYawn = m_targetPosObject->GetYaw();
	m_rotCam = Matrix::CreateFromYawPitchRoll(m_newYawn , m_newPitch - offsetY, 0.0f);
	m_target = m_targetPosObject->GetPos();
	m_target.y += 30;
	Vector3 pos = m_target + Vector3::Transform(m_dpos, m_rotCam);
	m_pos = pos;
	Camera::Tick(_GameData);
}

Vector3 FPSCamera::GetCenterOfScreen(GameData* _GameData)
{
	
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

