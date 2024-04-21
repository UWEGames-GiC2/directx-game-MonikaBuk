#pragma once

#include "ImageGO2D.h"
#include "Player.h"

class HealthBar: public GameObject2D
{ 
public:
	HealthBar(const string& _fileNameHB, const string& _fileNameB, ID3D11Device* _GameData, std::shared_ptr<Player> _owner);
	virtual~HealthBar();
	
	void UpdateHealthBar();
	std::shared_ptr<ImageGO2D> bar;
	std::shared_ptr<ImageGO2D> border;
	std::vector<std::shared_ptr<ImageGO2D>> images;

	virtual void Tick(GameData* _GameData)override;

protected:
	std::shared_ptr<Player> m_owner;

};

