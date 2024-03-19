#include "pch.h"
#include "Bullet.h"
#include "GameData.h"

Bullet::Bullet(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, FPSCamera* camera) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	//any special set up for Player goes here
	m_fudge = Matrix::CreateRotationY(XM_PI);
	m_camera = camera;
	m_pos.y = 10.0f;

	SetDrag(0.2);
	SetPhysicsOn(true);
	SetVisibility(false);

}

void Bullet::Tick(GameData* _GD)
{
	if (m_isShot)
	{
		if (!IsVisible())
		{
			m_lifeTime = 10.0f;
			SetVisibility(true);
			SetPos(m_camera->GetPos());

		}
		m_lifeTime -= _GD->m_dt;
		if (m_lifeTime < 0.0f)
		{
			m_isShot = false;
			m_lifeTime = 0;
			SetVisibility(false);

		}
	};
}


