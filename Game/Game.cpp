//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <time.h>

#include <iostream>

//Scarle Headers
#include "GameData.h"
#include "GameState.h"
#include "DrawData.h"
#include "DrawData2D.h"
#include "ObjectList.h"

#include "CMOGO.h"
#include <DirectXCollision.h>
#include "Collision.h"

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
    m_GameData->m_GameState = GS_PLAY_FPS_CAM;

    //set up systems for 2D rendering
    m_DrawData2D = std::make_unique<DrawData2D>();
    m_DrawData2D->m_Sprites.reset(new SpriteBatch(m_d3dContext.Get()));
    m_DrawData2D->m_Font.reset(new SpriteFont(m_d3dDevice.Get(), L"..\\Assets\\italic.spritefont"));
    m_states = new CommonStates(m_d3dDevice.Get());

    //set up DirectXTK Effects system
    m_fxFactory =  new EffectFactory(m_d3dDevice.Get());
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

    //create a base light
    m_light = std::make_shared<Light>(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
    m_GameObjects.push_back(m_light);

    //find how big my window is to correctly calculate my aspect ratio
    float AR = (float)_width / (float)_height;

  
    float derekszog = XMConvertToRadians(90);


    //MAP:

    std::shared_ptr<Terrain> wall1 = std::make_shared<Terrain>("testwall", m_d3dDevice.Get(), m_fxFactory, Vector3(100, 40.0f, 75.002f), derekszog, 0.0f, 0.0f, 0.1f * Vector3::One);
    m_GameObjects.push_back(wall1);
    m_ColliderObjects.push_back(wall1);
    std::shared_ptr<Terrain> wall2 = std::make_shared<Terrain>("testwall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0f, 40.0f, 0.0f), derekszog, 0.0f, 0.0f, 0.1f * Vector3::One);
    m_GameObjects.push_back(wall2);
    m_ColliderObjects.push_back(wall2);
    std::shared_ptr<Terrain> wall3 = std::make_shared<Terrain>("testwall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0f, 40.0f, -75.001f), derekszog, 0.0f, 0.0f, 0.1f * Vector3::One);
    m_GameObjects.push_back(wall3);
    m_ColliderObjects.push_back(wall3);
    std::shared_ptr<Terrain> wall4 = std::make_shared<Terrain>("testwall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0f, 40.0f, -75.001f), derekszog, 0.0f, derekszog, 0.1f * Vector3::One);
    m_GameObjects.push_back(wall4);
    m_ColliderObjects.push_back(wall4);
    std::shared_ptr<Terrain> wall5 = std::make_shared<Terrain>("testwall", m_d3dDevice.Get(), m_fxFactory, Vector3(25.0f, 40.0f, -75.001f), derekszog, 0.0f, derekszog, 0.1f * Vector3::One);
    m_GameObjects.push_back(wall5);
    m_ColliderObjects.push_back(wall5);

    std::shared_ptr<Terrain> floor = std::make_shared<Terrain>("floor", m_d3dDevice.Get(), m_fxFactory, Vector3(-75, 0, 150), 0.0f, 0.0f, 0.0f, 0.15f * Vector3::One);
    floor->SetType(TerrainType::FLOOR);
    m_GameObjects.push_back(floor);
    m_ColliderObjects.push_back(floor);
    

    //add Player
    pPlayer = std::make_shared<Player>("Test", m_d3dDevice.Get(), m_fxFactory);
    pPlayer->SetPos(Vector3(20, 20, 20));
    m_GameObjects.push_back(pPlayer);
    m_PhysicsObjects.push_back(pPlayer);
 
    //create a base camera
    m_FPScam = std::make_shared <FPSCamera>(0.4f * XM_PI, AR, 1.0f, 1000.0f, pPlayer,  Vector3::UnitY, Vector3(0.0f,0.0f, 0.001f), _width, _height);
    m_GameObjects.push_back(m_FPScam);

    //add a secondary camera
    m_TPScam = std::make_shared <TPSCamera>(0.25f * XM_PI, AR, 1.0f, 1000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 300.0f, 300.0f));
    m_GameObjects.push_back(m_TPScam);

    //bullets
    for (size_t i = 0; i < 20; i++)
    {
        std::shared_ptr<Bullet> pBullet = std::make_shared<Bullet>("cat", m_d3dDevice.Get(), m_fxFactory, *m_FPScam);
        pBullet->SetVisibility(false);
        pBullet->SetScale(0.5);
        m_GameObjects.push_back(pBullet);
        m_PhysicsObjects.push_back(pBullet);
        p_bullets.push_back(pBullet);
    }
    pPlayer->bullets = p_bullets;


    //create DrawData struct and populate its pointers
    m_DrawData = std::make_shared<DrawData>();
    m_DrawData->m_pd3dImmediateContext =nullptr;
    m_DrawData->m_states = m_states;
    m_DrawData->m_cam = m_FPScam;
    m_DrawData->m_light = m_light;

    //example basic 2D stuff
    std::shared_ptr<ImageGO2D> croshair = std::make_shared<ImageGO2D>("croshair", m_d3dDevice.Get());
    RECT window;
    GetWindowRect(m_window, &window);
    croshair->SetPos(Vector2(_width /2, _height/2));
    croshair->SetScale(0.05);
    m_FPS_GameObjects2D.push_back(croshair);

    std::shared_ptr<ImageGO2D> weapon =  std::make_shared<ImageGO2D>("pistol1", m_d3dDevice.Get());
    weapon->SetPos(Vector2(_width / 1.5, _height - 150));
    weapon->SetScale(2.5);
    m_FPS_GameObjects2D.push_back(weapon);

    std::shared_ptr<TextGO2D> text = std::make_shared<TextGO2D>("000");
    text->SetPos(Vector2(10 , 10));
    text->SetScale(0.5);
    text->SetColour(Color((float*)&Colors::White));
    m_GameObjects2D.push_back(text);
    

    //Test Sounds
    Loop* loop = new Loop(m_audioEngine.get(), "NightAmbienceSimple_02");
    loop->SetVolume(0.5f);
    loop->Play();
    m_Sounds.push_back(loop);

    TestSound* TS = new TestSound(m_audioEngine.get(), "Explo1");
    TS->SetVolume(0.5f);
    m_Sounds.push_back(TS);
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
    if (m_GameData->m_KeyBoardState_tracker.pressed.T)
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
 

    //update all objects
    for (std::vector<std::shared_ptr<GameObject>>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
    {
        (*it)->Tick(m_GameData.get());
    }
    for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_GameObjects2D.begin(); it != m_GameObjects2D.end(); it++)
    {
        (*it)->Tick(m_GameData.get());
    }

    CheckCollision();
    CheckCollisionGroundWithPlayer();
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

    //Draw 3D Game Obejects
    for (std::vector<std::shared_ptr<GameObject>>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
    {
        if ((*it)->IsVisible())
        {
            (*it)->Draw(m_DrawData.get());
        }
    }

    // Draw sprite batch stuff 
    m_DrawData2D->m_Sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
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
    //quit game on hiting escape
    if (m_GameData->m_KeyBoardState.Escape)
    {
        ExitGame();
    }

    m_GameData->m_Mouse = m_mouse->GetState();
    m_GameData->m_Mouse_tracker.Update(m_GameData->m_Mouse);
 

    //lock the cursor to the centre of the window
    RECT window;
    GetWindowRect(m_window, &window);
    SetCursorPos((window.left + window.right) >> 1, (window.bottom + window.top) >> 1);
}

void Game::CheckCollision()
{
    for (int i = 0; i < m_PhysicsObjects.size(); i++) for (int j = 0; j < m_ColliderObjects.size(); j++)
    {
        if (m_PhysicsObjects[i]->Intersects(*m_ColliderObjects[j])) //std::cout << "Collision Detected!" << std::endl;
        {
            XMFLOAT3 eject_vect = Collision::ejectionCMOGO(*m_PhysicsObjects[i], *m_ColliderObjects[j]);
            auto pos = m_PhysicsObjects[i]->GetPos();
            m_PhysicsObjects[i]->SetPos(pos - eject_vect);
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