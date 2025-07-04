//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <time.h>

#include <iostream>
#include <sstream>

//Scarle Headers
#include "GameData.h"
#include "GameState.h"
#include "DrawData.h"
#include "DrawData2D.h"
#include "ObjectList.h"

#include "CMOGO.h"
#include <DirectXCollision.h>
#include "Collision.h"
#include <cmath>
#include <iomanip>

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;



Game::Game() noexcept :
    m_window(nullptr),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_11_0)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND _window, int _width, int _height)
{
    m_window = _window;
    m_outputWidth = std::max(_width, 1);
    m_outputHeight = std::max(_height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    //seed the random number generator
    srand((UINT)time(NULL));

    //set up keyboard and mouse system
    //documentation here: https://github.com/microsoft/DirectXTK/wiki/Mouse-and-keyboard-input
    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(_window);
    m_mouse->SetMode(Mouse::MODE_RELATIVE);
    //Hide the mouse pointer
    ShowCursor(false);

    //create GameData struct and populate its pointers
    m_GameData = std::make_unique <GameData>();
    m_GameData->m_GameState = GS_START_MENU;

    //set up systems for 2D rendering
    m_DrawData2D = std::make_unique<DrawData2D>();
    m_DrawData2D->m_Sprites.reset(new SpriteBatch(m_d3dContext.Get()));
    m_DrawData2D->m_Font.reset(new SpriteFont(m_d3dDevice.Get(), L"..\\Assets\\italic.spritefont"));
    m_states = new CommonStates(m_d3dDevice.Get());

    //set up DirectXTK Effects system
    m_fxFactory = new EffectFactory(m_d3dDevice.Get());
    //Tell the fxFactory to look to the correct build directory to pull stuff in from
    ((EffectFactory*)m_fxFactory)->SetDirectory(L"..\\Assets");
    //init render system for VBGOs
    VBGO::Init(m_d3dDevice.Get());

    //set audio system
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif
    m_audioEngine = std::make_unique<AudioEngine>(eflags);

    //create a set of dummy things to show off the engine
    float AR = (float)_width / (float)_height;
    //create a base light
    m_DrawData = std::make_shared<DrawData>();
    m_DrawData->m_pd3dImmediateContext = nullptr;
    m_DrawData->m_states = m_states;
    m_DrawData->m_cam = m_FPScam;
    m_DrawData->m_light = m_light;

    LoadObjects(  _width,  _height);
    
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& _timer)
{
    if (!m_GameData->isPaused)
    {
        m_timeLimit -= m_GameData->m_DeltaTime;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(3) << m_timeLimit;
        std::string tempTime = stream.str();
        timerText->SetText(tempTime);
    }
    if (m_timeLimit <= 0.0f || pPlayer->GetHealth() <= 0.0f)
    {
        m_GameData->isPaused = true;
        m_GameData->m_GameState = GS_GAME_OVER;
    }
    if (m_endDialoge->GetHasEnded())
    {
        if (!m_GameData->isPaused) {
            m_GameData->isPaused = true;
            m_score = m_score * m_timeLimit;
            finalSocreText->SetText(std::to_string(m_score));
            m_GameData->m_GameState = GS_WIN;
        }
    }


    float elapsedTime = float(_timer.GetElapsedSeconds());
    m_GameData->m_DeltaTime = elapsedTime;

    //this will update the audio engine but give us chance to do somehting else if that isn't working
    if (!m_audioEngine->Update())
    {
        if (m_audioEngine->IsCriticalError())
        {
            // We lost the audio device!
        }
    }
    else
    {
        //update sounds playing
        for (list<Sound*>::iterator it = m_Sounds.begin(); it != m_Sounds.end(); it++)
        {
            (*it)->Tick(m_GameData.get());
        }
    }
    ReadInput();
    //upon space bar switch camera state
    //see docs here for what's going on: https://github.com/Microsoft/DirectXTK/wiki/Keyboard
    if (m_GameData->isPlaying)
    {
        if (m_GameData->m_KeyBoardState_tracker.pressed.T && !m_GameData->isPaused)
        {
            if (m_GameData->m_GameState == GS_PLAY_FPS_CAM)
            {
                m_GameData->m_GameState = GS_PLAY_TPS_CAM;
                m_GameData->gameStateChanged = true;
            }
            else
            {
                m_GameData->m_GameState = GS_PLAY_FPS_CAM;
                m_GameData->gameStateChanged = true;
            }
        }
    }
    if (m_GameData->m_GameState == GS_START_MENU)
    {
        if (m_GameData->m_KeyBoardState_tracker.pressed.Enter)
        {
            m_GameData->isPlaying = true;
            m_GameData->m_GameState = GS_PLAY_FPS_CAM;
        }
    }
    if (m_GameData->m_GameState == GS_WIN || m_GameData->m_GameState == GS_GAME_OVER)
    {
        if (m_GameData->m_KeyBoardState_tracker.pressed.Enter)
        {
            LoadObjects(m_windowWidth,  m_windowHeight);
            m_GameData->isPlaying = false;
            m_GameData->isPaused = true;
            m_GameData->m_GameState = GS_START_MENU;
        }
    }

    
    //update all objects
  
    for (std::vector<std::shared_ptr<GameObject>>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
    {
        (*it)->Tick(m_GameData.get());
    }

    for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_GameObjects2D.begin(); it != m_GameObjects2D.end(); it++)
    {
        (*it)->Tick(m_GameData.get());
    }

    if (m_GameData->m_GameState == GS_PLAY_FPS_CAM)
    {
        for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_FPS_GameObjects2D.begin(); it != m_FPS_GameObjects2D.end(); it++)
        {
            (*it)->Tick(m_GameData.get());
        }
    }
    CheckCollision();
    CheckCollisionGroundWithPlayer();
    CheckCollisionEnemyBullet();
    CheckCollisionMapWithPlayer(800, 600);
    CheckCollisionMapWithEnemies();
    CheckCollisionBulletWithPlayer();
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    //set immediate context of the graphics device
    m_DrawData->m_pd3dImmediateContext = m_d3dContext.Get();

    //set which camera to be used
    m_DrawData->m_cam = m_FPScam;
    if (m_GameData->m_GameState == GS_PLAY_TPS_CAM)
    {
        m_DrawData->m_cam = m_TPScam;
    }

    //update the constant buffer for the rendering of VBGOs
    VBGO::UpdateConstantBuffer(m_DrawData.get());

    m_DrawData2D->m_Sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
    if (m_GameData->m_GameState == GS_PLAY_FPS_CAM || m_GameData->m_GameState == GS_PLAY_TPS_CAM)
    {
        //Draw 3D Game Obejects
        for (std::vector<std::shared_ptr<GameObject>>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
        {
            if ((*it)->IsVisible())
            {
                (*it)->Draw(m_DrawData.get());
            }
        }

        // Draw sprite batch stuff 
  
        for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_GameObjects2D.begin(); it != m_GameObjects2D.end(); it++)
        {
            (*it)->Draw(m_DrawData2D.get());
        }


        if (m_GameData->m_GameState == GS_PLAY_FPS_CAM)
        {
            for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_FPS_GameObjects2D.begin(); it != m_FPS_GameObjects2D.end(); it++)
            {
                (*it)->Draw(m_DrawData2D.get());
            }
        }
    }
    else if (m_GameData->m_GameState == GS_START_MENU)
    {
        for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_Menu_GameObjects2D.begin(); it != m_Menu_GameObjects2D.end(); it++)
        {
            (*it)->Draw(m_DrawData2D.get());
        }
    }
    else if (m_GameData->m_GameState == GS_GAME_OVER)
    {
        for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_Lose_GameObjects2D.begin(); it != m_Lose_GameObjects2D.end(); it++)
        {
            (*it)->Draw(m_DrawData2D.get());
        }
    }
    else if (m_GameData->m_GameState == GS_WIN)
    {
        for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_Win_GameObjects2D.begin(); it != m_Win_GameObjects2D.end(); it++)
        {
            (*it)->Draw(m_DrawData2D.get());
        }
    }


    m_DrawData2D->m_Sprites->End();
    //drawing text screws up the Depth Stencil State, this puts it back again!
    m_d3dContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int _width, int _height)
{
    m_outputWidth = std::max(_width, 1);
    m_outputHeight = std::max(_height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& _width, int& _height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    _width = 800;
    _height = 600;

}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    //creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
    //something missing on the machines in 2Q28
    //this should work!
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    DX::ThrowIfFailed(D3D11CreateDevice(
        nullptr,                            // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        static_cast<UINT>(std::size(featureLevels)),
        D3D11_SDK_VERSION,
        device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
        &m_featureLevel,                    // returns feature level of device created
        context.ReleaseAndGetAddressOf()    // returns the device immediate context
        ));

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(device.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    DX::ThrowIfFailed(device.As(&m_d3dDevice));
    DX::ThrowIfFailed(context.As(&m_d3dContext));

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(static_cast<UINT>(std::size(nullViews)), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    const UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    const UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    constexpr UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory2> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = backBufferCount;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a SwapChain from a Win32 window.
        DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
            m_d3dDevice.Get(),
            m_window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            m_swapChain.ReleaseAndGetAddressOf()
            ));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_d3dContext.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}

