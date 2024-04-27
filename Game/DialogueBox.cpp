#include "pch.h"
#include "DialogueBox.h"
#include <iostream>
#include "GameData.h"

DialogueBox::DialogueBox(const string& _fileNameBox, const vector<std::string>& _texts, ID3D11Device* _GameData)
{
	m_texts = _texts;
	m_border = std::make_shared<ImageGO2D>(_fileNameBox, _GameData);
	m_border->SetScale(4);
	m_border->SetPos(Vector2(400, 500));
	m_textDialog = std::make_shared<TextGO2D>(m_texts[0]);
	m_textDialog->SetPos(Vector2(30, 420));
	m_textDialog->SetColour(Color((float*)&Colors::Red));
	m_textDialog->SetOrigin(Vector2(0, 0.5));
	m_textDialog->SetScale(0.8);
	m_skipText = std::make_shared<TextGO2D>("Press Enter to skip");
	m_skipText->SetPos(Vector2(30, 530));
	m_skipText->SetScale(0.7);
	m_skipText->SetColour(Color((float*)&Colors::Red));
	m_skipText->SetOrigin(Vector2(0, 0.5));
	m_GameObjects2D.push_back(m_border);
	m_GameObjects2D.push_back(m_textDialog);
	m_GameObjects2D.push_back(m_skipText);
	HideElements();
	m_counter = 0;

}

DialogueBox::~DialogueBox()
{
}

void DialogueBox::Tick(GameData* _GameData)
{
	if (_GameData->m_KeyBoardState_tracker.pressed.Enter)
	{
		std::cout << m_counter<< " " << active << "Enter pressed \n";
		if (active)
		{
			std::cout << "Dialogue is active\n";
			NextText(_GameData);
		}
	}
}


void DialogueBox::NextText(GameData* _GameData)
{
	std::cout << "NextText called" << m_counter;
	{
		m_counter++;
		if (m_counter < m_texts.size())
		{
			m_textDialog->SetText(m_texts[m_counter]);
			std::cout << "Setting text: " << m_texts[m_counter] << std::endl;
		}
		else
		{
			HideDialouge(_GameData);
			_GameData->isPaused = false;
		}
	}
}


void DialogueBox::HideDialouge(GameData* _GameData)
{
	std::cout << "hide dialog called. " << active  << std::endl;
	HideElements();
	active = false;
}

void DialogueBox::ShowDialouge(GameData* _GameData)
{
	if (!active)
	{
		m_counter = 0;
		active = true;
		std::cout << "Dialogue is now active." << std::endl;
		for (size_t j = 0; j < m_GameObjects2D.size(); j++)
		{
			m_GameObjects2D[j]->SetVisibility(true);
		}
		_GameData->isPaused = true;
	}
}


void DialogueBox::HideElements()
{
	for (size_t j = 0; j < m_GameObjects2D.size(); j++)
	{
		m_GameObjects2D[j]->SetVisibility(false);
	}
	m_counter = 0;
}
