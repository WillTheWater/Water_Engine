#pragma once
#include "Framework/Core.h"
#include <SFML/Graphics.hpp>
#include "Framework/Object.h"
#include "Framework/Delegate.h"
#include "EActorTypes.h"

class b2Body;

namespace we
{
	class World;

	class Actor : public Object
	{
	public:
		Actor(World* OwningWorld, const string& TexturePath = "");
		virtual ~Actor();

		void BeginPlayGlobal();
		void TickGlobal(float DeltaTime);
		void Render(class Renderer& GameRenderer);
		
		const World* GetWorld() const { return OwningWorld; }
		World* GetWorld() { return OwningWorld; }
		sf::Vector2u GetWindowSize() const;

		virtual void BeginPlay();
		virtual void Tick(float DeltaTime);
		virtual void Destroy() override;
		virtual void OnActorBeginOverlap(Actor* OtherActor);
		virtual void OnActorEndOverlap(Actor* OtherActor);
		virtual void Damage(float Amount);
		Delegate<Actor*> OnActorDestroyed;

		void SetPhysicsEnabled(bool Enabled);
		b2Body* GetPhysicsBody() const { return PhysicsBody; }
		void SetActorID(EActorID NewID) { ActorID = NewID; }
		EActorID GetActorID() const { return ActorID; }
		static EActorID GetNeutralActorID() { return NeutralID; }
		bool IsHostile(Actor* OtherActor);

		void SetActorLocation(const sf::Vector2f& NewLocation);
		void AddActorLocationOffset(const sf::Vector2f& Offset);
		void SetActorRotation(const sf::Angle& NewRotation);
		void AddActorRotationOffset(const sf::Angle& RotOffset);
		void SetActorScale(const sf::Vector2f NewScale);
		void SetActorExtents(const sf::Vector2f& HalfSize);
		void SetActorSizeSource(EActorSize Source);
		sf::Vector2f GetActorExtents() const;
		sf::Vector2f GetActorLocation() const { return ActorLocation; };
		sf::Angle GetActorRotation() const { return ActorRotation; };

		sf::Vector2f GetActorForwardVector() const;
		sf::Vector2f GetActorRightVector() const;
		bool IsActorOutOfBounds(float Allowance = 10.f) const;

		void SetTexture(const std::string& TexturePath);
		void SetSpriteLocationOffset(const sf::Vector2f& Position);
		void SetSpriteRotationOffset(const sf::Angle& Rotation);
		void SetSpriteScale(const sf::Vector2f& Scale);
		void SetSpriteFrame(int FrameWidth, int FrameHeight);
		sf::FloatRect GetSpriteBounds() const;
		sf::Vector2f GetSpriteScale() const { return SpriteScale; }
		sf::Sprite& GetSprite() { return *Sprite;}
		const sf::Sprite& GetSprite() const { return *Sprite;}
		void UpdateSpriteTransform();
		void CenterPivot();

	protected:
		sf::Vector2f ActorExtents{ 0.f, 0.f };
		EActorSize SizeSource{ EActorSize::SpriteBounds };
		
	private:
		void InitializePhysics();
		void UninitializePhysics();
		void UpdatePhysicsBodyTransform();
		
		World* OwningWorld;
		bool bHasBegunPlay;

		shared<sf::Texture> Texture;
		shared<sf::Sprite> Sprite;

		sf::Vector2f ActorLocation;
		sf::Angle ActorRotation;
		sf::Vector2f ActorScale;

		sf::Vector2f SpriteLocationOffset;
		sf::Angle SpriteRotationOffset;
		sf::Vector2f SpriteScale;

		b2Body* PhysicsBody;
		bool bPhysicsEnabled;

		EActorID ActorID;
		const static EActorID NeutralID = EActorID::Neutral;

		// Debug Shapes
		sf::VertexArray ForwardVectorDebugShape(float Length = 150.f, sf::Color Color = sf::Color::Red) const;
		sf::VertexArray CollisionShapeDebug(sf::Color Color = sf::Color(255, 0, 0, 100)) const;
		bool bRenderDebugShapes;
	};
}