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
	//Set up position of camera and target position of camera based on new position and orientation of target object

	m_pos = m_targetObject->GetPos() + m_dpos;
	Matrix rotCam = Matrix::CreateLookAt(m_pos, m_targetObject->GetPos(), Vector3::Up);
	m_viewMat = rotCam;


	//and then set up proj and view matrices
	Camera::Tick(_GameData);
}

