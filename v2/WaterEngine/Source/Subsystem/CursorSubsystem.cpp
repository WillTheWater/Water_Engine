// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "EngineConfig.h"

namespace we
{
	CursorSubsystem::CursorSubsystem()
		: CursorShape(EC.DefaultCursorSize)
		, CursorSpeed(EC.DefaultCursorSpeed)
		, bIsVisible(true)
	{
		InitializeCursor();
	}

	void CursorSubsystem::InitializeCursor()
	{
		// Default visual state since Resource Subsystem is unhooked
		CursorShape.setFillColor(color::White);
		CursorShape.setOutlineColor(color::Black);
		CursorShape.setOutlineThickness(1.0f);

		// Set starting position to center of logical resolution
		CursorShape.setPosition(vec2f(EC.RenderResolution) / 2.0f);
	}

	void CursorSubsystem::Update(float DeltaTime)
	{
		// Joystick Logic
		const vec2f JoystickDirection(
			sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 100.0f,
			sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) / 100.0f);

		if (JoystickDirection.length() > EC.JoystickDeadzone)
		{
			CursorShape.move(JoystickDirection * CursorSpeed * DeltaTime);

			// Clamp to the logical render resolution (internal pixels)
			vec2f ClampedPos = {
				std::clamp(CursorShape.getPosition().x, 0.0f, (float)EC.RenderResolution.x),
				std::clamp(CursorShape.getPosition().y, 0.0f, (float)EC.RenderResolution.y)
			};
			CursorShape.setPosition(ClampedPos);
		}
	}

	void CursorSubsystem::Render(RenderSubsystem& Renderer) const
	{
		if (bIsVisible)
		{
			// Pushes the shape to the dedicated Cursor layer in the renderer
			Renderer.Draw(CursorShape, ERenderLayer::Cursor);
		}
	}

	void CursorSubsystem::SetPosition(vec2f Position)
	{
		CursorShape.setPosition(Position);
	}

	vec2f CursorSubsystem::GetPosition() const
	{
		return CursorShape.getPosition();
	}
}