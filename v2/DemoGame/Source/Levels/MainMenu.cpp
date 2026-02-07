// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/AsyncResourceSubsystem.h"
#include "GameStateTokens.h"
#include "UI/MainMenuUI.h"
#include "Utility/Log.h"

namespace we
{
	MainMenu::MainMenu(EngineSubsystem& Subsystem)
		: World(Subsystem)
	{
		BgTexture = Asset().LoadTexture(EC.DefaultBackground);
		if (BgTexture)
		{
			Background.emplace(*BgTexture);
		}

		UI = make_unique<MainMenuUI>(Subsystem);
		UI->OnPlayButtonClicked.Bind(this, &MainMenu::OnPlayClicked);
		Subsystem.Audio->PlayMusic(EC.DefaultMusic);
		Subsystem.Audio->PlayAmbient(EC.DefaultAmbient);
	}

	MainMenu::~MainMenu()
	{
	}

	void MainMenu::BeginPlay()
	{
		// Original Test 1: Async with Tick polling
		TestHandle = AsyncAsset().LoadTextureAsync(EC.WindowIcon, AssetPriority::High);
		LOG("Test 1: Async load started (Tick polling)");

		// Original Test 2: Async with Render polling
		TestHandleRender = AsyncAsset().LoadTextureAsync(EC.WindowIcon, AssetPriority::Normal);
		LOG("Test 2: Async load started (Render polling)");

		// Original Test 3: Sync load (blocking, immediate)
		LOG("Test 3: Sync load starting...");
		TestTextureSync = AsyncAsset().LoadTextureSync(EC.WindowIcon);
		if (TestTextureSync)
		{
			TestSpriteSync.emplace(*TestTextureSync);
			LOG("Test 3: Sync load complete, sprite ready");
		}

		// Power Test 1: Mass async load (500 textures)
		LOG("POWER TEST 1: Queuing 500 async textures...");
		MassTestStartTime = Subsystem.Time->GetElapsedTime();
		for (int i = 0; i < 500; i++)
		{
			auto Handle = AsyncAsset().LoadTextureAsync(EC.WindowIcon, AssetPriority::Normal);
			MassHandles.push_back(Handle);
		}
		float QueueTime = Subsystem.Time->GetElapsedTime() - MassTestStartTime;
		LOG("POWER TEST 1: Queued 500 textures in {:.6f}s - NO FRAME HITCH!", QueueTime);

		// Power Test 2: Priority test (low priority first, then high)
		LOG("POWER TEST 2: Priority queue test...");
		for (int i = 0; i < 5; i++)
		{
			PriorityHandles.push_back(AsyncAsset().LoadTextureAsync(EC.WindowIcon, AssetPriority::Low));
		}
		// This high priority one should finish first despite being queued last
		PriorityHandles.push_back(AsyncAsset().LoadTextureAsync(EC.WindowIcon, AssetPriority::High));
		LOG("POWER TEST 2: Queued 5 low + 1 high priority");
	}

	void MainMenu::Tick(float DeltaTime)
	{
		// Original Test 1: Check in Tick
		if (TestHandle.IsReady() && !TestSpriteTick.has_value())
		{
			TestSpriteTick.emplace(*TestHandle);
			TestSpriteTick->setPosition({ 100, 100 });
			LOG("Test 1: Tick poll - Texture ready, sprite created!");
		}

		// Power Test 1: Count mass loads
		if (!bMassTestLogged)
		{
			int ReadyCount = 0;
			for (auto& Handle : MassHandles)
			{
				if (Handle.IsReady()) ReadyCount++;
			}

			if (ReadyCount == 500)
			{
				float Elapsed = Subsystem.Time->GetElapsedTime() - MassTestStartTime;
				LOG("POWER TEST 1: All 500 textures ready in {:.3f}s!", Elapsed);
				bMassTestLogged = true;
			}
		}

		// Power Test 2: Check priority order
		if (!bPriorityTestLogged && PriorityHandles.size() == 6)
		{
			// Check if high priority (index 5) finished before any low priority
			bool bHighReady = PriorityHandles.back().IsReady();
			int LowReadyCount = 0;
			for (int i = 0; i < 5; i++)
			{
				if (PriorityHandles[i].IsReady()) LowReadyCount++;
			}

			if (bHighReady && LowReadyCount == 0)
			{
				LOG("POWER TEST 2: SUCCESS - High priority loaded before low priority!");
				bPriorityTestLogged = true;
			}
			else if (bHighReady && LowReadyCount > 0)
			{
				LOG("POWER TEST 2: High priority ready, but {} low priority also ready", LowReadyCount);
				bPriorityTestLogged = true;
			}
		}
	}

	void MainMenu::Render()
	{
		if (Background)
		{
			Subsystem.Render->Draw(*Background);
		}

		// Original Test 2: Check in Render
		if (TestHandleRender.IsReady() && !TestSpriteRender.has_value())
		{
			TestSpriteRender.emplace(*TestHandleRender);
			TestSpriteRender->setPosition({ 300, 100 });
			LOG("Test 2: Render poll - Texture ready, sprite created!");
		}

		if (TestSpriteRender)
		{
			Subsystem.Render->Draw(*TestSpriteRender);
		}

		if (TestSpriteTick)
		{
			Subsystem.Render->Draw(*TestSpriteTick);
		}

		if (TestSpriteSync)
		{
			TestSpriteSync->setPosition({ 500, 100 });
			Subsystem.Render->Draw(*TestSpriteSync);
		}

		RenderActors();
	}

	void MainMenu::OnPlayClicked()
	{
		Subsystem.GameState->RequestStateChange(MakeState(EGameState::LevelOne));
	}
}