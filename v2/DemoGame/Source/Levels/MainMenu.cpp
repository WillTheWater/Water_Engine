// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/World/Actor.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
	MainMenu::MainMenu(EngineSubsystem& Subsystem)
		: World(Subsystem)
	{
	}

	void MainMenu::BeginPlay()
	{
		// Create background rectangle
		auto BgActor = SpawnActor<Actor>();
		if (auto bg = BgActor.lock())
		{
			bg->SetAsRectangle({ 1920.0f, 1080.0f }, color(20, 30, 50));
			bg->SetPosition({ 960.0f, 540.0f });  // Center of 1920x1080
			Background = bg;
		}

		// Create title card
		auto TitleActor = SpawnActor<Actor>();
		if (auto title = TitleActor.lock())
		{
			title->SetAsRectangle({ 400.0f, 150.0f }, color(200, 180, 100));
			title->SetPosition({ 960.0f, 300.0f });
			TitleCard = title;
		}

		// Create some floating shapes
		for (int i = 0; i < 5; ++i)
		{
			auto ShapeActor = SpawnActor<Actor>();
			if (auto shape = ShapeActor.lock())
			{
				float x = 300.0f + i * 350.0f;
				float y = 700.0f + (i % 2) * 100.0f;
				shape->SetAsRectangle({ 100.0f, 100.0f }, color(100 + i * 30, 150, 200));
				shape->SetPosition({ x, y });
			}
		}
	}

	void MainMenu::Tick(float DeltaTime)
	{
		// Simple animation - bob the title card
		if (auto title = TitleCard.lock())
		{
			vec2f Pos = title->GetPosition();
			static float Time = 0.0f;
			Time += DeltaTime;
			Pos.y = 300.0f + std::sin(Time * 2.0f) * 20.0f;
			title->SetPosition(Pos);
		}
	}
}
