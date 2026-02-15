// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Cursor/CursorSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
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
            sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 100.f,
            sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) / 100.f);

        if (JoystickDirection.length() > EC.JoystickDeadzone)
        {
            CursorShape.move(JoystickDirection * CursorSpeed * DeltaTime);

            sf::Vector2f NewPos = {
                std::clamp(CursorShape.getPosition().x, 0.f, EC.WindowSize.x - 1),
                std::clamp(CursorShape.getPosition().y, 0.f, EC.WindowSize.y - 1)
            };

            CursorShape.setPosition(NewPos);
            SetPosition(NewPos);
        }
    }

    void CursorSubsystem::Render() const
    {
        if (!bIsVisible) return;
        
        //Window.draw(CursorShape);
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

    void CursorSubsystem::EventUpdatePosition(vec2f Position)
    {
        CursorShape.setPosition(Position);
    }

    void CursorSubsystem::SetPosition(vec2f Position)
    {
       // sf::Mouse::setPosition(Window.mapCoordsToPixel(Position, Window.getDefaultView()), Window);
        CursorShape.setPosition(Position);
    }

    vec2f CursorSubsystem::GetPosition() const
    {
        //return GetPosition(Window.getDefaultView());
        return vec2f{ 0,0 };
    }

    vec2f CursorSubsystem::GetPosition(const view& View) const
    {
        //return Window.mapPixelToCoords(sf::Mouse::getPosition(Window), View);
        return vec2f{ 0,0 };
    }
}