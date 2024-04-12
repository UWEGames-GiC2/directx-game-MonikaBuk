#include "pch.h"
#include "gameobject.h"
#include "GameData.h"

GameObject::GameObject()
{
	//set the Gameobject to the origin with no rotation and unit scaling 
	m_pos = Vector3::Zero;
	m_pitch = 0.0f;
	m_yaw = 0.0f;
	m_roll = 0.0f;
	m_scale = Vector3::One;

	m_worldMat = Matrix::Identity;
	m_fudge = Matrix::Identity;
}

GameObject::~GameObject()
{

}

void GameObject::Tick(GameData* _GameData)
{
	if (m_physicsOn)
	{
		Vector3 newVel = m_vel + _GameData->m_DeltaTime * (m_acc - m_drag*m_vel);
		Vector3 newPos = m_pos + _GameData->m_DeltaTime * m_vel;

		m_vel = newVel;
		m_pos = newPos;
	}

	//build up the world matrix depending on the new position of the GameObject
	//the assumption is that this class will be inherited by the class that ACTUALLY changes this
	Matrix  scaleMat = Matrix::CreateScale(m_scale);
	Quaternion rotationQuaternion = Quaternion::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);
	rotationQuaternion.Normalize();
	m_rotMat = Matrix::CreateFromQuaternion(rotationQuaternion);
	Matrix  transMat = Matrix::CreateTranslation(m_pos);

	m_worldMat = m_fudge * scaleMat * m_rotMat * transMat;

	//zero acceleration ready for the next time round
	m_acc = Vector3::Zero;
}