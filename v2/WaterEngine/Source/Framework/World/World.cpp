// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/World.h"
#include "Framework/World/Actor.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Utility/Math.h"
#include "Utility/Log.h"

namespace we
{
    World::World(EngineSubsystem& InSubsystem)
        : Object()
        , Subsystem{ InSubsystem }
    {
    }

    World::~World()
    {
        Actors.clear();
        PendingActors.clear();
    }

    void World::BeginPlayGlobal()
    {
        if (!bHasBegunPlay)
        {
            bHasBegunPlay = true;
            LOG("World BeginPlay");
            BeginPlay();
        }
    }

    void World::EndPlayGlobal()
    {
        if (bHasBegunPlay)
        {
            bHasBegunPlay = false;
            LOG("World EndPlay");
            EndPlay();
        }
    }

    void World::TickGlobal(float DeltaTime)
    {
        ManageActors(DeltaTime);
        Tick(DeltaTime);
    }

    void World::Render()
    {
        // Ensure pending actors are added before rendering
        // (In edit mode without tick, actors would stay pending otherwise)
        if (!PendingActors.empty())
        {
            bRenderOrderDirty = true;
            for (auto& NewActor : PendingActors)
            {
                Actors.push_back(NewActor);
                if (!NewActor->HasBegunPlay())
                {
                    NewActor->BeginPlay();
                }
            }
            PendingActors.clear();
        }

        vector<RenderDepth> Depths;
        CollectRenderDepths(Depths);

        for (const auto& RD : Depths)
        {
            if (RD.Drawable)
            {
                Subsystem.Render->Draw(*RD.Drawable, ERenderLayer::World);
            }
        }
    }

    void World::ManageActors(float DeltaTime)
    {
        // Spawn pending actors
        if (!PendingActors.empty())
        {
            bRenderOrderDirty = true;
            for (auto& NewActor : PendingActors)
            {
                Actors.push_back(NewActor);
                if (!NewActor->HasBegunPlay())
                {
                    NewActor->BeginPlay();
                }
            }
            PendingActors.clear();
        }

        // Tick actors and remove destroyed
        for (auto It = Actors.begin(); It != Actors.end();)
        {
            if ((*It)->IsPendingDestroy())
            {
                It = Actors.erase(It);
                bRenderOrderDirty = true;
            }
            else
            {
                if ((*It)->HasBegunPlay())
                {
                    vec2f OldPos = (*It)->GetActorPosition();
                    (*It)->Tick(DeltaTime);
                    
                    // Mark dirty if actor moved in Y
                    if (!FloatEquals((*It)->GetActorPosition().y, OldPos.y))
                    {
                        bRenderOrderDirty = true;
                    }
                }
                ++It;
            }
        }
    }

    void World::CollectRenderDepths(vector<RenderDepth>& OutDepths)
    {
        if (bRenderOrderDirty)
        {
            CachedRenderDepths.clear();

            // Add manual drawables first
            for (const auto& RD : ManualRenderDepths)
            {
                CachedRenderDepths.push_back(RD);
            }

            // Add actors
            for (const auto& Actor : Actors)
            {
                if (Actor && !Actor->IsPendingDestroy() && Actor->IsVisible())
                {
                    if (const auto* Drawable = Actor->GetDrawable())
                    {
                        CachedRenderDepths.push_back({ Drawable, Actor->GetRenderDepth() });
                    }
                }
            }

            // Sort by depth (back to front)
            std::sort(CachedRenderDepths.begin(), CachedRenderDepths.end());
            bRenderOrderDirty = false;
        }

        OutDepths = CachedRenderDepths;
    }

    void World::AddRenderDepth(const drawable* Drawable, float Depth)
    {
        ManualRenderDepths.push_back({ Drawable, Depth });
        bRenderOrderDirty = true;
    }

    void World::ClearManualRenderDepths()
    {
        if (!ManualRenderDepths.empty())
        {
            ManualRenderDepths.clear();
            bRenderOrderDirty = true;
        }
    }

    void World::PreConstruct() {}
    void World::BeginPlay() {}
    void World::EndPlay() {}
    void World::Tick(float DeltaTime) {}
}
