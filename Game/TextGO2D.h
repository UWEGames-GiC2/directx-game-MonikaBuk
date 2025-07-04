#ifndef _TEXT_GO_2D_H_
#define _TEXT_GO_2D_H_

//=================================================================
//a Game Object 2D for displaying text
//=================================================================

#include "GameObject2D.h"

class TextGO2D :public GameObject2D
{
public:
	TextGO2D(const string& _text);

	virtual void Tick(GameData* _GameData);
	virtual void Draw(DrawData2D* _DrawData);

	void SetText(string text) { m_text = text; }
protected:
	string m_text;
};

#endif