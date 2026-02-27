// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/CameraSubsystem.h"

namespace we
{
	bool CameraSubsystem::HasActiveCamera() const
	{
		return !Active.expired();
	}

	optional<CameraView> CameraSubsystem::GetCurrentView() const
	{
		if (auto camera = Active.lock())
		{
			return camera->CalculateView();
		}
		return std::nullopt;
	}
}
