#include "pch.h"
#include "DialogueBox.h"

DialogueBox::DialogueBox(const string& _fileNameBox, const string& _text, ID3D11Device* _GameData)
{
	m_border = std::make_shared<ImageGO2D>(_fileNameBox, _GameData);
	m_border->SetScale(4);
	m_border->SetPos(Vector2(400, 500));
	m_textDialog = std::make_shared<TextGO2D>(_text);
	m_textDialog->SetPos(Vector2(20, 420));
	m_textDialog->SetColour(Color((float*)&Colors::Red));
	m_textDialog->SetScale(0.7);
	m_textDialog->SetOrigin(Vector2(0, 0.5));
	m_GameObjects2D.push_back(m_border);
	m_GameObjects2D.push_back(m_textDialog);
}

DialogueBox::~DialogueBox()
{
}

void DialogueBox::Tick(GameData* _GameData)
{
}

void DialogueBox::HideDialouge()
{
	for (size_t j = 0; j < m_GameObjects2D.size(); j++)
	{
		m_GameObjects2D[j]->SetVisibility(false);
	}
}
