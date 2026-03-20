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

	class Camera : public Actor
	{
	public:
		explicit Camera(World& InWorld);
		~Camera();

		void BeginPlay() override;
		void Tick(float DeltaTime) override;
		void EndPlay() override;

		CameraComponent* GetCameraComponent() const;

		void SetActive();
		bool IsActive() const;

	private:
		shared<CameraComponent> CamComp;
	};
}