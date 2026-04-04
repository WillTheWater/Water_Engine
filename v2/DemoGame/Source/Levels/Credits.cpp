// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/Credits.h"
#include "Levels/MainMenu.h"
#include "Framework/World/Actor.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/AudioSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Subsystem/CursorSubsystem.h"
#include "Component/PostProcessingComponent.h"
#include "PostProcess/Effects/PPEClouds.h"
#include "Input/InputActions.h"
#include "Utility/Log.h"

namespace we
{
    Credits::Credits(WorldSubsystem& Subsystem)
        : World(Subsystem)
    {
    }

    void Credits::BeginPlay()
    {
        PlayAudio().CrossfadeMusic("Assets/Audio/Default/defaultForestMusic.ogg", 2);
        PlayAudio().CrossfadeAmbient("Assets/Audio/Default/defaultForestAmbient.ogg", 2);

        // Load clouds texture and set up scrolling effect
        Clouds = LoadAsset().LoadTexture("Assets/Textures/Game/clouds.png");
        CloudImage = SpawnActor<Actor>().lock();
        CloudImage->SetPosition({ 0, 0 });
        
        // Set up cloud scrolling post-processing
        CloudsPPC = make_shared<PostProcessingComponent>(CloudImage.get());
        CloudsPPC->SetTexture(Clouds);
        CloudsPPC->AddEffect(make_unique<PPEClouds>());
        CloudsPPC->BeginPlay();
        
        // Load ground/credits background
        Ground = LoadAsset().LoadTexture("Assets/Textures/Game/credits.png");
        GroundImage = SpawnActor<Actor>().lock();
        GroundImage->SetSprite(Ground);
        
        // Setup and auto-start credits dialog
        DialogUI.Initialize();
        DialogUI.SetDialog({
            "Welcome, traveler.",
            "You stand at the threshold of something new...",
            "The Water Engine was born from a simple dream.",
            "The life task of it's creator Will The Water.",
            "Not buried in complexity, but clear as water.",
            "Every tool you need, flowing together naturally.",
            "Physics, rendering, input, UI... all connected.",
            "You don't need a team.",
            "You don't need years of engine experience.",
            "You just need an idea, and the will to build it.",
            "The demo you played? Built with the Water Engine.",
            "The characters, the world, the very framework of it's existance.",
            "all possible because someone decided to begin.",
            "The forest ahead is yours to shape.",
            "Your story starts with a single line of code.",
            "Welcome to the Water Engine.",
            "Make it extraordinary."
        });
        
        // Setup button callbacks
        DialogUI.SetOnMainMenu([this]() {
            Subsystem.LoadWorld<MainMenu>();
        });
        
        DialogUI.SetOnQuit([this]() {
            Subsystem.Quit();
        });
        
        // Bind 'E' key and South button for dialog advancement
        InputController().Bind(ACTION_INTERACT, Input::Gamepad{ GamepadButton::South, 0 });
        DialogBinding = InputController().BindAction(ACTION_INTERACT, this, &Credits::AdvanceDialog);
        
        // Auto-show dialog
        DialogUI.Show();

        // Cursor hidden during dialog (shown only when buttons appear)
        GetCursor().SetVisibility(false);
    }

    void Credits::Tick(float DeltaTime)
    {
        // Update cloud scrolling effect
        if (CloudsPPC)
        {
            CloudsPPC->Tick(DeltaTime);
        }
        
        DialogUI.Tick(DeltaTime);
    }

    void Credits::AdvanceDialog()
    {
        if (!DialogUI.IsComplete())
        {
            DialogUI.Advance();
        }
    }

    void Credits::EndPlay()
    {
        // Release input binding
        DialogBinding.Release();
        
        // Clean up cloud post-processing
        if (CloudsPPC)
        {
            CloudsPPC->EndPlay();
            CloudsPPC.reset();
        }
        
        // Hide dialog
        DialogUI.Hide();
    }

    void Credits::Quit()
    {
    }
}