void Game::ReadInput()
{
    m_GameData->m_KeyBoardState = m_keyboard->GetState();
    m_GameData->m_KeyBoardState_tracker.Update(m_GameData->m_KeyBoardState);
    if (m_GameData->m_KeyBoardState.Escape)
    {
        ExitGame();
    }
    m_GameData->m_Mouse = m_mouse->GetState();
    m_GameData->m_Mouse_tracker.Update(m_GameData->m_Mouse);


    RECT window;
    GetWindowRect(m_window, &window);
}
 
void Game::CheckCollision()
{
    for (int i = 0; i < m_PhysicsObjects.size(); i++) for (int j = 0; j < m_ColliderObjects.size(); j++)
    {
        float distanceToTarget = (m_PhysicsObjects[i]->GetPos() - m_ColliderObjects[j]->GetPos()).Length();
        distanceToTarget = abs(distanceToTarget);
        if (m_ColliderObjects[j]->GetType() == TerrainType::FLOOR || distanceToTarget < 300 ) {
            if (m_PhysicsObjects[i]->Intersects(*m_ColliderObjects[j])) //std::cout << "Collision Detected!" << std::endl;
            {
                XMFLOAT3 eject_vect = Collision::ejectionCMOGO(*m_PhysicsObjects[i], *m_ColliderObjects[j]);
                auto pos = m_PhysicsObjects[i]->GetPos();
                m_PhysicsObjects[i]->SetPos(pos - eject_vect);
            }
        }
    }
    if (m_GameData->m_GameState == GS_PLAY_FPS_CAM)
    {
        m_FPScam->Tick(m_GameData.get());
    }
    else
    {
        m_TPScam->Tick(m_GameData.get());
    }

    for (size_t i = 0; i < m_Eniemies.size(); i++) {
        for (size_t j = i + 1; j < m_Eniemies.size(); j++) {
            // Safely get references to the enemies
            auto& enemy_i = *m_Eniemies[i];
            auto& enemy_j = *m_Eniemies[j];

            // Compute the distance
            auto diff = enemy_i.GetPos() - enemy_j.GetPos();
            float distanceToTarget = diff.Length();

            // Check distance and intersection
            if (distanceToTarget < 300 && enemy_i.Intersects(enemy_j)) {
                // Calculate the ejection vector
                XMFLOAT3 eject_vect = Collision::ejectionCMOGO(enemy_i, enemy_j);

                // Adjust positions
                enemy_i.SetPos(enemy_i.GetPos() - eject_vect);
                enemy_j.SetPos(enemy_j.GetPos() + eject_vect);
            }
        }
    }
}

