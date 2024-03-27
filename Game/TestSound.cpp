#include "pch.h"
#include "TestSound.h"
#include "GameData.h"
#include <iostream>


TestSound::TestSound(AudioEngine* _audEngine, string _filename) :Sound(_audEngine, _filename)
{
}


TestSound::~TestSound()
{
}

void TestSound::Tick(GameData * _GameData)
{
	if (_GameData->m_Mouse_tracker.leftButton == _GameData->m_Mouse_tracker.PRESSED)
	{
		Play();
	}
	
}
