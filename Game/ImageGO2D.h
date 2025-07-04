#ifndef _IMAGE_GO_2D_H_
#define _IMAGE_GO_2D_H_
#include "GameObject2D.h"

//=================================================================
//A Game Object 2D for displaying images
//image needs to have been converted to a dds by the Assets project
//add orginal file to this project and set it to "Image Content Pipeline"
// in a similar fashion as getting the models
//=================================================================

class ImageGO2D :public GameObject2D
{
public:
	ImageGO2D(const string& _fileName, ID3D11Device* _GameData);
	virtual ~ImageGO2D();


	virtual void Tick(GameData* _GameData)override;
	virtual void Draw(DrawData2D* _DrawData)override;
protected:

	ID3D11ShaderResourceView* m_pTextureRV;
};

#endif