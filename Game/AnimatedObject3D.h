#pragma once

#include "gameobject.h"
#include "Model.h"
#include <string>


class AnimatedObject3D: public GameObject
{
public:
	AnimatedObject3D(std::string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);
	virtual ~AnimatedObject3D();

	virtual void Tick(GameData* _GameData) override;
	virtual void Draw(DrawData* _DrawData) override;

	virtual bool Intersects(const AnimatedObject3D& other) const;

	BoundingOrientedBox& getCollider()		noexcept { return m_collider; }
	const BoundingOrientedBox& getCollider() const noexcept { return m_collider; }

protected:
	std::unique_ptr<Model>  m_model;
	BoundingOrientedBox m_collider;
	DirectX::SimpleMath::Matrix m_world;

	//needs a slightly different raster state that the VBGOs so create one and let them all use it
	static ID3D11RasterizerState* s_pRasterState;
	static int m_count;
	DirectX::ModelBone::TransformArray m_drawBones;
	DirectX::ModelBone::TransformArray m_animBones;
	float timer = 0.0f;


};


