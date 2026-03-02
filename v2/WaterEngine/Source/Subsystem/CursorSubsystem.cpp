// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CursorSubsystem.h"

namespace we
{
	CursorSubsytem::CursorSubsytem(const EngineConfig::CursorConfig& Config)
		: Config{Config}
		, CursorSize{Config.CursorSize}
		, CursorSpeed{Config.CursorSpeed}
		, bIsVisible{true}
	{

	}

	void CursorSubsytem::Update(float DeltaTime)
	{

	}

	void CursorSubsytem::SetSpeed(float Speed)
	{
		CursorSpeed = Speed; 
	}

	void CursorSubsytem::SetVisibility(bool Visible)
	{
		bIsVisible = Visible;
	}

	void CursorSubsytem::SetCursorSize(vec2f Size)
	{
		CursorSize = Size;
	}

	void CursorSubsytem::SetPosition(vec2f Position)
	{

	}

	vec2f CursorSubsytem::GetPosition() const
	{
		return vec2f{ 0.f,0.f };
	}

	void CursorSubsytem::UpdateFromMouse(vec2f MousePosition)
	{

	}

	void CursorSubsytem::ApplyCursorSize()
	{

	}

	void CursorSubsytem::CenterCursor()
	{

	}

	vec2f CursorSubsytem::GetWindowSize() const
	{
		return vec2f{ 0.f,0.f };
	}
}