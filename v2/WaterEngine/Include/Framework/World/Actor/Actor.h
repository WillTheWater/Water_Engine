//// =============================================================================
//// Water Engine v2.0.0
//// Copyright(C) 2026 Will The Water
//// =============================================================================
//
//#pragma once
//
//#include "Core/CoreMinimal.h"
//#include "Framework/World/Object/Object.h"
//
//namespace we
//{
//	class World;
//	class IActorComponent;
//
//	class Actor : public Object
//	{
//	public:
//		explicit Actor(World* OwningWorld);
//		explicit Actor(World* OwningWorld, const string& TexturePath);
//		virtual ~Actor();
//
//		void BeginPlayGlobal();
//		virtual void BeginPlay();
//		virtual void Tick(float DeltaTime);
//		virtual void Destroy() override;
//
//	public:
//		void SetPosition(const vec2f& Position);
//		vec2f GetPosition() const;
//
//		void SetRotation(angle Angle);
//		angle GetRotation() const;
//
//		void SetScale(const vec2f& Scale);
//		vec2f GetScale() const;
//
//		rectf GetBounds() const;
//
//		void SetOrigin(const vec2f& Origin);
//		void CenterOrigin();
//
//		void SetColor(const color& Color);
//		void SetTextureSmooth() { Texture.get()->setSmooth(true); }
//
//		void SetVelocity(vec2f NewVelocity) { Velocity = NewVelocity; }
//		vec2f GetVelocity() const { return Velocity; }
//
//	public:
//		World* GetWorld() const { return OwnerWorld; }
//		bool HasSprite() const { return Sprite.has_value(); }
//		const sprite& GetSprite() const { return *Sprite; }
//		void SetSpriteTexture(shared<texture> NewTexture);
//		shared<texture> GetTexture() const { return Texture; }
//
//		Delegate<Actor*> OnActorDestroyed;
//
//		void AddComponent(shared<IActorComponent> Component);
//
//		template<typename T, typename... Args>
//		shared<T> CreateComponent(Args&&... args);
//
//	protected:
//		World* OwnerWorld;
//
//	private:
//		vector<shared<IActorComponent>> Components;
//		shared<texture> Texture;
//		optional<sprite> Sprite;
//		vec2f Position;
//		angle Rotation;
//		vec2f Scale;
//		vec2f Velocity;
//		bool bHasBegunPlay;
//	};
//
//	template<typename T, typename... Args>
//	shared<T> Actor::CreateComponent(Args&&... args)
//	{
//		static_assert(std::is_base_of_v<IActorComponent, T>);
//		auto Comp = make_shared<T>(std::forward<Args>(args)...);
//		AddComponent(Comp);
//		return Comp;
//	}
//}