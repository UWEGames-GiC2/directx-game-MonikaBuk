#pragma once

#include "ImageGO2D.h"
#include <vector>

class AnimatedImage : public ImageGO2D
{
private:
    std::vector<ID3D11ShaderResourceView*> m_animationFrames;
    int m_currentFrameIndex;
    float m_frameRate;
    float m_frameTimer;
public:
    AnimatedImage(std::vector<std::string> frameFileNames, ID3D11Device* device, float frameRate);
    ~AnimatedImage();
    virtual void Tick(GameData* _GameData);
    virtual void Draw(DrawData2D* _DrawData);
};