#include "pch.h"
#include "ImageGO2D.h"
#include "DDSTextureLoader.h"
#include "DrawData2D.h"
#include "GameData.h"
#include "helper.h"
#include <iostream>
#include <sstream>

ImageGO2D::ImageGO2D(const string& _fileName, ID3D11Device* _GameData) :m_pTextureRV(nullptr)
{
	stringstream fullfilename;
	fullfilename << "../Assets/" << _fileName << ".dds";
	HRESULT hr = CreateDDSTextureFromFile(_GameData, Helper::charToWChar(fullfilename.str().c_str()), nullptr, &m_pTextureRV);
	if (hr != S_OK)
	{
		CreateDDSTextureFromFile(_GameData, L"../Assets/white.dds", nullptr, &m_pTextureRV);
		cout << "Missing Texture : " << _fileName << endl;
	}

	//this nasty thing is required to find out the size of this image!
	ID3D11Resource* pResource;
	D3D11_TEXTURE2D_DESC Desc;
	m_pTextureRV->GetResource(&pResource);
	((ID3D11Texture2D*)pResource)->GetDesc(&Desc);

	m_origin = 0.5f * Vector2((float)Desc.Width, (float)Desc.Height);//around which rotation and scaing is done

	if (hr != S_OK)
	{
		SetScale(20.0f);
		m_colour = Colors::HotPink;
	}

}

ImageGO2D::~ImageGO2D()
{
	if (m_pTextureRV)
	{
		m_pTextureRV->Release();
		m_pTextureRV = nullptr;
	}
}

void ImageGO2D::Tick(GameData* _GameData)
{
	//spins!
	//m_rotation += _GameData->m_DeltaTime;
}



void ImageGO2D::Draw(DrawData2D* _DrawData)
{
	if (m_isVisible) {
		//nullptr can be changed to a RECT* to define what area of this image to grab
		//you can also add an extra value at the end to define layer depth
		//right click and "Go to Defintion/Declaration" to see other version of this in DXTK
		_DrawData->m_Sprites->Draw(m_pTextureRV, m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
	}
}