// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Actor.h"
#include "Framework/World/World.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/Physics/PhysicsProxy.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "Utility/Math.h"
#include "Utility/Log.h"

namespace we
{
	// =========================================================================
	// Constructor / Destructor
	// =========================================================================
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
		// Clean up physics proxies
		if (OwningWorld && PrimaryProxy)
		{
			// Destroy all extra proxies first
			for (auto* proxy : ExtraProxies)
			{
				OwningWorld->GetSubsystem().Physics->DestroyProxy(proxy);
			}
			ExtraProxies.clear();

			// Destroy primary
			OwningWorld->GetSubsystem().Physics->DestroyProxy(PrimaryProxy);
			PrimaryProxy = nullptr;
		}
	}

	void Actor::BeginPlay()
	{
		if (bHasBegunPlay) return;
		bHasBegunPlay = true;
	}

	// =========================================================================
	// Tick - Sync physics to actor
	// =========================================================================
	void Actor::Tick(float DeltaTime)
	{
		if (!bHasBegunPlay) return;

		// Read from PhysicsProxy
		if (PrimaryProxy && PrimaryProxy->GetSimulationType() != PhysicsType::Static)
		{
			float PhysicsScale = GetPhysicsScale();
			
			vec2f Pos = PrimaryProxy->GetPosition();
			float Rot = PrimaryProxy->GetRotation();

			ActorPosition = { Pos.x / PhysicsScale, Pos.y / PhysicsScale };
			ActorRotation = sf::radians(Rot);

			UpdateTransform();

			if (OwningWorld)
			{
				OwningWorld->MarkRenderDirty();
			}
		}
	}

	// =========================================================================
	// Transform
	// =========================================================================
	void Actor::SetPosition(const vec2f& NewPosition)
	{
		ActorPosition = NewPosition;
		
		if (PrimaryProxy)
		{
			float PhysicsScale = GetPhysicsScale();
			PrimaryProxy->SetPosition({ ActorPosition.x * PhysicsScale, ActorPosition.y * PhysicsScale });
		}
		
		UpdateTransform();
		
		if (OwningWorld)
		{
			OwningWorld->MarkRenderDirty();
		}
	}

	void Actor::SetRotation(angle NewRotation)
	{
		ActorRotation = NewRotation;
		
		if (PrimaryProxy)
		{
			PrimaryProxy->SetRotation(ActorRotation.asRadians());
		}
		
		UpdateTransform();
	}

	void Actor::SetScale(const vec2f& NewScale)
	{
		ActorScale = NewScale;
		
		if (PrimaryProxy)
		{
			// Rebuild shapes with new scale
			PrimaryProxy->ClearShapes();
			auto Extents = GetActorExtents();
			float PhysicsScale = GetPhysicsScale();
			PrimaryProxy->AddBox({ Extents.x * PhysicsScale, Extents.y * PhysicsScale });
		}
		
		UpdateTransform();
	}

	void Actor::UpdateTransform()
	{
		if (ActorShape)
		{
			ActorShape->setPosition(ActorPosition);
			ActorShape->setRotation(ActorRotation);
			ActorShape->setScale(ActorScale);
		}

		if (ActorSprite)
		{
			ActorSprite->setPosition(ActorPosition);
			ActorSprite->setRotation(ActorRotation);
			ActorSprite->setScale(ActorScale);
		}
	}

	// =========================================================================
	// Physics
	// =========================================================================
	void Actor::SetPhysicsType(PhysicsType Type)
	{
		if (PhysicsBodyType == Type) return;
		
		// Clean up proxy if exists
		if (PrimaryProxy && OwningWorld)
		{
			OwningWorld->GetSubsystem().Physics->DestroyProxy(PrimaryProxy);
			PrimaryProxy = nullptr;
		}
		
		PhysicsBodyType = Type;
		
		// Create new physics proxy if needed
		if (Type != PhysicsType::None && OwningWorld)
		{
			GetOrCreatePhysicsProxy();
		}
	}

	float Actor::GetPhysicsScale() const
	{
		if (OwningWorld)
		{
			PhysicsSubsystem* Subsystem = OwningWorld->GetSubsystem().Physics.get();
			if (Subsystem)
			{
				return Subsystem->GetPhysicsScale();
			}
		}
		return 0.01f;
	}

	PhysicsProxy* Actor::GetOrCreatePhysicsProxy()
	{
		if (PrimaryProxy)
			return PrimaryProxy;
		
		if (!OwningWorld)
			return nullptr;
		
		PhysicsSubsystem* Subsystem = OwningWorld->GetSubsystem().Physics.get();
		if (!Subsystem)
			return nullptr;
		
		// Create default proxy
		PhysicsProxyConfig config;
		config.BodyType = PhysicsBodyType != PhysicsType::None ? PhysicsBodyType : PhysicsType::Dynamic;
		
		PrimaryProxy = Subsystem->CreateProxy(this, config);
		if (PrimaryProxy)
		{
			PrimaryProxy->SetCallback(this);
			
			// Set initial position from actor
			float Scale = GetPhysicsScale();
			PrimaryProxy->SetPosition({ ActorPosition.x * Scale, ActorPosition.y * Scale });
			PrimaryProxy->SetRotation(ActorRotation.asRadians());
			PrimaryProxy->WakeUp();
			
			LOG("[ACTOR] Created PhysicsProxy for {} at [{}, {}]", 
			    typeid(*this).name(), ActorPosition.x, ActorPosition.y);
		}
		
		return PrimaryProxy;
	}

	PhysicsProxy* Actor::CreatePhysicsProxy(PhysicsType Type, CollisionChannel Channel)
	{
		if (!OwningWorld)
			return nullptr;
		
		PhysicsSubsystem* Subsystem = OwningWorld->GetSubsystem().Physics.get();
		if (!Subsystem)
			return nullptr;
		
		PhysicsProxyConfig config;
		config.BodyType = Type;
		config.CollisionChannel = Channel;
		
		auto* proxy = Subsystem->CreateProxy(this, config);
		if (proxy)
		{
			proxy->SetCallback(this);
			ExtraProxies.push_back(proxy);
			LOG("[ACTOR] Created extra PhysicsProxy for {} on channel {}", 
			    typeid(*this).name(), static_cast<uint>(Channel));
		}
		
		return proxy;
	}

	void Actor::DestroyPhysicsProxy(PhysicsProxy* Proxy)
	{
		if (!Proxy || !OwningWorld)
			return;
		
		// Remove from extra list
		auto it = std::find(ExtraProxies.begin(), ExtraProxies.end(), Proxy);
		if (it != ExtraProxies.end())
		{
			ExtraProxies.erase(it);
		}
		
		// Destroy via subsystem
		OwningWorld->GetSubsystem().Physics->DestroyProxy(Proxy);
	}

	// =========================================================================
	// Collision Events - IPhysicsProxyCallback implementation
	// =========================================================================
	void Actor::OnBeginOverlap(PhysicsProxy* Self, PhysicsProxy* Other)
	{
		// Base class logs by default - derived classes override
		LOG("[ACTOR] OnBeginOverlap: {} overlaps with {}", 
		    typeid(*this).name(), 
		    Other && Other->GetOwner() ? typeid(*Other->GetOwner()).name() : "unknown");
	}

	void Actor::OnEndOverlap(PhysicsProxy* Self, PhysicsProxy* Other)
	{
		// Base class logs by default - derived classes override
		LOG("[ACTOR] OnEndOverlap: {} ended overlap with {}", 
		    typeid(*this).name(),
		    Other && Other->GetOwner() ? typeid(*Other->GetOwner()).name() : "unknown");
	}

	// =========================================================================
	// Sprite
	// =========================================================================
	void Actor::SetTexture(shared<texture> NewTexture)
	{
		if (NewTexture)
		{
			ActorTexture = NewTexture;
			
			ActorSprite.emplace(*ActorTexture);
			
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
		if (ActorShape)
		{
			auto bounds = ActorShape->getLocalBounds();
			return { bounds.size.x * 0.5f, bounds.size.y * 0.5f };
		}

		if (ActorSprite.has_value())
		{
			auto bounds = ActorSprite->getLocalBounds();
			return { bounds.size.x * 0.5f, bounds.size.y * 0.5f };
		}

		return { 32.0f, 32.0f };
	}

	// =========================================================================
	// Drawable
	// =========================================================================
	const drawable* Actor::GetDrawable() const
	{
		if (!bIsVisible) return nullptr;
		
		if (ActorShape) return ActorShape.get();
		if (ActorSprite.has_value()) return &ActorSprite.value();
		
		return nullptr;
	}
}
