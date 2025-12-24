#pragma once
#include "Enemy/EnemySpaceship.h"

namespace we
{
	class Boss : public EnemySpaceship
	{
	public:
		Boss(World* World);


		virtual void Tick(float DeltaTime) override;

	private:
		virtual void Shoot() override;
		float Speed;
		float SwitchDirection;
	};
}