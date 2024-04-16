#include "pch.h"
#include "Map.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <DirectXMath.h>


GameMap::GameMap(const std::string& _mapfileName, int width, int height, const XMFLOAT3& _tileSize) : MAP_WIDTH(width), MAP_HEIGHT(height), map(height, std::vector<int>(width))
    
{
 
    std::string useFileName = "../Models/" + _mapfileName + ".txt";
    std::ifstream file(useFileName);
    if (!file.is_open())
    {
        std::cout << "Error: Unable to open file: " << _mapfileName << std::endl;
        return;
    }

    // Read values from the file and initialize the map
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

    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
      

        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            std::cout << map[y][x];
        }
    }

    GameObject::Tick(nullptr);
}

GameMap::~GameMap()
{
}



