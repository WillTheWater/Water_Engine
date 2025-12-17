#include "GameFramework/Game.h"
#include "Levels/GameLevelOne.h"
#include "Framework/Assetmanager.h"
#include "config.h"

we::Application* GetApplication()
{
	return new we::Game{};
}

namespace we
{
	Game::Game()
		: Application{1920, 1080, "Game Window", sf::Style::Default}
	{
		AssetManager::Get().SetAssetRootDirctory(GetAssetDirectory());
		weak<LevelOne> NewWorld = LoadWorld<LevelOne>();
	}
}