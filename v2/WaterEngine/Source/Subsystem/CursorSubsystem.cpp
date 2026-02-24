// =============================================================================
// Water Engine v2.0.0
// Copyright (C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Subsystem/CameraSubsystem.h"
#include "Subsystem/InputSubsystem.h"
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
			
			vec2f Delta = NormalizedDir * CursorSpeed * ScaledT * DeltaTime;
			CursorSprite.move(Delta);

			// Clamp to window bounds
			vec2f WindowSize = GetWindowSize();
			vec2f ClampedPos = {
				Clamp(CursorSprite.getPosition().x, 0.0f, WindowSize.x),
				Clamp(CursorSprite.getPosition().y, 0.0f, WindowSize.y)
			};
			CursorSprite.setPosition(ClampedPos);
		}
	}

	void CursorSubsystem::Render(RenderSubsystem& Renderer, const CameraSubsystem* Camera) const
	{
		if (!bIsVisible)
			return;

		// Draw cursor sprite
		Renderer.Draw(CursorSprite, ERenderLayer::Cursor);

		// Draw debug text for mouse position
		RenderDebugText(Renderer, Camera);
	}

	void CursorSubsystem::RenderDebugText(RenderSubsystem& Renderer, const CameraSubsystem* Camera) const
	{
		// Lazy-load debug font
		if (!DebugFont)
		{
			DebugFont = LoadAsset().LoadFontSync(EC.DefaultFont);
		}

		if (!DebugFont)
			return;

		// Get window position
		vec2f WindowPos = GetPosition();

		// Get world position if camera is available
		vec2f WorldPos = WindowPos;
		if (Camera)
		{
			WorldPos = GetWorldPosition(*Camera);
		}

		// Format: "Mouse: 1234x567 | World: 1234x567" (no decimals)
		string Content = std::format("Mouse: {:.0f}x{:.0f} | World: {:.0f}x{:.0f}", 
			WindowPos.x, WindowPos.y, WorldPos.x, WorldPos.y);

		// Use render resolution for positioning (works in both windowed and fullscreen)
		// Position at top-right with padding
		vec2f TextPos = { EC.RenderResolution.x - 10.0f, 10.0f };

		text TextObj(*DebugFont, Content, 16);
		TextObj.setPosition(TextPos);
		TextObj.setFillColor(color::Red);
		
		// Right-align the text
		TextObj.setOrigin({ TextObj.getLocalBounds().size.x, 0.0f });
		
		Renderer.Draw(TextObj, ERenderLayer::ScreenUI);
	}

	void CursorSubsystem::ApplyCursorSize()
	{
		vec2f textureSize = vec2f(CursorTexture.getSize());
		CursorSprite.setScale(CursorSize.componentWiseDiv(textureSize));
	}

	vec2f CursorSubsystem::GetWindowSize() const
	{
		return vec2f(Config.Window.getSize());
	}

	void CursorSubsystem::CenterCursor()
	{
		vec2f Center = GetWindowSize() / 2.0f;
		CursorSprite.setPosition(Center);
	}

	void CursorSubsystem::SetVisibility(bool Visible)
	{
		if (bIsVisible == Visible)
			return;

		bIsVisible = Visible;

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

	void CursorSubsystem::UpdateFromMouse(vec2f MousePosition)
	{
		if (MousePosition != LastMousePosition)
		{
			LastMousePosition = MousePosition;
			SetPosition(MousePosition);
		}
	}

	vec2f CursorSubsystem::GetPosition() const
	{
		return CursorSprite.getPosition();
	}

	vec2f CursorSubsystem::GetWorldPosition(const CameraSubsystem& Camera) const
	{
		CameraView View;
		if (!Camera.GetCurrentView(View))
		{
			return CursorSprite.getPosition();
		}

		float AspectRatio = EC.AspectRatio.x / EC.AspectRatio.y;
		vec2f ViewSize = View.GetViewSize(AspectRatio);

		vec2f PixelPos = CursorSprite.getPosition();
		vec2f WindowSize = GetWindowSize();

		float NormalizedX = (PixelPos.x / WindowSize.x) * 2.0f - 1.0f;
		float NormalizedY = (PixelPos.y / WindowSize.y) * 2.0f - 1.0f;

		vec2f WorldPos;
		WorldPos.x = View.Position.x + NormalizedX * ViewSize.x * 0.5f;
		WorldPos.y = View.Position.y + NormalizedY * ViewSize.y * 0.5f;

		return WorldPos;
	}
}
