// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CursorSubsystem.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "EngineConfig.h"

namespace we
{
	CursorSubsystem::CursorSubsystem()
		: CursorTexture{ *LoadAsset().LoadTextureSync(EC.DefaultCursor) }
		, CursorSprite(CursorTexture)
		, CursorSize{EC.DefaultCursorSize}
		, CursorSpeed(EC.DefaultCursorSpeed)
		, bIsVisible(true)
	{
		ApplyCursorSize();
	}

	void CursorSubsystem::Update(float DeltaTime)
	{
		const vec2f JoystickDirection(
			sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 100.0f,
			sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) / 100.0f);

		if (JoystickDirection.length() > EC.JoystickDeadzone)
		{
			CursorSprite.move(JoystickDirection * CursorSpeed * DeltaTime);

			vec2f ClampedPos = {
				std::clamp(CursorSprite.getPosition().x, 0.0f, (float)EC.RenderResolution.x),
				std::clamp(CursorSprite.getPosition().y, 0.0f, (float)EC.RenderResolution.y)
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