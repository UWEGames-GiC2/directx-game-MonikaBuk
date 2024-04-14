#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "terrain.h"

class Enemy : public Terrain
{

public:
	Enemy(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale);
	~Enemy();
};


#endif