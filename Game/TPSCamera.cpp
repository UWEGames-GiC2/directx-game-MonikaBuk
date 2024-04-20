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

	auto newYawn = m_targetObject->GetYaw();
	
	auto rotCam = Matrix::CreateFromYawPitchRoll(newYawn, 0, 0.0f);
	m_target = m_targetObject->GetPos();
	m_target.y += 30;
	Vector3 pos = m_target + Vector3::Transform(m_dpos, rotCam);
	m_pos = pos;



	Camera::Tick(_GameData);
}
