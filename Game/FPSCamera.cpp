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

    // Calculate the camera position relative to the target based on the offset
    Vector3 cameraOffset = Vector3::Transform(m_dpos, rotCam);
    m_pos = targetPos + cameraOffset;

    // Set the target position of the camera to be the same as the target object's position
    m_target = targetPos;

    // Set up projection matrix (assuming Camera::Tick updates the projection matrix)
    Camera::Tick(_GD);
}

