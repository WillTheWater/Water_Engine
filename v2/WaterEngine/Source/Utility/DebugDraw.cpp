// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Utility/DebugDraw.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include <cmath>
#include <format>

namespace we
{
    vector<DebugLine> DebugDraw::Lines;
    vector<DebugCircle> DebugDraw::Circles;
    vector<DebugRect> DebugDraw::Rects;
    vector<DebugCapsule> DebugDraw::Capsules;
    vector<DebugText> DebugDraw::Texts;
    bool DebugDraw::bIsEnabled = true;
    shared<font> DebugDraw::DebugFont = nullptr;

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

    void DebugDraw::Capsule(const vec2f& Position, float HalfHeight, float Radius, const color& Color, float Thickness)
    {
        if (!bIsEnabled) return;
        Capsules.push_back({ Position, HalfHeight, Radius, Color, Thickness });
    }

    void DebugDraw::Text(const string& Content, vec2f Position, const color& Color, uint CharacterSize)
    {
        if (!bIsEnabled) return;
        Texts.push_back({ Content, Position, Color, CharacterSize });
    }

    void DebugDraw::DrawMousePosition(vec2f MousePos, vec2f WindowSize, const color& Color)
    {
        if (!bIsEnabled) return;

        // Format: "Mouse Pos: 1234x567" (no decimals)
        string Content = std::format("Mouse Pos: {:.0f}x{:.0f}", MousePos.x, MousePos.y);

        // Position at top-right with some padding
        vec2f Position = { WindowSize.x - 200.0f, 10.0f };

        Texts.push_back({ Content, Position, Color, 16 });
    }

    void DebugDraw::Render(RenderSubsystem& Render)
    {
        if (!bIsEnabled) {
            Clear();  // Discard any accumulated primitives when disabled
            return;
        }

        // Render lines (as thin rectangles)
        for (const auto& L : Lines)
        {
            vec2f Delta = L.End - L.Start;
            float Length = std::sqrt(Delta.x * Delta.x + Delta.y * Delta.y);
            
            if (Length < 0.001f) continue;
            
            // Create a rectangle for the line
            rectangle LineRect({ Length, L.Thickness });
            LineRect.setPosition((L.Start + L.End) * 0.5f);
            LineRect.setOrigin({ Length * 0.5f, L.Thickness * 0.5f });
            LineRect.setRotation(sf::radians(std::atan2(Delta.y, Delta.x)));
            LineRect.setFillColor(L.Color);
            Render.Draw(LineRect, ERenderLayer::World);
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
            Render.Draw(Shape, ERenderLayer::World);
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
            Render.Draw(Shape, ERenderLayer::World);
        }

        // Render capsules (as two circles + connecting lines)
        for (const auto& C : Capsules)
        {
            vec2f TopCenter = C.Position + vec2f{ 0.0f, -C.HalfHeight };
            vec2f BottomCenter = C.Position + vec2f{ 0.0f, C.HalfHeight };

            // Top cap (semicircle approximated with circle for now)
            circle TopCap(C.Radius);
            TopCap.setPosition(TopCenter);
            TopCap.setOrigin({ C.Radius, C.Radius });
            TopCap.setFillColor(color::Transparent);
            TopCap.setOutlineColor(C.Color);
            TopCap.setOutlineThickness(C.Thickness);
            Render.Draw(TopCap, ERenderLayer::World);

            // Bottom cap
            circle BottomCap(C.Radius);
            BottomCap.setPosition(BottomCenter);
            BottomCap.setOrigin({ C.Radius, C.Radius });
            BottomCap.setFillColor(color::Transparent);
            BottomCap.setOutlineColor(C.Color);
            BottomCap.setOutlineThickness(C.Thickness);
            Render.Draw(BottomCap, ERenderLayer::World);

            // Connecting lines (left and right sides)
            float LineLength = C.HalfHeight * 2.0f;
            
            // Left side - origin must be centered so line extends equally up/down
            rectangle LeftLine({ C.Thickness, LineLength });
            LeftLine.setOrigin({ C.Thickness / 2.0f, LineLength / 2.0f });
            LeftLine.setPosition(C.Position + vec2f{ -C.Radius, 0.0f });
            LeftLine.setFillColor(C.Color);
            Render.Draw(LeftLine, ERenderLayer::World);

            // Right side
            rectangle RightLine({ C.Thickness, LineLength });
            RightLine.setOrigin({ C.Thickness / 2.0f, LineLength / 2.0f });
            RightLine.setPosition(C.Position + vec2f{ C.Radius, 0.0f });
            RightLine.setFillColor(C.Color);
            Render.Draw(RightLine, ERenderLayer::World);
        }

        // Render text (screen-space, use default view)
        if (!Texts.empty())
        {
            // Lazy-load debug font if needed
            if (!DebugFont)
            {
                DebugFont = LoadAsset().LoadFontSync("Assets/Font/Default/defaultFont.otf");
            }

            if (DebugFont)
            {
                for (const auto& T : Texts)
                {
                    text TextObj(*DebugFont, T.Content, T.CharacterSize);
                    TextObj.setPosition(T.Position);
                    TextObj.setFillColor(T.Color);
                    Render.Draw(TextObj, ERenderLayer::ScreenUI);
                }
            }
        }

        // Auto-clear after rendering (no manual Clear() needed)
        Clear();
    }

    void DebugDraw::Clear()
    {
        Lines.clear();
        Circles.clear();
        Rects.clear();
        Capsules.clear();
        Texts.clear();
    }
}
