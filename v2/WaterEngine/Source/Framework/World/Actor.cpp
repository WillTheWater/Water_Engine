// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Utility/Math.h"
#include "box2d/b2_body.h"

namespace we
{
	Actor::Actor(World* InWorld)
		: Object()
		, OwningWorld{ InWorld }
		, ActorPosition{}
		, ActorRotation{}
		, ActorScale{ 1.0f, 1.0f }
		, bIsVisible{ true }
	{
	}

	Actor::~Actor()
	{
		// Clean up physics body
		if (PhysicsBody && OwningWorld)
		{
			// TODO: Need access to subsystem - will be handled in actor destruction path
		}
	}

	void Actor::BeginPlay()
	{
		if (bHasBegunPlay) return;
		bHasBegunPlay = true;
	}

	void Actor::Tick(float DeltaTime)
	{
		if (!bHasBegunPlay) return;

		// Tick physics bodies that can move (Kinematic or Dynamic)
		// Static bodies never move, so they don't need tick updates
		if (PhysicsBody && PhysicsBodyType != PhysicsType::Static)
		{
			// TODO: Get physics scale from EngineConfig (Physics.PhysicsScale)
			// For now, hardcode to match EngineConfig default (0.01f = 1/100)
			float PhysicsScale = 0.01f;
			
			b2Vec2 Pos = PhysicsBody->GetPosition();
			float Rot = PhysicsBody->GetAngle();

			// Update actor transform from physics
			ActorPosition = { Pos.x / PhysicsScale, Pos.y / PhysicsScale };
			ActorRotation = sf::radians(Rot);

			// Update visual representation
			UpdateTransform();

			// Mark world render dirty
			if (OwningWorld)
			{
				OwningWorld->MarkRenderDirty();
			}
		}
	}

	const drawable* Actor::GetDrawable() const
	{
		if (!bIsVisible) return nullptr;
		
		// Prefer shape for grey boxing, fallback to sprite
		if (ActorShape) return ActorShape.get();
		if (ActorSprite.has_value()) return &ActorSprite.value();
		
		return nullptr;
	}

	void Actor::SetPosition(const vec2f& NewPosition)
	{
		ActorPosition = NewPosition;
		
		// If physics is enabled, update physics body transform
		if (PhysicsBodyType != PhysicsType::None && PhysicsBody)
		{
			float PhysicsScale = GetPhysicsScale();
			b2Vec2 Pos{ ActorPosition.x * PhysicsScale, ActorPosition.y * PhysicsScale };
			PhysicsBody->SetTransform(Pos, ActorRotation.asRadians());
		}
		
		UpdateTransform();
		
		// Notify world that render order may need update
		if (OwningWorld)
		{
			OwningWorld->MarkRenderDirty();
		}
	}

	void Actor::SetRotation(angle NewRotation)
	{
		ActorRotation = NewRotation;
		
		// If physics is enabled, update physics body transform
		if (PhysicsBodyType != PhysicsType::None && PhysicsBody)
		{
			// TODO: Get physics scale from EngineConfig (Physics.PhysicsScale = 0.01f)
			float PhysicsScale = 0.01f;
			b2Vec2 Pos{ ActorPosition.x * PhysicsScale, ActorPosition.y * PhysicsScale };
			PhysicsBody->SetTransform(Pos, ActorRotation.asRadians());
		}
		
		UpdateTransform();
	}

	void Actor::SetScale(const vec2f& NewScale)
	{
		ActorScale = NewScale;
		
		// If physics is enabled, update collision shape
		if (PhysicsBodyType != PhysicsType::None && PhysicsBody)
		{
			// TODO: Update fixture shape based on new scale
		}
		
		UpdateTransform();
	}

	// =========================================================================
	// Physics
	// =========================================================================

	void Actor::SetPhysicsType(PhysicsType Type)
	{
		if (PhysicsBodyType == Type) return;
		
		// Clean up old body if exists
		if (PhysicsBody && OwningWorld)
		{
			// Access subsystem through world -> engine subsystem
			auto* Subsystem = reinterpret_cast<EngineSubsystem*>(
				reinterpret_cast<char*>(OwningWorld) - offsetof(EngineSubsystem, World));
			if (Subsystem && Subsystem->Physics)
			{
				Subsystem->Physics->RemoveListener(PhysicsBody);
			}
			PhysicsBody = nullptr;
		}
		
		PhysicsBodyType = Type;
		
		// Create new body if not None
		if (Type != PhysicsType::None && OwningWorld)
		{
			// Access subsystem through world
			auto* Subsystem = reinterpret_cast<EngineSubsystem*>(
				reinterpret_cast<char*>(OwningWorld) - offsetof(EngineSubsystem, World));
			if (Subsystem && Subsystem->Physics)
			{
				PhysicsBody = Subsystem->Physics->AddListener(this);
			}
		}
	}