void Game::CheckCollisionGroundWithPlayer()
{
 for (int j = 0; j < m_ColliderObjects.size(); j++)
    {
        if (pPlayer->Intersects(*m_ColliderObjects[j]) && m_ColliderObjects[j]->GetType() == TerrainType::FLOOR) //std::cout << "Collision Detected!" << std::endl;
        {
            pPlayer->SetIsGrounded(true);
        }
    }
}

void Game::CheckCollisionEnemyBullet()
{
    for (int i = 0; i < m_Eniemies.size(); )
    {
        bool enemyErased = false;

        for (int j = 0; j < p_bullets.size() && !enemyErased; j++)
        {
            if (p_bullets[j]->IsVisible())
            {
                if (p_bullets[j]->Intersects(*m_Eniemies[i]))
                {
                    m_Eniemies[i]->SetVisibility(false);
                    p_bullets[j]->Stop();
                    RemoveEnemyFromAllContainers(m_Eniemies[i]);
                    m_Eniemies.erase(m_Eniemies.begin() + i);
                    enemyErased = true;
                    m_score++;
                    string tempScore = std::to_string(m_score);
                    textScore->SetText(tempScore);
                }
            }
        }
        if (!enemyErased)
        {
            i++;
        }
    }
}

void Game::RemoveEnemyFromAllContainers(std::shared_ptr<GameObject> enemy)
{
    auto itCollider = std::find(m_ColliderObjects.begin(), m_ColliderObjects.end(), enemy);
    if (itCollider != m_ColliderObjects.end()) {
        m_ColliderObjects.erase(itCollider);
    }

    auto itPhysics = std::find(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), enemy);
    if (itPhysics != m_PhysicsObjects.end()) {
        m_PhysicsObjects.erase(itPhysics);
    }

    auto itGameObjects = std::find(m_GameObjects.begin(), m_GameObjects.end(), enemy);
    if (itGameObjects != m_GameObjects.end()) {
        m_GameObjects.erase(itGameObjects);
    }
}

