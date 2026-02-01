// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Cursor/Cursor.h"
#include "Framework/GameWindow.h"
#include "Subsystem/ResourceSubsystem.h"
#include "EngineConfig.h"

namespace we
{
    // CHANGED: Constructor takes InputSubsystem
    Cursor::Cursor(GameWindow& Window)
        : Window(Window)
        , CursorShape(EC.DefaultCursorSize)
        , CursorSpeed(EC.DefaultCursorSpeed)
        , bIsVisible(true)
    {
        InitializeCursor();
    }

    void Cursor::InitializeCursor()
    {
        CursorShape.setFillColor(sf::Color::White);
        CursorTexture = Asset().LoadTexture(EC.DefaultCursor);
        CursorShape.setTexture(CursorTexture.get());
        CursorShape.setPosition(EC.WindowSize / 2.f);
    }

    void Cursor::Update(float DeltaTime)
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

    void Cursor::Render() const
    {
        if (!bIsVisible) return;
        
        Window.draw(CursorShape);
    }

    void Cursor::SetSpeed(float Speed)
    {
        CursorSpeed = Speed;
    }

    float Cursor::GetSpeed() const
    {
        return CursorSpeed;
    }

    void Cursor::SetVisibility(bool Visible)
    {
        bIsVisible = Visible;
    }

    bool Cursor::IsVisible() const
    {
        return bIsVisible;
    }

    void Cursor::SetPosition(vec2f Position)
    {
        sf::Mouse::setPosition(Window.mapCoordsToPixel(Position, Window.getDefaultView()), Window);
    }

    vec2f Cursor::GetPosition() const
    {
        return GetPosition(Window.getDefaultView());
    }

    vec2f Cursor::GetPosition(const view& View) const
    {
        return Window.mapPixelToCoords(sf::Mouse::getPosition(Window), View);
    }
}