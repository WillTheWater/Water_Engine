// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	// =========================================================================
	// CameraView - Output of a camera calculation, consumed by the renderer
	// =========================================================================
	struct CameraView
	{
		vec2f Position = { 0.0f, 0.0f };  // Center of view in world space
		float Rotation = 0.0f;            // Rotation in radians
		float OrthographicSize = 540.0f;  // Half-height in world units (1080p/2 default)
		float Zoom = 1.0f;                // 1.0 = normal, 2.0 = 2x zoomed in

		// Calculate final view size with zoom applied
		vec2f GetViewSize(float AspectRatio) const
		{
			float Height = (OrthographicSize * 2.0f) / Zoom;
			float Width = Height * AspectRatio;
			return { Width, Height };
		}
	};

	// =========================================================================
	// ICamera - Interface for anything that can provide a camera view
	// Implemented by CameraActor or CameraComponent (future)
	// =========================================================================
	class ICamera
	{
	public:
		virtual ~ICamera() = default;
		virtual CameraView CalculateView() const = 0;
	};
}
