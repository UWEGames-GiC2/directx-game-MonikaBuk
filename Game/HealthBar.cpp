#include "pch.h"
#include "HealthBar.h"

HealthBar::HealthBar(const string& _fileNameHB, const string& _fileNameB, ID3D11Device* _GameData, std::shared_ptr<Player> _owner)
{
	bar = std::make_shared<ImageGO2D>(_fileNameHB, _GameData);
	border = std::make_shared<ImageGO2D>(_fileNameB, _GameData);
	bar->SetOrigin(Vector2(0, 0));
	border->SetOrigin(Vector2(0, 0));
	border->SetScale(Vector2(1.05f, 1.2f));
	bar->SetPos(Vector2(560, 13));
	border->SetPos(Vector2(555, 10));
	images.push_back(border);
	images.push_back(bar);

	m_owner = _owner;
}


HealthBar::~HealthBar()
{
}

void HealthBar::UpdateHealthBar()
{
	float newSize = m_owner->GetHealth() / 100.0f;
	bar->SetScale(Vector2(newSize,1));
	m_owner->SetHealthChanged(false);
}

void HealthBar::Tick(GameData* _GameData)
{
	if (m_owner->IsHealthChanged())
	{
		UpdateHealthBar();
	}
}
