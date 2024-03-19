#include "pch.h"
#include "FPSCamera.h"
#include "GameData.h"



FPSCamera::FPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _targetPos, GameObject* _targetForRor,  Vector3 _up, Vector3 _dpos)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	m_targetPosObject = _targetPos;
	m_targetRotObject = _targetForRor;
	m_dpos = _dpos;
}

FPSCamera::~FPSCamera()
{

}

void FPSCamera::Tick(GameData* _GD)
{
	float rotSpeed = _GD->m_dt;
	auto mouse = _GD->m_MS;
	if (mouse.positionMode == Mouse::MODE_RELATIVE)
	{
		Vector3 delta = Vector3(mouse.x, mouse.y, 0.f)
			* rotSpeed;
		newPitch -= delta.y;
		if (m_pitch > maxPitch) newPitch = maxPitch;
		if (m_pitch < minPitch) newPitch = minPitch;
	}
	Matrix rotCam = Matrix::CreateFromYawPitchRoll(m_targetPosObject->GetYaw(), newPitch, 0.0f);
	m_target = m_targetPosObject->GetPos();
	m_pos = m_target + Vector3::Transform(m_dpos, rotCam);


	Camera::Tick(_GD);
}