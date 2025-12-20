#include "Weapons/TripleShot.h"

namespace we
{
	TripleShot::TripleShot(Actor* OwningActor, float CooldownTime, const sf::Vector2f& ShooterPosOffset, const sf::Angle& ShooterRotOffset)
		: Shooter{OwningActor}
		, ShooterL{OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{-45.f, -50.f}, sf::degrees(-10.f), "SpaceShooterRedux/PNG/Lasers/laserRed01.png"}
		, ShooterM{OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{0.f, 0.f}, sf::degrees(0.f), "SpaceShooterRedux/PNG/Lasers/laserRed16.png"}
		, ShooterR{OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{45.f, -50.f}, sf::degrees(10.f), "SpaceShooterRedux/PNG/Lasers/laserRed01.png" }
	{
	}
	void TripleShot::ShootImplementation()
	{
		ShooterL.Shoot();
		ShooterM.Shoot();
		ShooterR.Shoot();
	}
}