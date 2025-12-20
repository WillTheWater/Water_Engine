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

	protected:
		Shooter(Actor* OwningActor);

	private:
		virtual void ShootImplementation() = 0;
		Actor* OwningActor;
	};
}