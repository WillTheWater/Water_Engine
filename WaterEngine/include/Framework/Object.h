#pragma once
#include "Framework/Core.h"
#include "Framework/Delegate.h"

namespace we
{
	class Object : public std::enable_shared_from_this<Object>
	{
	public:
		Object();
		virtual ~Object();

		virtual void Destroy();
		bool IsPendingDestroy() const { return bIsPendingDestroy; }

		weak<Object> GetObject();
		weak<const Object> GetObject() const;

		Delegate<Object*> OnDestroy;

	private:
		bool bIsPendingDestroy;
	};
}