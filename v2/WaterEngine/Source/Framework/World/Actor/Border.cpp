// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor/Border.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Interface/Component/IPhysicsComponent.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Utility/DebugDraw.h"
#include "Utility/Log.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

#include <cmath>

namespace we
{
    Border::Border(World* OwningWorld, const vector<vec2f>& InPoints)
        : Actor(OwningWorld, "")  // Borders have no texture
        , Points(InPoints)
    {
        // Borders are invisible by default (no sprite rendering)
        SetVisible(false);
    }

    Border::~Border()
    {
        DestroyPhysics();
    }

    void Border::BeginPlay()
    {
        CreatePhysics();
        Actor::BeginPlay();
    }

    void Border::Tick(float DeltaTime)
    {
        // Debug draw the border if enabled
        if (DebugDraw::IsEnabled())
        {
            DrawDebug();
        }

        Actor::Tick(DeltaTime);
    }

    void Border::Destroy()
    {
        DestroyPhysics();
        Actor::Destroy();
    }

    void Border::SetClosedLoop(bool bClosed)
    {
        if (bIsClosedLoop != bClosed)
        {
            bIsClosedLoop = bClosed;
            // Recreate physics with new loop setting
            if (bPhysicsCreated)
            {
                DestroyPhysics();
                CreatePhysics();
            }
        }
    }

    void Border::SetPoints(const vector<vec2f>& InPoints)
    {
        Points = InPoints;
        if (bPhysicsCreated)
        {
            DestroyPhysics();
            CreatePhysics();
        }
    }

    void Border::CreatePhysics()
    {
        if (Points.size() < 2) return;

        auto& Physics = *GetWorld()->GetSubsystem().Physics;
        float Scale = Physics.GetPhysicsScale();

        // Create static body for the border
        vec2f Pos = GetPosition();
        Body = Physics.CreateBody(nullptr, BodyType::Static, Pos, 0.0f);

        if (!Body) return;

        // Create thin box shapes for two-sided collision
        // Each segment becomes a rotated box (4px thick)
        const float Thickness = 4.0f * Scale;  // 4 pixels thick
        
        std::vector<b2Vec2> Vertices;
        Vertices.reserve(Points.size());
        
        for (const auto& P : Points)
        {
            Vertices.push_back(b2Vec2{ P.x * Scale, P.y * Scale });
        }

        size_t SegmentCount = bIsClosedLoop ? Vertices.size() : Vertices.size() - 1;
        
        for (size_t i = 0; i < SegmentCount; ++i)
        {
            b2Vec2 V1 = Vertices[i];
            b2Vec2 V2 = Vertices[(i + 1) % Vertices.size()];
            
            // Calculate center and half-length of segment
            b2Vec2 Center = b2Vec2{ (V1.x + V2.x) * 0.5f, (V1.y + V2.y) * 0.5f };
            float Length = b2Distance(V1, V2);
            float Angle = std::atan2(V2.y - V1.y, V2.x - V1.x);
            
            // Create thin box centered on the segment
            b2PolygonShape Box;
            Box.SetAsBox(Length * 0.5f, Thickness * 0.5f, Center, Angle);
            
            b2FixtureDef Def;
            Def.shape = &Box;
            Def.density = 0.0f;
            Def.friction = 0.3f;
            Def.restitution = 0.0f;
            Def.isSensor = false;
            
            Body->CreateFixture(&Def);
        }
        bPhysicsCreated = true;

        LOG("Border created with {} points (closed={})", Points.size(), bIsClosedLoop);
    }

    void Border::DestroyPhysics()
    {
        if (Body && GetWorld())
        {
            GetWorld()->GetSubsystem().Physics->DestroyBody(Body);
            Body = nullptr;
            Fixture = nullptr;
            bPhysicsCreated = false;
        }
    }

    void Border::DrawDebug()
    {
        if (Points.size() < 2) return;

        vec2f ActorPos = GetPosition();

        // Draw lines between each point
        for (size_t i = 0; i < Points.size() - 1; ++i)
        {
            vec2f Start = ActorPos + Points[i];
            vec2f End = ActorPos + Points[i + 1];
            DebugDraw::Line(Start, End, DebugColor, 2.0f);
        }

        // If closed loop, draw line from last back to first
        if (bIsClosedLoop && Points.size() >= 3)
        {
            vec2f Start = ActorPos + Points.back();
            vec2f End = ActorPos + Points.front();
            DebugDraw::Line(Start, End, DebugColor, 2.0f);
        }
    }
} // namespace we
