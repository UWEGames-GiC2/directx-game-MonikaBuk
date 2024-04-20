#include "pch.h"
#include "Bullet.h"
#include "GameData.h"
#include <iostream>
#include <string>
#include <sstream>

Bullet::Bullet(const std::string& _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, FPSCamera& camera) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_fudge = Matrix::CreateRotationY(XM_PI);
	m_camera = &camera;
	m_pos.y = 200.0f;

	SetDrag(0);
	SetPhysicsOn(false);
	SetVisibility(false);
	static int asd = 0;
	std::cout << asd;
	

}

Bullet::~Bullet()
{
}

void Bullet::Tick(GameData* _GameData)
{
	if (m_isShot)
	{
		if (firedNow)
		{
			Vector3 directon;
				
			float yawn = m_camera->GetNewYawn();
			float pitch = m_camera->GetNewPitch();
			SetYaw(yawn);
			SetPitch(pitch);
			if (_GameData->m_GameState == GameState::GS_PLAY_FPS_CAM)
			{
				directon = m_camera->GetCenterOfScreen(_GameData);
			}
			else 
			{
				directon = Vector3::Forward;  
				Matrix  rot = Matrix::CreateRotationY(yawn);
				directon = Vector3::Transform(directon, rot);
			}
		
			SetDrag(0.01f);
			m_lifeTime = 10.0f;
			
			SetPhysicsOn(true);
			auto pos = m_camera->GetPos();
			m_pos = (pos);
			SetAcceleration(directon * 30000.0f);
		}
			firedNow = false;
		}

		m_lifeTime -= _GameData->m_DeltaTime;
		if (m_lifeTime <= 9.95f && !IsVisible())
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
		
	
	CMOGO::Tick(_GameData);
}

void Bullet::Fire()
{
	firedNow = true;
	m_isShot = true;
}


