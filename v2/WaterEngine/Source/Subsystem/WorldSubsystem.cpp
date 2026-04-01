// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WorldSubsystem.h"
#include "Framework/World/World.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Subsystem/CameraSubsystem.h"
#include "Subsystem/SaveSubsystem.h"
#include "Framework/GameInstance.h"

namespace we
{
    void WorldSubsystem::SetPhysicsRef(shared<PhysicsSubsystem> InPhysics)
    {
        Physics = InPhysics;
    }

    void WorldSubsystem::SetCameraRef(shared<CameraSubsystem> InCamera)
    {
        Camera = InCamera;
    }

    PhysicsSubsystem& WorldSubsystem::GetPhysics()
    {
        return *Physics.lock();
    }

    CameraSubsystem& WorldSubsystem::GetCamera()
    {
        return *Camera.lock();
    }

    void WorldSubsystem::SetSaveRef(shared<SaveSubsystem> InSave)
    {
        Save = InSave;
    }

    SaveSubsystem& WorldSubsystem::GetSave()
    {
        return *Save.lock();
    }

    void WorldSubsystem::SetGameInstanceRef(shared<GameInstance> InGameInstance)
    {
        GameInst = InGameInstance;
    }

    GameInstance& WorldSubsystem::GetGameInstance()
    {
        return *GameInst.lock();
    }

    void WorldSubsystem::Tick(float DeltaTime)
    {
        if (PendingWorld)
        {
            if (CurrentWorld)
            {
                CurrentWorld->EndingPlay();
            }

            CurrentWorld = PendingWorld;
            PendingWorld = nullptr;
            
            // Update physics subsystem's current world for contact callbacks
            if (auto PhysicsPtr = Physics.lock())
            {
                PhysicsPtr->SetCurrentWorld(CurrentWorld.get());
            }
            
            CurrentWorld->StartPlay();
        }

        if (CurrentWorld)
        {
            // Only tick actors if not paused
            if (!bIsPaused)
            {
                CurrentWorld->StartTick(DeltaTime);
            }
            
            // Garbage collect always runs
            GarbageCollect(DeltaTime);
        }
    }

    void WorldSubsystem::GarbageCollect(float DeltaTime)
    {
        GCTimer += DeltaTime;
        if (GCTimer >= GCInterval)
        {
            GCTimer = 0.0f;
            CurrentWorld->GarbageCollection();
        }
    }

    vector<const drawable*>& WorldSubsystem::GetOrderedDrawables() const
    {
        static vector<const drawable*> Drawables;
        static vector<pair<uint, float>> Indices;
        static vector<const drawable*> ActorDrawables;
        
        Drawables.clear();
        Indices.clear();
        ActorDrawables.clear();
        
        if (!CurrentWorld)
            return Drawables;
        
        const auto& Actors = CurrentWorld->GetActors();
        
        for (uint i = 0; i < Actors.size(); ++i)
        {
            const auto& Actor = Actors[i];
            if (!Actor->IsPendingDestroy() && Actor->IsVisible())
            {
                ActorDrawables.clear();
                Actor->GetDrawables(ActorDrawables);
                
                for (const auto* Drawable : ActorDrawables)
                {
                    if (Drawable)
                    {
                        Indices.emplace_back(Drawables.size(), Actor->GetRenderDepth());
                        Drawables.push_back(Drawable);
                    }
                }
            }
        }
        
        std::sort(Indices.begin(), Indices.end(),
            [](const auto& A, const auto& B) { return A.second < B.second; });
        
        vector<const drawable*> SortedDrawables;
        SortedDrawables.reserve(Drawables.size());
        for (const auto& [Index, Depth] : Indices)
        {
            SortedDrawables.push_back(Drawables[Index]);
        }
        
        Drawables = std::move(SortedDrawables);
        return Drawables;
    }
}