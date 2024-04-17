#include "pch.h"
#include "Pathfinding.h"


float Pathfinding::octile_distance(int start_x, int start_y, int goal_x, int goal_y)
{
    float dx = abs(start_y - goal_y);
    float   dy = abs(start_x - goal_x);
    return sqrt(dx * dx + dy * dy);
}


std::vector<node> Pathfinding::path(node start, node end)
{
    return m_path;
}