void Game::CheckCollisionMapWithPlayer(int _width, int _height)
{
    for (int j = 0; j < m_Map.size(); j++)
    {
        if (pPlayer->Intersects(*m_Map[j]))
        {
            pPlayer->SetTilePos(m_Map[j]->GetTile());
            if (m_Map[j]->GetWorth() == 4)
            {
                if (!dialougeIsTrigerred) {
                    if (m_score == enemySpawnPoses.size())
                    {
                        m_endDialoge->ShowDialouge(m_GameData.get());
                    }
                    else
                    {
                        m_missingEnemyDialoge->ShowDialouge(m_GameData.get());
                    }
                    dialougeIsTrigerred = true;
                }
            }
            else
            {
                dialougeIsTrigerred = false;
            }
            break;
        }  
    }
}

void Game::CheckCollisionMapWithEnemies()
{
    for (int i = 0; i < m_Eniemies.size(); ++i)
    {
        for (int j = 0; j < m_Map.size(); ++j)
        {
            if (m_Eniemies[i]->Intersects(*m_Map[j]))
            {
                // Update the tile position of the current enemy
                m_Eniemies[i]->SetTilePos(m_Map[j]->GetTile());
                int x = m_Map[j]->GetTile().x;
                int y = m_Map[j]->GetTile().y;
                break;
            }
        }
    }
    if (m_GameData->m_GameState == GS_PLAY_FPS_CAM)
    {
        m_FPScam->Tick(m_GameData.get());
    }
    else
    {
        m_TPScam->Tick(m_GameData.get());
    }
}

void Game::CheckCollisionBulletWithPlayer()
{
    for (int j = 0; j < p_Ebullets.size(); j++)
    {
        if (p_Ebullets[j]->IsVisible())
        {
            {
                if (pPlayer->Intersects(*p_Ebullets[j]))
                {
                    pPlayer->TakeDamage(20);
                    p_Ebullets[j]->Stop();
                    break;
                }
            }
        }
    }
}

