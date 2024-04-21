#include "pch.h"
#include "AnimatedImage.h"
#include "DDSTextureLoader.h"
#include "DrawData2D.h"
#include "GameData.h"
#include "helper.h"
#include <iostream>
#include <sstream>

AnimatedImage::AnimatedImage(const vector<std::string>& frameFileNames, ID3D11Device* device, float frameRate, bool shoudLoop)
    : ImageGO2D(frameFileNames[0], device),
    m_currentFrameIndex(0),
    m_frameRate(frameRate),
    m_frameTimer(0.0f)
{
        isLooping = shoudLoop;
    for (const std::string& fileName : frameFileNames) {
        stringstream fullfilename;
        fullfilename << "../Assets/" << fileName << ".dds";
        ID3D11ShaderResourceView* srv;
        HRESULT hr = CreateDDSTextureFromFile(device, Helper::charToWChar(fullfilename.str().c_str()), nullptr, &srv);
        if (hr != S_OK) {
            // Handle error loading texture
            std::cerr << "Failed to load texture: " << fileName << std::endl;
            continue;
        }

        else {
            m_animationFrames.push_back(std::shared_ptr<ID3D11ShaderResourceView>(srv, [](ID3D11ShaderResourceView* p) { p->Release(); }));
        }
    }
}
void AnimatedImage::Tick(GameData* _GameData)
{
    if (isLooping)
    {
        m_frameTimer += _GameData->m_DeltaTime;
        if (m_frameTimer >= 1.0f / m_frameRate) {
            m_currentFrameIndex = (m_currentFrameIndex + 1) % m_animationFrames.size();
            m_frameTimer = 0.0f;
        }
    }
    else
    {
        if (_GameData->m_Mouse_tracker.leftButton == _GameData->m_Mouse_tracker.PRESSED)
        {
            play = true;
           
        }
    }
    if (play)
    {
        if (m_currentFrameIndex < m_animationFrames.size() -1 ) // Corrected condition
        {
            m_frameTimer += _GameData->m_DeltaTime;
            if (m_frameTimer >= 1.0f / m_frameRate) {
                m_currentFrameIndex += 1;
                m_frameTimer = 0.0f;
            }
        }
        else
        {
            play = false;
            m_currentFrameIndex = 0;
        }
    }
}

void AnimatedImage::Draw(DrawData2D* _DrawData)
{
    ID3D11ShaderResourceView* currentFrame = m_animationFrames[m_currentFrameIndex].get();
    _DrawData->m_Sprites->Draw(currentFrame, m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
}

AnimatedImage::~AnimatedImage()
{
}
