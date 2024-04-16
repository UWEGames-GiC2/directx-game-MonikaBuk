#include "pch.h"
#include "Enemy.h"
#include <iostream>



Enemy::Enemy(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, std::shared_ptr<Player> _target) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_pos = _pos;
	m_pitch = _pitch;
	m_roll = _roll;
	m_yaw = _yaw;
	m_scale = _scale;
	target = _target;
	SetVisibility(true);
	
}

Enemy::~Enemy()
{
}

void Enemy::Tick(GameData* _GameData)
{
    MoveTo(target->GetPos());
    // Call the base class Tick function
    CMOGO::Tick(_GameData);
}


void Enemy::MoveTo(Vector3 targetPos)
{
   Vector3 direction = targetPos - m_pos;
    direction.Normalize();

    m_yaw = atan2f(direction.x, direction.z);
    m_pitch = 0.0f;
    m_roll = 0.0f; 
    float speed = 0.1f; 
    float minDistance = 20.0f; 
    float distanceToTarget = (targetPos - m_pos).Length();
    if (distanceToTarget > minDistance)
    {
        m_pos += direction * speed;
    }
}

