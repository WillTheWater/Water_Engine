//// =============================================================================
//// Water Engine v2.0.0
//// Copyright(C) 2026 Will The Water
//// =============================================================================
//
//#pragma once
//
//#include "Core/CoreMinimal.h"
//#include "Framework/World/Object/Object.h"
//#include "EngineConfig.h"
//
//namespace we
//{
//	class Actor;
//	struct EngineSubsystem;
//
//	class World : public Object
//	{
//	public:
//		World(EngineSubsystem& Subsystem);
//		virtual ~World();
//
//		void BeginPlayGlobal();
//		void TickGlobal(float DeltaTime);
//		
//		virtual void BeginPlay();
//		virtual void Tick(float DeltaTime);
//		virtual void Render();
//
//
//	public:
//		template<typename ActorType, typename... Args>
//		weak<ActorType> SpawnActor(Args&&... args);
//
//		vector<shared<Actor>> GetActors() const { return Actors; }
//		EngineSubsystem& GetSubsystem() const { return Subsystem; }
//
//	protected:
//		EngineSubsystem& Subsystem;
//
//		void RenderActors();
//
//	private:
//		vector<shared<Actor>> Actors;
//		vector<shared<Actor>> PendingActors;
//		bool bHasBegunPlay;
//
//	private:
//		void ManageActors(float DeltaTime);
//	};
//
//	template<typename ActorType, typename... Args>
//	inline weak<ActorType> World::SpawnActor(Args&&... args)
//	{
//		shared<ActorType> NewActor = make_shared<ActorType>(this, std::forward<Args>(args)...);
//		PendingActors.push_back(NewActor);
//		return NewActor;
//	}
//}