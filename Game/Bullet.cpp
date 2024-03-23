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

		if (!IsVisible())
		{
			/*
		
			float yawn = m_camera->GetNewYawn();
			float pitch = m_camera->GetNewPitch();

			Matrix rotMove = Matrix::CreateFromYawPitchRoll(yawn, pitch, 0);
			m_forwardMove = Vector3::Transform(m_forwardMove, rotMove);
		
		
			

			
			SetYaw(yawn);
			SetPitch(pitch);
		
			*/
			SetDrag(0.01f);
			m_lifeTime = 10.0f;
			Vector3 directon = m_camera->GetCenterOfScreen(_GD);
			SetPhysicsOn(true);
			m_forwardMove = m_camera->GetForwardVector() * 5;
			m_pos = (m_camera->GetPos() + m_forwardMove);
			SetAcceleration(directon * 5000.0f);
			SetVisibility(true);



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
		
	}
	CMOGO::Tick(_GD);
}

void Bullet::Fire()
{
	m_isShot = true;
}


