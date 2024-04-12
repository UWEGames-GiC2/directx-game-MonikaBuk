#include "pch.h"
#include "AnimatedImage.h"
#include "DDSTextureLoader.h"
#include "DrawData2D.h"
#include "GameData.h"
#include "helper.h"
#include <iostream>

AnimatedImage::AnimatedImage(std::vector<std::string> frameFileNames, ID3D11Device* device, float frameRate)
    : ImageGO2D(frameFileNames[0], device),
    m_currentFrameIndex(0),
    m_frameRate(frameRate),
    m_frameTimer(0.0f)
{
    for (const std::string& fileName : frameFileNames) {
        string fullfilename = "../Assets/" + fileName + ".dds";
        ID3D11ShaderResourceView* srv;
        HRESULT hr = CreateDDSTextureFromFile(device, Helper::charToWChar(fullfilename.c_str()), nullptr, &srv);
        if (hr != S_OK) {
            // Handle error loading texture
            std::cerr << "Failed to load texture: " << fileName << std::endl;
            continue;
        }
        m_animationFrames.push_back(srv);
    }
}
void AnimatedImage::Tick(GameData* _GameData)
{
    m_frameTimer += _GameData->m_DeltaTime;
    if (m_frameTimer >= 1.0f / m_frameRate) {
        m_currentFrameIndex = (m_currentFrameIndex + 1) % m_animationFrames.size();
        m_frameTimer = 0.0f;
    }
}

void AnimatedImage::Draw(DrawData2D* _DrawData)
{
    // Render the current frame of the animation
    ID3D11ShaderResourceView* currentFrame = m_animationFrames[m_currentFrameIndex];
    // Draw the current frame using the drawData (e.g., Direct3D sprite batch)
    // Example:
    _DrawData->m_Sprites->Draw(currentFrame, m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
}

AnimatedImage::~AnimatedImage()
{
}
