// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/ICamera.h"
#include <optional>

namespace we
{
	// =========================================================================
	// CameraSubsystem - Manages the active camera and provides view to renderer
	// =========================================================================
	class CameraSubsystem
	{
	public:
		CameraSubsystem() = default;
		~CameraSubsystem() = default;

		// Set active camera - stores weak_ptr (owner is Actor/Component)
		void SetActiveCamera(weak<ICamera> Camera) { Active = Camera; }
		void ClearActiveCamera() { Active.reset(); }

		// Check if we have a valid camera
		bool HasActiveCamera() const;

		// Get current view - returns nullopt if no valid camera
		optional<CameraView> GetCurrentView() const;

	private:
		weak<ICamera> Active;
	};
}
