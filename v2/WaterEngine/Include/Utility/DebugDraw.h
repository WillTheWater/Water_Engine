// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
    class RenderSubsystem;

    // Debug primitive types
    struct DebugLine
    {
        vec2f Start;
        vec2f End;
        color Color;
        float Thickness;
    };

    struct DebugCircle
    {
        vec2f Position;
        float Radius;
        color Color;
        float Thickness;
    };

    struct DebugRect
    {
        vec2f Position;  // Center
        vec2f HalfExtents;
        float Rotation;  // Radians
        color Color;
        float Thickness;
    };

    struct DebugCapsule
    {
        vec2f Position;  // Center
        float HalfHeight;
        float Radius;
        color Color;
        float Thickness;
    };

    struct DebugText
    {
        string Content;
        vec2f Position;  // Screen-space position (pixels from top-left)
        color Color;
        uint CharacterSize;
    };

    // Debug drawing system - renders wireframe primitives on top of game world
    // Thread-safe accumulation, automatically cleared after rendering
    class DebugDraw
    {
    public:
        // Enable/disable debug drawing (can be toggled at runtime)
        static void SetEnabled(bool bEnabled) { bIsEnabled = bEnabled; }
        static bool IsEnabled() { return bIsEnabled; }

        // Queue primitives for rendering this frame
        static void Line(const vec2f& Start, const vec2f& End, const color& Color = color::Red, float Thickness = 2.0f);
        static void Circle(const vec2f& Position, float Radius, const color& Color = color::Red, float Thickness = 2.0f);
        static void Rect(const vec2f& Position, const vec2f& HalfExtents, float Rotation, const color& Color = color::Red, float Thickness = 2.0f);
        static void Capsule(const vec2f& Position, float HalfHeight, float Radius, const color& Color = color::Red, float Thickness = 2.0f);

        // Screen-space text (position in pixels from top-left)
        static void Text(const string& Content, vec2f Position, const color& Color = color::White, uint CharacterSize = 16);

        // Draw mouse position at top-right of screen (call every frame to update)
        static void DrawMousePosition(vec2f MousePos, vec2f WindowSize, const color& Color = color::Yellow);

        // Render all queued primitives and clear them (single call, auto-clears)
        static void Render(RenderSubsystem& Render);

        // Explicit clear (rarely needed since Render() auto-clears)
        static void Clear();

    private:
        static vector<DebugLine> Lines;
        static vector<DebugCircle> Circles;
        static vector<DebugRect> Rects;
        static vector<DebugCapsule> Capsules;
        static vector<DebugText> Texts;
        static bool bIsEnabled;
        static shared<font> DebugFont;
    };
}
