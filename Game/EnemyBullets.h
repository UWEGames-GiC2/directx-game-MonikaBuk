#pragma once

#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_
#include "CMOGO.h"
class EnemyBullets : public CMOGO
{

public:
	EnemyBullets(const string& _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, std::shared_ptr<CMOGO> _owner);
	~EnemyBullets();

	virtual void Tick(GameData* _GameData) override;
	bool IsShot() { return m_isShot; };
	void Fire();
	void Stop();

protected:
	bool m_isShot = false;
	float m_lifeTime = 0.0f;
	Vector3 m_forwardMove;
	bool firedNow = false;
	std::shared_ptr<CMOGO> m_owner;
};
#endif

