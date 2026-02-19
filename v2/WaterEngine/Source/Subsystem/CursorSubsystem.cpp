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
		, Config{ InConfig }
	{
		ApplyCursorSize();
	}

	void CursorSubsystem::Update(float DeltaTime)
	{
		// Don't update position when cursor is not visible
		if (!bIsVisible)
			return;

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
			
			// Move the cursor sprite (in render coordinates)
			vec2f Delta = NormalizedDir * CursorSpeed * ScaledT * DeltaTime;
			CursorSprite.move(Delta);

			// Clamp to render resolution bounds
			vec2f ClampedPos = {
				Clamp(CursorSprite.getPosition().x, 0.0f, Config.RenderResolution.x),
				Clamp(CursorSprite.getPosition().y, 0.0f, Config.RenderResolution.y)
			};
			CursorSprite.setPosition(ClampedPos);
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

	void CursorSubsystem::CenterCursor()
	{
		vec2f Center = Config.RenderResolution / 2.0f;
		CursorSprite.setPosition(Center);
	}

	void CursorSubsystem::SetVisibility(bool Visible)
	{
		if (bIsVisible == Visible)
			return;

		bIsVisible = Visible;

		// When becoming visible, center the cursor on screen
		if (bIsVisible)
		{
			CenterCursor();
		}
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

	vec2f CursorSubsystem::GetPixelPosition() const
	{
		// Convert from render coordinates to window pixel coordinates
		vec2f Pos = CursorSprite.getPosition();
		vec2u WindowSize = Config.Window.getSize();
		
		float ScaleX = static_cast<float>(WindowSize.x) / Config.RenderResolution.x;
		float ScaleY = static_cast<float>(WindowSize.y) / Config.RenderResolution.y;
		
		return vec2f(Pos.x * ScaleX, Pos.y * ScaleY);
	}

	void CursorSubsystem::SetPixelPosition(vec2f PixelPos)
	{
		// Convert from window pixel coordinates to render coordinates
		vec2u WindowSize = Config.Window.getSize();
		
		float ScaleX = static_cast<float>(Config.RenderResolution.x) / WindowSize.x;
		float ScaleY = static_cast<float>(Config.RenderResolution.y) / WindowSize.y;
		
		vec2f RenderPos = vec2f(PixelPos.x * ScaleX, PixelPos.y * ScaleY);
		CursorSprite.setPosition(RenderPos);
	}
}
