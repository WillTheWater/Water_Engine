#pragma once

namespace we
{
	class Actor;

	class RangedWeapon
	{
	public:
		virtual bool CanShoot() const { return true; }
		virtual bool IsOnCooldown() const { return false; }

		void Shoot();

		Actor* GetOwner() const { return OwningActor; }

	protected:
		RangedWeapon(Actor* OwningActor);

	private:
		virtual void ShootImplimentation() = 0;

		Actor* OwningActor;
	};
}