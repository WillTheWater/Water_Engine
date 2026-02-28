// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Object.h"
#include <optional>

namespace we
{
	class World;

	// Actor can render as sprite, shape, or both
	enum class EActorRenderMode { None, Sprite, Shape, Both };

	class Actor : public Object
	{
	public:
		Actor(World* OwningWorld);
		virtual ~Actor() = default;

		// Lifecycle
		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);
		bool HasBegunPlay() const { return bHasBegunPlay; }

		// Transform
		void SetPosition(const vec2f& Position);
		vec2f GetPosition() const { return ActorPosition; }

		void SetRotation(angle Rotation);
		angle GetRotation() const { return ActorRotation; }

		void SetScale(const vec2f& Scale);
		vec2f GetScale() const { return ActorScale; }

		// Rendering - returns primary drawable (shape or sprite)
		virtual const drawable* GetDrawable() const;
		
		// Render depth - returns custom depth if set, otherwise Y position
		virtual float GetRenderDepth() const { return CustomRenderDepth.value_or(ActorPosition.y); }
		
		// Custom render depth - overrides Y-based sorting
		void SetCustomRenderDepth(float Depth) { CustomRenderDepth = Depth; }
		void ClearCustomRenderDepth() { CustomRenderDepth.reset(); }
		bool HasCustomRenderDepth() const { return CustomRenderDepth.has_value(); }
		
		void SetVisible(bool bVisible) { bIsVisible = bVisible; }
		bool IsVisible() const { return bIsVisible; }

		// Sprite (optional - for textured actors)
		void SetTexture(shared<texture> NewTexture);
		void SetTextureRect(const recti& TexRect);
		void SetSpriteOrigin(const vec2f& Origin);
		bool HasSprite() const { return ActorSprite.has_value(); }

		// Shape (optional - for prototyping/grey boxing)
		void SetShape(unique<shape> NewShape);
		void SetShapeColor(const color& Color);
		shape* GetShape() const { return ActorShape.get(); }
		bool HasShape() const { return ActorShape != nullptr; }

		// Utility for grey boxing - quick rectangle
		void SetAsRectangle(vec2f Size, color FillColor = color::White);
		void SetAsCircle(float Radius, color FillColor = color::White);

		World* GetWorld() const { return OwningWorld; }

	protected:
		void UpdateTransform();

	private:
		World* OwningWorld;
		
		// Render data - use shape OR sprite (or both with SetRenderMode)
		unique<shape> ActorShape;        // For prototyping (rectangles, circles, etc.)
		shared<texture> ActorTexture;    // For textured sprites
		optional<sprite> ActorSprite;    // SFML sprite (requires texture)
		
		// Transform
		vec2f ActorPosition;
		angle ActorRotation;
		vec2f ActorScale;
		
		// State
		bool bIsVisible = true;
		bool bHasBegunPlay = false;
		
		// Custom render depth (overrides Y-based sorting)
		std::optional<float> CustomRenderDepth;
	};
}
