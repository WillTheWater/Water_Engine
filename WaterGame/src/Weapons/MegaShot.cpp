#include "Weapons/MegaShot.h"

namespace we
{
	MegaShot::MegaShot(Actor* OwningActor, float CooldownTime, const sf::Vector2f& ShooterPosOffset, const sf::Angle& ShooterRotOffset)
		: Shooter{ OwningActor }
		, ShooterL1{ OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{-45.f, -40.f}, sf::degrees(-15.f), "SpaceShooterRedux/PNG/Lasers/laserBlue02.png" }
		, ShooterL2{ OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{-27.f, -30.f}, sf::degrees(-7.f), "SpaceShooterRedux/PNG/Lasers/laserGreen06.png" }
		, ShooterM{ OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{0.f, 20.f}, sf::degrees(0.f), "SpaceShooterRedux/PNG/Lasers/laserRed08.png" }
		, ShooterR1{ OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{27.f, -30.f}, sf::degrees(7.f), "SpaceShooterRedux/PNG/Lasers/laserGreen06.png" }
		, ShooterR2{ OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{45.f, -40.f}, sf::degrees(15.f), "SpaceShooterRedux/PNG/Lasers/laserBlue02.png" }
	{
	}
	void MegaShot::ShootImplementation()
	{
		ShooterL1.Shoot();
		ShooterL2.Shoot();
		ShooterM.Shoot();
		ShooterR1.Shoot();
		ShooterR2.Shoot();
	}
}