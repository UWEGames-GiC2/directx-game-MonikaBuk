#include "pch.h"
#include "Weapon.h"
#include <dinput.h>
#include "GameData.h"



Weapon::Weapon(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, GameObject& _owner) :CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_pos = _pos;
	m_pitch = _pitch;
	m_roll = _roll;
	m_yaw = _yaw;
	m_scale = _scale;
	m_owner = &_owner;
    derekszog = XMConvertToRadians(90);
}

Weapon::~Weapon()
{
	//Nothing additional here but add this just in case
}

void Weapon::Tick(GameData* _GameData)
{
	float rotSpeed = 2.0f * _GameData->m_DeltaTime;
	auto mouse = _GameData->m_Mouse;
	if (mouse.positionMode == Mouse::MODE_RELATIVE)
	{
		Vector3 delta = Vector3(mouse.x, mouse.y, 0.f)
			* rotSpeed;
		m_pitch -= delta.y;
		auto m_maxrot = XMConvertToRadians(60);
		if (m_pitch > m_maxrot) m_pitch = m_maxrot;
		if (m_pitch < -m_maxrot) m_pitch = -m_maxrot;
	}

    Vector3 ownerPos = m_owner->GetPos();
	
    float ownerYaw = m_owner->GetYaw();
    float ownerPitch = m_owner->GetPitch();

    m_pos = ownerPos;

  

    // Call the Tick function of the base class
    CMOGO::Tick(_GameData);
}




