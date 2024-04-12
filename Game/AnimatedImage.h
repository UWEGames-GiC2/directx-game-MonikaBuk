
#ifndef _ANIMATED_IMAGE_H_
#define _ANIMATED_IMAGE_H_

#include "ImageGO2D.h"
#include <vector>

class AnimatedImage : public ImageGO2D
{
private:
    std::vector<std::shared_ptr<ID3D11ShaderResourceView>> m_animationFrames;
    int m_currentFrameIndex;
    float m_frameRate;
    float m_frameTimer;
public:
    AnimatedImage(std::vector<std::string> frameFileNames, ID3D11Device* device, float frameRate, bool shouldLoop);
    ~AnimatedImage();
    virtual void Tick(GameData* _GameData)override;
    virtual void Draw(DrawData2D* _DrawData)override;
    bool isLooping = false;
    bool play = false;
};

#endif