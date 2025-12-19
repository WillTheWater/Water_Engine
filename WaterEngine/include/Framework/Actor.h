#pragma once
#include "Framework/Core.h"
#include <SFML/Graphics.hpp>
#include "Framework/Object.h"
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

		void SetPhysicsEnabled(bool Enabled);
		void SetShouldDrawDebugShapes(bool ToDraw) { bDrawDebug = ToDraw; }
		sf::VertexArray ForwardVectorDebugShape(float Length = 150.f, sf::Color Color = sf::Color::Red) const;

		void SetActorID(EActorID NewID) { ActorID = NewID; }
		EActorID GetActorID() const { return ActorID; }
		static EActorID GetNeutralActorID() { return NeutralID; }
		bool IsHostile(Actor* OtherActor);

		void SetActorLocation(const sf::Vector2f& NewLocation);
		void AddActorLocationOffset(const sf::Vector2f& Offset);
		void SetActorRotation(const sf::Angle& NewRotation);
		void AddActorRotationOffset(const sf::Angle& RotOffset);
		void SetActorScale(const sf::Vector2f NewScale);
		sf::Vector2f GetActorLocation() const { return ActorLocation; };
		sf::Angle GetActorRotation() const { return ActorRotation; };

		void SetLocalForwardVector(sf::Vector2f& Forward);
		sf::Vector2f GetActorFowardVector() const;
		sf::Vector2f GetActorRightVector() const;
		bool IsOutOfBounds(float Allowance = 10.f) const;

		void SetTexture(const std::string& TexturePath, float SpriteScale = 1.0f);
		void SetSpriteFrame(int FrameWidth, int FrameHeight);
		sf::Vector2u GetTextureSize() const { return ATexture->getSize(); }
		sf::FloatRect GetSpriteBounds() const;
		sf::Vector2f GetSpriteScale() const { return ASprite->getScale(); }
		sf::Sprite& GetSprite() { return *ASprite;}
		const sf::Sprite& GetSprite() const { return *ASprite;}
		void CenterPivot();
		
	private:
		void InitialziePhysics();
		void UninitialziePhysics();
		void UpdatePhysicsBodyTransform();
		
		World* OwningWorld;
		bool bHasBegunPlay;
		bool bDrawDebug = false;

		shared<sf::Texture> ATexture;
		shared<sf::Sprite> ASprite;

		sf::Vector2f LocalForward = { 1.f, 0.f };
		sf::Vector2f ActorLocation;
		sf::Angle ActorRotation;
		sf::Vector2f ActorScale;

		b2Body* PhysicsBody;
		bool bPhysicsEnabled;

		EActorID ActorID;
		const static EActorID NeutralID = EActorID::Neutral;
	};
}