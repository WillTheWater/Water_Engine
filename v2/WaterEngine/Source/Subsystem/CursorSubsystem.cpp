// =============================================================================
// Water Engine v2.0.0
// Copyright (C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Utility/Math.h"
#include "EngineConfig.h"
#include "Utility/Log.h"

namespace we
{
	CursorSubsystem::CursorSubsystem(const CursorConfig& InConfig)
		: CursorTexture{ *LoadAsset().LoadTextureSync(std::string(InConfig.DefaultCursor)) }
		, CursorSprite(CursorTexture)
		, CursorSize{ InConfig.DefaultCursorSize }
		, CursorSpeed{ InConfig.DefaultCursorSpeed }
		, bIsVisible{ true }
		, PixelPosition{ 0.f, 0.f }
		, Config{ InConfig }
	{
		ApplyCursorSize();
	}

	void CursorSubsystem::Update(float DeltaTime, WindowSubsystem& Window)
	{
		// Joystick control for cursor movement
		const float AxisX = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
		const float AxisY = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
		
		// Create joystick vector and calculate its length
		vec2f JoystickDirection(AxisX / 100.0f, AxisY / 100.0f);
		float InputLength = Length(JoystickDirection);
		
		// Only process if beyond deadzone
		if (InputLength > Config.JoystickDeadzone)
		{
			// Normalize the direction for consistent speed in all directions
			vec2f NormalizedDir = Normalize(JoystickDirection);
			
			// Calculate output speed: map [deadzone, 1.0] to [0.0, 1.0]
			float RawT = (InputLength - Config.JoystickDeadzone) / (1.0f - Config.JoystickDeadzone);
			float ScaledT = Clamp01(RawT);
			
			// Move the cursor sprite
			vec2f Delta = NormalizedDir * CursorSpeed * ScaledT * DeltaTime;
			CursorSprite.move(Delta);

			// Clamp to screen bounds
			vec2f ClampedPos = {
				Clamp(CursorSprite.getPosition().x, 0.0f, Config.RenderResolution.x),
				Clamp(CursorSprite.getPosition().y, 0.0f, Config.RenderResolution.y)
			};
			CursorSprite.setPosition(ClampedPos);

			// Sync system mouse cursor for UI hit-testing
			sf::Mouse::setPosition(static_cast<sf::Vector2i>(ClampedPos), Window);
		}
	}

	void CursorSubsystem::Render(RenderSubsystem& Renderer) const
	{
		if (bIsVisible)
		{
			Renderer.Draw(CursorSprite, ERenderLayer::Cursor);
		}
	}

	void CursorSubsystem::ApplyCursorSize()
	{
		vec2f textureSize = vec2f(CursorTexture.getSize());
		CursorSprite.setScale(CursorSize.componentWiseDiv(textureSize));
	}

	void CursorSubsystem::SetCursorSize(vec2f Size)
	{
		CursorSize = Size;
		ApplyCursorSize();
	}

	void CursorSubsystem::SetPosition(vec2f Position)
	{
		CursorSprite.setPosition(Position);
	}

	vec2f CursorSubsystem::GetPosition() const
	{
		return CursorSprite.getPosition();
	}
}
