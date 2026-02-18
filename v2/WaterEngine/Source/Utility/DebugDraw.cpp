// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Utility/DebugDraw.h"
#include "Subsystem/RenderSubsystem.h"

namespace we
{
    vector<DebugLine> DebugDraw::Lines;
    vector<DebugCircle> DebugDraw::Circles;
    vector<DebugRect> DebugDraw::Rects;
    bool DebugDraw::bIsEnabled = true;

    void DebugDraw::Line(const vec2f& Start, const vec2f& End, const color& Color, float Thickness)
    {
        if (!bIsEnabled) return;
        Lines.push_back({ Start, End, Color, Thickness });
    }

    void DebugDraw::Circle(const vec2f& Position, float Radius, const color& Color, float Thickness)
    {
        if (!bIsEnabled) return;
        Circles.push_back({ Position, Radius, Color, Thickness });
    }

    void DebugDraw::Rect(const vec2f& Position, const vec2f& HalfExtents, float Rotation, const color& Color, float Thickness)
    {
        if (!bIsEnabled) return;
        Rects.push_back({ Position, HalfExtents, Rotation, Color, Thickness });
    }

    void DebugDraw::Render(RenderSubsystem& Render)
    {
        if (!bIsEnabled) {
            Clear();  // Discard any accumulated primitives when disabled
            return;
        }

        // Render circles
        for (const auto& C : Circles)
        {
            circle Shape(C.Radius);
            Shape.setPosition(C.Position);
            Shape.setOrigin({ C.Radius, C.Radius });
            Shape.setFillColor(color::Transparent);
            Shape.setOutlineColor(C.Color);
            Shape.setOutlineThickness(C.Thickness);
            Render.Draw(Shape, ERenderLayer::Game);
        }

        // Render rects
        for (const auto& R : Rects)
        {
            rectangle Shape({ R.HalfExtents.x * 2.0f, R.HalfExtents.y * 2.0f });
            Shape.setPosition(R.Position);
            Shape.setRotation(sf::radians(R.Rotation));
            Shape.setOrigin({ R.HalfExtents.x, R.HalfExtents.y });
            Shape.setFillColor(color::Transparent);
            Shape.setOutlineColor(R.Color);
            Shape.setOutlineThickness(R.Thickness);
            Render.Draw(Shape, ERenderLayer::Game);
        }

        // Auto-clear after rendering (no manual Clear() needed)
        Clear();
    }

    void DebugDraw::Clear()
    {
        Lines.clear();
        Circles.clear();
        Rects.clear();
    }
}