void Game::LoadObjects(int _width, int _height)
{
    m_windowHeight = _height;
    m_windowWidth = _width;
    float AR = (float)_width / (float)_height;
    m_timeLimit = 30.00f;
    int m_score = 0;

    ClearAllVectors();
   
    m_light = std::make_shared<Light>(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
    m_GameObjects.push_back(m_light);

    std::shared_ptr<AnimatedObject3D> test = std::make_shared<AnimatedObject3D>("test1", m_d3dDevice.Get(), m_fxFactory);
    m_GameObjects.push_back(test);

    LoadMap();

    hostageSpawn.x -= 30;
    hostageSpawn.z += 70;
    std::shared_ptr<Terrain> pHostile = std::make_shared<Terrain>("BirdModelV1", m_d3dDevice.Get(), m_fxFactory, hostageSpawn, 0.0f, quadrant*2 , 0.0f, 10.0f * Vector3::One);
    m_GameObjects.push_back(pHostile);

    LoadPlayerObject();

    LoadEnemyObjects();

    m_FPScam = std::make_shared <FPSCamera>(0.25f * XM_PI, AR, 1.0f, 1000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 0.0f, 0.001f), _width, _height);
    m_GameObjects.push_back(m_FPScam);

    m_TPScam = std::make_shared <TPSCamera>(0.25f * XM_PI, AR, 1.0f, 1000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 0.00f, -500.0f));
    m_GameObjects.push_back(m_TPScam);


    LoadPlayerBullets();

    LoadGameUIElements(_width, _height);
    LoadDialoges();

    m_GameData->isPaused = true;

    TestSound* TS = new TestSound(m_audioEngine.get(), "Explo1");
    TS->SetVolume(0.5f);
    m_Sounds.push_back(TS);

    LoadStartMenuUIElements();
    LoadLoosetMenuUIElements();
    LoadWintMenuUIElements();
}

void Game::ClearAllVectors()
{
    m_GameObjects.clear(); 
    m_GameObjects2D.clear(); 
    m_FPS_GameObjects2D.clear();
    m_ColliderObjects.clear();
    m_PhysicsObjects.clear();
    p_bullets.clear();
    p_Ebullets.clear();
    m_Eniemies.clear();
    m_Map.clear();
    enemySpawnPoses.clear();
}

void Game::LoadDialoges()
{
    std::vector<string> missionTexts;
    missionTexts.push_back(std::string("Your mission is to find the missing \n person in this maze..."));
    missionTexts.push_back(std::string("Defeat all the enemies..."));
    missionTexts.push_back(std::string("All within the time limit."));

    m_missionDialoge = std::make_shared<DialogueBox>("grey_button00", missionTexts, m_d3dDevice.Get());
    for (const auto& image2 : m_missionDialoge->Get2DObjects())
    {
        m_GameObjects2D.push_back(image2);
    }
    m_GameObjects2D.push_back(m_missionDialoge);
    m_missionDialoge->ShowDialouge(m_GameData.get());

    std::vector<string> victoryTexts;
    victoryTexts.push_back(std::string("You saved me from your clones"));
    victoryTexts.push_back(std::string("I hope I can trust you"));
    victoryTexts.push_back(std::string("Lets Get out of here"));
    m_endDialoge = std::make_shared<DialogueBox>("grey_button00", victoryTexts, m_d3dDevice.Get());
    for (const auto& image : m_endDialoge->Get2DObjects())
    {
        m_GameObjects2D.push_back(image);
    }
    m_GameObjects2D.push_back(m_endDialoge);


    std::vector<string> missingEnemyTexts;
    missingEnemyTexts.push_back(std::string("You found me..."));
    missingEnemyTexts.push_back(std::string("But there is still some after us"));
    missingEnemyTexts.push_back(std::string("Eliminate them first."));
    m_missingEnemyDialoge = std::make_shared<DialogueBox>("grey_button00", missingEnemyTexts, m_d3dDevice.Get());
    for (const auto& object : m_missingEnemyDialoge->Get2DObjects())
    {
        m_GameObjects2D.push_back(object);
    }
    m_GameObjects2D.push_back(m_missingEnemyDialoge);
}

