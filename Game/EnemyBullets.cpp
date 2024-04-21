#include "pch.h"
#include "EnemyBullets.h"
#include "GameData.h"

EnemyBullets::EnemyBullets(const string& _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, std::shared_ptr<CMOGO> _owner): CMOGO(_fileName, _pd3dDevice, _EF)
{
m_fudge = Matrix::CreateRotationY(XM_PI);
m_owner = _owner;
m_pos.y = 200.0f;

SetDrag(0);
SetPhysicsOn(false);
SetVisibility(false);

}

EnemyBullets::~EnemyBullets()
{
}

void EnemyBullets::Tick(GameData* _GameData)
{
	if (m_owner != nullptr)
	{
		if (m_isShot)
		{
			if (firedNow)
			{
				Vector3 directon;

				float yawn = m_owner->GetYaw();
				SetYaw(yawn);

			
				Matrix  rot = Matrix::CreateRotationY(yawn);
				directon =  - Vector3::Forward;
				directon = Vector3::Transform(directon, rot);
				SetVisibility(true);


				SetDrag(0.01f);
				m_lifeTime = 9.0f;

				SetPhysicsOn(true);
				auto pos = m_owner->GetPos();
				m_pos = (pos);
				SetAcceleration(directon * 30000.0f);
			}
			firedNow = false;
		}
		m_lifeTime -= _GameData->m_DeltaTime;
		if (m_lifeTime < 0.0f || !m_isShot)
		{
			m_isShot = false;
			m_lifeTime = 0;
			SetVisibility(false);
			SetDrag(0);
			SetPhysicsOn(false);
		}


		CMOGO::Tick(_GameData);
	}
}

void EnemyBullets::Fire()
{
	firedNow = true;
	m_isShot = true;
}

void EnemyBullets::Stop()
{
	m_isShot = false;
}