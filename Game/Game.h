//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include <list>
#include "CommonStates.h"
#include "../DirectXTK/Inc/Effects.h" //this clashes with a SDK file so must explitily state it
#include "Keyboard.h"
#include "Mouse.h"
#include "Audio.h"
#include "CMOGO.h"
#include "Bullet.h"
#include "Player.h" 
#include "Terrain.h"
#include "AnimatedObject3D.h"
#include "Enemy.h"
#include "GameMap.h"
#include "EnemyBullets.h"
#include "HealthBar.h"
#include "TextGO2D.h"
#include "Tile.h"
#include "DialogueBox.h"

using std::list;

// Forward declarations
struct GameData;
struct DrawData;
struct DrawData2D;
class GameObject;
class GameObject2D;
class Camera;
class TPSCamera;
class FPSCamera;
class Light;
class Sound;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game() noexcept;
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and 
    void ClearAllVectors();
    void Initialize(HWND _window, int _width, int _height);
   
    void LoadObjects(int _width, int _height);
    void LoadDialoges();
    void LoadUIElements(float _width, float _height);
    void LoadMap();

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int _width, int _height);

    // Properties
    void GetDefaultSize( int& _width, int& _height ) const noexcept;

private:

    void Update(DX::StepTimer const& _timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

    //Scarle Added stuff
    std::shared_ptr<GameData> m_GameData = NULL;			//Data to be shared to all Game Objects as they are ticked
    std::shared_ptr<DrawData> m_DrawData = NULL;			//Data to be shared to all 3D Game Objects as they are drawn
    std::shared_ptr<DrawData2D> m_DrawData2D = NULL;	    //Data to be passed by game to all 2D Game Objects via Draw 

    //Basic 3D renderers
    std::shared_ptr<FPSCamera> m_FPScam = NULL; //principle camera
    std::shared_ptr <TPSCamera> m_TPScam = NULL;//TPS cam
    std::shared_ptr<Light> m_light = NULL; //base light


    //required for the CMO model rendering system
    DirectX::CommonStates* m_states = NULL;
   DirectX::IEffectFactory* m_fxFactory = NULL;

    //basic keyboard and mouse input system
    void ReadInput(); //Get current Mouse and Keyboard states
    std::unique_ptr<DirectX::Keyboard> m_keyboard;
    std::unique_ptr<DirectX::Mouse> m_mouse;

    std::vector < std::shared_ptr<GameObject>> m_GameObjects; //data structure to hold pointers to the 3D Game Objects
    std::vector < std::shared_ptr<GameObject2D>> m_GameObjects2D; //data structure to hold pointers to the 2D Game Objects 
    std::vector < std::shared_ptr<GameObject2D>> m_FPS_GameObjects2D;

    //list<CMOGO*> m_CMOGameObjects; //data structure to hold pointers to all 3D CMO Game Objects
    //list<CMOGO*> m_PhysicsObjects

    std::vector <std::shared_ptr<Terrain>> m_ColliderObjects;
    std::vector < std::shared_ptr<CMOGO>> m_PhysicsObjects;
    std::vector < std::shared_ptr<Bullet>> p_bullets;
    std::vector < std::shared_ptr<EnemyBullets>> p_Ebullets;
    std::vector < std::shared_ptr<Enemy>> m_Eniemies;
    std::vector <std::shared_ptr<Tile>> m_Map;

    std::shared_ptr <Player> pPlayer = NULL;
    std::shared_ptr<GameMap> map = NULL;
    Vector3 playerSpawn;
    vector<Vector3> enemySpawnPoses;
    Vector3 hostageSpawn;

   
    std::shared_ptr<TextGO2D> timerText = NULL;
    float m_timeLimit = 60.00f;
    std::shared_ptr<TextGO2D> textScore = NULL;
    int m_score = 0;
    float quadrant = XMConvertToRadians(90);

    std::shared_ptr<DialogueBox> m_missionDialoge;
    std::shared_ptr<DialogueBox> m_missingEnemyDialoge;
    std::shared_ptr<DialogueBox> m_endDialoge;

    void CheckCollision();
    void CheckCollisionGroundWithPlayer();

    void CheckCollisionEnemyBullet();

    void RemoveEnemyFromAllContainers(std::shared_ptr<GameObject> gameObject);


    void CheckCollisionMapWithPlayer(int _width, int _height);

    void CheckCollisionMapWithEnemies();

    void CheckCollisionBulletWithPlayer();
                                         
    //sound stuff
	//This uses a simple system, but a better pipeline can be used using Wave Banks
	//See here: https://github.com/Microsoft/DirectXTK/wiki/Creating-and-playing-sounds Using wave banks Section
    std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
    list<Sound*>m_Sounds;
};
