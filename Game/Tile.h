#ifndef _TILEMAP_H_
#define _TILEMAP_H_

#include "terrain.h"
#include "terrain.h"
#include "GameData.h"
#include <iostream>

class Tile : public Terrain
{
public:
	Tile(const std::string& _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, Vector2 _assignedTilePos);
	~Tile();
	Vector2 GetTile() { return m_assignedTilePos;};
protected:
	Vector2 m_assignedTilePos;
};

#endif