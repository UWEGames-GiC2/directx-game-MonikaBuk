#include "pch.h"
#include "Tile.h"

Tile::Tile(const std::string& _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, Vector2 _assignedTilePos) : Terrain( _fileName, _pd3dDevice,  _EF,  _pos,  _pitch,  _yaw,  _roll,  _scale)
{
	m_assignedTilePos = _assignedTilePos;
}

Tile::~Tile()
{
}
