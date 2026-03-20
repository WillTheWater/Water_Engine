// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor.h"

namespace we
{
	class CameraComponent;

	// =========================================================================
	// Camera - An actor that provides a camera view
	// Simple wrapper around CameraComponent for easy placement in levels
	// =========================================================================
	class Camera : public Actor
	{
	public:
		explicit Camera(World& InWorld);
		~Camera();

		// Lifecycle
		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;

		// Get the camera component
		CameraComponent* GetCameraComponent() const;

		// Convenience: Set as active camera
		void SetActive();
		bool IsActive() const;

	private:
		shared<CameraComponent> CamComp;
	};
}
