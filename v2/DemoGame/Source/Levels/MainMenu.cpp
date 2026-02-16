//// =============================================================================
//// Water Engine v2.0.0
//// Copyright(C) 2026 Will The Water
//// =============================================================================
//
//#include "Levels/MainMenu.h"
//#include "Framework/EngineSubsystem.h"
//#include "Subsystem/ResourceSubsystem.h"
//#include "Subsystem/ResourceSubsystem.h"
//#include "GameStateTokens.h"
//#include "UI/MainMenuUI.h"
//#include "UI/SettingsMenuUI.h"
//#include "Utility/Log.h"
//
//namespace we
//{
//	MainMenu::MainMenu(EngineSubsystem& Subsystem)
//		: World(Subsystem)
//	{
//		MainUI = make_unique<MainMenuUI>(Subsystem);
//		SettingsUI = make_unique<SettingsMenuUI>(Subsystem);
//		SettingsUI->OnExitClicked.Bind(this, &MainMenu::ToggleSettings);
//		SettingsUI->OnToggleFSClicked.Bind(this, &MainMenu::ToggleFullscreen);
//		MainUI->OnPlayButtonClicked.Bind(this, &MainMenu::OnPlayClicked);
//		MainUI->OnSettingsButtonClicked.Bind(this, &MainMenu::ToggleSettings);
//	}
//
//	MainMenu::~MainMenu()
//	{
//	}
//
//	void MainMenu::BeginPlay()
//	{
//		Subsystem.Audio->PlayMusic(EC.DefaultMusic);
//		Subsystem.Audio->PlayAmbient(EC.DefaultAmbient);
//
//		BgHandle = AsyncAsset().LoadTextureAsync(EC.DefaultBackground, AssetPriority::High);
//	}
//
//	void MainMenu::Tick(float DeltaTime)
//	{
//		if (BgHandle.IsReady() && !Background.has_value())
//		{
//			Background.emplace(*BgHandle);
//		}
//	}
//
//	void MainMenu::Render()
//	{
//		if (Background)
//		{
//			Subsystem.Render->Draw(*Background);
//		}
//
//		RenderActors();
//	}
//
//	void MainMenu::OnPlayClicked()
//	{
//		Subsystem.GameState->RequestStateChange(MakeState(EGameState::LevelOne));
//	}
//
//	void MainMenu::ToggleSettings()
//	{
//		if (SettingsUI->IsVisible())
//		{
//			SettingsUI->Hide();
//		}
//		else { SettingsUI->Show(); }
//	}
//
//	void MainMenu::ToggleFullscreen()
//	{
//		//Window.EventToggleBorderlessFullscreen();
//		Subsystem.GUI->EventToggleFullscreen();
//	}
//}