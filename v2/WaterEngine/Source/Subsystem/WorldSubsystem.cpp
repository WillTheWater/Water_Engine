// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/WorldSubsystem.h"
#include "Framework/World/World.h"

namespace we
{
	void WorldSubsystem::Tick(float DeltaTime)
	{
        if (PendingWorld)
        {
            if (CurrentWorld)
            {
                CurrentWorld->EndPlay();
            }

            CurrentWorld = PendingWorld;
            PendingWorld = nullptr;
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
        
        Drawables.clear();
        Indices.clear();
        
        if (!CurrentWorld)
            return Drawables;
        
        const auto& Actors = CurrentWorld->GetActors();
        
        for (uint i = 0; i < Actors.size(); ++i)
        {
            const auto& Actor = Actors[i];
            if (!Actor->IsPendingDestroy() && Actor->IsVisible() && Actor->GetDrawable())
            {
                Indices.emplace_back(i, Actor->GetRenderDepth());
            }
        }
        
        std::sort(Indices.begin(), Indices.end(),
            [](const auto& A, const auto& B) { return A.second < B.second; });
        
        Drawables.reserve(Indices.size());
        for (const auto& [Index, Depth] : Indices)
        {
            Drawables.push_back(Actors[Index]->GetDrawable());
        }
        
        return Drawables;
    }
}