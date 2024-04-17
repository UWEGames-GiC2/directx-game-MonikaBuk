#include "pch.h"
#include "Player.h"
#include <dinput.h>
#include "GameData.h"
#include <iostream>

Player::Player(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	//any special set up for Player goes here
	m_fudge = Matrix::CreateRotationY(XM_PI);
	SetDrag(5);
	SetPhysicsOn(true);
	SetVisibility(false);
	isGrounded = true;
}

Player::~Player()
{
	//tidy up anything I've created
}


void Player::Tick(GameData* _GameData)
{
	//TURN AND FORWARD CONTROL HERE
	Vector3 forwardMove = moveSpeed * Vector3::Forward;
	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	forwardMove = Vector3::Transform(forwardMove, rotMove);
	m_acc.y = (m_acc.y - gravity) * _GameData->m_DeltaTime;
	if (_GameData->m_KeyBoardState.Space && isGrounded)
	{
		m_vel.y += jumpSpeed;
		isGrounded = false;
	}
	if (m_vel.y < 0) {
		m_acc.y -= gravity * 200.0f * _GameData->m_DeltaTime;
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
	
	m_acc += Vector3(0, -gravity, 0);
	m_vel += m_acc * _GameData->m_DeltaTime;
	m_pos += m_vel * _GameData->m_DeltaTime;

	//change orinetation of player
	Vector3 rightMove = moveSpeed * Vector3::Right;
	rightMove = Vector3::Transform(rightMove, rotMove);

	float rotSpeed =  _GameData->m_DeltaTime /2 ;
	if (_GameData->m_KeyBoardState.A)
	{
		m_acc -= rightMove;
	}
	if (_GameData->m_KeyBoardState.D)
	{
		m_acc += rightMove;
	}
	auto mouse = _GameData->m_Mouse;
	

	if (_GameData->m_Mouse_tracker.leftButton == _GameData->m_Mouse_tracker.PRESSED)
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
		float delta = mouse.x
			* rotSpeed;
		m_yaw -= delta;
	}


	//apply my base behaviour
	CMOGO::Tick(_GameData);
}

