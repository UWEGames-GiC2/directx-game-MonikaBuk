#include "pch.h"
#include "Player.h"
#include <dinput.h>
#include "GameData.h"
#include <iostream>
#include "Model.h"

Player::Player(const std::string& _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	//any special set up for Player goes here
	m_fudge = Matrix::CreateRotationY(XM_PI);
	SetDrag(7);
	SetPhysicsOn(true);
	SetVisibility(false);
	m_isGrounded = false;
}

Player::~Player()
{
	//tidy up anything I've created
}


void Player::Tick(GameData* _GameData)
{
	//TURN AND FORWARD CONTROL HERE
	float dt = _GameData->m_DeltaTime;
	float speedModifier = dt * 5;
	Vector3 forwardMove = m_moveSpeed * Vector3::Forward * speedModifier;
	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	forwardMove = Vector3::Transform(forwardMove, rotMove);

	if (_GameData->m_KeyBoardState.Space && m_isGrounded)
	{
		m_vel.y += m_jumpSpeed;
		m_isGrounded = false;
	}
	if (m_vel.y < 0) {
		m_acc.y -= m_gravity * 2000.0f * dt;
	}
	if (_GameData->m_KeyBoardState.W)
	{
		m_acc += forwardMove;
	}
	if (_GameData->m_KeyBoardState.S)
	{
		m_acc -= forwardMove;
	}
	if (_GameData->gameStateChanged)
	{
		if (_GameData->m_GameState == GS_PLAY_FPS_CAM)
		{

			SetVisibility(false);
		}
		else
		{
			SetVisibility(true);
		}
		_GameData->gameStateChanged = false;
	}

	if (!m_isGrounded)
	{
		m_acc += Vector3(0, -m_gravity, 0);
	}
	m_vel += m_acc * _GameData->m_DeltaTime;
	m_pos += m_vel * _GameData->m_DeltaTime;

	//change orinetation of player
	Vector3 rightMove = m_moveSpeed * Vector3::Right * speedModifier;
	rightMove = Vector3::Transform(rightMove, rotMove);


	if (_GameData->m_KeyBoardState.A)
	{
		m_acc -= rightMove;
	}
	if (_GameData->m_KeyBoardState.D)
	{
		m_acc += rightMove;
	}

	if (_GameData->m_Mouse_tracker.leftButton == _GameData->m_Mouse_tracker.PRESSED)
	{
		for (size_t i = 0; i < m_bullets.size(); i++)
		{
			if (!m_bullets[i]->IsShot())
			{
				m_bullets[i]->Fire();
				break;
			}
		}
	}

	float rotSpeed = _GameData->m_DeltaTime;
	auto mouse = _GameData->m_Mouse;
	if (mouse.positionMode == Mouse::MODE_RELATIVE)
	{
		m_yaw -= (mouse.x * rotSpeed);
	}

	//apply my base behaviour
	CMOGO::Tick(_GameData);
}

void Player::TakeDamage(int damage)
{
	m_health -= damage;
	m_healthChanged = true;
}

