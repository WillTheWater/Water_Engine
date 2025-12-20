#pragma once
#include "Framework/Core.h"
#include "Player/Player.h"

namespace we
{
	class PlayerManager
	{
	public:
		static PlayerManager& Get();
		Player& CreatePlayer();
		Player* GetPlayer(int PlayerIndex = 0);
		const Player* GetPlayer(int PlayerIndex = 0) const ;

	protected:
		PlayerManager();


	private:
		List<Player> Players;
		static unique<PlayerManager> PlayerMgr;
	};
}