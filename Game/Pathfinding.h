#pragma once
#include <vector>
#include "gameobject.h"
#include "Model.h"
#include <string>
#include <DirectXMath.h> 
#include "GameMap.h"
#include "GameData.h"

class Pathfinding
{
public:

    Pathfinding();
    ~Pathfinding();


    float octile_distance(int start_x, int start_y, int goal_x, int goal_y);
    node goal;
    node start;
    std::vector<Vector2> GetPath(Vector2 start, Vector2 end, GameData* _GameData);
   

private:
    GameMap m_map;
    std::vector<Vector2> m_path;
};