void Game::LoadMap()
{
    std::shared_ptr<Terrain> floor = std::make_shared<Terrain>("floor", m_d3dDevice.Get(), m_fxFactory, Vector3(-150, 0, 150), 0.0f, 0.0f, 0.0f, 1 * Vector3::One);
    floor->SetType(TerrainType::FLOOR);
    m_GameObjects.push_back(floor);
    m_ColliderObjects.push_back(floor);
    floor->SetPos(Vector3(-floor->GetSize().x / 2, -floor->GetSize().y, -floor->GetSize().z / 2));

    std::shared_ptr<Terrain> cube = std::make_shared<Terrain>("cube", m_d3dDevice.Get(), m_fxFactory, Vector3(25.0f, 40.0f, -75.001f), quadrant, 0.0f, quadrant, 1.0f * Vector3::One);

    m_GameObjects.push_back(cube);
    cube->SetVisibility(false);

    map = std::make_shared<GameMap>("map", 7, 7, cube->GetSize());
    m_GameObjects.push_back(map);

    float mapwidth = map->MAP_WIDTH * cube->GetSize().x;
    map->basePosition = { -mapwidth / 2, cube->GetSize().y / 2, -mapwidth / 2 };
    for (int y = 0; y < map->MAP_HEIGHT; ++y) {
        for (int x = 0; x < map->MAP_WIDTH; ++x) {

            float posX = map->basePosition.x + x * cube->GetSize().x;
            float posY = map->basePosition.y;
            float posZ = map->basePosition.z + y * cube->GetSize().x;

            std::shared_ptr<Tile> cube = std::make_shared<Tile>(
                "cube", m_d3dDevice.Get(), m_fxFactory,
                DirectX::XMFLOAT3(posX, posY, posZ),
                quadrant, 0.0f, 0.0f, 1.0f * DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
                Vector2(x, y));

            if (map->map[y][x] == 1) {
                m_GameObjects.push_back(cube);
                m_ColliderObjects.push_back(cube);
            }
            else
            {
                cube->SetVisibility(false);
                m_PhysicsObjects.push_back(cube);
            }

            if (map->map[y][x] == 2)
            {
                Vector3 newSpawnPos = cube->GetPos();
                newSpawnPos.y -= 40;
                enemySpawnPoses.push_back(newSpawnPos);
            }
            else if (map->map[y][x] == 3)
            {
                playerSpawn = cube->GetPos();
            }
            else if (map->map[y][x] == 4)
            {
                hostageSpawn = cube->GetPos();
            }
            cube->SetWorth(map->map[y][x]);
            m_GameObjects.push_back(cube);
            m_Map.push_back(cube);
        }
    }
}

void Game::LoadPlayerObject()
{
    pPlayer = std::make_shared<Player>("Test", m_d3dDevice.Get(), m_fxFactory);
    pPlayer->SetScale(1.4);
    m_GameObjects.push_back(pPlayer);
    m_PhysicsObjects.push_back(pPlayer);
    pPlayer->SetPos(playerSpawn);
}

void Game::LoadPlayerBullets()
{
    for (size_t i = 0; i < 10; i++)
    {
        std::shared_ptr<Bullet> pBullet = std::make_shared<Bullet>("cube", m_d3dDevice.Get(), m_fxFactory, *m_FPScam);
        pBullet->SetVisibility(false);
        pBullet->SetScale(0.02);
        m_GameObjects.push_back(pBullet);
        m_PhysicsObjects.push_back(pBullet);
        p_bullets.push_back(pBullet);
    }
    pPlayer->SetBullets(p_bullets);
}

void Game::LoadEnemyObjects()
{
    for (int j = 0; j < enemySpawnPoses.size(); ++j)
    {
        std::shared_ptr<Enemy> test = std::make_shared<Enemy>("Test", m_d3dDevice.Get(), m_fxFactory, enemySpawnPoses[j], 0.0f, 0.0f, 0.0f, 1.4f * Vector3::One, pPlayer, map);
        m_GameObjects.push_back(test);
        m_PhysicsObjects.push_back(test);
        m_Eniemies.push_back(test);
        for (size_t i = 0; i < 2; i++)
        {
            std::shared_ptr<EnemyBullets> pEnemyBullet = std::make_shared<EnemyBullets>("cube", m_d3dDevice.Get(), m_fxFactory, test);
            pEnemyBullet->SetVisibility(false);
            pEnemyBullet->SetScale(0.02);
            m_GameObjects.push_back(pEnemyBullet);
            m_PhysicsObjects.push_back(pEnemyBullet);
            test->SetBullet(pEnemyBullet);
            p_Ebullets.push_back(pEnemyBullet);
        }
    }
}

