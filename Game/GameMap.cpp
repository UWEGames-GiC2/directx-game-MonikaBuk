#include "pch.h"
#include "GameMap.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <DirectXMath.h>


namespace std {
    template<>
    struct hash<Vector2>
    {
        size_t operator()(const Vector2& key) const
        {
            size_t hashX = std::hash<float>()(key.x);
            size_t hashY = std::hash<float>()(key.y);
            return hashX ^ (hashY + 0x9e3779b9 + (hashX << 6) + (hashX >> 2));
        }
    };
}

GameMap::GameMap(const std::string& _mapfileName, int width, int height, const XMFLOAT3& _tileSize) : MAP_WIDTH(width), MAP_HEIGHT(height), map(height, std::vector<int>(width))
    
{
    basePosition = Vector3::Zero;
    std::stringstream useFileName;
    useFileName << "../Models/" << _mapfileName << ".txt";

  
    std::ifstream file(useFileName.str().c_str());
    if (!file.is_open())
    {
        std::cout << "Error: Unable to open file: " << _mapfileName << std::endl;
        return;
    }

    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        std::string line;
      

        if (!std::getline(file, line))
        {
            std::cout << "Error: Insufficient data in file." << std::endl;
            return;
        }
        std::istringstream iss(line);
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            if (!(iss >> map[y][x]))
            {
                std::cout << "Error: Invalid data format in file." << std::endl;
                return;
            }
        } 
    }

    file.close();
    tileSize = Vector2(_tileSize.x, _tileSize.y);

    GameObject::Tick(nullptr);
}

GameMap::~GameMap()
{
}


bool GameMap::isWalkable(int x, int y) const {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return false;  
    }
    return map[y][x] != 1;  
}

std::vector<Vector2> GameMap::getNeighbors(int x, int y) const {
    std::vector<Vector2> neighbors;
    const std::vector<std::pair<int, int>> directions = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

    for (const auto& dir : directions) {
        int nx = x + dir.first;  
        int ny = y + dir.second; 

        if (nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT && isWalkable(nx, ny)) {
            neighbors.emplace_back(nx, ny);
        }
    }

    return neighbors;
}

std::vector<Vector2> GameMap::FindPath(Vector2 start, Vector2 goal) const {
    std::queue<Vector2> frontier;
    std::unordered_map<Vector2, Vector2> came_from;

    if (!isWalkable(start.x, start.y) || !isWalkable(goal.x, goal.y)) {
    
        return {};
    }

    frontier.push(start);
    came_from[start] = start;

    while (!frontier.empty()) {
        Vector2 current = frontier.front();
        frontier.pop();

        if (current == goal) {
            break; 
        }

        for (const auto& neighbor : getNeighbors(current.x, current.y)) {
            if (!came_from.count(neighbor) && isWalkable(neighbor.x, neighbor.y)) {
                frontier.push(neighbor);
                came_from[neighbor] = current;
            }
        }
    }
    std::vector<Vector2> path;
    Vector2 current = goal;
    while (current != start) {
        path.push_back(current);
        current = came_from[current];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return path;
}


Vector2 GameMap::TileToWorld(Vector2 nextTile)
{
    Vector2 tes = (nextTile * tileSize);
    tes.x += basePosition.x;
    tes.y += basePosition.z;
    return  tes;
}
