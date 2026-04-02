// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/Credits.h"
#include "Framework/World/Actor.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/InputSubsystem.h"
#include "Component/PostProcessingComponent.h"
#include "PostProcess/Effects/PPEClouds.h"
#include "Utility/Log.h"

namespace we
{
    Credits::Credits(WorldSubsystem& Subsystem)
        : World(Subsystem)
    {
    }

    void Credits::BeginPlay()
    {
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
    }

    void Credits::Tick(float DeltaTime)
    {
        // Update cloud scrolling effect
        if (CloudsPPC)
        {
            CloudsPPC->Tick(DeltaTime);
        }
    }

    void Credits::EndPlay()
    {
        // Clean up cloud post-processing
        if (CloudsPPC)
        {
            CloudsPPC->EndPlay();
            CloudsPPC.reset();
        }
    }

    void Credits::Quit()
    {
    }
}