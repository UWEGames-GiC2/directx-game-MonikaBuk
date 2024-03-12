#include "pch.h"
#include "FPSCamera.h"


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
	
	Matrix rotCam = Matrix::CreateFromYawPitchRoll(m_targetPosObject->GetYaw(), m_targetRotObject->GetPitch(), 0.0f);
	m_target = m_targetPosObject->GetPos();
	m_pos = m_target + Vector3::Transform(m_dpos, rotCam);


	Camera::Tick(_GD);
}