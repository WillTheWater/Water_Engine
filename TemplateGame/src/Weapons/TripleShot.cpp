#include "Weapons/TripleShot.h"

namespace we
{
	TripleShot::TripleShot(Actor* OwningActor, float CooldownTime, const sf::Vector2f& ShooterPosOffset, const sf::Angle& ShooterRotOffset)
		: Shooter{OwningActor}
		, ShooterL{OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{-45.f, -50.f}, sf::degrees(-10.f), "SpaceShooterRedux/PNG/Lasers/laserRed01.png"}
		, ShooterM{OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{0.f, 0.f}, sf::degrees(0.f), "SpaceShooterRedux/PNG/Lasers/laserRed16.png"}
		, ShooterR{OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{45.f, -50.f}, sf::degrees(10.f), "SpaceShooterRedux/PNG/Lasers/laserRed01.png" }
		, MaxLvlShooterL{OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{-55.f, -50.f}, sf::degrees(-15.f), "SpaceShooterRedux/PNG/Lasers/laserRed01.png" }
		, MaxLvlShooterR{OwningActor, CooldownTime, ShooterPosOffset + sf::Vector2f{55.f, -50.f}, sf::degrees(1.f), "SpaceShooterRedux/PNG/Lasers/laserRed01.png" }
	{
	}
	void TripleShot::AddWeaponLevel(int LevelToAdd)
	{
		Shooter::AddWeaponLevel(LevelToAdd);
		ShooterL.AddWeaponLevel(LevelToAdd);
		ShooterM.AddWeaponLevel(LevelToAdd);
		ShooterR.AddWeaponLevel(LevelToAdd);
	}
	void TripleShot::ShootImplementation()
	{
		ShooterL.Shoot();
		ShooterM.Shoot();
		ShooterR.Shoot();
		if (GetCurrentWeaponLevel() == GetMaxWeaponLevel())
		{
			MaxLvlShooterL.Shoot();
			MaxLvlShooterR.Shoot();
		}
	}
}