void Game::LoadGameUIElements(float _width,float _height)
{
    // 2D images
    std::shared_ptr<ImageGO2D> croshair = std::make_shared<ImageGO2D>("croshair", m_d3dDevice.Get());
    RECT window;
    GetWindowRect(m_window, &window);
    croshair->SetPos(Vector2(_width / 2, _height / 2));
    croshair->SetScale(0.05);
    m_FPS_GameObjects2D.push_back(croshair);

    //Text
    m_score = 0;
    textScore = std::make_shared<TextGO2D>("0");
    textScore->SetPos(Vector2(10, 10));
    textScore->SetColour(Color((float*)&Colors::Red));
    m_GameObjects2D.push_back(textScore);
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << m_timeLimit;
    std::string timeString = ss.str();
    timerText = std::make_shared<TextGO2D>(timeString);
    timerText->SetPos(Vector2(350, 10));
    timerText->SetColour(Color((float*)&Colors::Red));
    m_GameObjects2D.push_back(timerText);

    //weapon frames 
    std::vector<string> frameFileNames;
    frameFileNames.push_back(std::string("pistol1"));
    frameFileNames.push_back(std::string("pistol2"));
    frameFileNames.push_back(std::string("pistol3"));
    frameFileNames.push_back(std::string("pistol4"));
    std::shared_ptr<AnimatedImage> weapon = std::make_shared<AnimatedImage>(frameFileNames, m_d3dDevice.Get(), 20, false);
    weapon->SetPos(Vector2(_width / 1.5, _height - 200));
    weapon->SetRot(0.2);
    weapon->SetScale(2.5);
    m_FPS_GameObjects2D.push_back(weapon);

    //healtbar for player
    std::shared_ptr<HealthBar> pHealthbard = std::make_shared<HealthBar>("green_button00", "grey_button00", m_d3dDevice.Get(), pPlayer);
    for (const auto& image : pHealthbard->images)
    {
        m_GameObjects2D.push_back(image);
    }
    m_GameObjects2D.push_back(pHealthbard);
}

void Game::LoadStartMenuUIElements()
{
    std::shared_ptr<ImageGO2D> background = std::make_shared<ImageGO2D>("victory", m_d3dDevice.Get());
    background->SetScale(0.5);
    background->SetPos(Vector2(0, 40));
    background->SetOrigin(Vector2(0, 0));
    m_Menu_GameObjects2D.push_back(background);

    std::shared_ptr<TextGO2D> instructions = std::make_shared<TextGO2D>("Press Enter To Start \n \n Escape to Exit any point");
    instructions->SetPos(Vector2(80, 150));
    instructions->SetColour(Color((float*)&Colors::Yellow));
    instructions->SetScale(1.5);
    m_Menu_GameObjects2D.push_back(instructions);

}

void Game::LoadLoosetMenuUIElements()
{
    std::shared_ptr<ImageGO2D> background = std::make_shared<ImageGO2D>("red", m_d3dDevice.Get());
    background->SetOrigin(Vector2(0, 0));
    m_Lose_GameObjects2D.push_back(background);

    std::shared_ptr<TextGO2D> instructions = std::make_shared<TextGO2D>("GAME OVER \n Press enter to \n retun to menu");
    instructions->SetPos(Vector2(50, 200));
    instructions->SetColour(Color((float*)&Colors::Yellow));
    instructions->SetScale(1.5);
    m_Lose_GameObjects2D.push_back(instructions);
}

void Game::LoadWintMenuUIElements()
{
    std::shared_ptr<ImageGO2D> background = std::make_shared<ImageGO2D>("green", m_d3dDevice.Get());
    background->SetOrigin(Vector2(0, 0));
    background->SetScale(1.5);
    m_Win_GameObjects2D.push_back(background);

    finalSocreText = std::make_shared<TextGO2D>("Place Holder");
    finalSocreText->SetScale(2);
    finalSocreText->SetPos(Vector2(100, 400));
    m_Win_GameObjects2D.push_back(finalSocreText);
    std::shared_ptr<TextGO2D> instructions = std::make_shared<TextGO2D>("GAME WON \n Press enter to \n retun to menu \n Final score was:");
    instructions->SetPos(Vector2(100, 100));
    instructions->SetColour(Color((float*)&Colors::White));
    instructions->SetScale(1);
    m_Win_GameObjects2D.push_back(instructions);
}