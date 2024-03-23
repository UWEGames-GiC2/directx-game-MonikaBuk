#include "pch.h"
#include "Player.h"
#include <dinput.h>
#include "GameData.h"
#include <iostream>

Player::Player(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	//any special set up for Player goes here
	m_fudge = Matrix::CreateRotationY(XM_PI);

	m_pos.y = 10.0f;

	SetDrag(2);
	SetPhysicsOn(true);
	SetVisibility(false);
}

Player::~Player()
{
	//tidy up anything I've created
}


void Player::Tick(GameData* _GD)
{
	//TURN AND FORWARD CONTROL HERE
	Vector3 forwardMove = 60.0f * Vector3::Forward;
	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	forwardMove = Vector3::Transform(forwardMove, rotMove);
	if (_GD->m_KBS.W)
	{
		m_acc += forwardMove;
	}
	if (_GD->m_KBS.S)
	{
		m_acc -= forwardMove;
	}
	if (_GD->gameStateChanged)
	{
		if (_GD->m_GS == GS_PLAY_FPS_CAM)
		{
		
			SetVisibility(false);
		}
		else
		{
			SetVisibility(true);
		}
		_GD->gameStateChanged = false;
	}



	//change orinetation of player
	Vector3 rightMove = 60.0f * Vector3::Right;
	rightMove = Vector3::Transform(rightMove, rotMove);

	float rotSpeed =  _GD->m_dt;
	float speed = 10.0f;
	if (_GD->m_KBS.A)
	{
		m_acc -= rightMove;
	}
	if (_GD->m_KBS.D)
	{
		m_acc += rightMove;
	}
	auto mouse = _GD->m_MS;


	if (_GD->m_MS_tracker.leftButton == _GD->m_MS_tracker.PRESSED)
	{
		for (size_t i = 0; i < bullets.size(); i++)
		{
			if (!bullets[i]->IsShot())
			{
				bullets[i]->Fire();
				std::cout << bullets[i]->GetPos().x;
				break;
			}
		}
	}

	// Check if mouse input is relative for rotation
	if (mouse.positionMode == Mouse::MODE_RELATIVE)
	{
		Vector3 delta = Vector3(mouse.x, mouse.y, 0.f)
			* rotSpeed;
		//m_pitch -= delta.y;
		auto m_maxrot = XMConvertToRadians(60);
		if (m_pitch > m_maxrot) m_pitch = m_maxrot;
		if (m_pitch < -m_maxrot) m_pitch = -m_maxrot;
		m_yaw -= delta.x;
	}



	//limit motion of the player
	float length = m_pos.Length();
	float maxLength = 500.0f;
	if (length > maxLength)
	{
		m_pos.Normalize();
		m_pos *= maxLength;
		m_vel *= -0.9; //VERY simple bounce back
	}

	//apply my base behaviour
	CMOGO::Tick(_GD);
}