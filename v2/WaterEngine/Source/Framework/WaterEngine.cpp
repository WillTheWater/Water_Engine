// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EngineConfig.h"
#include "Framework/WaterEngine.h"
#include "AssetDirectory/PakDirectory.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

//============ TEST ==================
#include "Framework/World/World.h"
#include "Framework/World/Actor/Actor.h"
#include "Utility/Delegate.h"
//====================================

namespace we
{
	WaterEngine::WaterEngine()
	{
		Configure();
		Construct();
		WindowInit();
	}

	WaterEngine::~WaterEngine() = default;

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

		//============ TEST ==================
		WorldTest = make_unique<World>(Subsystem);
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

		if (auto TestActor = WorldTest->SpawnActor<Actor>(WorldTest.get(), "Assets/Textures/test.png").lock())
		{
			TestActor->SetPosition({ EC.WindowSize.x / 2,EC.WindowSize.y / 2 });
			TestActor->OnActorDestroyed.Bind([this](Actor* DestroyedActor) -> bool
				{
				    // We are inside the WaterEngine class, so we can call private methods
				    this->DelegateTest();
				    return true; // Must return bool for the Delegate to work correctly
				});
			TestActor->Destroy();
		}
	}

	void WaterEngine::ProcessEvents()
	{
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
		if (WorldTest)
		{
			WorldTest->Tick(Subsystem.Time->GetDeltaTime());
		}
		WindowCursor->Update(Subsystem.Time->GetDeltaTime());
	}

	void WaterEngine::Render()
	{
		Window->clear();

		Subsystem.Render->StartRender();

		if (WorldTest)
		{
			WorldTest->Render();
		}

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

	void WaterEngine::DelegateTest()
	{
		LOG("Delegate Called")
	}
}