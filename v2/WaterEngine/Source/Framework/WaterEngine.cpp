// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EngineConfig.h"
#include "Framework/WaterEngine.h"
#include "AssetDirectory/PakDirectory.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

namespace we
{
	WaterEngine::WaterEngine()
	{
		Configure();
		Construct();
		WindowInit();
	}

	void WaterEngine::Configure()
	{
		auto PD = make_shared<PakDirectory>(EC.AssetDirectory);
		Asset().SetAssetDirectory(PD);
		if (EC.DisableSFMLLogs) { sf::err().rdbuf(nullptr); }
	}

	void WaterEngine::Construct()
	{
		Subsystem.Time = make_unique<TimerSubsystem>();
		Subsystem.Render = make_unique<RenderSubsystem>();
		Subsystem.SaveLoad = make_unique<SaveLoadSubsystem>();
		Subsystem.Audio = make_unique<AudioSubsystem>();
		Subsystem.Input = make_unique<InputSubsystem>();
	}

	void WaterEngine::WindowInit()
	{
		Window = make_unique<GameWindow>();
		Window->OnResize = [this](vec2u newSize)
			{
				const auto CorrectedView = Subsystem.Render->ConstrainView(Window->getSize());
				Window->setView(CorrectedView);
			};
		WindowCursor = make_unique<Cursor>(*Window);
	}

	void WaterEngine::ProcessEvents()
	{
		LoadWorld();

		while (const auto Event = Window->pollEvent())
		{
			Event->visit(GameWindowEventHandler{ *Window });
			Subsystem.Input->HandleEvent(*Event);
		}
	}

	void WaterEngine::PostUpdate()
	{
		Subsystem.Input->PostUpdate();
	}

	void WaterEngine::GlobalTick()
	{
		Subsystem.Time->Tick();
		Subsystem.Input->ProcessHeld();
		WindowCursor->Update(Subsystem.Time->GetDeltaTime());
	}

	void WaterEngine::Render()
	{
		Window->clear();

		Subsystem.Render->StartRender();
		Window->draw(sprite(Subsystem.Render->FinishRender()));

		WindowCursor->Render();

		Window->display();
	}

	bool WaterEngine::IsRunning() const
	{
		return Window->isOpen();
	}

	bool we::WaterEngine::HasFocus() const
	{
		return Window->hasFocus();
	}

	void WaterEngine::ConstrainRender()
	{
		Subsystem.Render->ConstrainView(Window->getSize());
	}

	void WaterEngine::LoadWorld()
	{
		/*if (const auto NextLevel = Subsystem.World->GetNextLevel())
		{
			LoadLevel(*NextLevel);
		}*/
	}

	void WaterEngine::LoadLevel(const string& LevelName)
	{
		/*assert(Levels.contains(LevelName));
		Level* NextLevel = Levels.at(LevelName).get();

		if (CurrentLevel) { CurrentLevel->Cleanup(); }
		CurrentLevel = NextLevel;
		CurrentLevel->BeginPlay();*/
	}

	void WaterEngine::RestartLevel()
	{
		//Subsystem.World->RestartLevel();
	}
}