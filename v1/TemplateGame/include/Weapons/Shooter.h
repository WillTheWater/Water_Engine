#pragma once

namespace we
{
	class Actor;

	class Shooter
	{
	public:
		void Shoot();
		virtual bool CanShoot() const { return true; }
		virtual bool IsOnCooldown() const { return false; }
		Actor* GetOwner() const { return OwningActor;}

		int GetCurrentWeaponLevel() const { return CurrentWeaponLevel; }
		int GetMaxWeaponLevel() const { return MaxWeaponLevel; }
		virtual void AddWeaponLevel(int LevelToAdd = 1);

	protected:
		Shooter(Actor* OwningActor);

	private:
		virtual void ShootImplementation() = 0;
		Actor* OwningActor;
		int CurrentWeaponLevel;
		int MaxWeaponLevel;
	};
}