	float Actor::GetPhysicsScale() const
	{
		if (OwningWorld)
		{
			auto* Subsystem = reinterpret_cast<EngineSubsystem*>(
				reinterpret_cast<char*>(OwningWorld) - offsetof(EngineSubsystem, World));
			if (Subsystem && Subsystem->Physics)
			{
				return Subsystem->Physics->GetPhysicsScale();
			}
		}
		// Fallback to default
		return 0.01f;
	}

	// =========================================================================
	// Sprite
	// =========================================================================

	void Actor::SetTexture(shared<texture> NewTexture)
	{
		if (NewTexture)
		{
			ActorTexture = NewTexture;
			
			// Create sprite with new texture
			ActorSprite.emplace(*ActorTexture);
			
			// Center origin
			auto bounds = ActorSprite->getLocalBounds();
			ActorSprite->setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
			
			UpdateTransform();
		}
	}

	void Actor::SetTextureRect(const recti& TexRect)
	{
		if (ActorSprite) ActorSprite->setTextureRect(TexRect);
	}

	void Actor::SetSpriteOrigin(const vec2f& Origin)
	{
		if (ActorSprite) ActorSprite->setOrigin(Origin);
	}

	// =========================================================================
	// Shape (Grey Boxing)
	// =========================================================================

	void Actor::SetShape(unique<shape> NewShape)
	{
		ActorShape = std::move(NewShape);
		UpdateTransform();
	}

	void Actor::SetShapeColor(const color& Color)
	{
		if (ActorShape)
		{
			ActorShape->setFillColor(Color);
		}
	}

	void Actor::SetAsRectangle(vec2f Size, color FillColor)
	{
		auto rect = make_unique<rectangle>(Size);
		rect->setFillColor(FillColor);
		rect->setOrigin({ Size.x / 2.0f, Size.y / 2.0f });
		
		ActorShape = std::move(rect);
		UpdateTransform();
	}

	void Actor::SetAsCircle(float Radius, color FillColor)
	{
		auto circ = make_unique<circle>(Radius);
		circ->setFillColor(FillColor);
		circ->setOrigin({ Radius, Radius });
		
		ActorShape = std::move(circ);
		UpdateTransform();
	}

	// =========================================================================
	// Actor Extents
	// =========================================================================

	vec2f Actor::GetActorExtents() const
	{
		// If we have a shape, use its logical size (half-size)
		if (ActorShape)
		{
			// Get local bounds and return half-size
			auto bounds = ActorShape->getLocalBounds();
			return { bounds.size.x * 0.5f, bounds.size.y * 0.5f };
		}

		// If we have a sprite, use its bounds (half-size)
		if (ActorSprite.has_value())
		{
			auto bounds = ActorSprite->getLocalBounds();
			return { bounds.size.x * 0.5f, bounds.size.y * 0.5f };
		}

		// Default fallback - no visual = no collision
		return { 0.0f, 0.0f };
	}

	// =========================================================================
	// Physics
	// =========================================================================

	void Actor::UpdatePhysicsBodyTransform()
	{
		if (PhysicsBodyType != PhysicsType::None && PhysicsBody)
		{
			float PhysicsScale = GetPhysicsScale();
			b2Vec2 Pos{ ActorPosition.x * PhysicsScale, ActorPosition.y * PhysicsScale };
			PhysicsBody->SetTransform(Pos, ActorRotation.asRadians());
		}
	}

	// =========================================================================
	// Transform Update
	// =========================================================================

	void Actor::UpdateTransform()
	{
		// Update shape
		if (ActorShape)
		{
			ActorShape->setPosition(ActorPosition);
			ActorShape->setRotation(ActorRotation);
			ActorShape->setScale(ActorScale);
		}
		
		// Update sprite
		if (ActorSprite)
		{
			ActorSprite->setPosition(ActorPosition);
			ActorSprite->setRotation(ActorRotation);
			ActorSprite->setScale(ActorScale);
		}
	}
}
