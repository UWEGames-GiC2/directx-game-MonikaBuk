#include "pch.h"
#include "FPSCamera.h"


FPSCamera::FPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _target, Vector3 _up, Vector3 _dpos)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	m_targetObject = _target;
	m_dpos = _dpos;
}

FPSCamera::~FPSCamera()
{

}

void FPSCamera::Tick(GameData* _GD)
{
	// Get the target object's position and orientation
	Vector3 targetPos = m_targetObject->GetPos();
	float yaw = m_targetObject->GetYaw();
	float pitch = m_targetObject->GetPitch();
	float roll = m_targetObject->GetRoll();

	// Calculate the rotation matrix based on the target's orientation
	Matrix rotCam = Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);

	// Calculate the camera position relative to the target
	m_pos = targetPos;

	// Update the camera's view matrix
	m_viewMat = Matrix::CreateLookAt(m_pos, m_pos + Vector3::Transform(Vector3::Forward, rotCam), Vector3::Transform(Vector3::Up, rotCam));

	// Update the camera's projection matrix (assuming Camera::Tick updates the projection matrix)
	Camera::Tick(_GD);
}