// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Actor.h"

class b2Body;
class b2Fixture;

namespace we
{
	class Barrier : public Actor
	{
	public:
		explicit Barrier(World& InWorld);
		~Barrier();

		// Configure before BeginPlay
		void SetPoints(const vector<vec2f>& InPoints);
		void SetClosed(bool bInClosed);
		void SetThickness(float ThicknessPixels);

		// Actor overrides
		void BeginPlay() override;
		void EndPlay() override;
		void GetDrawables(vector<const drawable*>& OutDrawables) const override;

		// Debug visualization
		void DrawDebug() { bDebugDrawEnabled = true; }
		bool IsDebugDrawEnabled() const { return bDebugDrawEnabled; }

	private:
		void CreateBody();
		void DestroyBody();
		void CreateSegments();

	private:
		// Configuration
		vector<vec2f> Points;
		bool bClosed = false;
		float Thickness = 4.0f;

		// Physics
		b2Body* Body = nullptr;

		// Debug visualization
		mutable vector<rectangle> DebugRects;
		mutable bool bDebugInitialized = false;
		bool bDebugDrawEnabled = false;
	};
}
