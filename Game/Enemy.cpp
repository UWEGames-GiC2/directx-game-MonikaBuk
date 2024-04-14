#include "pch.h"
#include "Enemy.h"



Enemy::Enemy(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale) : Terrain( _fileName,  _pd3dDevice,  _EF,  _pos,  _pitch,  _yaw,  _roll,  _scale)
{
	m_pos = _pos;
	m_pitch = _pitch;
	m_roll = _roll;
	m_yaw = _yaw;
	m_scale = _scale;
}

Enemy::~Enemy()
{
}
