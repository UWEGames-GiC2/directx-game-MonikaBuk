#ifndef _GAME_DATA_H_
#define _GAME_DATA_H_

//=================================================================
//Data to be passed by game to all Game Objects via Tick
//=================================================================

#include "GameState.h"
#include "Keyboard.h"
#include "Mouse.h"


using namespace DirectX;

struct GameData
{
	float m_DeltaTime;  //time step since last frame
	GameState m_GameState; //global GameState
	bool gameStateChanged = false;
	bool isPaused = true;

	//player input
	Keyboard::State m_KeyBoardState;
	Mouse::State m_Mouse;
	Mouse::ButtonStateTracker m_Mouse_tracker;
	Keyboard::KeyboardStateTracker m_KeyBoardState_tracker;

	//map



};
#endif
