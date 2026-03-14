// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Object.h"

namespace we
{
	class World;

	class Actor : public Object
	{
	public:
		Actor(World& OwningWorld);
		virtual ~Actor();

		void StartPlay();
		void StartTick(float DeltaTime);
		bool HasBegunPlay() const { return bHasBegunPlay; }

		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);
		virtual void EndPlay();

	public:

		// Transform
		void SetPosition(const vec2f& NewPosition) { Position = NewPosition; }
		void SetRotation(angle NewRotation) { Rotation = NewRotation; }
		void SetScale(const vec2f& NewScale) { Scale = NewScale; }
		vec2f GetPosition() const { return Position; }
		angle GetRotation() const { return Rotation; }
		vec2f GetScale() const { return Scale; }
		void UpdateTransform();

		// Sprite
		void SetSprite(shared<texture> Texture);
		void SetSpriteOrigin(const vec2f& Origin);
		bool HasSprite() const { return ActorSprite.has_value(); }
		const drawable* GetDrawable() const;

		// Render Depth
		float GetRenderDepth() const { return CustomDepth.value_or(Position.y); }
		void SetCustomRenderDepth(float Depth) { CustomDepth = Depth; }
		bool HasCustomRenderDepth() const { return CustomDepth.has_value(); }

		// Visibility
		void SetVisible(bool bVisible) { bIsVisible = bVisible; }
		bool IsVisible() const { return bIsVisible; }

		World& GetWorld() const { return OwningWorld; }

	private:
		World& OwningWorld;

		// Transform
		vec2f Position;
		angle Rotation;
		vec2f Scale;

		// Render
		optional<sprite> ActorSprite;
		optional<float> CustomDepth;
		bool bIsVisible;
		bool bHasBegunPlay;
	};
}