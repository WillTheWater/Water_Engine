// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    class World;
    class PhysicsSubsystem;
    class CameraSubsystem;
    class SaveSubsystem;
    class GameInstance;
    
    using we::shared;

    class WorldSubsystem
    {
    public:
        void Tick(float DeltaTime);

        void SetPhysicsRef(shared<PhysicsSubsystem> InPhysics);
        PhysicsSubsystem& GetPhysics();
        
        void SetCameraRef(shared<CameraSubsystem> InCamera);
        CameraSubsystem& GetCamera();

        void SetSaveRef(shared<SaveSubsystem> InSave);
        SaveSubsystem& GetSave();
        
        void SetGameInstanceRef(shared<GameInstance> InGameInstance);
        GameInstance& GetGameInstance();

        shared<World> GetCurrentWorld() const { return CurrentWorld; }
        bool HasPendingWorld() const { return PendingWorld != nullptr; }
        vector<const drawable*>& GetOrderedDrawables() const;
        
        template<typename WorldType>
        void CreateWorld();
        
        template<typename WorldType>
        void LoadWorld() { CreateWorld<WorldType>(); }

        void Pause() { bIsPaused = true; }
        void Resume() { bIsPaused = false; }
        void TogglePause() { bIsPaused = !bIsPaused; }
        bool IsPaused() const { return bIsPaused; }
        void Quit() { bShouldQuit = true; }
        bool ShouldQuit() const { return bShouldQuit; }

    private:
        void GarbageCollect(float DeltaTime);

    private:
        weak<PhysicsSubsystem> Physics;
        weak<CameraSubsystem> Camera;
        weak<SaveSubsystem> Save;
        weak<GameInstance> GameInst;
        
        shared<World> CurrentWorld;
        shared<World> PendingWorld;

        float GCTimer = 0.0f;
        static constexpr float GCInterval = 3.0f;
        
        bool bIsPaused = false;
        bool bShouldQuit = false;
    };

    template<typename WorldType>
    inline void WorldSubsystem::CreateWorld()
    {
        PendingWorld = make_shared<WorldType>(*this);
    }
}