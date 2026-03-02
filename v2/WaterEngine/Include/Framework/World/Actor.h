// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Framework/World/Object.h"
#include "Framework/Physics/PhysicsTypes.h"
#include "Framework/Physics/PhysicsProxy.h"
#include <optional>

namespace we
{
	class World;
	class PhysicsProxy;

	// Actor can render as sprite, shape, or both
	enum class EActorRenderMode { None, Sprite, Shape, Both };

	// =============================================================================
	// Actor - Base class for all game objects
	// Implements IPhysicsProxyCallback to receive collision events
	// =============================================================================
	class Actor : public Object, public IPhysicsProxyCallback
	{
	public:
		Actor(World* OwningWorld);
		virtual ~Actor();

		// =================================================================
		// Lifecycle
		// =================================================================
		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);
		bool HasBegunPlay() const { return bHasBegunPlay; }

		// =================================================================
		// Transform
		// =================================================================
		void SetPosition(const vec2f& Position);
		vec2f GetActorPosition() const { return ActorPosition; }

		void SetRotation(angle Rotation);
		angle GetActorRotation() const { return ActorRotation; }

		void SetScale(const vec2f& Scale);
		vec2f GetScale() const { return ActorScale; }

		// =================================================================
		// Physics Type - Convenience wrapper around PhysicsProxy
		// =================================================================
		void SetPhysicsType(PhysicsType Type);
		PhysicsType GetPhysicsType() const { return PhysicsBodyType; }
		float GetPhysicsScale() const;

		// =================================================================
		// Physics Proxy Access
		// =================================================================
		// Get the primary physics proxy (returns null if not created)
		PhysicsProxy* GetPhysicsProxy() const { return PrimaryProxy; }
		
		// Ensure the primary physics proxy exists (creates one if needed)
		PhysicsProxy* GetOrCreatePhysicsProxy();
		
		bool HasPhysicsProxy() const { return PrimaryProxy != nullptr; }
		
		// Create a new physics proxy (for compound shapes, sensors, etc.)
		PhysicsProxy* CreatePhysicsProxy(PhysicsType Type, CollisionChannel Channel);
		
		void DestroyPhysicsProxy(PhysicsProxy* Proxy);

		// =================================================================
		// Collision Events
		// =================================================================
		virtual void OnBeginOverlap(PhysicsProxy* Self, PhysicsProxy* Other);
		virtual void OnEndOverlap(PhysicsProxy* Self, PhysicsProxy* Other);

		// =================================================================
		// Rendering
		// =================================================================
		virtual const drawable* GetDrawable() const;
		virtual float GetRenderDepth() const { return CustomRenderDepth.value_or(ActorPosition.y); }
		
		void SetCustomRenderDepth(float Depth) { CustomRenderDepth = Depth; }
		void ClearCustomRenderDepth() { CustomRenderDepth.reset(); }
		bool HasCustomRenderDepth() const { return CustomRenderDepth.has_value(); }
		
		void SetVisible(bool bVisible) { bIsVisible = bVisible; }
		bool IsVisible() const { return bIsVisible; }

		// =================================================================
		// Sprite
		// =================================================================
		void SetTexture(shared<texture> NewTexture);
		void SetTextureRect(const recti& TexRect);
		void SetSpriteOrigin(const vec2f& Origin);
		bool HasSprite() const { return ActorSprite.has_value(); }

		// =================================================================
		// Shape
		// =================================================================
		void SetShape(unique<shape> NewShape);
		void SetShapeColor(const color& Color);
		shape* GetShape() const { return ActorShape.get(); }
		bool HasShape() const { return ActorShape != nullptr; }
		void SetAsRectangle(vec2f Size, color FillColor = color::White);
		void SetAsCircle(float Radius, color FillColor = color::White);

		// =================================================================
		// Utility
		// =================================================================
		vec2f GetActorExtents() const;
		World* GetWorld() const { return OwningWorld; }

	protected:
		void UpdateTransform();

	private:
		World* OwningWorld;
		
		// Render data
		unique<shape> ActorShape;
		shared<texture> ActorTexture;
		optional<sprite> ActorSprite;
		
		// Transform
		vec2f ActorPosition;
		angle ActorRotation;
		vec2f ActorScale;
		
		// State
		bool bIsVisible = true;
		bool bHasBegunPlay = false;
		
		// Physics
		PhysicsType PhysicsBodyType = PhysicsType::None;
		PhysicsProxy* PrimaryProxy = nullptr;
		vector<PhysicsProxy*> ExtraProxies;
		
		// Custom render depth
		std::optional<float> CustomRenderDepth;
	};
}
