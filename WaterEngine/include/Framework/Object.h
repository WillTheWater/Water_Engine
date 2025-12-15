#pragma once

namespace we
{
	class Object
	{
	public:
		Object();
		virtual ~Object();

		virtual void Destroy();
		bool IsPendingDestroy() const { return bIsPendingDestroy; }

	private:
		bool bIsPendingDestroy;
	};
}