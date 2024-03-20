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

void TestSound::Tick(GameData * _GD)
{
	if (_GD->m_MS_tracker.leftButton == _GD->m_MS_tracker.PRESSED)
	{
		Play();
	}
	
}
