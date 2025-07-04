#pragma once

#ifndef _BULLET_H_
#define _BULLET_H_
#include "CMOGO.h"
#include "FPSCamera.h"

class Bullet : public CMOGO
{

public:
	Bullet(const std::string& _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, FPSCamera& camera);
	~Bullet();

	virtual void Tick(GameData* _GameData) override;
	bool IsShot() { return m_isShot; };
	void Fire();
	void Stop();


protected:
	bool m_isShot = false;
	float m_lifeTime = 0.0f;
	FPSCamera* m_camera;
	Vector3 m_forwardMove;
	bool firedNow = false;


};
#endif

