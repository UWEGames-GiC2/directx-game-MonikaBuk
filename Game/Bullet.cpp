#include "pch.h"
#include "Bullet.h"
#include "GameData.h"
#include <iostream>
#include <string>
#include <sstream>

Bullet::Bullet(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, FPSCamera& camera) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_fudge = Matrix::CreateRotationY(XM_PI);
	m_camera = &camera;
	m_pos.y = 200.0f;

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
		if (firedNow)
		{
			float yawn = m_camera->GetNewYawn();
			float pitch = m_camera->GetNewPitch();
			SetYaw(yawn);
			SetPitch(pitch);
			SetDrag(0.01f);
			m_lifeTime = 10.0f;
			Vector3 directon = m_camera->GetCenterOfScreen(_GD);
			SetPhysicsOn(true);
			m_forwardMove = m_camera->GetForwardVector();
			auto pos = m_camera->GetPos();
			m_pos = (pos);
			SetAcceleration(directon * 5000.0f);
			firedNow = false;
		}

		m_lifeTime -= _GD->m_dt;
		if (m_lifeTime <= 9.7f && !IsVisible())
		{
			SetVisibility(true);
		}
		if (m_lifeTime < 0.0f)
		{
			m_isShot = false;
			m_lifeTime = 0;
			SetVisibility(false);
			SetDrag(0);
			SetPhysicsOn(false);
		}
		
	}
	CMOGO::Tick(_GD);
}

void Bullet::Fire()
{
	firedNow = true;
	m_isShot = true;
}


