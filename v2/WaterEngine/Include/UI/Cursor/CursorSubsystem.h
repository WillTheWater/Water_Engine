// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

namespace we
{
    class GameWindow;

    class CursorSubsystem
    {
    public:
        CursorSubsystem(GameWindow& Window);

        void Update(float DeltaTime);
        void Render() const;

        void SetSpeed(float Speed);
        float GetSpeed() const;

        void SetVisibility(bool Visible);
        bool IsVisible() const;

        void SetPosition(vec2f Position);
        vec2f GetPosition() const;
        vec2f GetPosition(const view& View) const;

    private:
        GameWindow& Window;
        rectangle CursorShape;
        shared<texture> CursorTexture;
        float CursorSpeed;
        bool bIsVisible;

    private:
        void InitializeCursor();
    };
}