#ifndef _MAP_H_
#define _MAP_H_

#include <vector>
#include "gameobject.h"
#include "Model.h"
#include <string>
#include <DirectXMath.h> 


struct node
{
    int x;
    int y;
    int value;
};

class GameMap : public GameObject
{
public:
    GameMap(const std::string& _mapfileName, int width, int height, const XMFLOAT3& _tileSize);
    virtual~GameMap();

    int MAP_WIDTH;
    int MAP_HEIGHT;

    std::vector<std::vector<int>> map;
    virtual void Tick(GameData* _GameData) override { _GameData; };
    virtual void Draw(DrawData* _DrawData) override { _DrawData; };
};
#endif