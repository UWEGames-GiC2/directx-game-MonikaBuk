#ifndef _MAP_H_
#define _MAP_H_

#include <vector>
#include "gameobject.h"
#include "Model.h"
#include <string>
#include <DirectXMath.h> 
#include <unordered_set>
#include <unordered_map>
#include <queue>

struct Compare {
    bool operator()(const std::pair<float, Vector2>& a, const std::pair<float, Vector2>& b) {
        return a.first > b.first;
    }

};
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

    XMFLOAT3 basePosition;

    int MAP_WIDTH;
    int MAP_HEIGHT;

    std::vector<std::vector<int>> map;
    virtual void Tick(GameData* _GameData) override { _GameData; };
    virtual void Draw(DrawData* _DrawData) override { _DrawData; };
    bool isWalkable(int x, int y) const;
    std::vector<Vector2> getNeighbors(int x, int y) const;
    std::vector<Vector2> FindPath(Vector2 start, Vector2 goal) const;
    std::vector<Vector2> asd;
    Vector2 tileSize;
    Vector2 TileToWorld(Vector2 nextTile);
};
#endif