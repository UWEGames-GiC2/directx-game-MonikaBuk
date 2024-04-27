#pragma once
#include "ImageGO2D.h"
#include "TextGO2D.h"
#include <vector>

class DialogueBox : public GameObject2D
{
public:
	DialogueBox(const string& _fileNameBox, const vector<std::string>& _texts, ID3D11Device* _GameData);
	~DialogueBox();

	virtual void Tick(GameData* _GameData)override;

	std::vector < std::shared_ptr<GameObject2D>>Get2DObjects() { return m_GameObjects2D; }

	void HideDialouge(GameData* _GameData);
	void ShowDialouge(GameData* _GameData);
	void HideElements();
	void NextText(GameData* _GameData);

protected:
	std::shared_ptr<ImageGO2D> m_border;
	std::shared_ptr<TextGO2D> m_textDialog;
	std::shared_ptr<TextGO2D> m_skipText;
	std::vector<std::string> m_texts;
	std::vector < std::shared_ptr<GameObject2D>> m_GameObjects2D;
	int m_counter = 0;
	bool active = false;


};

