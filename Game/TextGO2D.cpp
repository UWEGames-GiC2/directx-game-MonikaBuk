#include "pch.h"
#include "TextGO2D.h"
#include "DrawData2D.h"
#include "helper.h"

TextGO2D::TextGO2D(const string& _text)
{
	m_text = _text;
}


void TextGO2D::Tick(GameData* _GameData)
{
}


void TextGO2D::Draw(DrawData2D* _DrawData)
{
	//right click and "Go to Defintion/Declaration" to see other version of this in DXTK
	_DrawData->m_Font->DrawString(_DrawData->m_Sprites.get(), Helper::charToWChar(m_text.c_str()), m_pos, m_colour,m_rotation,m_origin,m_scale);
}