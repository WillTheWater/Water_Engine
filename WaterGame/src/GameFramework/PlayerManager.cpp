#include "GameFramework/PlayerManager.h"

namespace we
{
	unique<PlayerManager> PlayerManager::PlayerMgr{ nullptr };
	PlayerManager::PlayerManager()
	{
	}

	Player& PlayerManager::CreatePlayer()
	{
		Players.emplace(Players.begin(), Player());
		return Players.back();
	}

	Player* PlayerManager::GetPlayer(int PlayerIndex)
	{
		if (PlayerIndex >= 0 && PlayerIndex <= Players.size())
		{
			return &(Players[PlayerIndex]);
		}
		return nullptr;
	}

	const Player* PlayerManager::GetPlayer(int PlayerIndex) const
	{
		if (PlayerIndex >= 0 && PlayerIndex <= Players.size())
		{
			return &(Players[PlayerIndex]);
		}
		return nullptr;
	}

	PlayerManager& PlayerManager::Get()
	{
		if (!PlayerMgr)
		{
			PlayerMgr = std::move(unique<PlayerManager>{new PlayerManager{}});
		}
		return *PlayerMgr;
	}
}