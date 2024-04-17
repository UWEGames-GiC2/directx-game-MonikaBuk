#pragma once
#include <vector>
#include "gameobject.h"
#include "Model.h"
#include <string>
#include <DirectXMath.h> 
#include "Map.h"

class Pathfinding
{
public:

    float octile_distance(int start_x, int start_y, int goal_x, int goal_y);
    node goal;
    node start;
    std::vector<node> path(node start, node end);
   

private:

    std::vector<node> m_path;
};

