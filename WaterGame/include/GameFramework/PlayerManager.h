#pragma once
#include "Framework/Core.h"
#include "Player/Player.h"

namespace we
{
    class Boss;

    class PlayerManager
    {
    public:
        static PlayerManager& Get();
        Player& CreatePlayer();
        Player* GetPlayer(int PlayerIndex = 0);
        const Player* GetPlayer(int PlayerIndex = 0) const;

        void SetBoss(weak<Boss> NewBoss);
        weak<Boss> GetBoss() const;

    protected:
        PlayerManager();

    private:
        List<Player> Players;
        static unique<PlayerManager> PlayerMgr;
        weak<Boss> CurrentBoss;
    };
}