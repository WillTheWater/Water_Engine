// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Cursor/CursorSubsystem.h"
#include "Framework/GameWindow.h"
#include "Subsystem/ResourceSubsystem.h"
#include "EngineConfig.h"

namespace we
{
    CursorSubsystem::CursorSubsystem(GameWindow& Window)
        : Window(Window)
        , CursorShape(EC.DefaultCursorSize)
        , CursorSpeed(EC.DefaultCursorSpeed)
        , bIsVisible(true)
    {
        InitializeCursor();
    }

    void CursorSubsystem::InitializeCursor()
    {
        CursorShape.setFillColor(sf::Color::White);
        CursorTexture = Asset().LoadTexture(EC.DefaultCursor);
        CursorShape.setTexture(CursorTexture.get());
        CursorShape.setPosition(EC.WindowSize / 2.f);
    }

    void CursorSubsystem::Update(float DeltaTime)
    {
        const sf::Vector2f JoystickDirection(
            sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 100,
            sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) / 100);

        if (JoystickDirection.length() > EC.JoystickDeadzone)
        {
            CursorShape.move(JoystickDirection * CursorSpeed * DeltaTime);
            CursorShape.setPosition(
                {
                    std::clamp(CursorShape.getPosition().x, 0.f,
                    EC.WindowSize.x - 1),
                    std::clamp(CursorShape.getPosition().y, 0.f,
                    EC.WindowSize.y - 1)
                });
            SetPosition(CursorShape.getPosition());
        }
        else
        {
            CursorShape.setPosition(GetPosition());
        }
    }

    void CursorSubsystem::Render() const
    {
        if (!bIsVisible) return;
        
        Window.draw(CursorShape);
    }

    void CursorSubsystem::SetSpeed(float Speed)
    {
        CursorSpeed = Speed;
    }

    float CursorSubsystem::GetSpeed() const
    {
        return CursorSpeed;
    }

    void CursorSubsystem::SetVisibility(bool Visible)
    {
        bIsVisible = Visible;
    }

    bool CursorSubsystem::IsVisible() const
    {
        return bIsVisible;
    }

    void CursorSubsystem::SetPosition(vec2f Position)
    {
        sf::Mouse::setPosition(Window.mapCoordsToPixel(Position, Window.getDefaultView()), Window);
    }

    vec2f CursorSubsystem::GetPosition() const
    {
        return GetPosition(Window.getDefaultView());
    }

    vec2f CursorSubsystem::GetPosition(const view& View) const
    {
        return Window.mapPixelToCoords(sf::Mouse::getPosition(Window), View);
    }
}