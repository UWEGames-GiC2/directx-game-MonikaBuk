#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "terrain.h"
#include "Player.h"

class Enemy : public CMOGO
{

public:
	Enemy(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, std::shared_ptr<Player> _target);
	~Enemy();

	virtual void Tick(GameData* _GameData) override;

	void MoveTo(Vector3 targetPos);

protected:
	std::shared_ptr<Player> target;
};


#endif