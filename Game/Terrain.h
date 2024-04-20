#ifndef _TERRAIN_H_
#define _TERRAIN_H_
#include "CMOGO.h"

//=================================================================
//Terrian Game Object Class (i.e. an Object that just sits there)
//=================================================================

struct GameData;

enum TerrainType
{
	FLOOR = 1,
	WALL = 2,
	OTHER = 3
};

class Terrain : public CMOGO
{
public:
	Terrain(const std::string& _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale);
	~Terrain();

	void Tick(GameData* _GameData) override { _GameData; };
	void SetType(TerrainType type) { terrainType = type;}
	TerrainType GetType() const { return terrainType;}
	int xAssignedForMap;
	int yAssignedForMap;
	

protected:
	TerrainType terrainType = OTHER;

};

#endif
