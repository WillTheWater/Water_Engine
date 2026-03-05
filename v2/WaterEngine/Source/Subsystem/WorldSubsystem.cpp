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
            CurrentWorld->StartTick(DeltaTime);
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
        
        // Get actors from world
        const auto& Actors = CurrentWorld->GetActors();
        
        // Collect visible indices with depths
        for (uint i = 0; i < Actors.size(); ++i)
        {
            const auto& Actor = Actors[i];
            if (Actor->IsVisible() && Actor->GetDrawable())
            {
                Indices.emplace_back(i, Actor->GetRenderDepth());
            }
        }
        
        // Sort by depth
        std::sort(Indices.begin(), Indices.end(),
            [](const auto& A, const auto& B) { return A.second < B.second; });
        
        // Fill drawables in sorted order
        Drawables.reserve(Indices.size());
        for (const auto& [Index, Depth] : Indices)
        {
            Drawables.push_back(Actors[Index]->GetDrawable());
        }
        
        return Drawables;
    }
}