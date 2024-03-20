#include "pch.h"
#include "Bullet.h"
#include "GameData.h"

Bullet::Bullet(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, FPSCamera* camera) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_fudge = Matrix::CreateRotationY(XM_PI);
	m_camera = camera;
	m_pos.y = 10.0f;

	SetDrag(0);
	SetPhysicsOn(false);
	SetVisibility(false);

}

Bullet::~Bullet()
{
}

void Bullet::Tick(GameData* _GD)
{
	if (m_isShot)
	{
		if (!IsVisible())
		{
			m_forwardMove = m_camera->getWorldTransform().Forward();
			Matrix rotMove = Matrix::CreateFromYawPitchRoll(m_camera->GetYaw(), m_camera->GetYaw(), 0);
			m_forwardMove = Vector3::Transform(m_forwardMove, rotMove);
			m_lifeTime = 10.0f;
			SetVisibility(true);
			SetPos(m_camera->GetPos());
			SetYaw(m_camera->GetYaw());
			SetPitch(m_camera->GetYaw());
			SetDrag(0.2);
			SetPhysicsOn(true);
			SetAcceleration(m_forwardMove * 1000.0f);
		}
		m_lifeTime -= _GD->m_dt;
		if (m_lifeTime < 0.0f)
		{
			m_isShot = false;
			m_lifeTime = 0;
			SetVisibility(false);
			SetDrag(0);
			SetPhysicsOn(false);
		}
	};
}

void Bullet::Fire()
{
	m_isShot = true;
}


