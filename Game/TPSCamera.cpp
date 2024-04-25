#include "pch.h"
#include "TPSCamera.h"
#include "GameData.h"

TPSCamera::TPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, std::shared_ptr<GameObject> _target, Vector3 _up, Vector3 _dpos)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	m_targetObject = _target;
	m_dpos = _dpos;
}

TPSCamera::~TPSCamera()
{

}

void TPSCamera::Tick(GameData* _GameData)
{
	float targetYaw = m_targetObject->GetYaw();
	float interpolationFactor = 0.1f; 
	float currentYaw = GetYaw(); 
	float targetOffsetY = XMConvertToRadians(90);
	Matrix rotCam = Matrix::CreateFromYawPitchRoll(targetYaw, targetOffsetY, 0);

	m_target = m_targetObject->GetPos();
	m_target.y += 30;
	Vector3 pos = m_target + Vector3::Transform(m_dpos, rotCam);
	m_pos = Vector3::Lerp(GetPos(), pos, 0.1);
	Camera::Tick(_GameData);
}

