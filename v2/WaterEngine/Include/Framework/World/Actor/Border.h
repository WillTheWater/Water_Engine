// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor/Actor.h"

class b2Body;
class b2Fixture;

namespace we
{
    // =========================================================================
    // Border - Static collision boundaries for level walls, floors, etc.
    //
    // Uses Box2D chain shapes for efficient static collision.
    // Points are defined in world space (or local to the border actor).
    //
    // Usage:
    //   auto Border = World->SpawnActor<Border>(
    //       std::vector<vec2f>{ {0,0}, {100,0}, {100,100}, {0,100} }
    //   );
    //   Border->SetClosedLoop(true);  // Connects last point back to first
    // =========================================================================
    class Border : public Actor
    {
    public:
        // Points should be in clockwise or counter-clockwise order
        Border(World* OwningWorld, const vector<vec2f>& Points);
        virtual ~Border();

        virtual void BeginPlay() override;
        virtual void Tick(float DeltaTime) override;
        virtual void Destroy() override;

        // If true, connects the last point back to the first (for enclosed areas)
        void SetClosedLoop(bool bClosed);
        bool IsClosedLoop() const { return bIsClosedLoop; }

        // Modify points (will recreate physics on next tick or immediately if playing)
        void SetPoints(const vector<vec2f>& Points);
        const vector<vec2f>& GetPoints() const { return Points; }

        // Visual properties (for debug drawing)
        void SetDebugColor(color InColor) { DebugColor = InColor; }
        color GetDebugColor() const { return DebugColor; }

    private:
        void CreatePhysics();
        void DestroyPhysics();
        void DrawDebug();

    private:
        vector<vec2f> Points;
        bool bIsClosedLoop = false;
        bool bPhysicsCreated = false;

        // Box2D objects (owned by this border)
        b2Body* Body = nullptr;
        b2Fixture* Fixture = nullptr;

        color DebugColor{ 255, 255, 0 };  // Yellow default
    };
} // namespace we
